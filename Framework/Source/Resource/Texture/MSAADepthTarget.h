#pragma once
#include"DepthTarget.h"

namespace framework {

	class MSAADepthTarget : public DepthTarget
	{
	public:
		MSAADepthTarget(int width, int height, DXGI_FORMAT format, int sampleCount);
		~MSAADepthTarget();

		/**
		* @brief		元リソースを返す
		* @return		ID3D11Texture2D*
		*/
		ID3D11Texture2D* getBuffer()override;

		/**
		* @brief		マルチサンプルをTexture2Dに解決
		*/
		void resolveTexture();

	private:
		void createResolveTexture2D(int width, int height, DXGI_FORMAT format);

	private:
		//マルチサンプル用に変換後のテクスチャ２D
		CComPtr<ID3D11Texture2D> m_pResolveTexture;
		DXGI_FORMAT m_Format;
	};



}