#pragma once
#include<memory>
#include<Source\Util\Math\Transform.h>
#include<atlbase.h>

namespace framework {
	class MultiRenderTarget;
	class MSAAMultiRenderTarget;
	class Texture2D;
	class DeferredLightRenderer;

	class DeferredLighting
	{
	public:
		DeferredLighting();
		~DeferredLighting();


		/**
		* @brief				テクスチャのセット
		*/
		void setTexture(const std::shared_ptr<MultiRenderTarget>& gBuffer);

		/**
		* @brief				テクスチャのセット
		*/
		void setTexture(const std::shared_ptr<MSAAMultiRenderTarget>& gBuffer);

		void setTexture(std::shared_ptr<Texture2D> texture);

		/**
		* @brief				ライティング結果を描画
		*/
		void draw();


	private:
		std::shared_ptr<framework::DeferredLightRenderer> m_pSpriteRenderer;
		util::Transform m_Trans;
		
		//ライティングを施した描画結果
		std::shared_ptr<MultiRenderTarget> m_pLBuffer;

	};

}