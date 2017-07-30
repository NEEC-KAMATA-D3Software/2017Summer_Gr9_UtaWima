#include"CubeDepthTarget.h"
#include<assert.h>

namespace framework {



	CubeDepthTarget::CubeDepthTarget(int width, int height, DXGI_FORMAT format)
	{
		createBuffer(width, height, format);
		auto viewFormat = createView(format);
		createShaderResourceView(viewFormat);

	}

	CubeDepthTarget::~CubeDepthTarget()
	{
	}

	ID3D11DepthStencilView *  CubeDepthTarget::getView()
	{
		return m_pView.p;
	}

	void CubeDepthTarget::clear()
	{
		framework::DirectXInstance::getInstance()->getContext()->ClearDepthStencilView(getView(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	void CubeDepthTarget::createBuffer(int width, int height, DXGI_FORMAT format, int sampleCount)
	{
		D3D11_TEXTURE2D_DESC depthDesc;
		ZeroMemory(&depthDesc, sizeof(depthDesc));
		depthDesc.Width = width;
		depthDesc.Height = height;
		depthDesc.MipLevels = 1;
		depthDesc.ArraySize = 6;
		depthDesc.Format = format;
		depthDesc.SampleDesc.Count = sampleCount;
		depthDesc.SampleDesc.Quality = 0;
		depthDesc.Usage = D3D11_USAGE_DEFAULT;
		depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		depthDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

		ID3D11Texture2D* result;
		HRESULT hr = DirectXInstance::getInstance()->getDevice()->CreateTexture2D(&depthDesc, NULL, &result);

		if (FAILED(hr)) {
			assert(false && "デプスバッファ作成失敗");
		}

		m_pBuffer.Attach(result);
	}

	void CubeDepthTarget::createShaderResourceView(DXGI_FORMAT format)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(srvDesc));
		switch (format)
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
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE; //2Dテクスチャとして扱う
		srvDesc.Texture2D.MipLevels = 1;
		srvDesc.Texture2D.MostDetailedMip = 0;

		ID3D11ShaderResourceView *result;
		HRESULT hr = DirectXInstance::getInstance()->getDevice()->CreateShaderResourceView(m_pBuffer.p, &srvDesc, &result);

		if (FAILED(hr)) {
			assert(false && "シェーダーリソースビュー作成失敗");
		}

		m_pShaderView.Attach(result);
	}

	DXGI_FORMAT CubeDepthTarget::createView(DXGI_FORMAT format)
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
		ZeroMemory(&dsvDesc, sizeof(dsvDesc));

		//TYPELESSに互換性のあるフォーマットに変換する
		switch (format)
		{
		case DXGI_FORMAT_R16_TYPELESS:
			dsvDesc.Format = DXGI_FORMAT_D16_UNORM;
			break;
		case DXGI_FORMAT_R32_TYPELESS:
			dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
			break;
		case DXGI_FORMAT_R24G8_TYPELESS:
			dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			break;
		}
		dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;//2Dテクスチャとして扱う
		dsvDesc.Texture2DArray.ArraySize = 6;
		dsvDesc.Texture2DArray.FirstArraySlice = 0;
		dsvDesc.Texture2DArray.MipSlice = 0;

		ID3D11DepthStencilView *result;
		HRESULT hr = DirectXInstance::getInstance()->getDevice()->CreateDepthStencilView(m_pBuffer.p, &dsvDesc, &result);

		if (FAILED(hr)) {
			assert(false && "デプスバッファビュー作成失敗");
		}

		m_pView.Attach(result);

		return dsvDesc.Format;
	}

}