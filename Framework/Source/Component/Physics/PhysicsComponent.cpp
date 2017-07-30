#include"PhysicsComponent.h"
#include<Source\Entity\Entity.h>
#include<Source\Task\TaskManager.h>
#include<Source\Util\Template\Template.h>

namespace framework {

	PhysicsComponent::PhysicsComponent()
	{
	}


	PhysicsComponent::~PhysicsComponent()
	{
	}

	void PhysicsComponent::active()
	{
		auto wcom = m_Entity.lock()->getComponent<PhysicsComponent>(this);

		TaskManager::getInstance()->addTask(wcom);
		m_isActive = true;
	}

	void PhysicsComponent::deActive()
	{
		auto wcom = m_Entity.lock()->getComponent<PhysicsComponent>(this);

		TaskManager::getInstance()->removeTask(wcom);
		m_isActive = false;
	}

	void PhysicsComponent::init()
	{
		m_HitContainer.clear();
	}

	void PhysicsComponent::collisionUpdate()
	{
		//��ɂȂ�܂�
		while (!onCollitionContainer.empty()) {
			onCollitionContainer.front().first.lock()->onCollisionEnter(onCollitionContainer.front().second);
			onCollitionContainer.pop_front();
		}

		while (!onStayContainer.empty()) {
			onStayContainer.front().first.lock()->onCollisionStay(onStayContainer.front().second);
			onStayContainer.pop_front();
		}

		while (!onExitContainer.empty()) {
			onExitContainer.front().first.lock()->onCollisionExit(onExitContainer.front().second);
			onExitContainer.pop_front();
		}

	}


	bool PhysicsComponent::isHit()
	{
		for (auto& hitObj : m_HitContainer) {
			//�P�ł��q�b�g���Ă�����̂������true
			if (hitObj.second) {
				return true;
			}
		}
		return false;
	}

	bool PhysicsComponent::isHit(std::weak_ptr<Entity> entity)
	{
		auto find = std::find_if(m_HitContainer.begin(), m_HitContainer.end(), [&](const HitPair& pair) {
			return pair.first.lock()->getGameObj()._Get() == entity._Get();
		});

		//�R���e�i�ɂȂ����false
		if (find == m_HitContainer.end())return false;

		return find->second;
	}

	void PhysicsComponent::hitCheck(std::weak_ptr<PhysicsComponent> other)
	{
		bool isHit= hitCheckCallBack(other);
		
		auto find = std::find_if(m_HitContainer.begin(), m_HitContainer.end(), [&](const HitPair& obj) {
			return obj.first.lock().get() == other.lock().get();
		});

		//���X�g�Ɋ܂܂�Ă��Ȃ���Βǉ�
		if (find == m_HitContainer.end()) {
			m_HitContainer.emplace_back(std::make_pair(other, isHit));
			if (isHit) {
				onCollitionContainer.emplace_front(std::make_pair(m_Entity, other.lock()->getGameObj()));
			}
		}
		else {//�܂܂�Ă���΃f�[�^�̍X�V

			//�O��false�ō���true�̏ꍇ�̓G���^�[
			if (!find->second && isHit) {
				onCollitionContainer.emplace_front(std::make_pair(m_Entity, other.lock()->getGameObj()));
			}else if (isHit) {//�����ƃq�b�g���Ă���΃X�e�C
				onStayContainer.emplace_front(std::make_pair(m_Entity, other.lock()->getGameObj()));
			}else if (find->second && !isHit) {
				onExitContainer.emplace_front(std::make_pair(m_Entity, other.lock()->getGameObj()));
			}

			//�X�V
			find->second = isHit;
		}
	}

	void PhysicsComponent::hitObjAction(bool hitFlag, std::function<void(std::weak_ptr<Entity> )> action)
	{
		for (auto& hitObj : m_HitContainer) {
			//�P�ł��q�b�g���Ă�����̂������true
			if (hitObj.second != hitFlag)return;
			action(hitObj.first.lock()->getGameObj());
		}
	}

	void PhysicsComponent::checkDelete()
	{
		m_HitContainer.remove_if([](HitPair hitObj) {
			if (hitObj.first.expired())return true;
			return  !hitObj.first.lock()->isActive();
		});
	}


}