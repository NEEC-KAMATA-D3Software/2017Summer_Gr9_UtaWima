#include"AttackComponent.h"
#include<Source\Component\Physics\CircleCollider.h>
#include<math.h>

namespace component {

	AttackComponent::AttackComponent()
	{
	}

	AttackComponent::~AttackComponent()
	{
	}

	void AttackComponent::init()
	{
		addEvent();

		auto trans = m_AttackEntity.lock()->getTransform();
		*trans = *m_Entity.lock()->getTransform();
	}

	void AttackComponent::componentSet(std::vector<std::weak_ptr<Component>>* componentInitalizer)
	{
		m_AttackEntity = framework::Entity::createEntity(m_Entity.lock()->getName() + "AttackEntity", "Attack", util::Transform());
		m_AttackEntity.lock()->setParent(m_Entity);
		m_Collider = m_AttackEntity.lock()->addComponent<component::CircleCollider>();
		std::vector < std::string > param = { std::to_string(m_Radius),std::to_string(0),std::to_string(0),std::to_string(0) };
		m_Collider.lock()->setParam(param);
		m_Collider.lock()->componentSet(componentInitalizer);
		m_Collider.lock()->deActive();
	}

	void AttackComponent::update()
	{
		auto attackTrans = m_AttackEntity.lock()->getTransform();

		auto myTrans = m_Entity.lock()->getTransform();

		int sign = util::sign(myTrans->m_Rotation.y);
		
		float x = cos(myTrans->m_Rotation.y) * sign * m_Shift.x;

		attackTrans->m_Position = myTrans->m_Position + util::Vec3(0, m_Shift.y, m_Shift.z);
		attackTrans->m_Position.x += x;
	}

	void AttackComponent::setParam(const std::vector<std::string>& param)
	{
		//m_Shift = std::atof(param[0].c_str());
		m_Shift = util::atof < util::Vec3 >(param).reault;
		m_Radius = std::atof(param[3].c_str());
	}

	bool AttackComponent::isEffectEnd()
	{
		return false;
	}

	void AttackComponent::addEvent()
	{
		//攻撃オブジェクト有効化
		m_Entity.lock()->addEvent("Attack", [&]() {
			m_Collider.lock()->active();
			framework::ResourceManager::getInstance()->playSound("Swing");
		});

		//攻撃オブジェクト無効化
		m_Entity.lock()->addEvent("Pay", [&]() {
			m_Collider.lock()->deActive();
		});
	}

}