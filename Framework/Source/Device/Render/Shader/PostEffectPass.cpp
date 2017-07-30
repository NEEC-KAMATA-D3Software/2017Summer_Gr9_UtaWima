#include"PostEffectPass.h"
#include<Source\Component\Effect\PostEffectComponent.h>
#include<Source\Resource\Texture\MultiRenderTarget.h>
#include<Source\Resource\Texture\MSAAMultiRenderTarget.h>

namespace framework {

	PostEffectPass::PostEffectPass(EffectContainer effects, std::shared_ptr<framework::MultiRenderTarget> gBuffer, std::shared_ptr<framework::MultiRenderTarget> lBuffer)
		:m_EffectContainer(effects),
		m_pGBuffer(gBuffer),
		m_pLBuffer(lBuffer)
	{
		m_SwapTarget.reserve(2);
		m_SwapTarget.emplace_back(std::make_shared<RenderTarget>(Screen::PIXEL_WIDTH, Screen::PIXEL_HEIGHT, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM));
		m_SwapTarget.emplace_back(std::make_shared<RenderTarget>(Screen::PIXEL_WIDTH, Screen::PIXEL_HEIGHT, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM));

		m_TextureContainer["Specular"] = lBuffer->getShaderView(1);
		m_TextureContainer["Velocity"] = gBuffer->getShaderView(7);
	}

	PostEffectPass::~PostEffectPass()
	{
	}

	void PostEffectPass::rendering()
	{
		m_TextureContainer["Main"] = m_pLBuffer->getShaderView(0);

		for (auto& effect : m_EffectContainer) {

			//テクスチャの選択
			effect.lock()->selectTexture(m_TextureContainer);

			util::getContext()->OMSetRenderTargets(1, m_SwapTarget[0]->getView(), NULL);

			m_SwapTarget[0]->clear(util::Vec4(0, 0, 0, 1));

			effect.lock()->doEffect(util::Vec2(Screen::WINDOW_WIDTH_HALF, Screen::WINDOW_HEIGHT_HALF));

			util::getContext()->OMSetRenderTargets(0, NULL, NULL);

			//メインとして使うターゲットを更新

			m_SwapTarget[1]->copy(*(m_SwapTarget[0].get()));

			m_TextureContainer["Main"] = m_SwapTarget[1]->getShaderView();
		}
	}

	void PostEffectPass::draw()
	{
		m_SwapTarget[0]->draw(util::Vec2(Screen::WINDOW_WIDTH_HALF, Screen::WINDOW_HEIGHT_HALF));
	}

}