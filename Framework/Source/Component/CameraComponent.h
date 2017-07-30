#pragma once
#include"UpdateComponent.h"
#include<Source\Device\Camera\Camera.h>
#include<functional>
#include<memory>
#include<list>
#include<Source\Resource\Texture\IMultiRenderTarget.h>
#include<Source\Resource\Texture\RenderTarget.h>

/**
* @file	CameraComponent.h
* @brief	Entity���J�����ɂ���N���X
* @authro	���{�D�P
* @date	2017/04/12
*/

namespace framework {
	class MSAAMultiRenderTarget;
	class MultiRenderTarget;
	class DepthTarget;
	class Texture2D;
	class MSAADepthTarget;
	class DeferredLighting;
	class PostEffectPass;
}

namespace component {

	class CameraComponent;
	class PostEffectComponent;

	using WeakCamera = std::weak_ptr<CameraComponent>;
	using WeakEntity = std::weak_ptr<framework::Entity>;

	class CameraComponent : public framework::UpdateComponent
	{
	public:
		CameraComponent();
		~CameraComponent();

		/**
		* @brief		������
		*/
		virtual void init()override;

		/**
		* @brief		�A�N�e�B�u�ɂ���
		*/
		virtual void active() override;

		/**
		* @brief		�f�B�A�N�e�B�u�ɂ���
		*/
		virtual void deActive() override;


		// UpdateComponent ����Čp������܂���
		virtual void update() override;

		/**
		* @brief		�r���[�ϊ��s��ɕϊ����Ď擾
		*/
		virtual util::Mat4 toViewMatrix();

		/**
		* @brief		�p�����[�^���Z�b�g
		* @param param  ������p�����[�^
		*/
		virtual void setParam(const std::vector<std::string>& param)override;

		/**
		* @brief					���C�e�B���O�o�b�t�@�ɕ`�悷��
		*/
		void doLighting();

		/**
		* @brief					�G�t�F�N�g����������
		*/
		void doEffect();

		/**
		* @brief					�����o�����_�[�^�[�Q�b�g�ɏ������݊J�n
		*/
		void beginToBuffer(std::shared_ptr<framework::DepthTarget>& inDepth);

		/**
		* @brief					�����o�[�����_�[�^�[�Q�b�g�̏������ݏI��
		*/
		void endToBuffer();

		/**
		* @brief					gBuffer�̓��e��`��
		* @param drawLocation	�@�@���Ԗڂ̃��P�[�V������
		*/
		void gBufferDraw(int drawLocation);

		/**
		* @brief					lightingBuffer�̓��e��`��
		* @param drawLocation	�@�@���Ԗڂ̃��P�[�V������
		*/
		void lightingDraw(int drawLocation);

		/**
		* @brief					�|�X�g�G�t�F�N�g�̂��������ŏI���ʕ`��
		*/
		void finalBufferDraw();

		//�e�X�g

		CComPtr<ID3D11ShaderResourceView> getShaderVeiw();

	public:
		/**
		* @brief		���ׂẴJ�������擾����
		*/
		static std::vector<WeakCamera> allCameras();

		/**
		* @brief		���ׂẴJ�������擾����
		* @return	�@�@�J�����������Ă���G���e�B�e�B�̖��O�ŃJ�������������� ������Ȃ����nullptr
		* @detail		�擾�͈�ԍŏ��Ɍ����������̂Ɍ���̂ŕ������Ă�����̂�findCameras���g��
		*/
		static WeakCamera findCamera(const std::string& cameraEntityName);

		/**
		* @brief		���ׂẴJ�������擾����
		* @return	�@�@�J�����������Ă���G���e�B�e�B�̖��O�ŃJ�������������� ������Ȃ����nullptr
		* @detail		�擾�͈�ԍŏ��Ɍ����������̂Ɍ���̂ŕ������Ă�����̂�findCameras���g��
		*/
		static std::vector<WeakCamera> findCameras(const std::string& cameraEntityName);

		/**
		* @brief		���݂̃��C���J�������擾����
		*/
		static WeakCamera getMainCamera();

	private:

		void lookSet(const std::vector<std::string>& param);

		void renderTargetNumSet(const std::vector<std::string>& param);

		/**
		* @brief		 ���g�̃G���e�B�e�B�[�ɂ��Ă���|�X�g�G�t�F�N�g��T��
		*/
		void findPostEffect();

		/**
		* @brief		 ���ׂẴJ�����R���|�[�l���g�ɑ΂��Ċ֐��|�C���^�ő��삪�ł���
		* @param cameraAction  �J�����𑀍삷��֐��|�C���^
		*/
		static void cameraLoop(std::function<void(WeakCamera)> cameraAction);

		/**
		* @brief		 �J�����������Ă���G���e�B�e�B�[���������đ��삷�郋�[�v
		* @param action  Entity�ɂ�������֐��|�C���^
		*/
		static void haveCameraEntityLoop(std::function<void(WeakEntity)> entityAction);


	private:

		framework::Camera m_Camera;

		//!�����_�I�u�W�F�N�g
		std::weak_ptr<framework::Entity> m_LookEntity;

		std::shared_ptr<framework::MultiRenderTarget> m_pGBuffer;

		std::shared_ptr<framework::MultiRenderTarget> m_pLBuffer;

		//!�|�X�g�G�t�F�N�g��
		std::shared_ptr<framework::RenderTarget> m_pPostRender;

		std::list<std::weak_ptr<PostEffectComponent>> m_PostEfect;

		std::unique_ptr<framework::DeferredLighting> m_pLighting;

		std::unique_ptr<framework::PostEffectPass> m_pPass;

		static WeakCamera m_MainCamera;
	};



}