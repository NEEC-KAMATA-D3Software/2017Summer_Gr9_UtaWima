#pragma once
#include<string>
#include"../Util/Math/Transform.h"
#include<string>
#include<list>
#include<algorithm>
#include<memory>
#include<iterator>
#include<vector>
#include<Source\Entity\ComponentStateManager.h>
#include<Source\Util\Template\Event.h>


/**
* @file	Entity.h
* @brief	�Q�[���I�u�W�F�N�g�������N���X
* @authro	���{�D�P
* @date	2017/02/23
*/

namespace framework {

	class Component;
	class Entity
	{
	public:
		using EntityContainer = std::list<std::shared_ptr<Entity>>;
		Entity(std::string name, std::string tag, util::Transform transform);
		~Entity();

		/**
		* @brief	�g�����X�t�H�[���̎擾
		* @return	�g�����X�t�H�[��
		*/
		util::Transform* getTransform();

		/**
		* @brief	�o�^����Ă��閼�O�擾
		*/
		const std::string getName();

		/**
		* @brief	�o�^����Ă���^�O�擾
		*/
		const std::string getTag();


		/**
		* @brief	�A�N�e�B�u�ɂ���
		*/
		void active();

		/**
		* @brief	��A�N�e�B�u�ɂ���
		*/
		void deActive();

		/**
		* @brief	�A�N�e�B�u���ǂ���
		*/
		bool isActive();

		/**
		* @brief		�Փˎ��R�[���o�b�N�֐�
		* @param other	�Փ˂�������I�u�W�F�N�g
		* @detail		�����Ŏ�������Ă���R���|�[�l���g�ɏՓ˂�ʒm����
		*/
		void onCollisionEnter(const std::weak_ptr<Entity>& other);

		/**
		* @brief		�Փ˂��Ă���Ԃ̃R�[���o�b�N�֐�
		* @param other	�Փ˂��Ă��鑊��I�u�W�F�N�g
		*/
		void onCollisionStay(const std::weak_ptr<Entity>& other);

		/**
		* @brief		�Փ˂��甲�����Ƃ�
		* @param other	�Փ˂��Ă�������I�u�W�F�N�g
		*/
		void onCollisionExit(const std::weak_ptr<Entity>& other);


		/**
		* @brief				�R���|�[�l���g�̒ǉ�
		* @param component		�ǉ�����R���|�[�l���g
		* @return				�ǉ������R���|�[�l���g�̎�Q��
		*/
		std::weak_ptr<Component> addComponent(std::shared_ptr<Component> component);

		/**
		* @brief				�C�x���g�ǉ�
		* @param key			�C�x���g�̖��O
		* @param eventFunc		�C�x���g�����������Ƃ��ɌĂ΂��֐�
		*/
		void addEvent(const std::string& key, std::function<void(void)> eventFunc);

		/**
		* @brief				�C�x���g����
		* @param key			�C�x���g�̖��O
		*/
		void onEvent(const std::string& key);

		/**
		* @brief				�����Ă���C�x���g�����ׂč폜
		*/
		void clearEvent();

		/**
		* @brief				�e�I�u�W�F�N�g���Z�b�g
		*/
		void setParent(std::weak_ptr<Entity> parent);

		/**
		* @brief				�e�I�u�W�F�N�g���擾
		*/
		std::weak_ptr<Entity> getPearent();


		/**
		* @brief				�q�I�u�W�F�N�g����������
		*/
		WeakEntity findChildIf(std::function<bool(WeakEntity child)> action);


		static void initEntity();

		/**
		* @brief				�G���e�B�e�B�[�쐬
		*/
		static std::weak_ptr<Entity> createEntity(const std::string& name, const std::string & tag, util::Transform trans);

		/**
		* @brief				���O����
		*/
		static std::weak_ptr<Entity> findGameObj(const std::string& name);

		/**
		* @brief				�^�O����
		*/
		static std::list<std::weak_ptr<Entity>> findGameObjWithTags(const std::string& tag);

		/**
		* @brief				�G���e�B�e�B�[�폜
		*/
		static void removeEntity(Entity* entity);

	public:

		/**
		* @brief	�R���|�[�l���g�̒ǉ�
		* @detail 	�w�肵���^�̃R���|�[�l���g�𐶐����o�^����
		*/
		template<typename T>
		std::weak_ptr<T> addComponent()
		{
			std::shared_ptr<T> component = std::make_shared<T>();
			m_ComponentList.emplace_back(component);
			auto entity = Entity::findGameObj(m_Name);
			component->componentCreated(entity);
			component->active();
			return component;
		}

