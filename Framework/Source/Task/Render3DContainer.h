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
* @brief	3D描画のタスクを格納するコンテナ
* @author	高須優輝
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
		* @brief		初期化
		*/
		void init();

		/**
		* @brief		描画
		*/
		void draw();

		void removeTask(std::weak_ptr<Render3DComponent> task)override;

	private:
		/**
		* @brief		ジオメトリバッファ描画
		*/
		void gBufferDraw();
		/**
		* @brief		シャドウマップ描画
		*/
		void shadowMapDoraw();

		void cubeMapCreate();

		/**
		* @brief		無効タスク削除
		*/
		void checkDelete()override;

	private:
		//バックバッファ(最終的に画面に出力されるバッファ)
		std::unique_ptr<RenderTarget> m_pTarget;
		
		//!マルチサンプル深度バッファ
		std::shared_ptr<DepthTarget> m_pDepthTarget;

		std::unique_ptr<DepthTarget> m_pShadowMap;

		std::shared_ptr<Texture2D> m_pDepthBuffer;

		std::function<void(std::weak_ptr<component::CameraComponent>)> m_DrawCallBack;

		CComPtr<ID3D11BlendState> m_pBlend;

		//std::shared_ptr<util::ColorPalette> m_Bar;

	};

}