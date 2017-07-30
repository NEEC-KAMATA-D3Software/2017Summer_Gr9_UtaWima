#pragma once
#include"PostEffectComponent.h"
#include<Source\Device\Render\Renderer\2D\Single\PostEffectRenderer.h>

namespace component {

	class BlurEffect : public PostEffectComponent
	{
	public:
		BlurEffect();
		~BlurEffect();

		void init()override;

		/**
		* @brief		�|�X�g�G�t�F�N�g�ŕK�v�ȃe�N�X�`����I���ł���
		* @param texContainer	�e�N�X�`���R���e�i
		*/
		virtual void selectTexture(framework::TextureContainer& texContainer)override;

		// PostEffectComponent ����Čp������܂���
		virtual void effectDraw(util::Transform * trans) override;

	private:
		struct Blur
		{
			XMMATRIX world;
			util::Vec2 pixSize;
			util::Vec2 damy;
		};


		std::unique_ptr<framework::PostEffectRenderer<Blur>> m_pRenderer;

	};



}