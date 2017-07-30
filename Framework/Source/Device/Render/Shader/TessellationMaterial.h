#pragma once
#include"StandardMaterial.h"
#include<D3DX11.h>


namespace framework {

	class TessellationMaterial : public StandardMaterial
	{
	public:
		TessellationMaterial(ShaderData<ShaderType::Vertex> vs, CComPtr<ID3D11PixelShader> ps, CComPtr<ID3D11HullShader> hs, CComPtr<ID3D11DomainShader> ds);
		~TessellationMaterial();


		/**
		* @brief				シェーダーをパイプラインにセットして有効にする
		*/
		virtual void active() override;

		/**
		* @brief				シェーダーを無効にする
		*/
		virtual void deActive() override;

		/**
		* @brief				定数をセットする
		* @param buffer			ID3D11Buffer配列のアドレス
		* @param bufferNum		bufferの配列の要素数
		*/
		virtual void setConstantBuffer(ID3D11Buffer *const* buffer, UINT bufferNum) override;

	private:
		CComPtr<ID3D11HullShader> m_pHullShader;
		CComPtr<ID3D11DomainShader> m_pDomainShader;
	};


}