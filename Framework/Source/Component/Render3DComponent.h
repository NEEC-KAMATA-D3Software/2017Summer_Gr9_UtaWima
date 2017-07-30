#pragma once
#include<Source\Component\Component.h>
#include<list>

namespace util {

	class Transform;

}

namespace framework {


	class Render3DComponent : public framework::Component
	{
	public:
		Render3DComponent();
		~Render3DComponent();

		/**
		* @brief		�P�̃����_�[�^�[�Q�b�g�ɕ`��
		*/
		virtual void init();

		virtual void setup() {}

		/**
		* @brief		Task���X�g�ɒǉ�
		*/
		virtual void active() override;

		/**
		* @brief		Task���X�g����폜
		*/
		virtual void deActive() override;

		/**
		* @brief		�P�̃����_�[�^�[�Q�b�g�ɕ`��
		*/
		virtual void draw() = 0;

		/**
		* @brief				6�̃^�[�Q�b�g�ɕ`��
		* @param drawContainer	�L���[�u�}�b�v�ɏ������ޓ��e
		*/
		virtual void cubeMapCreate(std::list <std::weak_ptr<Render3DComponent>> drawContainer) = 0;


		/**
		* @brief			�L���[�u�e�N�X�`���ɕ`��
		*/
		virtual void cubeMapDraw(util::Transform* centerTransform) = 0;

		/**
		* @brief			�`�搔�擾
		*/
		virtual int drawNum() = 0;
	private:

	};

}