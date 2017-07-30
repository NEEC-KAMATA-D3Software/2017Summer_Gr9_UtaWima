#pragma once

/**
* @file	DepthBuffer.h
* @brief	深度ステンシルの描画先クラス　マルチサンプルには対応しない
* @authro	高須優輝
* @date	2017/02/22
*/

#include<Source\Device\Render\DirectXInstance.h>
#include"ITarget.h"
#include<Source\Util\Type.h>

namespace framework {


	class DepthTarget : public ITarget
	{
	public:
		/**
		* @brief				デプスバッファを作成する
		* @param width			バッファの横サイズ
		* @param height			バッファの縦サイズ
		* @param format			バッファのフォーマット　
		* @dital				フォーマットはTYPELESS系に限る
		*/
		DepthTarget(int width, int height, DXGI_FORMAT format);

		~DepthTarget();

		/**
		* @brief	  デプスビューの設定用変数を返す
		* @return	　ID3D11RenderTargetView*
		*/
		ID3D11DepthStencilView *  getView();

		/**
		* @brief				レンダーターゲットをクリアする
		*/
		void clear();

	protected:
		/**
		* @brief				デプスバッファを作成する
		* @param width			バッファの横サイズ
		* @param height			バッファの縦サイズ
		* @param format			バッファのフォーマット　
		* @param sampleCount	マルチサンプルの数
		* @dital				フォーマットはTYPELESS系に限る
		*/
		DepthTarget(int width, int height, DXGI_FORMAT format,int sampleCount);

	private:
		void createBuffer(int width, int height, DXGI_FORMAT format, int sampleCount = 1) override;
		DXGI_FORMAT createView(DXGI_FORMAT format, int sampleCount = 1);
		void createShaderResourceView(DXGI_FORMAT format)override;

	protected:
		//深度描画先に設定するための変数
		CComPtr<ID3D11DepthStencilView> m_pView;
	};

}