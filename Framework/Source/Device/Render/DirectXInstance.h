#pragma once
#include<D3DX11.h>
#include<atlbase.h>
#include<memory>

/**
* @file	DirectXInstance.h
* @brief	context,device,swapchain���Ǘ�
* @authro	���{�D�P
* @date	2017/02/22
*/

namespace framework {

	class DirectXInstance
	{
	public:

		~DirectXInstance();

		static DirectXInstance* getInstance();

		/**
		* @brief		�f�o�C�X,�R���e�L�X�g,�X���b�v�`�F�C���쐬
		* @param sd		�X���b�v�`�F�C���̍쐬�f��
		* @detail		�O���t�B�b�N�{�[�h�Ή��̏�Ԃō쐬����
		*/
		HRESULT create(const DXGI_SWAP_CHAIN_DESC& sd);

		/**
		* @brief		�f�o�C�X,�R���e�L�X�g,�X���b�v�`�F�C���쐬
		* @param sd		�X���b�v�`�F�C���̍쐬�f��
		* @detail		�����t��CPU���[�h�ō쐬����
		*/
		HRESULT createWarpMode(const DXGI_SWAP_CHAIN_DESC& sd);

		/**
		* @brief		�f�o�C�X,�R���e�L�X�g,�X���b�v�`�F�C���쐬
		* @param sd		�X���b�v�`�F�C���̍쐬�f��
		* @detail		CPU�݂̂̍Œ�������[�h�ō쐬
		*/
		HRESULT createReferenceMode(const DXGI_SWAP_CHAIN_DESC& sd);

		/**
		* @brief		�T�|�[�g����Ă���@�\���x���擾
		* @return		�T�|�[�g����Ă���@�\���x��
		*/
		D3D_FEATURE_LEVEL getSupportLevels();

		/**
		* @brief				�r���[�|�[�g�̐ݒ�
		* @param viewPort		�r���[�|�[�g�\���̂̔z��
		* @param portNum		�r���[�|�[�g�\���̔z��̐�
		*/
		void setViewPort(D3D11_VIEWPORT const* viewPort,int portNum);

		ID3D11Device* getDevice();

		ID3D11DeviceContext* getContext();

		IDXGISwapChain* getSwapChain();

	private:
		/**
		* @brief		�R���X�g���N�^
		*/
		DirectXInstance();

	private:
		static std::shared_ptr<DirectXInstance> m_pInstance;
		CComPtr<ID3D11Device> m_pDevice;
		CComPtr<ID3D11DeviceContext> m_pContext;
		CComPtr<IDXGISwapChain> m_pSwapChain;

		//! �f�o�C�X�쐬���ɕԂ����@�\���x��
		D3D_FEATURE_LEVEL m_FeatureLevelsSupported; 

	};

}


