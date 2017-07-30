#pragma once

/**
* @file	ITarget.h
* @brief	描画先抽象クラス
* @authro	高須優輝
* @date	2017/02/23
*/

#include<Source\Device\Render\DirectXInstance.h>
#include<memory>

namespace framework {

	class ITarget
	{
	public:
		ITarget() {

		}

		~ITarget() {

		}

		/**
		* @brief		元リソースを返す
		* @return		ID3D11Texture2D*
		*/
		virtual ID3D11Texture2D* getBuffer() {
			return m_pBuffer.p;
		}

		/**
		* @brief		シェーダーに渡すための変数
		* @return		ID3D11ShaderResourceView*
		*/
		virtual CComPtr<ID3D11ShaderResourceView> getShaderView() {
			return m_pShaderView;
		}

	protected:
		//各バッファを生成するメソッド
		virtual void createBuffer(int width, int height, DXGI_FORMAT format,int sampleCount = 1) = 0;
		virtual void createShaderResourceView(DXGI_FORMAT format) = 0;

	protected:

		//!テクスチャとして扱う変数
		CComPtr<ID3D11Texture2D> m_pBuffer;
		//!シェーダーアクセス用変数
		CComPtr<ID3D11ShaderResourceView> m_pShaderView;


	};

}