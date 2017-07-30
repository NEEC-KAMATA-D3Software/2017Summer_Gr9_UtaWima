#pragma once
#include"TaskContainer.h"
#include<Source\Component\Render2DComponent.h>
#include<memory>

namespace framework {

	class SpriteRenderer;
	class RenderTarget;

	class Render2DContainer : public TaskContainer<Render2DComponent>
	{
	public:
		Render2DContainer();
		~Render2DContainer();

		/**
		* @brief		‰Šú‰»
		*/
		void init();

		/**
		* @brief		•`‰æ
		*/
		void draw();

	private:
		/**
		* @brief		2D•`‰æ
		*/
		void draw2D();

		void backBufferDraw();

	private:
		//!2D—pƒŒƒ“ƒ_ƒ‰[
		std::shared_ptr<RenderTarget> m_pTarget;
		CComPtr<ID3D11BlendState> m_pBlend;

	};



}