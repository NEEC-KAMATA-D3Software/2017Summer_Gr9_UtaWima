#pragma once
#include<Framework.h>
namespace framework {
	class Entity;
}

namespace component {
	class CircleCollider;

	class AttackComponent : public framework::UpdateComponent
	{
	public:
		AttackComponent();
		~AttackComponent();

		void init()override;

		/**
		* @brief						��������������Ă���Entity�ɃR���|�[�l���g��ǉ�����
		* @param 	componentInitalizer	����vector�Ɋi�[���ꂽ�R���|�[�l���g�͈�Ăɏ����������
		*/
		virtual void componentSet(std::vector<std::weak_ptr<Component>>* componentInitalizer)override;

		// UpdateComponent ����Čp������܂���
		virtual void update() override;

		/**
		* @brief		�p�����[�^���Z�b�g
		* @param param  ������p�����[�^
		*/
		virtual void setParam(const std::vector<std::string>& param);


		/**
		* @brief		�G�t�F�N�g���I��������
		*/
		bool isEffectEnd();

	private:
		void addEvent();

	private:
		std::weak_ptr<framework::Entity> m_AttackEntity;
		std::weak_ptr<component::CircleCollider> m_Collider;
		//!�U������̂��炷��
		util::Vec3 m_Shift;
		float m_Radius = 50;
	};



}
