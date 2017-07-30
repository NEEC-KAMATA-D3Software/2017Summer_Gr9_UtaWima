#include"CameraMove.h"
#include<minmax.h>
#include<Source\Component\Player\PlayerComponent.h>

namespace component {

	CameraMove::CameraMove()
	{
	}

	CameraMove::~CameraMove()
	{
	}

	void CameraMove::init()
	{
		m_PlayerList = framework::Entity::findGameObjWithTags("Player");
		m_pCamera = framework::Entity::findGameObj("MainCamera");
		m_InitPos = m_pCamera.lock()->getTransform()->m_Position;
		m_pLookEntity = framework::Entity::findGameObj("LookPos");
	}

	void CameraMove::update()
	{
		checkDelete();
		if (m_PlayerList.empty()) {
			moveZero();
			return;
		}
		CameraDesc desc;
		getCameraDesc(&desc);
		xMove(desc);
		zMove(desc);
	}

	/*
	*@param0 摩擦係数
	*@param1 カメラのZ最小距離
	*@param2 カメラのZ最大距離
	*@param3 最端プレイヤーの最小距離
	*@param4 最端プレイヤーの最大距離
	*/
	void CameraMove::setParam(const std::vector<std::string>& param)
	{
		if (param.size() == 0)return;
		m_Friction = std::atof(param[0].c_str());
		if (param.size() == 1)return;
		m_ZDesc.near_ = std::atof(param[1].c_str());
		if (param.size() == 2)return;
		m_ZDesc.far_ = std::atof(param[2].c_str());
		if (param.size() == 3)return;
		m_ZDesc.minDistance = std::atof(param[3].c_str());
		if (param.size() == 4)return;
		m_ZDesc.maxDistance = std::atof(param[4].c_str());
		if (param.size() == 5)return;
		m_Boundary = std::atof(param[5].c_str());
	}

	void CameraMove::xMove(const CameraDesc& desc)
	{
		float ave = desc.minPos + desc.avePos;

		float velocity = ave - m_pCamera.lock()->getTransform()->m_Position.x;

		velocity *= m_Friction;

		m_pCamera.lock()->getTransform()->m_Position.x += velocity;
		m_pLookEntity.lock()->getTransform()->m_Position.x += velocity;
	}

	void CameraMove::zMove(const CameraDesc & desc)
	{
		float r = 0;
		auto distance = util::clamp<float>(desc.playerDistance, m_ZDesc.minDistance, m_ZDesc.maxDistance);

		r = distance / m_ZDesc.maxDistance;

		float velocity = (util::lerp(r, -m_ZDesc.near_, -m_ZDesc.far_) - m_pCamera.lock()->getTransform()->m_Position.z);
		velocity *= m_Friction;
		m_pCamera.lock()->getTransform()->m_Position.z += velocity;
	}

	void CameraMove::getCameraDesc(CameraDesc * result)
	{
		m_PlayerList.sort([](const std::weak_ptr<framework::Entity>& entity1, const std::weak_ptr<framework::Entity>& entity2) {
			return entity1.lock()->getTransform()->m_Position.x < entity2.lock()->getTransform()->m_Position.x;
		});

		result->minPos = m_PlayerList.front().lock()->getTransform()->m_Position.x;

		result->maxPos = m_PlayerList.back().lock()->getTransform()->m_Position.x;


		util::Vec3 maxVec(result->maxPos, 0, 0), minVec(result->minPos, 0, 0);

		result->playerDistance = maxVec.distance(minVec);
		//プレヤーの中間座標を見る
		result->avePos = result->playerDistance / 2;
	}

	void CameraMove::checkDelete()
	{
		m_PlayerList.remove_if([&](std::weak_ptr<framework::Entity> player) {
			return !player.lock()->isActive();
		});

		m_PlayerList.remove_if([&](std::weak_ptr<framework::Entity> player) {

			auto trans = player.lock()->getTransform();
			if (m_Boundary < abs(trans->m_Position.x) || m_Boundary < abs(trans->m_Position.y)) {
				return false;
			}

			auto pComponent = player.lock()->getComponent<PlayerComponent>();
			return pComponent.lock()->isEffectEnd();
		});
	}

	void CameraMove::moveZero()
	{
		//m_pCamera.lock()->getTransform()->m_Position =  m_InitPos;
	}

}