#include"DeferredLighting.h"
#include<Source\Resource\Texture\MultiRenderTarget.h>
#include<Source\Device\Render\Renderer\2D\Single\DeferredLightRenderer.h>
#include<Source\Application\Screen\Screen.h>
#include<string>
#include<Source\Resource\Texture\MSAAMultiRenderTarget.h>
#include<Source\Util\Render\RenderTargetStack.h>

namespace framework {

	DeferredLighting::DeferredLighting()
	{
		m_pSpriteRenderer = std::make_shared<DeferredLightRenderer>();
		m_Trans.m_Position.x = Screen::WINDOW_WIDTH_HALF;
		m_Trans.m_Position.y = Screen::WINDOW_HEIGHT_HALF;
	//	m_Trans.scaling(0.5f);

		//スプライトウィンドウサイズで描画する
		m_pSpriteRenderer->setSize(util::Vec2(Screen::WINDOW_WIDTH, Screen::WINDOW_HEIGHT));

		m_pLBuffer = std::make_shared<MultiRenderTarget>(2,Screen::PIXEL_WIDTH, Screen::PIXEL_HEIGHT, DXGI_FORMAT_R8G8B8A8_UNORM);
	}

	DeferredLighting::~DeferredLighting()
	{
	}


	void DeferredLighting::setTexture(std::shared_ptr<Texture2D> texture)
	{
		m_pSpriteRenderer->setTexture(texture);
	}

	void DeferredLighting::setTexture(const std::shared_ptr<MultiRenderTarget>& gBuffer)
	{
		m_pSpriteRenderer->setTexture(gBuffer);
	}
	void DeferredLighting::setTexture(const std::shared_ptr<MSAAMultiRenderTarget>& gBuffer)
	{
		m_pSpriteRenderer->setTexture(gBuffer);
	}

	void DeferredLighting::draw()
	{
		m_pSpriteRenderer->draw(&m_Trans);
	}


}