#include"MSAAMultiRenderTarget.h"
#include<Source\Util\WrapFunc.h>

namespace framework {

	MSAAMultiRenderTarget::MSAAMultiRenderTarget(int numTexture, int width, int height, DXGI_FORMAT format, int sampleCount)
	{
		m_Targets.reserve(numTexture);
		util::foreach(numTexture, [&](int i) {
			m_Targets.emplace_back(std::make_unique<MSAARenderTarget>(width,height,format,sampleCount));
		});
		
		m_pResult = std::make_unique<ID3D11RenderTargetView*[]>(numTexture);
	}

	MSAAMultiRenderTarget::~MSAAMultiRenderTarget()
	{
	}

	ID3D11RenderTargetView * const * MSAAMultiRenderTarget::getView(int location)
	{
		return m_Targets[location]->getView();;
	}

	ID3D11RenderTargetView * const * MSAAMultiRenderTarget::getView()
	{
		util::foreach(m_Targets.size(), [&](int i) {
			m_pResult[i] = *(m_Targets[i]->getView());
		});
		return &m_pResult[0];
	}

	CComPtr<ID3D11ShaderResourceView> MSAAMultiRenderTarget::getShaderView(int location)
	{
		return m_Targets[location]->getShaderView();
	}

	int MSAAMultiRenderTarget::getBufferNum()
	{
		return m_Targets.size();
	}

	void MSAAMultiRenderTarget::clear(const util::Vec4 & color)
	{
		for (auto& target : m_Targets) {
			target->clear(color);
		}
	}

	util::Vec2 MSAAMultiRenderTarget::getSize()
	{
		return m_Targets[0]->getSize();
	}

	void MSAAMultiRenderTarget::resolveTexutre()
	{
		for (auto& target : m_Targets) {
			target->resolveTexture();
		}
	}

	void MSAAMultiRenderTarget::draw(int drawLoacation)
	{
		m_Targets[drawLoacation]->draw(util::Vec2(Screen::WINDOW_WIDTH_HALF, Screen::WINDOW_HEIGHT_HALF));
	}

}