#pragma once
#include<Source\Util\Render\DXFunc.h>
#include<Source\Util\Type.h>

namespace framework {

	class CubeTextureArray
	{
	public:
		CubeTextureArray(int width,int height,int arrayNum, DXGI_FORMAT format);
		~CubeTextureArray();

	private:
		void createTexture2D();
		void createRenderTargetView();
		void createShaderResourceView();

	private:
		CComPtr<ID3D11Texture2D> m_pTexture2D;
		DXGI_FORMAT m_Format;
		util::Vec2 m_Size;
		const int m_ArrayNum;
	};



}