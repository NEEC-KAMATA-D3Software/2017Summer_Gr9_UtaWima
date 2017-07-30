#include"MSAADepthTarget.h"
#include<Source\Util\Render\DXFunc.h>

namespace framework {

	MSAADepthTarget::MSAADepthTarget(int width, int height, DXGI_FORMAT format, int sampleCount)
		:DepthTarget(width,height,format,sampleCount),
		m_Format(format)
	{

		createResolveTexture2D(width, height, format);
	}

	MSAADepthTarget::~MSAADepthTarget()
	{
	}
	ID3D11Texture2D * MSAADepthTarget::getBuffer()
	{
		return 	m_pResolveTexture.p;
	}
	void MSAADepthTarget::resolveTexture()
	{
		util::getContext()->ResolveSubresource(
			m_pResolveTexture.p, 0, m_pBuffer.p, 0,
			m_Format);
	}

	void MSAADepthTarget::createResolveTexture2D(int width, int height, DXGI_FORMAT format)
	{
		//深度バッファように変換

		D3D11_TEXTURE2D_DESC descTex;
		ZeroMemory(&descTex, sizeof(descTex));
		descTex.Width = width;
		descTex.Height = height;
		descTex.MipLevels = 1;
		descTex.ArraySize = 1;
		descTex.Format = format;
		descTex.SampleDesc.Count = 1;
		descTex.SampleDesc.Quality = 0;
		descTex.Usage = D3D11_USAGE_DEFAULT;
		descTex.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		descTex.MiscFlags = 0;

		ID3D11Texture2D* pResolveTexture = NULL;
		auto hr = util::getDevice()->CreateTexture2D(&descTex, NULL, &pResolveTexture);
		if (FAILED(hr)) {
			assert(false && "Texture2D作成失敗");
		}

		DXGI_FORMAT viewFormat;
		switch (format)
		{
		case DXGI_FORMAT_R16_TYPELESS:
			viewFormat = DXGI_FORMAT_D16_UNORM;
			break;
		case DXGI_FORMAT_R32_TYPELESS:
			viewFormat = DXGI_FORMAT_D32_FLOAT;
			break;
		case DXGI_FORMAT_R24G8_TYPELESS:
			viewFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
			break;
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(srvDesc));
		switch (viewFormat)
		{
		case DXGI_FORMAT_R8_UNORM:
			srvDesc.Format = DXGI_FORMAT_R8_UNORM;
			break;
		case DXGI_FORMAT_D16_UNORM:
			srvDesc.Format = DXGI_FORMAT_R16_UNORM;
			break;
		case DXGI_FORMAT_D32_FLOAT:
			srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
			break;
		case DXGI_FORMAT_D24_UNORM_S8_UINT:
			srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
			break;
		}
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D; //2Dテクスチャとして扱う
		srvDesc.Texture2D.MipLevels = 1;

		ID3D11ShaderResourceView* pRenderTextureSRV = NULL;
		hr = util::getDevice()->CreateShaderResourceView(pResolveTexture, &srvDesc, &pRenderTextureSRV);
		if (FAILED(hr)) {
			assert(false && "ShaderResourceView作成失敗");
		}

		m_pResolveTexture.Attach(pResolveTexture);
		m_pShaderView.Attach(pRenderTextureSRV);
	}
}