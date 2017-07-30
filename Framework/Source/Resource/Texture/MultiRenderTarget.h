#pragma once
#include<D3DX11.h>
#include<vector>
#include<memory>
#include<Source\Util\Type.h>
#include<atlbase.h>
#include<Source\Util\Type.h>
#include<Source\Resource\Texture\IMultiRenderTarget.h>

namespace framework {

	class RenderTarget;

	class MultiRenderTarget : public IMultiRenderTarget
	{
	public:
		MultiRenderTarget(unsigned int bufferNum, int width, int height, DXGI_FORMAT format);
		~MultiRenderTarget();

		/**
		* @brief	  単体のターゲっとを返す
		* @return	　ID3D11RenderTargetView*const*
		*/
		ID3D11RenderTargetView * const* getView(int location) override;

		/**
		* @brief	  マルチターゲットの配列の先頭アドレスを返す
		* @return	　ID3D11RenderTargetView*const*
		*/
		ID3D11RenderTargetView * const* getView() override;

		/**
		* @brief	  指定のロケーションのシェーダーリソースビューを返す
		* @return	　ID3D11ShaderResourceView*
		*/
		CComPtr<ID3D11ShaderResourceView> getShaderView(int location) override;
		/**
		* @brief	  バッファの数取得
		* @return	　バッファの数
		*/
		int getBufferNum() override;

		/**
		* @brief		バッファのクリアー
		* @param color	クリアに使う色
		*/
		void clear(const util::Vec4& color) override;

		util::Vec2 getSize() override;

		/**
		* @brief					lightingBufferの内容を描画
		* @param drawLocation	　　何番目のロケーションか
		*/
		void draw(int drawLocation);

	private:
		std::vector<std::unique_ptr<RenderTarget>> m_pTargets;
		std::unique_ptr<ID3D11RenderTargetView*[]> m_pResult;
	};



}
