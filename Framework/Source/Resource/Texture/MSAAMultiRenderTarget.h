#pragma once

#include<Source\Resource\Texture\MSAARenderTarget.h>
#include<memory>
#include<vector>
#include<D3DX11.h>
#include<Source\Util\Type.h>
#include<Source\Resource\Texture\IMultiRenderTarget.h>

namespace framework {

	class MSAARenderTarget;

	class MSAAMultiRenderTarget : public IMultiRenderTarget
	{
	public:
		MSAAMultiRenderTarget(int numTexture,int width,int height,DXGI_FORMAT format,int sampleCount);
		~MSAAMultiRenderTarget();

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
		

		void resolveTexutre();

		/**
		* @brief			  レンダーターゲットの描画
		* @param drawLocation 描画するターゲットのロケーション
		*/
		void draw(int drawLoacation);

	private:
		std::vector<std::unique_ptr<MSAARenderTarget>> m_Targets;
		std::unique_ptr<ID3D11RenderTargetView*[]> m_pResult;
	};
}
