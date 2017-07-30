#pragma once
#include"RenderTarget.h"
#include<Source\Util\Type.h>
#include<memory>

namespace framework {

	class MSAARenderTarget : public RenderTarget
	{
	public:
		MSAARenderTarget(int width, int height, DXGI_FORMAT format,int sampleCout);
		~MSAARenderTarget();


		/**
		* @brief		元リソースを返す
		* @return		ID3D11Texture2D*
		*/
		ID3D11Texture2D* getBuffer()override;

		void resolveTexture();

	private:
		void createResolveTexture2D(int width, int height, DXGI_FORMAT format);

	private:
		//マルチサンプル用に変換後のテクスチャ２D
		CComPtr<ID3D11Texture2D> m_pResolveTexture;
		DXGI_FORMAT m_Format;
	};



}