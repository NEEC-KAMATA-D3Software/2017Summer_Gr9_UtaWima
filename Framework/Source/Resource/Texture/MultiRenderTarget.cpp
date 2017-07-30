#include"MultiRenderTarget.h"
#include<Source\Resource\Texture\RenderTarget.h>
#include<Source\Util\WrapFunc.h>

namespace framework {

	MultiRenderTarget::MultiRenderTarget(unsigned int bufferNum, int width, int height, DXGI_FORMAT format)
	{
		m_pTargets.resize(bufferNum);

		util::foreach(bufferNum, [&](int i) {
			m_pTargets[i] = std::make_unique<RenderTarget>(width, height, format);
		});

		m_pResult = std::make_unique<ID3D11RenderTargetView*[]>(bufferNum);
	}

	MultiRenderTarget::~MultiRenderTarget()
	{
	}

	ID3D11RenderTargetView * const * MultiRenderTarget::getView(int location)
	{
		return m_pTargets[location]->getView();
	}

	ID3D11RenderTargetView * const * MultiRenderTarget::getView()
	{
		util::foreach(m_pTargets.size(), [&](int i) {
			m_pResult[i] = *(m_pTargets[i]->getView());
		});

		return &m_pResult[0];
	}

	CComPtr<ID3D11ShaderResourceView> MultiRenderTarget::getShaderView(int location)
	{
		return m_pTargets[location]->getShaderView();
	}

	int MultiRenderTarget::getBufferNum()
	{
		return m_pTargets.size();
	}

	void MultiRenderTarget::clear(const util::Vec4 & color)
	{
		for (auto& target : m_pTargets) {
			target->clear(color);
		}
	}

	util::Vec2 MultiRenderTarget::getSize()
	{
		return m_pTargets[0]->getSize();
	}

	void MultiRenderTarget::draw(int drawLocation)
	{
		m_pTargets[drawLocation]->draw(util::Vec2(Screen::WINDOW_WIDTH_HALF, Screen::WINDOW_HEIGHT_HALF));
	}


}