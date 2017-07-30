#include"DepthTarget.h"
#include<iostream>
#include<Source\Util\Win\WinFunc.h>
#include<assert.h>
#include<Source\Util\Render\DXFunc.h>


namespace framework {

	DepthTarget::DepthTarget(int width, int height, DXGI_FORMAT format) {
		createBuffer(width,height,format);
		auto viewFormat = createView(format);
		createShaderResourceView(viewFormat);
	}

	DepthTarget::DepthTarget(int width, int height, DXGI_FORMAT format, int sampleCount)
	{
		createBuffer(width, height, format, sampleCount);
		auto viewFormat = createView(format, sampleCount);
	}

	DepthTarget::~DepthTarget() {
	}

	ID3D11DepthStencilView * DepthTarget::getView()
	{
		return m_pView.p;
	}

	void DepthTarget::createBuffer(int width, int height, DXGI_FORMAT format, int sampleCount)
	{
		DXGI_SAMPLE_DESC sampleDesc;
		if (sampleCount == 1) {
			sampleDesc.Count = sampleCount;
			sampleDesc.Quality = 0;
		}
		else {
			sampleDesc.Count = sampleCount;
			UINT quality;
			util::getDevice()->CheckMultisampleQualityLevels(format, sampleCount, &quality);
			sampleDesc.Quality = quality - 1;
		}

		D3D11_TEXTURE2D_DESC depthDesc;
		ZeroMemory(&depthDesc, sizeof(depthDesc));
		depthDesc.Width = width;
		depthDesc.Height = height;
		depthDesc.MipLevels = 1;
		depthDesc.ArraySize = 1;
		depthDesc.Format = format;
		depthDesc.SampleDesc = sampleDesc;
		depthDesc.Usage = D3D11_USAGE_DEFAULT;
		depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		depthDesc.MiscFlags = 0;

		ID3D11Texture2D* result;
		HRESULT hr = DirectXInstance::getInstance()->getDevice()->CreateTexture2D(&depthDesc, NULL, &result);

		if (FAILED(hr)) {
			assert(false && "デプスバッファ作成失敗");
		}

		m_pBuffer.Attach(result);
	}

	DXGI_FORMAT DepthTarget::createView(DXGI_FORMAT format,int sampleCount)
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
		if(sampleCount == 1)
			dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;//2Dテクスチャとして扱う
		else
			dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;//マルチサンプル2Dテクスチャとして扱う

		ID3D11DepthStencilView *result;
		HRESULT hr = DirectXInstance::getInstance()->getDevice()->CreateDepthStencilView(m_pBuffer.p, &dsvDesc, &result);

		if (FAILED(hr)) {
			assert(false && "デプスバッファビュー作成失敗");
		}

		m_pView.Attach(result);

		return dsvDesc.Format;
	}

	void DepthTarget::createShaderResourceView(DXGI_FORMAT format)
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
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D; //2Dテクスチャとして扱う
		srvDesc.Texture2D.MipLevels = 1;

		ID3D11ShaderResourceView *result;
		HRESULT hr = DirectXInstance::getInstance()->getDevice()->CreateShaderResourceView(m_pBuffer.p, &srvDesc, &result);

		if (FAILED(hr)) {
			assert(false && "シェーダーリソースビュー作成失敗");
		}

		m_pShaderView.Attach(result);
	}

	void DepthTarget::clear()
	{
		framework::DirectXInstance::getInstance()->getContext()->ClearDepthStencilView(getView(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

}