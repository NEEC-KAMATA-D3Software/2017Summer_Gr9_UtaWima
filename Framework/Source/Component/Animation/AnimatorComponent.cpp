#include"AnimatorComponent.h"
#include<Source\Resource\Motion\Motion.h>
#include<assert.h>
#include<Source\Resource\ResourceManager.h>
#include<Source\Entity\Entity.h>
#include<Source\Resource\ResourceManager.h>
#include<Source\Util\IO\CSVLoader.h>
#include<Source\Resource\Motion\Motion.h>
#include<assert.h>

namespace component {

	using namespace framework;

	AnimatorComponent::AnimatorComponent()
	{
	}

	AnimatorComponent::~AnimatorComponent()
	{
	}

	void AnimatorComponent::init()
	{
		m_AnimeDatas.clear();
		m_AnimeCount = 0;
		m_CurrentFrame = 0;

		for (auto& data : m_AddList) {
			registData(data);
		}

		if (m_FirstName == "") {
			//最初に読み込んだものを初期モーションにする
			changeAnime(m_AddList[0].name);
		}
		else {
			changeAnime(m_FirstName);
		}
	}

	void AnimatorComponent::addAnime(const std::string & stateName, AnimData data)
	{
		assert(m_AnimeDatas.find(stateName) == m_AnimeDatas.end() && "このキーはすでに登録されています");
		data.id = m_AnimeCount;
		data.name = stateName;
		m_AnimeDatas[stateName] = data;
		m_AnimeCount += 1;
	}

	void AnimatorComponent::update()
	{
		updateEvent();
		updateMotion();
	}

	void AnimatorComponent::clear()
	{
		m_AnimeDatas.clear();
	}

	void AnimatorComponent::changeAnime(const std::string & stateName)
	{
		assert(m_AnimeDatas.find(stateName) != m_AnimeDatas.end() && "このキーは登録されていません");
		//同じものを指定してたら何もしない
		if (m_CurrentName == stateName)return;

		m_CurrentAnime = m_AnimeDatas[stateName];

		m_CurrentName = stateName;

		//アニメーションが変わったらフレームをリセットする
		m_CurrentFrame = 0;
	}

	/*
	param1 モーション設定csv
	param2 モーションエベントcsv
	*/

	void AnimatorComponent::setParam(const std::vector<std::string>& param)
	{
		assert(param.size() > 2 && "パラメータが足りません");

		util::CSVLoader settingLoader(param[0]);
		auto loadData = settingLoader.load();

		//!コメント行削除
		loadData.erase(loadData.begin());
		m_AddList.reserve(loadData.size());
		for (auto data : loadData) {
			assert(data.size() >= 3 && "情報が正しく入っていません");

			LoadData ld;
			ld.name = data[0];
			ld.isLoop = (bool)std::atoi(data[1].c_str());
			ld.speed = std::atoi(data[2].c_str());
			m_AddList.emplace_back(ld);
		}

		m_FirstName = m_AddList[0].name;

		util::CSVLoader eventLoader(param[1]);

		auto eventData = eventLoader.load();
		eventData.erase(eventData.begin());
		for (auto& param : eventData) {
			registEvent(param);
		}
	}

	const int AnimatorComponent::getFrame()
	{
		return m_CurrentFrame;
	}

	const int AnimatorComponent::getCurrentMotionID()
	{
		return m_CurrentAnime.id;
	}

	bool AnimatorComponent::isEnd()
	{
		return m_CurrentFrame >= m_CurrentAnime.endFrame;
	}

	float AnimatorComponent::rate()
	{
		return (float)m_CurrentFrame / m_CurrentAnime.endFrame;
	}

	std::list<std::string> AnimatorComponent::getMotionNames()
	{
		std::vector<framework::AnimData> aniData;

		aniData.reserve(m_AnimeDatas.size());

		for (auto container : m_AnimeDatas) {
			aniData.emplace_back(container.second);
		}

		std::sort(aniData.begin(), aniData.end(), [&](const framework::AnimData& left, const framework::AnimData& right) {
			return left.id <= right.id;
		});

		std::list<std::string> nameContainer;
		for (auto data : aniData) {
			nameContainer.emplace_back(data.name);
		}

		return nameContainer;
	}

	void AnimatorComponent::updateMotion()
	{
		//エンドフレーム達していてかつループ設定になっていれば0に戻して何もしない
		if (m_CurrentFrame >= m_CurrentAnime.endFrame) {
			if (m_CurrentAnime.isLoop) {
				m_CurrentFrame = 0;
				return;
			}
		}

		m_CurrentFrame += m_CurrentAnime.frameSpeed;
		//endフレームを超えないようにする
		m_CurrentFrame = min(m_CurrentFrame, m_CurrentAnime.endFrame);
	}

	void AnimatorComponent::registData(const LoadData & data)
	{
		auto motion = ResourceManager::getInstance()->getMotion(data.name);

		AnimData anim;
		anim.isLoop = data.isLoop;
		anim.frameSpeed = data.speed;
		anim.endFrame = motion->getMaxFrame() - 1;
		
		addAnime(data.name,anim);
	}
	void AnimatorComponent::registEvent(const std::vector<std::string> param)
	{
		m_EmitterContainer.emplace_back(EmitEvent(m_Entity, param[0], param[1], std::atoi(param[2].c_str()), std::atoi(param[3].c_str())));

	}

	void AnimatorComponent::updateEvent()
	{
		for (auto& emitter : m_EmitterContainer) {
			emitter.sourchEvent(m_CurrentAnime,m_CurrentFrame);
		}
	}

	AnimatorComponent::EmitEvent::EmitEvent(std::weak_ptr<framework::Entity> entity, const std::string& animationName, 
																const std::string& eventName, int emitFrame, int liveFrame)
		:m_Entity(entity),
		m_AnimationName(animationName),
		m_EventName(eventName),
		m_EmitFrame(emitFrame),
		m_LiveFrame(liveFrame)
	{
	}
	AnimatorComponent::EmitEvent::~EmitEvent()
	{
	}
	void AnimatorComponent::EmitEvent::emitEvent()
	{
		if (m_Entity.expired())return;
		m_Entity.lock()->onEvent(m_EventName);
	}
	void AnimatorComponent::EmitEvent::sourchEvent(const framework::AnimData& currentAnime,int currentFrame)
	{
		//現在の名前と一致してなければ何もしない
		if (m_AnimationName != currentAnime.name)return;
		if (!isFrameCondition(currentFrame))return;

		emitEvent();
	}
	bool AnimatorComponent::EmitEvent::isFrameCondition(int currentFrame)
	{
		return m_EmitFrame <= currentFrame && m_EmitFrame + m_LiveFrame >= currentFrame;
	}
}