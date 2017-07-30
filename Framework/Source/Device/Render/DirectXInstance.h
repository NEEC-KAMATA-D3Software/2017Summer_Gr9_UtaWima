#pragma once
#include<D3DX11.h>
#include<atlbase.h>
#include<memory>

/**
* @file	DirectXInstance.h
* @brief	context,device,swapchainを管理
* @authro	高須優輝
* @date	2017/02/22
*/

namespace framework {

	class DirectXInstance
	{
	public:

		~DirectXInstance();

		static DirectXInstance* getInstance();

		/**
		* @brief		デバイス,コンテキスト,スワップチェイン作成
		* @param sd		スワップチェインの作成素材
		* @detail		グラフィックボード対応の状態で作成する
		*/
		HRESULT create(const DXGI_SWAP_CHAIN_DESC& sd);

		/**
		* @brief		デバイス,コンテキスト,スワップチェイン作成
		* @param sd		スワップチェインの作成素材
		* @detail		制限付きCPUモードで作成する
		*/
		HRESULT createWarpMode(const DXGI_SWAP_CHAIN_DESC& sd);

		/**
		* @brief		デバイス,コンテキスト,スワップチェイン作成
		* @param sd		スワップチェインの作成素材
		* @detail		CPUのみの最低条件モードで作成
		*/
		HRESULT createReferenceMode(const DXGI_SWAP_CHAIN_DESC& sd);

		/**
		* @brief		サポートされている機能レベル取得
		* @return		サポートされている機能レベル
		*/
		D3D_FEATURE_LEVEL getSupportLevels();

		/**
		* @brief				ビューポートの設定
		* @param viewPort		ビューポート構造体の配列
		* @param portNum		ビューポート構造体配列の数
		*/
		void setViewPort(D3D11_VIEWPORT const* viewPort,int portNum);

		ID3D11Device* getDevice();

		ID3D11DeviceContext* getContext();

		IDXGISwapChain* getSwapChain();

	private:
		/**
		* @brief		コンストラクタ
		*/
		DirectXInstance();

	private:
		static std::shared_ptr<DirectXInstance> m_pInstance;
		CComPtr<ID3D11Device> m_pDevice;
		CComPtr<ID3D11DeviceContext> m_pContext;
		CComPtr<IDXGISwapChain> m_pSwapChain;

		//! デバイス作成時に返される機能レベル
		D3D_FEATURE_LEVEL m_FeatureLevelsSupported; 

	};

}


