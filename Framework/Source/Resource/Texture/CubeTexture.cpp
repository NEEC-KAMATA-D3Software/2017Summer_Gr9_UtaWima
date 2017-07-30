#include "CubeTexture.h"
#include<D3DX11.h>
#include<Source\Util\Render\DXFunc.h>
#include<assert.h>
#include<Source\Util\Type.h>
#include<Source\Util\WrapFunc.h>

framework::CubeTexture::CubeTexture(const std::string texNames[6])
{
	//0番目のテクスチャの情報でTexture2D作成
	createTexture2D(texNames[0]);
	D3D11_TEXTURE2D_DESC desc;
	m_pTexture2D->GetDesc(&desc);

	createUpdateSubResource(texNames, desc);
	createShaderResourceView(desc);
}

framework::CubeTexture::~CubeTexture()
{
}

CComPtr<ID3D11ShaderResourceView> framework::CubeTexture::getShaderResourceView()
{
	return m_pShaderResourceView.p;
}

void framework::CubeTexture::createTexture2D(const std::string &texName)
{
	auto pTexture2D = util::loadTexture2D(texName);

	D3D11_TEXTURE2D_DESC desc;
	pTexture2D->GetDesc(&desc);//情報取り出し
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.ArraySize = 6;//キューブテクスチャなので6
	desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	ID3D11Texture2D* pCreateTexture2D;
	auto hr = util::getDevice()->CreateTexture2D(&desc,NULL,&pCreateTexture2D);
	assert(!FAILED(hr) && "Texture2D作成失敗");

	//メンバへ保存
	m_pTexture2D.Attach(pCreateTexture2D);
}

void framework::CubeTexture::createUpdateSubResource(const std::string texNames[6], const D3D11_TEXTURE2D_DESC& desc)
{
	//キューブテクスチャの毎数分回る
	util::foreach(6, [&](int i) {
		auto pTexture2D = util::loadTexture2D(texNames[i]);

		//auto pTexture2D = testCreate(texNames[i]);

		//テクスチャを更新
		util::writeArrayTexutre(m_pTexture2D.p,i, pTexture2D.p, D3D11_MAP::D3D11_MAP_READ);
	});
}

void framework::CubeTexture::createShaderResourceView(const D3D11_TEXTURE2D_DESC& desc)
{
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	ZeroMemory(&SRVDesc, sizeof(SRVDesc));
	SRVDesc.Format = desc.Format;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;//キューブテクスチャ
	SRVDesc.TextureCube.MipLevels = 1;
	SRVDesc.TextureCube.MostDetailedMip = 0;

	// シェーダーリソースビューを作成する
	ID3D11ShaderResourceView* pShaderResourceView;
	auto hr = util::getDevice()->CreateShaderResourceView(m_pTexture2D.p, &SRVDesc, &pShaderResourceView);
	assert(!FAILED(hr) && "ShaderResourceView作成失敗");

	m_pShaderResourceView.Attach(pShaderResourceView);
}

CComPtr<ID3D11Texture2D> framework::CubeTexture::testCreate(std::string texName)
{

	auto pTexture2D = util::loadTexture2D(texName);
	D3D11_TEXTURE2D_DESC desc;
	pTexture2D->GetDesc(&desc);//情報取り出し


	D3D11_SUBRESOURCE_DATA subData;
	ZeroMemory(&subData,sizeof(subData));

	std::unique_ptr<char[]> data = std::make_unique<char[]>(desc.Width * desc.Height);

	//util::foreach(desc.Height, [&](int y) {
	//	util::foreach(desc.Width, [&](int x) {
	//		UINT colorStart = x * 4;
	//		data[]
	//	});
	//});


	subData.pSysMem = data.get();
	subData.SysMemPitch = desc.Width;

	ID3D11Texture2D* result;
	util::getDevice()->CreateTexture2D(&desc, &subData, &result);
	CComPtr<ID3D11Texture2D> tex;
	tex.Attach(result);
	return tex;
}

void framework::CubeTexture::testWrite(D3D11_MAPPED_SUBRESOURCE * pMapData, int x, int y, std::function<void(float*r, float*g, float*b, float*a)> action)
{
	float* pTexel = (float*)pMapData->pData;
	const int yShift = pMapData->RowPitch / sizeof(float);
	//y分ポインタを一つずらす
	pTexel += yShift * y;

	const int xShift = x * 4;

	action(&pTexel[xShift + 0], &pTexel[xShift + 1], &pTexel[xShift + 2], &pTexel[xShift + 3]);

}
