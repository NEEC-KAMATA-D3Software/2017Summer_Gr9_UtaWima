#pragma once
#include"Render3DComponent.h"
#include<Source\Device\Render\Renderer\3D\Instance\CubeInstanceRenderer.h>
#include<Source\Component\BoxRenderClientComponent.h>
#include<memory>
#include<Source\Entity\Entity.h>

namespace component {

	using BoxElement = std::pair<std::weak_ptr<framework::Entity>, std::weak_ptr<BoxRenderClientComponent>>;
	using BoxContainer = std::list<BoxElement>;

	class BoxRenderOwnerComponent : public framework::Render3DComponent
	{
	public:
		BoxRenderOwnerComponent();
		~BoxRenderOwnerComponent();

		/**
		* @brief		�P�̃����_�[�^�[�Q�b�g�ɕ`��
		*/
		virtual void init();

		// Render3DComponent ����Čp������܂���
		virtual void draw() override;

		virtual void cubeMapCreate(std::list<std::weak_ptr<Render3DComponent>> BoxContainer) override;

		virtual void cubeMapDraw(util::Transform * centerTransform) override;

		virtual int drawNum() override;

		/**
		* @brief		�p�����[�^���Z�b�g
		* @param param  ������p�����[�^
		*/
		virtual void setParam(const std::vector<std::string>& param);

	private:

		/**
		* @brief			�`��ɕs���ȃI�u�W�F�N�g���Ȃ������ׂ�Ė��g�����X�t�H�[�������Ԃ�
		*/
		void  cheackEntity(std::function<void(BoxElement)> action);

		/**
		* @brief			�L���ȃg�����X�t�H�[���擾
		*/
		std::list<util::Transform*> getActiveTransform();

		/**
		* @brief			�L���ȕ`��v�f�擾
		*/
		std::list<BoxElement> getActiveElement();

		/**
		* @brief			�`��`������𖞂����Ă��邩�ǂ���
		*/
		bool isOK(const BoxElement& element);

		/**
		* @brief			��Q�Ƃ��s���ɂȂ��̂����X�g����r������
		*/
		void cheackDelete();

		void eachEntity(std::function<void(std::weak_ptr<framework::Entity>)> action);

	private:
		framework::CubeInstanceRenderer m_CubeRenderer;

		//!�`�悷��g�����X�t�H�[�����X�g
		BoxContainer m_DrawObject;

	};



}