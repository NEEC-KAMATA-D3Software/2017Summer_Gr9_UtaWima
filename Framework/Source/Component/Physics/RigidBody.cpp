#include"RigidBody.h"
#include"BoxCollider.h"
#include<Source\Entity\Entity.h>
#include<math.h>
#include<vector>
#include"BoxCollider.h"

namespace component {


	RigidBody::RigidBody()
	{
	}

	RigidBody::~RigidBody()
	{
	}

	void RigidBody::active()
	{
		UpdateComponent::active();
		if (!m_pBocCollider.expired())
			m_pBocCollider.lock()->active();
	}

	void RigidBody::deActive()
	{
		UpdateComponent::deActive();
		if (!m_pBocCollider.expired())
			m_pBocCollider.lock()->deActive();
	}

	void RigidBody::componentSet(std::vector<std::weak_ptr<Component>>* componentInitalizer)
	{
		m_pBocCollider = m_Entity.lock()->addComponent<BoxCollider>();

		std::vector<std::string> param = { "0","50","0","10","50","50" };

		m_pBocCollider.lock()->setParam(param);

		componentInitalizer->emplace_back(m_pBocCollider);
		m_pBocCollider.lock()->componentSet(componentInitalizer);
	}

	void RigidBody::init()
	{
		m_Gravity = 1;
	}

	void RigidBody::onCollisionEnter(const std::weak_ptr<framework::Entity>& other)
	{
		m_Gravity = 1;
	}

	void RigidBody::onCollisionStay(const std::weak_ptr<framework::Entity>& other)
	{
		m_Gravity = 1;
		moveOut(other);
	}

	void RigidBody::onCollisionExit(const std::weak_ptr<framework::Entity>& other)
	{
	}

	void RigidBody::moveOut(const std::weak_ptr<framework::Entity>& other)
	{
		auto otherCollider = other.lock()->getComponent<BoxCollider>();
		if (otherCollider.expired())return;

		auto otherTrans = other.lock()->getTransform();
		auto myTrans = m_Entity.lock()->getTransform();
		auto collisionDir = otherTrans->m_Position - myTrans->m_Position;
		auto&& dirVec = collisionDir.normalize();
		Direction dir = DirDecision(dirVec, other);

		dirVec.z = 0;

		if (dir == RigidBody::Direction::Side) {
			dirVec.y = 0;
		}
		if (dir == RigidBody::Direction::Under || dir == RigidBody::Direction::Up) {
			dirVec.x = 0;
		}

		if (other.lock()->getTag() == "Ground") {
			dirVec.y = -abs(dirVec.y);
		}

		auto enterAmount = otherCollider.lock()->getEnterAmount();

		do {
			myTrans->m_Position -= dirVec;
		} while (m_pBocCollider.lock()->hitCheckCallBack(otherCollider));
	}

	RigidBody::Direction RigidBody::DirDecision(util::Vec3& dir, const std::weak_ptr<framework::Entity>& other)
	{
		//TODO このままだと上にGroundがある場合もしたにあることになってしまうので修正が必要
		//タグがGroundの場合強制で下判定
		if (other.lock()->getTag() == "Ground") {
			return Direction::Under;
		}

		float dot = m_UpVec.dot(dir);
		if (abs(dot) <= 0.25) {
			return Direction::Side;
		}
		if (dot < -0.25) {
			return Direction::Under;
		}

		return Direction::Up;
	}

	void RigidBody::update()
	{
		m_Entity.lock()->getTransform()->m_Position.y -= m_Gravity;
		m_Gravity += m_G;
	}



}