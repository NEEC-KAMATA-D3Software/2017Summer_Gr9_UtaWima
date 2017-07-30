#pragma once

#include<memory>
#include<vector>
#include<string>
#include<functional>

/**
* @file	Component.h
* @brief	���ׂẴR���|�[�l���g���p�������N���X
* @authro	���{�D�P
* @date	2017/02/23
*/

namespace framework {

	class Entity;

	using WeakEntity = std::weak_ptr<Entity>;

	class Component
	{
	public:
		Component();
		virtual ~Component();

		/**
		* @brief		�Q�[���I�u�W�F�N�g�ɐڑ����ꂽ�u��
		*/
		virtual void onConect();

		/**
		* @brief						��������������Ă���Entity�ɃR���|�[�l���g��ǉ�����
		* @param 	componentInitalizer	����vector�Ɋi�[���ꂽ�R���|�[�l���g�͈�Ăɏ����������
		*/
		virtual void componentSet(std::vector<std::weak_ptr<Component>>* componentInitalizer);

		/**
		* @brief		������
		* @Detail		���ׂẴR���|�[�l���g����ĂɌĂ΂��
		*/
		virtual void init();

		/**
		* @brief		���̃R���|�[���g���A�N�e�B�u�ɂ���
		*/
		virtual void active();

		/**
		* @brief		���̃R���|�[���g���f�B�A�N�e�B�u�ɂ���
		*/
		virtual void deActive();

		/**
		* @brief		�A�N�e�B�u���ǂ���
		*/
		bool isActive();

		/**
		* @brief		�Փ˂����Ƃ��̏���
		* @param other	�Փ˂�������I�u�W�F�N�g
		*/
		virtual void onCollisionEnter(const std::weak_ptr<Entity>& other);

		/**
		* @brief		�Փ˂��Ă���Ԃ̏���
		* @param other	�Փ˂��Ă��鑊��I�u�W�F�N�g
		*/
		virtual void onCollisionStay(const std::weak_ptr<Entity>& other);

		/**
		* @brief		�Փ˂��甲�������̏���
		* @param other	�Փ˂��Ă�������I�u�W�F�N�g
		*/
		virtual void onCollisionExit(const std::weak_ptr<Entity>& other);

		/**
		* @brief		��������������Ă���Q�[���I�u�W�F�N�g���Z�b�g����
		*/
		void setGameObj(std::weak_ptr<Entity> entity);

		/**
		* @brief		��������������Ă���Q�[���I�u�W�F�N�g���擾����
		*/
		std::weak_ptr<Entity> getGameObj();

		/**
		* @brief		�p�����[�^���Z�b�g
		* @param param  ������p�����[�^
		*/
		virtual void setParam(const std::vector<std::string>& param);

		/**
		* @brief		�R���|�[�l���g����������ɕK�v�Ȋ֐��܂Ƃ�
		* @param entity ��������������Ă���Entity
		*/
		void componentCreated(std::weak_ptr<Entity> entity);

		/**
		* @brief		�J������
		*/
		virtual void remove();

		/**
		* @brief		�Ăяo���l�擾
		*/
		int getCallOrder();

		/**
		* @brief		�N���A�[
		*/
		virtual void clear();

	protected:
		void paramCheck(const std::vector<std::string>& param);

		template<typename T>
		static void haveComponentEntityLoop(std::function<void(WeakEntity)> entityAction)
		{
			//�J�����R���|�[�l���g�������Ă���G���e�B�e�B�[�����ׂĒT��
			auto entityList = framework::Entity::findHaveComponentEntity<T>();
			for (auto& entity : entityList) {
				entityAction(entity);
			}
		}

	protected:
		//!��������������Ă���Q�[���I�u�W�F�N�g
		std::weak_ptr<Entity> m_Entity;
		//!���������邩�ǂ������ǂ���
		bool m_isActive = true;
		//!���l���Ⴂ�قǑ������s�����
		float m_CallOrder = 0;
	};


}