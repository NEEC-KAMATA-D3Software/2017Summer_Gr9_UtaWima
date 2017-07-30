
#include"RenderTarget.h"
#include<iostream>
#include<Source\Util\Win\WinFunc.h>
#include<assert.h>
#include<Source\Application\Screen\Screen.h>
#include<Source\Util\Render\DXFunc.h>
#include<Source\Device\Render\Renderer\2D\Single\SpriteRenderer.h>

namespace framework {

	RenderTarget::RenderTarget(int width, int height, DXGI_FORMAT format)
		:m_Size(width, height)
	{
		createBuffer(width, height, format);
		createView(format);
		createShaderResourceView(format);

		createSpriteRenderer(width, height);
	}

	RenderTarget::RenderTarget(CComPtr<ID3D11Texture2D> resource, DXGI_FORMAT format)
		:m_Size(Screen::PIXEL_WIDTH, Screen::PIXEL_HEIGHT)//バックバッファの場合はスクリーンサイズと一緒
	{
		m_pBuffer = resource;
		createView(format);

		D3D11_TEXTURE2D_DESC desc;
		resource->GetDesc(&desc);
		//バックバッファーはシェーダーリソースとして扱えない
	}

	RenderTarget::RenderTarget(int width, int height, DXGI_FORMAT format, int sampleCount)
		:m_Size(width, height)
	{
		createBuffer(width, height, format, sampleCount);
		createView(format, sampleCount);
	}

	RenderTarget::~RenderTarget()
	{
	}

	ID3D11RenderTargetView * const * RenderTarget::getView()
	{
		return &m_pView.p;
	}

	void RenderTarget::clear(const util::Vec4& clearColor)
	{
		float _clearColor[4] = { clearColor.x,clearColor.y ,clearColor.z ,clearColor.w };
		framework::DirectXInstance::getInstance()->getContext()->ClearRenderTargetView(*(getView()), _clearColor);
	}

	util::Vec2 RenderTarget::getSize()
	{
		return m_Size;
	}


	void RenderTarget::createSpriteRenderer(int width, int height)
	{
		m_pRenderer = std::make_unique<SpriteRenderer>();
		m_pRenderer->setSize(util::Vec2(Screen::WINDOW_WIDTH, Screen::WINDOW_HEIGHT));
		m_pRenderer->setTexture(std::make_shared<Texture2D>(width, height, m_pShaderView));
	}

	void RenderTarget::createBuffer(int width, int height, DXGI_FORMAT format, int sampleCount)
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

		D3D11_TEXTURE2D_DESC colorDesc;
		ZeroMemory(&colorDesc, sizeof(colorDesc));
		colorDesc.Width = width;
		colorDesc.Height = height;
		colorDesc.MipLevels = 1;
		colorDesc.ArraySize = 1;
		colorDesc.Format = format;
		colorDesc.SampleDesc = sampleDesc;
		colorDesc.Usage = D3D11_USAGE_DEFAULT;
		colorDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		colorDesc.MiscFlags = 0;

		ID3D11Texture2D* result;
		HRESULT hr = DirectXInstance::getInstance()->getDevice()->CreateTexture2D(&colorDesc, NULL, &result);

		if (FAILED(hr)) {
			assert(false && "カラーバッファ作成失敗");
		}

		m_pBuffer.Attach(result);
	}

	void RenderTarget::createView(DXGI_FORMAT format, int sampleCount)
	{

		//D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
		//ZeroMemory(&rtvDesc, sizeof(rtvDesc));
		//rtvDesc.Format = format;
		//if(sampleCount == 1)
		//	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;//2Dテクスチャとして扱う
		//else
		//	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;//マルチサンプル2Dテクスチャとして扱う
		ID3D11RenderTargetView *result;
		HRESULT hr = DirectXInstance::getInstance()->getDevice()->CreateRenderTargetView(m_pBuffer.p, NULL, &result);

		if (FAILED(hr)) {
			assert(false && "カラーバッファビュー作成失敗");
		}

		m_pView.Attach(result);
	}

	void RenderTarget::createShaderResourceView(DXGI_FORMAT format)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(srvDesc));
		srvDesc.Format = format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D; //2Dテクスチャとして扱う
		srvDesc.Texture2D.MipLevels = 1;
		srvDesc.Texture2D.MostDetailedMip = 0;


		ID3D11ShaderResourceView *result;
		HRESULT hr = DirectXInstance::getInstance()->getDevice()->CreateShaderResourceView(m_pBuffer.p, &srvDesc, &result);

		if (FAILED(hr)) {
			assert(false && "シェーダーリソースビュー作成失敗");
		}

		m_pShaderView.Attach(result);
	}

	void RenderTarget::draw(const util::Vec2 & drawPos)
	{
		util::Transform drawTrans(util::Vec3(drawPos.x, drawPos.y, 0), util::Vec3(0, 0, 0), util::Vec3(1, 1, 1));
		m_pRenderer->draw(&drawTrans);
	}

	void RenderTarget::draw(util::Transform* trans)
	{
		m_pRenderer->draw(trans);
	}

	void RenderTarget::copy(const RenderTarget & other)
	{
		util::getContext()->CopyResource(m_pBuffer.p,other.m_pBuffer.p);
	}

}