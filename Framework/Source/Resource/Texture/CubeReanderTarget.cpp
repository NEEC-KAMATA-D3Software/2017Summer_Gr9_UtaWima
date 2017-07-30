#include"CubeReanderTarget.h"
#include<assert.h>

namespace framework {
	CubeRenderTrget::CubeRenderTrget(int width, int height, DXGI_FORMAT format)
		:m_Size(width,height)
	{
		createBuffer(width, height, format);
		createView(format);
		createShaderResourceView(format);
	}
	CubeRenderTrget::~CubeRenderTrget()
	{
	}
	ID3D11RenderTargetView * const * CubeRenderTrget::getView()
	{
		return &m_pView.p;
	}
	void CubeRenderTrget::clear(const util::Vec4 & clearColor)
	{
		float _clearColor[4] = { clearColor.x,clearColor.y ,clearColor.z ,clearColor.w };
		framework::DirectXInstance::getInstance()->getContext()->ClearRenderTargetView(*(getView()), _clearColor);

	}
	const util::Vec2 & CubeRenderTrget::getSize()
	{
		return m_Size;
	}
	void CubeRenderTrget::createBuffer(int width, int height, DXGI_FORMAT format, int sampleCount)
	{
		D3D11_TEXTURE2D_DESC colorDesc;
		ZeroMemory(&colorDesc, sizeof(colorDesc));
		colorDesc.Width = width;
		colorDesc.Height = height;
		colorDesc.MipLevels = 1;
		colorDesc.ArraySize = 60;
		colorDesc.Format = format;
		colorDesc.SampleDesc.Count = sampleCount;
		colorDesc.SampleDesc.Quality = 0;
		colorDesc.Usage = D3D11_USAGE_DEFAULT;
		colorDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		colorDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS | D3D11_RESOURCE_MISC_TEXTURECUBE;//キューブマップ

		ID3D11Texture2D* result;
		HRESULT hr = DirectXInstance::getInstance()->getDevice()->CreateTexture2D(&colorDesc, NULL, &result);

		if (FAILED(hr)) {
			assert(false && "カラーバッファ作成失敗");
		}

		m_pBuffer.Attach(result);
	}
	void CubeRenderTrget::createShaderResourceView(DXGI_FORMAT format)
	{

		//TODO キューブアレイができなければTexture2DArrayに戻す
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(srvDesc));
		srvDesc.Format = format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBEARRAY; //キューブテクスチャとして扱う
		srvDesc.TextureCubeArray.MipLevels = 1;
		srvDesc.TextureCubeArray.MostDetailedMip = 0;
		srvDesc.TextureCubeArray.NumCubes = 10;
		srvDesc.TextureCubeArray.First2DArrayFace = 0;

		ID3D11ShaderResourceView *result;
		HRESULT hr = DirectXInstance::getInstance()->getDevice()->CreateShaderResourceView(m_pBuffer.p, &srvDesc, &result);

		if (FAILED(hr)) {
			assert(false && "シェーダーリソースビュー作成失敗");
		}

		m_pShaderView.Attach(result);
	}
	void CubeRenderTrget::createView(DXGI_FORMAT format)
	{
		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
		ZeroMemory(&rtvDesc, sizeof(rtvDesc));
		rtvDesc.Format = format;
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;//2Dテクスチャ配列として扱う
		rtvDesc.Texture2DArray.ArraySize = 60;
		rtvDesc.Texture2DArray.FirstArraySlice = 0;
		rtvDesc.Texture2DArray.MipSlice = 0;

		ID3D11RenderTargetView *result;
		HRESULT hr = DirectXInstance::getInstance()->getDevice()->CreateRenderTargetView(m_pBuffer.p, &rtvDesc, &result);

		if (FAILED(hr)) {
			assert(false && "カラーバッファビュー作成失敗");
		}

		m_pView.Attach(result);
	}
}