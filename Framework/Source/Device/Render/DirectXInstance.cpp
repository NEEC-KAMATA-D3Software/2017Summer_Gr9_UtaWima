#include"DirectXInstance.h"
#include<Windows.h>
#include <crtdbg.h>
#include<assert.h>
#include<iostream>
#include"../../Util/Win/WinFunc.h"

namespace framework {

	std::shared_ptr<DirectXInstance> DirectXInstance::m_pInstance = std::shared_ptr<DirectXInstance>(new DirectXInstance());

	DirectXInstance::DirectXInstance()
	{

	}

	DirectXInstance::~DirectXInstance()
	{
	}

	DirectXInstance * DirectXInstance::getInstance()
	{
		return m_pInstance.get();
	}

	HRESULT DirectXInstance::create(const DXGI_SWAP_CHAIN_DESC& sd)
	{
		//機能レベルの配列
		D3D_FEATURE_LEVEL pFeatureLevels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0 };
		UINT              featureLevels = 3;   // 配列の要素数

		ID3D11Device* pDevice;
		ID3D11DeviceContext* pContext;
		IDXGISwapChain* pSwapChain;

		UINT debugFlag;
#ifdef _MDEBUG
		debugFlag = D3D11_CREATE_DEVICE_DEBUG;
#else 
		debugFlag = 0;
#endif

		//デバイス、コンテキスト、スワップチェインを同時に作成
		HRESULT hr = D3D11CreateDeviceAndSwapChain(
			NULL,
			D3D_DRIVER_TYPE_HARDWARE,
			NULL,
			debugFlag,
			pFeatureLevels,
			featureLevels,
			D3D11_SDK_VERSION,
			&sd,
			&pSwapChain,
			&pDevice,
			&m_FeatureLevelsSupported,
			&pContext
		);

		if (FAILED(hr)) {
			util::WinFunc::OutLog("D3D11CreateDeviceAndSwapChain TYPE_HARDWAREで作成失敗");
			//グラフィックボードが対応していなかった場合制限つきCPUモードで作成できるか試みる
			createWarpMode(sd);
		}

		m_pDevice.Attach(pDevice);
		m_pContext.Attach(pContext);
		m_pSwapChain.Attach(pSwapChain);

		return S_OK;
	}

	HRESULT DirectXInstance::createWarpMode(const DXGI_SWAP_CHAIN_DESC & sd)
	{
		//機能レベルの配列
		D3D_FEATURE_LEVEL pFeatureLevels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0 };
		UINT              featureLevels = 3;   // 配列の要素数

		ID3D11Device* pDevice;
		ID3D11DeviceContext* pContext;
		IDXGISwapChain* pSwapChain;

		UINT debugFlag;
#ifdef _MDEBUG
		debugFlag = D3D11_CREATE_DEVICE_DEBUG;
#else 
		debugFlag = 0;
#endif

		//デバイス、コンテキスト、スワップチェインを同時に作成
		HRESULT hr = D3D11CreateDeviceAndSwapChain(
			NULL,
			D3D_DRIVER_TYPE_WARP,
			NULL,
			debugFlag,
			pFeatureLevels,
			featureLevels,
			D3D11_SDK_VERSION,
			&sd,
			&pSwapChain,
			&pDevice,
			&m_FeatureLevelsSupported,
			&pContext
		);

		m_pDevice.Attach(pDevice);
		m_pContext.Attach(pContext);
		m_pSwapChain.Attach(pSwapChain);

		if (FAILED(hr)) {
			util::WinFunc::OutLog("D3D11CreateDeviceAndSwapChain TYPE_WARPで作成失敗");
			//制限付きCPUモードに対応していなかった場合CPUのみモードで作成できるか試みる
			createReferenceMode(sd);
		}
		return S_OK;
	}

	HRESULT DirectXInstance::createReferenceMode(const DXGI_SWAP_CHAIN_DESC & sd)
	{
		//機能レベルの配列
		D3D_FEATURE_LEVEL pFeatureLevels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0 };
		UINT              featureLevels = 3;   // 配列の要素数

		ID3D11Device* pDevice;
		ID3D11DeviceContext* pContext;
		IDXGISwapChain* pSwapChain;

		UINT debugFlag;
#ifdef _MDEBUG
		debugFlag = D3D11_CREATE_DEVICE_DEBUG;
#else 
		debugFlag = 0;
#endif

		//デバイス、コンテキスト、スワップチェインを同時に作成
		HRESULT hr = D3D11CreateDeviceAndSwapChain(
			NULL,
			D3D_DRIVER_TYPE_REFERENCE,
			NULL,
			debugFlag,
			pFeatureLevels,
			featureLevels,
			D3D11_SDK_VERSION,
			&sd,
			&pSwapChain,
			&pDevice,
			&m_FeatureLevelsSupported,
			&pContext
		);

		m_pDevice.Attach(pDevice);
		m_pContext.Attach(pContext);
		m_pSwapChain.Attach(pSwapChain);

		if (FAILED(hr)) {
			assert(false && "D3D11CreateDeviceAndSwapChain TYPE_REFERENCEで作成失敗");
			return S_FALSE;
		}

		return S_OK;
	}

	D3D_FEATURE_LEVEL DirectXInstance::getSupportLevels()
	{
		return m_FeatureLevelsSupported;
	}

	void DirectXInstance::setViewPort(D3D11_VIEWPORT const * viewPort,int portNum)
	{
		m_pContext->RSSetViewports(portNum, viewPort);
	}

	ID3D11Device * DirectXInstance::getDevice()
	{
		return m_pDevice.p;
	}

	ID3D11DeviceContext * DirectXInstance::getContext()
	{
		return m_pContext.p;
	}

	IDXGISwapChain * DirectXInstance::getSwapChain()
	{
		return m_pSwapChain.p;
	}

}