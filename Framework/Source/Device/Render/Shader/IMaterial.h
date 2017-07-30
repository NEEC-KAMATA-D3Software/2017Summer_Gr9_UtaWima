#pragma once

#include<D3DX11.h>
#include"../../../Util/Render/DXFunc.h"
#include<Source\Device\Render\Shader\ShaderData.h>

/**
* @file	IMaterial.h
* @brief	マテリアルのインターフェース
* @authro	高須優輝
* @date	2017/03/7
*/

namespace framework {

	class Texture2D;

	struct MaterialParam {
		MaterialParam()
			:specular(0),
			height(0)
		{
		}

		float specular;
		float height;
		//!ライティングするかどうか
		bool isNotLighting;
	};

	class IMaterial
	{
	public:
		IMaterial(ShaderData<ShaderType::Vertex> vertexShaderData)
			:m_pVertexShader(vertexShaderData)
		{};
		~IMaterial() {};

		/**
		* @brief		シェーダを有効にする
		*/
		virtual void active() = 0;

		/**
		* @brief		シェーダを無効にする
		*/
		virtual void deActive() = 0;

		/**
		* @brief				定数をセットする
		* @param buffer			ID3D11Buffer配列のアドレス
		* @param bufferNum		bufferの配列の要素数
		*/
		virtual void setConstantBuffer(ID3D11Buffer*const* buffer,UINT bufferNum) = 0;

		/**
		* @brief				テクスチャをセットする
		* @param baindNum		何番目からセットするか
		* @param texNum			何個テクスチャをセットするか
		* @param texture		セットするテクスチャ
		*/
		virtual void setTexture(int startSlot,int texNum, ID3D11ShaderResourceView* const* texture);

		/**
		* @brief				サンプラをセットする
		* @param baindNum		何番目からセットするか
		* @param samplerNum		何個サンプラをセットするか
		* @param sampler		セットするステートオブジェクト
		*/
		virtual void setSamplerState(int startSlot, int samplerNum, ID3D11SamplerState*const* sampler);

		/**
		* @brief				頂点シェーダのバイナリ取得
		*/
		ID3DBlob* getVertexByte();

	protected:
		ShaderData<ShaderType::Vertex> m_pVertexShader;
	};
}