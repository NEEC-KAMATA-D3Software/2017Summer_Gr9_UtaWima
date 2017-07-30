#pragma once
#include"TaskContainer.h"
#include<Source\Util\Math\Transform.h>
#include<functional>
#include<Source\Component\Render3DComponent.h>
#include<Source\Resource\Texture\MSAADepthTarget.h>
#include<Source\Resource\Texture\MSAARenderTarget.h>
#include<Source\Util\AntTweak\AntWeak.h>
#include<Source\Util\AntTweak\ColorPalette.h>
#include<Source\Device\Render\Renderer\2D\Single\PostEffectRenderer.h>
#include<Source\Device\Render\Shader\GradationFilter.h>
#include<Source\Util\Timer\Timer.h>


/**
* @file		Render3DContainer.h
* @brief	3D�`��̃^�X�N���i�[����R���e�i
* @author	���{�D�P
* @date	2017/3/19
*/

namespace util {
	class AntWeak;
}

namespace component {
	class CameraComponent;

}

namespace framework {
	class RenderTarget;
	class DepthTarget;
	class MultiRenderTarget;
	class Texture2D;
	class SpriteRenderer;
	class DeferredLighting;
	class CubeRenderTrget;
	class CubeDepthTarget;
	class MSAAMultiRenderTarget;
	class IMultiRenderTarget;
	class EffectManager;

	class Render3DContainer : public TaskContainer<Render3DComponent>
	{
	public:
		Render3DContainer();
		~Render3DContainer();

		/**
		* @brief		������
		*/
		void init();

		/**
		* @brief		�`��
		*/
		void draw();

		void removeTask(std::weak_ptr<Render3DComponent> task)override;

	private:
		/**
		* @brief		�W�I���g���o�b�t�@�`��
		*/
		void gBufferDraw();
		/**
		* @brief		�V���h�E�}�b�v�`��
		*/
		void shadowMapDoraw();

		void cubeMapCreate();

		/**
		* @brief		�����^�X�N�폜
		*/
		void checkDelete()override;

	private:
		//�o�b�N�o�b�t�@(�ŏI�I�ɉ�ʂɏo�͂����o�b�t�@)
		std::unique_ptr<RenderTarget> m_pTarget;
		
		//!�}���`�T���v���[�x�o�b�t�@
		std::shared_ptr<DepthTarget> m_pDepthTarget;

		std::unique_ptr<DepthTarget> m_pShadowMap;

		std::shared_ptr<Texture2D> m_pDepthBuffer;

		std::function<void(std::weak_ptr<component::CameraComponent>)> m_DrawCallBack;

		CComPtr<ID3D11BlendState> m_pBlend;

		//std::shared_ptr<util::ColorPalette> m_Bar;

	};

}