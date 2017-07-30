#include"MSAARenderTarget.h"
#include<Source\Util\Render\DXFunc.h>

namespace framework {

	MSAARenderTarget::MSAARenderTarget(int width, int height, DXGI_FORMAT format, int sampleCout)
		:RenderTarget(width, height, format, sampleCout)
		, m_Format(format)
	{
		createResolveTexture2D(width, height,format);

		//RenderTargetのコンストラクタではshaderViewが作成されてないのでここでスプライトレンダラを作る必要がある
		createSpriteRenderer(width, height);
	}

	MSAARenderTarget::~MSAARenderTarget()
	{
	}

	ID3D11Texture2D * MSAARenderTarget::getBuffer()
	{
		return m_pResolveTexture.p;
	}

	void MSAARenderTarget::resolveTexture()
	{
		util::getContext()->ResolveSubresource(
			m_pResolveTexture.p, 0, m_pBuffer.p, 0,
			m_Format);
	}

	void MSAARenderTarget::createResolveTexture2D(int width, int height, DXGI_FORMAT format)
	{

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
		ID3D11ShaderResourceView* pRenderTextureSRV = NULL;
		auto hr = util::getDevice()->CreateTexture2D(&descTex, NULL, &pResolveTexture);
		if (FAILED(hr)) {
			assert(false && "Texture2D作成失敗");
		}

		hr = util::getDevice()->CreateShaderResourceView(pResolveTexture, NULL, &pRenderTextureSRV);
		if (FAILED(hr)) {
			assert(false && "ShaderResourceView作成失敗");
		}

		m_pResolveTexture.Attach(pResolveTexture);
		m_pShaderView.Attach(pRenderTextureSRV);
	}


}