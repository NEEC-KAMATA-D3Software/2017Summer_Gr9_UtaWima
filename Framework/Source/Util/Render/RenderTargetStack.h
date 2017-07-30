#pragma once
#include<D3DX11.h>

/**
* @file	RenderTargetStack.h
* @brief	DirectXのパイプラインのRenderTargetを一時的に退避させるクラス
* @authro	高須優輝
* @date	2017/04/13
*/

namespace util {


	class RenderTargetStack
	{
	public:
		/**
		* @brief			コンストラクタ
		* @param numView	現在パイプラインにあるターゲットの数
		* @deteil			レンダーターゲットを一時退避
		*/
		RenderTargetStack(int numView);

		/**
		* @brief			デストラクタ
		* @deteil			レンダーターゲットをもとに戻す
		*/
		~RenderTargetStack();

	private:
		int m_numView = 0;
		ID3D11RenderTargetView* m_TargetStack = nullptr;
		ID3D11DepthStencilView* m_DepthStack = nullptr;
	};



}