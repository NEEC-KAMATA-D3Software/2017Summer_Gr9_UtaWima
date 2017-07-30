#include"BoneTexture.h"
#include<Source\Util\WrapFunc.h>
#include<Source\Resource\ResourceManager.h>

namespace framework {



	BoneTexture::BoneTexture(int boneNum, int frameNum)
		:m_Width( boneNum * 4),
		m_Height(frameNum * 4)
	{
		createTexture2D(m_Width, m_Height);
		createShaderResourceView();
	}

	BoneTexture::~BoneTexture()
	{
	}

	void BoneTexture::copyTexture(BoneTexture * other, int insertSlot, int sourceSloat)
	{
		auto tex = other->getTexture2D();
		util::getContext()->CopySubresourceRegion(m_pTexture2D.p, D3D11CalcSubresource(0, insertSlot, 1),0,0,0,tex, D3D11CalcSubresource(0, sourceSloat, 1),NULL);	
		//util::getContext()->CopyResource(m_pTexture2D.p, tex);
	}


	void BoneTexture::begin()
	{
		assert(!m_IsWriting && "一度beginが呼ばれたあとendが飛ばれていません");
		m_IsWriting = false;
		
		auto hr = util::getContext()->Map(m_pTexture2D.p, NULL, D3D11_MAP_WRITE_DISCARD, 0, &m_MappedResource);
		assert(!FAILED(hr) && "テクスチャをマップ失敗");
	}

	void BoneTexture::end()
	{
		m_IsWriting = false;
		util::getContext()->Unmap(m_pTexture2D.p, NULL);
	}

	void BoneTexture::begonToCopy(D3D11_MAPPED_SUBRESOURCE * result)
	{
		const int pixSize = m_MappedResource.RowPitch / m_Width;
		std::unique_ptr<float*[]> pix = std::make_unique<float*[]>(sizeof(float) * m_Height);
		float* dataPointer = (float*)m_MappedResource.pData;

		util::foreach(m_Height, [&](int i) {
			std::memcpy(pix[i], dataPointer, pixSize * m_Width);
			dataPointer += pixSize * m_Width;
		});	
	
	}

	void BoneTexture::begonToSet(D3D11_MAPPED_SUBRESOURCE * data)
	{
		m_MappedResource.pData = data->pData;
	}

	void BoneTexture::beginToBuffer(int boneSloat, int frameSloat, std::function<void(float* r, float* g, float* b, float* a)> action)
	{
		writeSubResource(&m_MappedResource, boneSloat, frameSloat, action);
	}


	ID3D11ShaderResourceView * BoneTexture::getShaderView()
	{
		return m_pShaderView.p;
	}

	util::Vec2 BoneTexture::getSize()
	{
		return util::Vec2(m_Width, m_Height);
	}

	int BoneTexture::getPixBufferSize()
	{
		return m_Width * m_Height;
	}

	void BoneTexture::createTexture2D(int boneNum, int frameNum)
	{
		DXGI_SAMPLE_DESC sampleDesc;
		sampleDesc.Count = 1;
		sampleDesc.Quality = 0;

		D3D11_TEXTURE2D_DESC colorDesc;
		ZeroMemory(&colorDesc, sizeof(colorDesc));
		colorDesc.Width = boneNum;
		colorDesc.Height = frameNum;
		colorDesc.MipLevels = 1;
		colorDesc.ArraySize = 1;
		colorDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;
		colorDesc.SampleDesc = sampleDesc;
		colorDesc.Usage = D3D11_USAGE_DYNAMIC;
		colorDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		colorDesc.MiscFlags = 0;
		colorDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;//CPUアクセス不可にする


		ID3D11Texture2D* result;
		HRESULT hr = DirectXInstance::getInstance()->getDevice()->CreateTexture2D(&colorDesc, NULL, &result);

		if (FAILED(hr)) {
			assert(false && "カラーバッファ作成失敗");
		}
		m_pTexture2D.Attach(result);

		m_Width = colorDesc.Width;
		m_Height = colorDesc.Height;
	}

	void BoneTexture::createShaderResourceView()
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(srvDesc));
		srvDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D; //2Dテクスチャとして扱う
		srvDesc.Texture2D.MipLevels = 1;
		srvDesc.Texture2D.MostDetailedMip = 0;


		ID3D11ShaderResourceView *result;
		HRESULT hr = DirectXInstance::getInstance()->getDevice()->CreateShaderResourceView(m_pTexture2D.p, &srvDesc, &result);

		if (FAILED(hr)) {
			assert(false && "シェーダーリソースビュー作成失敗");
		}

		m_pShaderView.Attach(result);
	}

	void BoneTexture::writeSubResource(D3D11_MAPPED_SUBRESOURCE * pMapData, int x, int y, std::function<void(float*r, float*g, float*b, float*a)> action)
	{
		float* pTexel = (float*)pMapData->pData;
		const int yShift = pMapData->RowPitch / sizeof(float);
		//y分ポインタを一つずらす
		pTexel += yShift * y;

		const int xShift = x * 4;

		action(&pTexel[xShift + 0], &pTexel[xShift + 1], &pTexel[xShift + 2], &pTexel[xShift + 3]);
	}

	void BoneTexture::eachPix(std::function<void(int, int)> action)
	{
		util::foreach(m_Height, [&](int y) {
			util::foreach(m_Width / 4, [&](int x) {
				action(x * 4, y);
			});
		});
	}
	ID3D11Texture2D * BoneTexture::getTexture2D()
	{
		return m_pTexture2D.p;
	}

}