#pragma once
#include"PhysicsComponent.h"
#include<Source\Util\Type.h>

namespace component {

	class BoxRenderClientComponent;

	class BoxCollider : public framework::PhysicsComponent
	{
	public:
		struct Rect;

		BoxCollider();
		~BoxCollider();

		/**
		* @brief						��������������Ă���Entity�ɃR���|�[�l���g��ǉ�����
		* @param 	componentInitalizer	����vector�Ɋi�[���ꂽ�R���|�[�l���g�͈�Ăɏ����������
		*/
		virtual void componentSet(std::vector<std::weak_ptr<Component>>* componentInitalizer);

		/**
		* @brief		�p�����[�^���Z�b�g
		* @param param  ������p�����[�^
		*/
		virtual void setParam(const std::vector<std::string>& param);

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

		/**
		* @brief		�ǂꂭ�炢���荞��ł��邩
		*/
		const util::Vec3& getEnterAmount();

	private:
		/**
		* @brief		�Փ˂��Ă��邩�ǂ���
		*/
		bool isCollision(std::shared_ptr<BoxCollider>& other);

		void getRect(Rect* result);


	private:

		struct BoxCollisionDesc
		{
			//!�g�����X�t�H�[���̃|�W�V�������炸�炷���߂̃I�t�Z�b�g
			util::Vec3 posOffset;
			//!�e�ӂ̒���
			util::Vec3 edge;
		};

		struct Rect {
			float right, left, top, bottom, _near, _far;
		};

		/**
		* @brief		����p��`�����
		*/
		void createRect(Rect* result, BoxCollider* obj);
	private:
		std::weak_ptr<BoxRenderClientComponent> m_BoxRender;
		BoxCollisionDesc m_Desc;
		//!�ǂꂭ�炢���荞��ł��邩
		util::Vec3 m_EnterAmount;
	};



}