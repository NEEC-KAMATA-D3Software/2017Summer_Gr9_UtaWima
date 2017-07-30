#pragma once
#include<Source\Component\Render3DComponent.h>
#include<memory>
#include<list>
#include<functional>
#include<Source\Resource\Texture\CubeTexture.h>
#include<Source\Device\Render\Shader\IMaterial.h>

/**
* @file	RigidInstanceRenderComponent.h
* @brief	�C���X�^���X�`��I�[�i�[�R���|�[�l���g�N���X
* @dital	�N���C�A���g���C���X�^���X�`�悷��
* @authro	���{�D�P
* @date	2017/03/13
*/

namespace framework {
	class RigidInstanceRenderer;
	class CubeMapInstanceRenderer;
	class IInstanceRenderer;
	struct Model;
}

namespace util {
	struct Transform;
}

namespace component {

	class IInstanceRenderer;
	class RenderClientComponent;
	class AnimatorComponent;

	using DrawElement = std::pair<std::weak_ptr<framework::Entity>, std::weak_ptr<RenderClientComponent>>;
	using DrawContainer = std::list<DrawElement>;

	class RenderOwnerComponent : public framework::Render3DComponent
	{
	public:
		RenderOwnerComponent();
		~RenderOwnerComponent();

		/**
		* @brief			������
		*/
		void init()override;

		/**
		* @brief			1�̃����_�[�^�[�Q�b�g�ɕ`��
		*/
		virtual void draw() override;

		/**
		* @brief			�L���[�u�e�N�X�`���쐬
		*/
		virtual void cubeMapCreate(std::list <std::weak_ptr<framework::Render3DComponent>> drawContainer) override;

		/**
		* @brief			�L���[�u�e�N�X�`���ɕ`��
		* @param
		*/
		virtual void cubeMapDraw(util::Transform* centerTransform)override;

		/**
		* @brief			�p�����[�^�Z�b�g
		* @param param		������p�����[�^
		*/
		void setParam(const std::vector<std::string>& param)override;

		/**
		* @brief			���f���̖��O�擾
		*/
		std::string getModelName();

		/**
		* @brief			���f���̃e�N�X�`���̖��O�擾
		*/
		std::vector<std::string> getTexNames();

		/**
		* @brief			���̃����_���[�̃N���C�A���g�����ׂĎ擾
		*/
		std::vector<std::weak_ptr<RenderClientComponent>> getClients();

		/**
		* @brief			�`�搔�擾
		*/
		int drawNum()override;

		void clear()override;

	private:
		/**
		* @brief			�`��ɕs���ȃI�u�W�F�N�g���Ȃ������ׂ�Ė��g�����X�t�H�[�������Ԃ�
		*/
		void  cheackEntity(std::function<void(DrawElement)> action);

		/**
		* @brief			�L���ȃg�����X�t�H�[���擾
		*/
		std::list<util::Transform*> getActiveTransform();

		/**
		* @brief			�L���ȕ`��v�f�擾
		*/
		std::list<DrawElement> getActiveElement();

		/**
		* @brief			�`��`������𖞂����Ă��邩�ǂ���
		*/
		bool isOK(const DrawElement& element);

		/**
		* @brief			��Q�Ƃ��s���ɂȂ��̂����X�g����r������
		*/
		void cheackDelete();

		void eachEntity(std::function<void(std::weak_ptr<framework::Entity>)> action);

	private:
		void skinningSetting(const std::vector<std::string>& param, std::shared_ptr<framework::Model> model);

	private:
		//!�`�悷�郂�f���̖��O
		std::string m_ModelName;
		//!�`�悷�郂�f���̃e�N�X�`���̖��O
		std::vector<std::string> m_TextureNames;
		//!�ʏ�̃W�I���g�������_��
		std::shared_ptr<framework::IInstanceRenderer> m_Renderer;
		//!6�̃r���[�ɕ`�悳��郌���_��
		std::shared_ptr<framework::CubeMapInstanceRenderer> m_CubeMapRenderer;
		//!�`�悷��g�����X�t�H�[�����X�g
		DrawContainer m_DrawObject;
		//!�}�e���A���̃p�����[�^
		framework::MaterialParam m_MaterialParam;
		//!�N���C�A���g�̃A�j���[�^�[
		std::vector<std::weak_ptr<AnimatorComponent>> m_AnimatorContainer;
		//�e�X�g
		std::shared_ptr<framework::CubeTexture> m_pCubTexture;
	};



}