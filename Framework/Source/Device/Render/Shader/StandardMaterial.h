#pragma once
#include"IMaterial.h"
#include<atlbase.h>

namespace framework {

	class StandardMaterial : public IMaterial
	{
	public:
		StandardMaterial(ShaderData<ShaderType::Vertex> vs, CComPtr<ID3D11PixelShader> ps);
		~StandardMaterial();

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
		CComPtr<ID3D11PixelShader> m_pPixelShader;
	};
}
