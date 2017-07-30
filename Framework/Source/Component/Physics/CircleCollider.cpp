#include "CircleCollider.h"
#include "CircleCollider.h"
#include"CircleCollider.h"
#include<Source\Component\BoxRenderClientComponent.h>

namespace component{

	CircleCollider::CircleCollider()
	{
	}

	CircleCollider::~CircleCollider()
	{
	}

	void CircleCollider::active()
	{
		PhysicsComponent::active();
		if(!m_BoxRender.expired())
			m_BoxRender.lock()->active();
	}

	void CircleCollider::deActive()
	{
		PhysicsComponent::deActive();
		if (!m_BoxRender.expired())
		m_BoxRender.lock()->deActive();
	}

	void CircleCollider::componentSet(std::vector<std::weak_ptr<Component>>* componentInitalizer)
	{
#ifdef _MDEBUG
		m_BoxRender = m_Entity.lock()->addComponent<BoxRenderClientComponent>();
		std::vector<std::string> param = { "0","0","0",	std::to_string(m_Offset.x),std::to_string(m_Offset.y),std::to_string(m_Offset.z),
			std::to_string(m_Radius),std::to_string(m_Radius),std::to_string(m_Radius),
			"0","0","0" };
		m_BoxRender.lock()->setParam(param);
		componentInitalizer->emplace_back(m_BoxRender);
#endif _MDEBUG
	}

	bool CircleCollider::hitCheckCallBack(std::weak_ptr<PhysicsComponent> other)
	{
		if (!m_isActive)return false;
		//!�^�������ƈႤ���̂ł���Ή������Ȃ�
		std::shared_ptr<CircleCollider> circleComp;
		if (!isMold<CircleCollider>(other, &circleComp)) {
			return false;
		}

		auto myPos = m_Entity.lock()->getTransform()->m_Position;
		auto otherPos = other.lock()->getGameObj().lock()->getTransform()->m_Position;

		auto sub = myPos - otherPos;
		float distance = sub.length();
		float width = m_Radius + circleComp->m_Radius;

		//�q�b�g���Ă���Ό��ݔ��肵�Ă���I�u�W�F�N�g��ێ�����
		return distance <= width;	
	}

	void CircleCollider::setParam(const std::vector<std::string>& param)
	{
		m_Radius = std::atof(param[0].c_str());
		m_Offset = util::atof<util::Vec3>(param,1).reault;
	}

	void CircleCollider::onCollisionEnter(const std::weak_ptr<framework::Entity>& other)
	{
		if (m_BoxRender.expired())return;
		m_BoxRender.lock()->setColor(util::Vec3(1,0,0));
	}

	void CircleCollider::onCollisionStay(const std::weak_ptr<framework::Entity>& other)
	{
	}

	void CircleCollider::onCollisionExit(const std::weak_ptr<framework::Entity>& other)
	{
		if (m_BoxRender.expired())return;
		if (isHit())return;
		m_BoxRender.lock()->setColor(util::Vec3(0, 0, 1));
	}



}