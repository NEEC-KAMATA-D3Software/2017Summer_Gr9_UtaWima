#include"CubeTextureArray.h"

/**
* @file	CubeTextureArray.h
* @brief	キューブテクスチャ配列オブジェクトクラス
* @authro	高須優輝
* @date	2017/04/15
*/


namespace framework {

	CubeTextureArray::CubeTextureArray(int width, int height, int arrayNum, DXGI_FORMAT format)
		:m_Size(width,height),
		m_ArrayNum(arrayNum),
		m_Format(format)
	{
		
	}

	CubeTextureArray::~CubeTextureArray()
	{
	}
	void CubeTextureArray::createTexture2D()
	{
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
		desc.Width = m_Size.x;
		desc.Height = m_Size.y;
		desc.MipLevels = 1;
		desc.ArraySize = m_ArrayNum;
		desc.Format = m_Format;
		desc.SampleDesc.Count = 1;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;//描画ターゲットとシェーダリソースで使用可能

		ID3D11Texture2D* temp;
		auto hr = util::getDevice()->CreateTexture2D(&desc, NULL, &temp);
		assert(!FAILED(hr) && "Texture2D作成失敗");

		m_pTexture2D.Attach(temp);
	}
	void CubeTextureArray::createRenderTargetView()
	{
		D3D11_RENDER_TARGET_VIEW_DESC desc;
		ZeroMemory(&desc,sizeof(desc));
		desc.Format = m_Format;
		//desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	}
	void CubeTextureArray::createShaderResourceView()
	{
	}
}