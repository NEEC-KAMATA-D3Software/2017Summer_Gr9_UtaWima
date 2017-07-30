#pragma once
#include"PhysicsComponent.h"
#include<Source\Util\Type.h>
#include<Source\Entity\Entity.h>

namespace component {

	class BoxRenderClientComponent;

	class CircleCollider : public framework::PhysicsComponent
	{
	public:
		CircleCollider();
		~CircleCollider();

		void active()override;

		void deActive()override;

		/**
		* @brief						��������������Ă���Entity�ɃR���|�[�l���g��ǉ�����
		* @param 	componentInitalizer	����vector�Ɋi�[���ꂽ�R���|�[�l���g�͈�Ăɏ����������
		*/
		virtual void componentSet(std::vector<std::weak_ptr<Component>>* componentInitalizer);

		/**
		* @brief		�p�����[�^���Z�b�g
		* @param param  ������p�����[�^
		*/
		virtual void setParam(const std::vector<std::string>& param)override;

		/**
		* @brief		�Փ˂����Ƃ��̏���
		* @param other	�Փ˂�������I�u�W�F�N�g
		*/
		virtual void onCollisionEnter(const std::weak_ptr<framework::Entity>& other)override;

		/**
		* @brief		�Փ˂��Ă���Ԃ̏���
		* @param other	�Փ˂��Ă��鑊��I�u�W�F�N�g
		*/
		virtual void onCollisionStay(const std::weak_ptr<framework::Entity>& other)override;

		/**
		* @brief		�Փ˂��甲�������̏���
		* @param other	�Փ˂��Ă�������I�u�W�F�N�g
		*/
		virtual void onCollisionExit(const std::weak_ptr<framework::Entity>& other)override;

		// PhysicsComponent ����Čp������܂���
		virtual bool hitCheckCallBack(std::weak_ptr<PhysicsComponent> other) override;

	private:
		//!���イ�̔��a
		int m_Radius;
		util::Vec3 m_Offset;
		std::weak_ptr<BoxRenderClientComponent> m_BoxRender;

		//!�ǂꂭ�炢���荞��ł��邩
		float m_EnterAmount;
	};



}