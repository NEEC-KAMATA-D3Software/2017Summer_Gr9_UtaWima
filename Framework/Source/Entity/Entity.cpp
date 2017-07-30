#include"Entity.h"
#include<assert.h>
#include<algorithm>
#include<functional>

namespace framework {

	Entity::EntityContainer Entity::m_EntityContainer;

	Entity::Entity(std::string name, std::string tag, util::Transform transform)
		:m_Name(name),
		m_Tag(tag),
		m_pTransform(transform),
		m_InitTransform(transform)
	{
		//std::hash<std::string> hash;
		//m_Hash = hash(name);
	}

	Entity::~Entity()
	{
		for (auto& component : m_ComponentList) {
			component->remove();
		}
		m_ComponentList.clear();
	}

	util::Transform * framework::Entity::getTransform()
	{
		return &m_pTransform;
	}

	const std::string Entity::getName()
	{
		return m_Name;
	}

	const std::string Entity::getTag()
	{
		return m_Tag;
	}

	void Entity::active()
	{
		m_IsActive = true;
		//自身のdeActiveで非アクティブになったコンポーネントをすべてアクティブにする
		if (m_ComponentList.empty())return;

		for (auto& component : m_ComponentList) {
			auto state = m_ComponentStateManager.popComponentState(component);
			if (state == ComponentState::notFind) continue;
			
			auto stateFunc = state == ComponentState::active ? std::bind(&Component::active, component) : std::bind(&Component::deActive, component);
			stateFunc();
		}
	}

	void Entity::deActive()
	{
		m_IsActive = false;
		//コンポーネントをすべて非アクティブにする
		for (auto component : m_ComponentList) {
			m_ComponentStateManager.registComponent(component);
			component->deActive();
		}
	}

	bool Entity::isActive()
	{
		return m_IsActive;
	}

	void Entity::onCollisionEnter(const std::weak_ptr<Entity>& other)
	{
		for (auto& component : m_ComponentList) {
			component->onCollisionEnter(other);
		}
	}

	void Entity::onCollisionStay(const std::weak_ptr<Entity>& other)
	{
		for (auto& component : m_ComponentList) {
			component->onCollisionStay(other);
		}
	}

	void Entity::onCollisionExit(const std::weak_ptr<Entity>& other)
	{
		for (auto& component : m_ComponentList) {
			component->onCollisionExit(other);
		}
	}

	std::weak_ptr<Component> Entity::addComponent(std::shared_ptr<Component> component)
	{
		m_ComponentList.emplace_back(component);
		return component;
	}

	void Entity::addEvent(const std::string & key, std::function<void(void)> eventFunc)
	{
		m_Event.addEvent(key,eventFunc);
	}

	void Entity::onEvent(const std::string & key)
	{
		m_Event.onEvent(key);
	}

	void Entity::clearEvent()
	{
		m_Event.clear();
	}

	void Entity::setParent(std::weak_ptr<Entity> parent)
	{
		m_pParent = parent;

		//親に自分を子として追加
		m_pParent.lock()->addChild(Entity::findGameObj(m_Name));
	}

	std::weak_ptr<Entity> Entity::getPearent()
	{
		return m_pParent;
	}

	WeakEntity Entity::findChildIf(std::function<bool(WeakEntity child)> action)
	{
		auto find = std::find_if(m_Child.begin(), m_Child.end(),action);

		return find == m_Child.end() ? WeakEntity() : *find;
	}

	void Entity::initEntity()
	{
		for (auto& entity : m_EntityContainer) {
			entity->init();
		}
	}

	std::weak_ptr<Entity> Entity::createEntity(const std::string & name, const std::string & tag, util::Transform trans)
	{
		assert(findGameObj(name).expired() && "同じ名前のエンティティーが存在します");
		auto entity = std::make_shared<Entity>(name,tag, trans);
		m_EntityContainer.emplace_back(entity);
		return entity;
	}

	std::weak_ptr<Entity> Entity::findGameObj(const std::string & name)
	{
		//名前が一致するものを検索
		auto it = std::find_if(std::begin(m_EntityContainer), std::end(m_EntityContainer), [&](std::shared_ptr<Entity> entity) {
			return entity->getName() == name;
		});

		return it != std::end(m_EntityContainer) ? *it : nullptr;
	}

	std::list<std::weak_ptr<Entity>>  Entity::findGameObjWithTags(const std::string & tag)
	{
		std::list<std::weak_ptr<Entity>> result;

		std::copy_if(m_EntityContainer.begin(), m_EntityContainer.end(),std::back_inserter(result),
			[&](std::weak_ptr<Entity> entity) {
			return entity.lock()->getTag() == tag;
		});

		return result;
	}

	void Entity::removeEntity(Entity* entity)
	{
		m_EntityContainer.remove_if([&](std::shared_ptr<Entity> _entity) {
			return _entity.get() == entity;
		});
	}

	void Entity::clear()
	{
		m_EntityContainer.clear();
	}

	void Entity::allComponentClear()
	{
		for (auto& entity : m_EntityContainer) {
			entity->componentClear();
		}
	}

	void Entity::init()
	{
		m_pTransform = m_InitTransform;
	}
	 void Entity::componentClear()
	 {
		 for (auto& component : m_ComponentList) {
			 component->clear();

		 }
	 }
	 void Entity::addChild(WeakEntity childEntity)
	 {
		 m_Child.emplace_back(childEntity);
	 }
}