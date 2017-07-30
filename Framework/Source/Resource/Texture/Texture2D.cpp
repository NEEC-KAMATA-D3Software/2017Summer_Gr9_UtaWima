#include"Texture2D.h"
#include<Source\Util\Render\DXFunc.h>
#include<Source\Util\Win\WinFunc.h>


namespace framework {

	Texture2D::Texture2D(const std::string & filePath)
	{
		D3DX11_IMAGE_INFO info;
		HRESULT hr;
		D3DX11GetImageInfoFromFile(filePath.c_str(),NULL, &info, &hr);
		if (FAILED(hr)) {
			std::string error = "ì«Ç›çûÇ›é∏îs : "+ filePath;
			util::WinFunc::OutLog(error);
			assert(false && "ì«Ç›çûÇ›é∏îs");
		}

		m_Size.x = info.Width;
		m_Size.y = info.Height;

		ID3D11ShaderResourceView* resourceView;
		D3DX11CreateShaderResourceViewFromFile(util::getDevice(), filePath.c_str(), NULL, NULL, &resourceView, &hr);
		
		if (FAILED(hr)) {
			std::string error = "ì«Ç›çûÇ›é∏îs : " + filePath;
			util::WinFunc::OutLog(error);
			assert(false && error.c_str());
		}

		m_ShaderView.Attach(resourceView);

		createRenderer();
	}

	Texture2D::Texture2D(int width, int height, CComPtr<ID3D11ShaderResourceView> resourceView)
	{
		m_ShaderView = resourceView;
		m_Size.x = width;
		m_Size.y = height;

		createRenderer();
	}


	Texture2D::~Texture2D()
	{
	}

	ID3D11ShaderResourceView *const* Texture2D::getReource()
	{
		return &m_ShaderView.p;
	}

	const util::Vec2& Texture2D::getSize()
	{
		return m_Size;
	}

	void Texture2D::draw(const util::Vec2 & pos)
	{
		util::Transform trans(util::Vec3(pos.x,pos.y,0),util::Vec3(0,0,0),util::Vec3(1,1,1));
		draw(&trans);
	}

	void Texture2D::draw(util::Transform * trans)
	{
		//m_pRenderer->draw(trans);
	}

	void Texture2D::createRenderer()
	{

	}

}