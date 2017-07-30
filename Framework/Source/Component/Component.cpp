#include"Component.h"
#include<assert.h>

namespace framework {

	Component::Component()
	{
		//コンポーネントはデフォルトでアクティブ
		active();
	}

	Component::~Component()
	{
		int i = 0;
	}

	void Component::onConect()
	{
	}

	void Component::componentSet(std::vector<std::weak_ptr<Component>>* componentInitalizer)
	{
	}

	void Component::init()
	{
	}

	void Component::active()
	{
		m_isActive = true;
	}

	void Component::deActive()
	{
		m_isActive = false;
	}

	bool Component::isActive()
	{
		return m_isActive;
	}

	void Component::onCollisionEnter(const std::weak_ptr<Entity>& other)
	{
	}

	void Component::onCollisionStay(const std::weak_ptr<Entity>& other)
	{
	}

	void Component::onCollisionExit(const std::weak_ptr<Entity>& other)
	{
	}

	void Component::setGameObj(std::weak_ptr<Entity> entity)
	{
		m_Entity = entity;
	}

	std::weak_ptr<Entity> Component::getGameObj()
	{
		return m_Entity;
	}

	void Component::setParam(const std::vector<std::string>& param)
	{
		
	}

	void Component::componentCreated(std::weak_ptr<Entity> entity)
	{
		setGameObj(entity);
		onConect();
	}

	void Component::remove()
	{
	}

	int Component::getCallOrder()
	{
		return m_CallOrder;
	}

	void Component::clear()
	{
	}

	void Component::paramCheck(const std::vector<std::string>& param)
	{
		assert(param.size() != 0 && "パラメータがセットされていません");
	}

}