#include"BoxCollider.h"
#include<Source\Task\TaskManager.h>
#include<Source\Component\BoxRenderClientComponent.h>

namespace component {


	BoxCollider::BoxCollider()
	{
	}

	BoxCollider::~BoxCollider()
	{
	}


	void BoxCollider::componentSet(std::vector<std::weak_ptr<Component>>* componentInitalizer)
	{
#ifdef _MDEBUG
		m_BoxRender = m_Entity.lock()->addComponent<BoxRenderClientComponent>();
		std::vector<std::string> param = { "0","0","0",	std::to_string(m_Desc.posOffset.x),std::to_string(m_Desc.posOffset.y),std::to_string(m_Desc.posOffset.z),
			std::to_string(m_Desc.edge.x),std::to_string(m_Desc.edge.y),std::to_string(m_Desc.edge.z),
			"0","0","0" };
		m_BoxRender.lock()->setParam(param);
		componentInitalizer->emplace_back(m_BoxRender);
#endif
	}

	bool BoxCollider::hitCheckCallBack(std::weak_ptr<PhysicsComponent> other)
	{
		if (!m_isActive)return false;
		//!型が自分と違うものであれば何もしない
		std::shared_ptr<BoxCollider> circleComp;
		if (!isMold<BoxCollider>(other, &circleComp)) {
			return false;
		}

		return isCollision(circleComp);
	}

	const util::Vec3 & BoxCollider::getEnterAmount()
	{
		return m_EnterAmount;
	}

	void BoxCollider::setParam(const std::vector<std::string>& param)
	{
		assert(param.size() >= 6 && "パラメータが正しくありません");

		m_Desc.posOffset = util::atof<util::Vec3>(param).reault;
		m_Desc.edge = util::atof<util::Vec3>(param, 3).reault;
	}

	void BoxCollider::onCollisionEnter(const std::weak_ptr<framework::Entity>& other)
	{
		if (m_BoxRender.expired())return;
		m_BoxRender.lock()->setColor(util::Vec3(1, 0, 0));
	}

	void BoxCollider::onCollisionStay(const std::weak_ptr<framework::Entity>& other)
	{
	}

	void BoxCollider::onCollisionExit(const std::weak_ptr<framework::Entity>& other)
	{
		if (m_BoxRender.expired())return;
		if (isHit())return;
		m_BoxRender.lock()->setColor(util::Vec3(0, 0, 1));
	}

	bool BoxCollider::isCollision(std::shared_ptr<BoxCollider>& other)
	{
		Rect myRect;
		getRect(&myRect);

		Rect otherRect;
		other->getRect(&otherRect);

		m_EnterAmount.x = myRect.right - otherRect.left;
		m_EnterAmount.y = myRect.top - otherRect.bottom;

		return  (myRect.right > otherRect.left) &&
			(myRect.left < otherRect.right) &&
			(myRect.top > otherRect.bottom) &&
			(myRect.bottom < otherRect.top);

		//return  (myRect.right > otherRect.left) &&
		//	(myRect.left < otherRect.right) &&
		//	(myRect.top > otherRect.bottom) &&
		//	(myRect.bottom < otherRect.top) &&
		//	(myRect._near < otherRect._far) &&
		//	(myRect._far > otherRect._near);
	}

	void BoxCollider::getRect(Rect * result)
	{
		createRect(result, this);
	}

	void BoxCollider::createRect(Rect * result, BoxCollider * obj)
	{
		auto pos = obj->getGameObj().lock()->getTransform()->m_Position;

		result->right = m_Desc.posOffset.x + pos.x + m_Desc.edge.x;
		result->left = m_Desc.posOffset.x + pos.x - m_Desc.edge.x;
		result->top = m_Desc.posOffset.y + pos.y + m_Desc.edge.y;
		result->bottom = m_Desc.posOffset.y + pos.y - +m_Desc.edge.y;

		//TODO Zはまだ考慮していないので後で修正する
		result->_near = m_Desc.posOffset.z + pos.z;
		result->_far = m_Desc.posOffset.z + pos.z + m_Desc.edge.z;
	}


}