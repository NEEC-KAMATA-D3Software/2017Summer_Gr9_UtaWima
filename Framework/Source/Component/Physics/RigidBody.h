#pragma once

#include<Source\Component\UpdateComponent.h>
#include<Source\Util\Type.h>


namespace framework {
	class Entity;
}

namespace component {

	class BoxCollider;

	class RigidBody : public framework::UpdateComponent
	{
	public:
		enum class Direction
		{
			Up,
			Side,
			Under,
		};


		RigidBody();
		~RigidBody();

		/**
		* @brief		�A�N�e�B�u�ɂ���
		*/
		virtual void active() override;

		/**
		* @brief		�f�B�A�N�e�B�u�ɂ���
		*/
		virtual void deActive() override;

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
		* @brief		�Փ˂����Ƃ��̏���
		* @param other	�Փ˂�������I�u�W�F�N�g
		*/
		virtual void onCollisionEnter(const std::weak_ptr<framework::Entity>& other);

		/**
		* @brief		�Փ˂��Ă���Ԃ̏���
		* @param other	�Փ˂��Ă��鑊��I�u�W�F�N�g
		*/
		virtual void onCollisionStay(const std::weak_ptr<framework::Entity>& other);

		/**
		* @brief		�Փ˂��甲�������̏���
		* @param other	�Փ˂��Ă�������I�u�W�F�N�g
		*/
		virtual void onCollisionExit(const std::weak_ptr<framework::Entity>& other);


		// UpdateComponent ����Čp������܂���
		virtual void update() override;

	private:

		/**
		* @brief		�R���W�����̊O�ɒǂ��o��
		*/
		void moveOut(const std::weak_ptr<framework::Entity>& other);
		
		/**
		* @brief		�Փ˂��������𔻒肷��
		*/
		Direction DirDecision(util::Vec3& dir, const std::weak_ptr<framework::Entity>& other);


	private:
		std::weak_ptr<BoxCollider> m_pBocCollider;
		util::Vec3 m_UpVec = util::Vec3(0,1,0);
		float m_Gravity;
		const float m_G = 0.98;
	};


}