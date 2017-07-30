#include"PlayerComponent.h"
#include<Source\Entity\Entity.h>
#include<Source\Actor\PlayerMoveState.h>
#include<Source\Actor\PlayerIdleState.h>
#include<Source\Util\IO\CSVLoader.h>
#include<Source\Resource\ResourceManager.h>
#include<Source\Resource\Motion\Motion.h>
#include<Source\Component\Physics\CircleCollider.h>
#include<Source\Component\BoxRenderClientComponent.h>
#include<Source\Component\Physics\RigidBody.h>
#include"AttackComponent.h"
#include<Source\Actor\PlayerDamageState.h>
#include<Source\Application\Scene\SceneDelivery.h>
#include<Source\Component\Player\PlayerComponent.h>
#include<Source\Component\Scene\GameMain\ReadyComponent.h>

namespace component {

	const float PlayerComponent::BORDER = 1450;

	PlayerComponent::PlayerComponent()
	{
		m_PlayerData = std::make_shared<PlayerData>();
	}

	PlayerComponent::~PlayerComponent()
	{
		if (!m_PlayerData->arrowEffect.expired()) {
			//寿命の設定がないため手動でエフェクトをストップさせる
			auto handle = m_PlayerData->arrowEffect.lock()->getHandle();
			framework::EffectManager::getInstance()->effectStop(handle);
		}
		if (!m_PlayerData->rendaEffect.expired()) {
			//寿命の設定がないため手動でエフェクトをストップさせる
			auto handle = m_PlayerData->rendaEffect.lock()->getHandle();
			framework::EffectManager::getInstance()->effectStop(handle);
		}
	}

	void PlayerComponent::init()
	{
		m_States = std::make_shared<PlayerIdleState>();

		m_PlayerData->currentState = PlayerState::idle;

		auto attackName = m_PlayerData->motionNames[PlayerState::attack];

		auto motion = framework::ResourceManager::getInstance()->getMotion(attackName);

		auto frameCount = motion->getMaxFrame();

		m_PlayerData->colliderMat.reserve(frameCount);
		util::foreach(frameCount, [&](int i) {
			m_PlayerData->colliderMat.emplace_back(motion->getFrameMotionWithName("R_Sholder", i));
		});

		activeCollision();

			//自分のコリジョン有効化
		m_Entity.lock()->addEvent("OnCollision", [&]() {
			activeCollision();
		});

		//自分のコリジョン無効化
		m_Entity.lock()->addEvent("OffCollision", [&]() {
			deActiveCollision();
		});


		m_pAttackComponent = m_Entity.lock()->findChildIf([&](framework::WeakEntity child) {
			return child.lock()->getTag() == "Attack";
		});

		isEnd = false;

		m_pReady = framework::Entity::findGameObj("Ready").lock()->getComponent<ReadyComponent>();


		std::string effectName[] = {"Player1","Player2" ,"Player3" ,"Player4" };

		auto arraow = framework::EffectManager::getInstance()->generate(effectName[m_PlayerData->controlID], util::Transform(m_Entity.lock()->getTransform()->m_Position, util::Vec3(), util::Vec3(1, 1, 1)));
		arraow.lock()->m_Trans.scaling(0.5);
		getPlayerData().lock()->arrowEffect = arraow;

	}

	void PlayerComponent::componentSet(std::vector<std::weak_ptr<Component>>* componentInitalizer)
	{
		m_RigidBody = m_Entity.lock()->addComponent<RigidBody>();

		componentInitalizer->emplace_back(m_RigidBody);

		m_RigidBody.lock()->componentSet(componentInitalizer);

		m_CircleCollider = m_Entity.lock()->addComponent<component::CircleCollider>();
		std::vector < std::string > param = { std::to_string(50),std::to_string(0),std::to_string(0),std::to_string(0) };
		m_CircleCollider.lock()->setParam(param);
		m_CircleCollider.lock()->componentSet(componentInitalizer);
	}

	const PlayerState & PlayerComponent::getSate()
	{
		return m_PlayerData->currentState;
	}

	void PlayerComponent::setParam(const std::vector<std::string>& param)
	{
		util::CSVLoader charaMotion(param[0]);
		auto motionDesc = charaMotion.load();

		//!csvコメント部分削除
		motionDesc.erase(motionDesc.begin());

		for (auto desc : motionDesc) {
			m_PlayerData->motionNames[m_PlayerData->convert(desc[0])] = desc[1];
		}

		m_PlayerData->controlID = std::atoi(param[1].c_str());
	}

	void PlayerComponent::update()
	{
		effectUpdate();

		if (!m_pReady.lock()->isGo())return;

		m_States = m_States->update(this);

		if (isBorer()) {
			//コントローラの番号はプレイヤーの番号
			SceneDelivery::getInstance()->addPlayer(m_PlayerData->controlID);
			m_Entity.lock()->deActive();
			getPlayerData().lock()->dethEffect = framework::EffectManager::getInstance()->generate("Deth", util::Transform(m_Entity.lock()->getTransform()->m_Position, util::Vec3(), util::Vec3(1, 1, 1)));
			framework::ResourceManager::getInstance()->playSound("Deth");
		}

		m_Entity.lock()->getTransform()->m_Position.x = util::clamp<float>(m_Entity.lock()->getTransform()->m_Position.x,-BORDER, BORDER);
		m_Entity.lock()->getTransform()->m_Position.y = util::clamp<float>(m_Entity.lock()->getTransform()->m_Position.y, -BORDER, BORDER);
	}

	std::weak_ptr<PlayerData> PlayerComponent::getPlayerData()
	{
		return m_PlayerData;
	}

	void PlayerComponent::onCollisionEnter(const std::weak_ptr<framework::Entity>& other)
	{
		if (other.lock()->getTag() != "Attack") return;
		if (m_pAttackComponent.expired())return;
		//自分の攻撃判定だったら処理しない
		if (m_pAttackComponent._Get() == other._Get()) return;


		m_States = std::make_shared<PlayerDamageState>(other.lock()->getPearent());
	}

	void PlayerComponent::activeCollision()
	{
		m_RigidBody.lock()->active();
		m_CircleCollider.lock()->active();
	}

	void PlayerComponent::deActiveCollision()
	{
		m_RigidBody.lock()->deActive();
		m_CircleCollider.lock()->deActive();
	}

	bool PlayerComponent::isEffectEnd()
	{
		if (isEnd)return true;
		if (m_PlayerData->dethEffect.expired())
			return false;
		if (framework::EffectManager::getInstance()->isEffectExits(m_PlayerData->dethEffect)) {
			isEnd = true;
		}
		return isEnd;
	}

	bool PlayerComponent::isBorer()
	{
		auto trans = m_Entity.lock()->getTransform();
		//return abs(trans->m_Position.x) >= component::PlayerComponent::BORDER || abs(trans->m_Position.y) >= component::PlayerComponent::BORDER;
		return abs(trans->m_Position.y) >= component::PlayerComponent::BORDER;
	}

	void PlayerComponent::effectUpdate()
	{
		auto effect = getPlayerData().lock()->arrowEffect;
		effect.lock()->m_Trans.m_Position = m_Entity.lock()->getTransform()->m_Position + util::Vec3(0,180,0);
		//auto effect = getPlayerData().lock()->arrowEffect;
		//effect.lock()->m_Trans.m_Position = ;
	}

}


