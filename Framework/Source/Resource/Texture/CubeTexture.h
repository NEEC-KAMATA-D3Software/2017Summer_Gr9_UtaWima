#pragma once
#include<string>
#include<vector>
#include<D3DX11.h>
#include<atlbase.h>
#include<functional>

namespace framework {

	class CubeTexture
	{
	public:
		CubeTexture(const std::string texNames[6]);
		~CubeTexture();

		CComPtr<ID3D11ShaderResourceView> getShaderResourceView();

	private:
		void createTexture2D(const std::string& texName);
		void createUpdateSubResource (const std::string texNames[6], const D3D11_TEXTURE2D_DESC& desc);
		void createShaderResourceView(const D3D11_TEXTURE2D_DESC& desc);

		CComPtr<ID3D11Texture2D> testCreate(std::string texName);

		void testWrite(D3D11_MAPPED_SUBRESOURCE* pMapData, int x, int y, std::function<void(float* r, float* g, float* b, float* a)> action);

	private:
		CComPtr<ID3D11Texture2D> m_pTexture2D;
		CComPtr<ID3D11ShaderResourceView> m_pShaderResourceView;
	};


}
