#pragma once

#include<Source\Device\Render\DirectXInstance.h>
#include"ITarget.h"
#include<Source\Util\Type.h>

namespace framework {

	class CubeDepthTarget : public ITarget
	{
	public:
		CubeDepthTarget(int width, int height, DXGI_FORMAT format);
		~CubeDepthTarget();

		/**
		* @brief	  ビューの設定用変数を返す
		* @return	　ID3D11RenderTargetView*
		*/
		ID3D11DepthStencilView * getView();

		/**
		* @brief				レンダーターゲットをクリアする
		* @param clearColor		クリア色
		*/
		void clear();
	private:
		virtual void createBuffer(int width, int height, DXGI_FORMAT format, int sampleCount = 1) override;

		virtual void createShaderResourceView(DXGI_FORMAT format) override;

		DXGI_FORMAT createView(DXGI_FORMAT format);

	private:
		//深度描画先に設定するための変数
		CComPtr<ID3D11DepthStencilView> m_pView;

	};


}
