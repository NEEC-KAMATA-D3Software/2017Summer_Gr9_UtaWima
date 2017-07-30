#pragma once
#include"PostEffectComponent.h"
#include<Source\Device\Render\Renderer\2D\Single\PostEffectRenderer.h>
#include<Source\Device\Render\Shader\GradationFilter.h>

namespace component {

	class BloomEffect : public PostEffectComponent
	{
	public:
		BloomEffect();
		~BloomEffect();

		void init()override;

		/**
		* @brief		ポストエフェクトで必要なテクスチャを選択できる
		* @param texContainer	テクスチャコンテナ
		*/
		virtual void selectTexture(framework::TextureContainer& texContainer)override;

		// PostEffectComponent を介して継承されました
		virtual void effectDraw(util::Transform * trans) override;

	private:
		struct BloomParam
		{
			XMMATRIX world;
			int bright;
			util::Vec2 pixSize;
			float damy;
		};

		
		std::unique_ptr<framework::PostEffectRenderer<BloomParam>> m_pRenderer;

	};

}