		/**
		* @brief	�R���|�[�l���g����擾
		* @detail	�����������ꍇ�͍ŏ��̈���擾
		*/
		template<typename T>
		std::weak_ptr<T> getComponent()
		{
			//	���Ă͂܂�R���|�[�l���g�����邩����
			auto& it = std::find_if(std::begin(m_ComponentList), std::end(m_ComponentList),
				[&](std::shared_ptr<Component> component) {
				return dynamic_cast<T*>(component.get()) != nullptr;
			});

			if (it == std::end(m_ComponentList)) return std::weak_ptr<T>();

			//	�_�E���L���X�g���ĕԂ�
			return std::move(std::static_pointer_cast<T>(*it));
		}

		/**
		* @brief	�R���|�[�l���g����擾
		* @detail	�����������ꍇ�͍ŏ��̈���擾
		*/
		template<typename T>
		std::weak_ptr<T> getComponent(Component *pom)
		{
			//	���Ă͂܂�R���|�[�l���g�����邩����
			auto& it = std::find_if(std::begin(m_ComponentList), std::end(m_ComponentList),
				[&](std::shared_ptr<Component> component) {
				return dynamic_cast<T*>(component.get()) == pom;
			});

			if (it == std::end(m_ComponentList)) return std::weak_ptr<T>();

			//	�_�E���L���X�g���ĕԂ�
			return std::move(std::static_pointer_cast<T>(*it));
		}

		/**
		* @brief	�R���|�[�l���g�������̂����ׂĎ擾
		*/
		template<typename T>
		std::list<std::weak_ptr<T>> getComponents()
		{
			std::list<std::shared_ptr<Component>> copyList;
			//	���Ă͂܂�R���|�[�l���g�����邩����
			std::copy_if(m_ComponentList.begin(), m_ComponentList.end(), std::back_inserter(copyList),
				[&](std::shared_ptr<Component> component) {
				return dynamic_cast<T*>(component.get()) != nullptr;
			});

			//!�ŏI�I�ɃL���X�g���ĕԂ����߂̕ϐ�
			std::list<std::weak_ptr<T>> castList;

			for (auto component : copyList) {
				castList.emplace_back(std::static_pointer_cast<T>(component));
			}

			return castList;
		}

		/**
		* @brief	static�����o�[�ϐ��̃N���A�Ăяo��
		*/
		static void clear();

		/**
		* @brief		�w�肵���R���|�[�l���g�������Ă���G���e�B�e�B�[��T��
		* @return       �������G���e�B�e�B�̓��������X�g
		*/
		template<typename T>
		static std::list<std::weak_ptr<Entity>> findHaveComponentEntity() {
			//!�R�s�[�����v�f�������̃��X�g
			std::list<std::weak_ptr<Entity>> copyList;

			std::copy_if(std::begin(m_EntityContainer), std::end(m_EntityContainer), std::back_inserter(copyList),
				[](std::shared_ptr<Entity> entity) {
				return entity->getComponent<T>().lock().get() != nullptr;
			});

			return copyList;
		}
		static void allComponentClear();
		
	private:
		/**
		* @brief	�R���|�[�l���g�͏������Ƀ|�W�V������������������
		*/
		void init();

		/**
		* @brief	�R���|�[�l���g�̂݃N���A�[
		*/
		void componentClear();

		/**
		* @brief	�q�I�u�W�F�N�g�ǉ�
		*/
		void addChild(WeakEntity childEntity);
	private:
		using ComponentContainer = std::list<std::shared_ptr<Component>>;

		//!�I�u�W�F�N�g�̖��O
		std::string m_Name;

		//!�I�u�W�F�N�g�̃^�O
		std::string m_Tag;

		//!���W�ϊ�
		util::Transform m_pTransform;

		//!�������W
		util::Transform m_InitTransform;

		ComponentContainer m_ComponentList;

		util::Event<std::string> m_Event;

		//!�A�N�e�B�u�t���O�@
		bool m_IsActive = true;

		//!���g�̃A�N�e�B�u�t���O�؂�ւ����ɃR���|�[�l���g�̏�Ԃ�ێ�������
		ComponentStateManager m_ComponentStateManager;

		//!�e�I�u�W�F�N�g
		WeakEntity m_pParent;

		//!�q�I�u�W�F�N�g
		std::vector<WeakEntity> m_Child;

		//!�G���e�B�e�B�[�ۑ��ꏊ
		static EntityContainer m_EntityContainer;
	};
}