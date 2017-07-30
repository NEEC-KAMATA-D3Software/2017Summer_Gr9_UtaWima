#include"GradationFilter.h"
#include<Source\Util\Type.h>
#include<Source\Util\Render\DXFunc.h>
#include<Source\Application\Screen\Screen.h>
#include<Source\Util\Render\RenderTargetStack.h>

namespace framework {
	GradationFilter::GradationFilter(std::shared_ptr<Texture2D> tex2D)
		:GradationFilter(tex2D,1)
	{
	}
	GradationFilter::GradationFilter(std::shared_ptr<Texture2D> tex2D,float scale)
		:renderer("Gradation")
		, m_pTexture2D(tex2D),
		m_Scale(scale)
	{
		auto size = tex2D->getSize();
		m_Target = std::make_unique<RenderTarget>(size.x/ m_Scale,size.y/ m_Scale,DXGI_FORMAT_R8G8B8A8_UNORM);
		renderer.setSize(size);
		m_Param.pixSize.x = 1.0f / m_Target->getSize().x;
		m_Param.pixSize.y = 1.0f / m_Target->getSize().y;
		renderer.setBuffer(m_Param);

		renderer.setTexture(m_pTexture2D);
	}

	GradationFilter::~GradationFilter()
	{
	}

	void GradationFilter::write(int viewNum)
	{
		//現在セットされている描画ターゲットを一時退避
		util::RenderTargetStack stack(viewNum);
		write();
	}

	void GradationFilter::write()
	{
		auto size = m_Target->getSize();
		util::setSingleViewPort(size.x, size.y);
		util::getContext()->OMSetRenderTargets(1, m_Target->getView(), NULL);
		m_Target->clear(util::Vec4(0,0,0,1));

		util::Transform trans(util::Vec3(Screen::WINDOW_WIDTH_HALF, Screen::WINDOW_HEIGHT_HALF, 0), util::Vec3(0, 0, 0), util::Vec3(1, 1, 1));
		renderer.draw(&trans);

		util::getContext()->OMSetRenderTargets(0, NULL, NULL);
	}

	ID3D11ShaderResourceView * GradationFilter::getShaderView()
	{
		return m_Target->getShaderView().p;
	}

	void GradationFilter::draw()
	{
		util::Transform trans(util::Vec3(Screen::WINDOW_WIDTH_HALF, Screen::WINDOW_HEIGHT_HALF,0) , util::Vec3(0,0,0) ,util::Vec3(m_Scale, m_Scale, m_Scale));
		m_Target->draw(&trans);
	}

}