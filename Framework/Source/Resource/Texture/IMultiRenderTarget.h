#pragma once


#pragma once
#include<D3DX11.h>
#include<vector>
#include<memory>
#include<Source\Util\Type.h>
#include<atlbase.h>
#include<Source\Util\Type.h>

namespace framework {

	class RenderTarget;

	class IMultiRenderTarget
	{
	public:
		
		/**
		* @brief	  単体のターゲっとを返す
		* @return	　ID3D11RenderTargetView*const*
		*/
		virtual ID3D11RenderTargetView * const* getView(int location) = 0;

		/**
		* @brief	  マルチターゲットの配列の先頭アドレスを返す
		* @return	　ID3D11RenderTargetView*const*
		*/
		virtual ID3D11RenderTargetView * const* getView() = 0;

		/**
		* @brief	  指定のロケーションのシェーダーリソースビューを返す
		* @return	　ID3D11ShaderResourceView*
		*/
		virtual CComPtr<ID3D11ShaderResourceView> getShaderView(int location) = 0;
		/**
		* @brief	  バッファの数取得
		* @return	　バッファの数
		*/
		virtual int getBufferNum() = 0;

		/**
		* @brief		バッファのクリアー
		* @param color	クリアに使う色
		*/
		virtual void clear(const util::Vec4& color) = 0;

		virtual util::Vec2 getSize() = 0;

	};



}
