#include"PlayerDamageState.h"
#include<Framework.h>
#include<Source\Device\Render\Renderer\Effekseer\EffectManager.h>
#include<Source\Resource\ResourceManager.h>
#include<Source\Actor\PlayerIdleState.h>

PlayerDamageState::PlayerDamageState(const std::weak_ptr<framework::Entity>& other)
	:m_ContactEntity(other)
{
	m_Y = sin(45);
}

PlayerDamageState::~PlayerDamageState()
{
}

void PlayerDamageState::entryAction(component::PlayerComponent * outerValue)
{
	//攻撃モーションに変更
	auto&& currentState = outerValue->getPlayerData().lock()->currentState = PlayerState::damage;

	m_Animator = outerValue->getGameObj().lock()->getComponent<component::AnimatorComponent>();
	assert(!m_Animator.expired() && "プレイヤーにアニメーターがついていません");
	//移動モーションに切り替える

	m_Animator.lock()->changeAnime(outerValue->getPlayerData().lock()->motionNames[currentState]);
	auto myTrans = outerValue->getGameObj().lock()->getTransform();
	auto dir = m_ContactEntity.lock()->getTransform()->m_Position - myTrans->m_Position;

	//衝突した相手の方向を向く
	outerValue->getGameObj().lock()->getTransform()->lookAt(dir);

	auto trans = outerValue->getGameObj().lock()->getTransform();
	//向いている方向を計算
	int sign = util::sign(trans->m_Rotation.y);

	m_Velocity = util::Vec3(sign, m_Y, 0) * 10;

	framework::ResourceManager::getInstance()->playSound("Hit");

	framework::EffectManager::getInstance()->generate("Hit", util::Transform(trans->m_Position - util::Vec3(0,0,50), util::Vec3(), util::Vec3(1, 1, 1)));

	m_ReliveCount = 0;

	auto renda = framework::EffectManager::getInstance()->generate("Renda", util::Transform(myTrans->m_Position, util::Vec3(), util::Vec3(1, 1, 1)));
	outerValue->getPlayerData().lock()->rendaEffect = renda;
}

Trans PlayerDamageState::inputAction(component::PlayerComponent * outerValue, std::shared_ptr<State<component::PlayerComponent>>* nextState)
{
	auto player = outerValue->getGameObj();
	auto trans = player.lock()->getTransform();
	trans->m_Position += m_Velocity;

	outerValue->getPlayerData().lock()->rendaEffect.lock()->m_Trans.m_Position = trans->m_Position + util::Vec3(150,50, 0);
	m_Timer.update();
	if (m_Timer.isEnd())
		m_Timer.init();
	auto scale = util::bezierCurve(m_Timer.rate(), 1.0f, 1.5f, 1.0f);
	outerValue->getPlayerData().lock()->rendaEffect.lock()->m_Trans.scaling(scale);
		
	reliveUpdate(outerValue);

	if (isRelive()) {
		*nextState = std::make_shared<PlayerIdleState>();
		return Trans::Trans_Occured;
	}

	//ダメージを受けてたら即死なので変わることはない
	*nextState = std::make_shared<PlayerDamageState>(*this);
	return Trans::Trans_Again;
}

void PlayerDamageState::exitAction(component::PlayerComponent * outerValue)
{
	framework::ResourceManager::getInstance()->playSound("Cure");

	auto effect = framework::EffectManager::getInstance()->generate("Recovery", util::Transform(outerValue->getGameObj().lock()->getTransform()-> m_Position - util::Vec3(0, 0, 50), util::Vec3(), util::Vec3(1, 1, 1)));

	effect.lock()->m_Trans.scaling(0.5f);


	auto handle = outerValue->getPlayerData().lock()->rendaEffect.lock()->getHandle();
	framework::EffectManager::getInstance()->effectStop(handle);
}

bool PlayerDamageState::isRelive()
{

#ifdef DEBUG | _DEBUG
	return m_ReliveCount == 9;
#else
	//20っ回連打で復活
	return m_ReliveCount == 9;
#endif

}

void PlayerDamageState::reliveUpdate(component::PlayerComponent * outerValue)
{
	if (framework::Input::getGamePad().getButtonTrriger(framework::ButtonCode::A, outerValue->getPlayerData().lock()->controlID) ||
		framework::Input::getGamePad().getButtonTrriger(framework::ButtonCode::B, outerValue->getPlayerData().lock()->controlID)) {
		m_ReliveCount++;
	}
}


