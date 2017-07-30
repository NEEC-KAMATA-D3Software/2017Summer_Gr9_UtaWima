#pragma once

/**
* @file	DepthBuffer.h
* @brief	�[�x�X�e���V���̕`���N���X�@�}���`�T���v���ɂ͑Ή����Ȃ�
* @authro	���{�D�P
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
		* @brief				�f�v�X�o�b�t�@���쐬����
		* @param width			�o�b�t�@�̉��T�C�Y
		* @param height			�o�b�t�@�̏c�T�C�Y
		* @param format			�o�b�t�@�̃t�H�[�}�b�g�@
		* @dital				�t�H�[�}�b�g��TYPELESS�n�Ɍ���
		*/
		DepthTarget(int width, int height, DXGI_FORMAT format);

		~DepthTarget();

		/**
		* @brief	  �f�v�X�r���[�̐ݒ�p�ϐ���Ԃ�
		* @return	�@ID3D11RenderTargetView*
		*/
		ID3D11DepthStencilView *  getView();

		/**
		* @brief				�����_�[�^�[�Q�b�g���N���A����
		*/
		void clear();

	protected:
		/**
		* @brief				�f�v�X�o�b�t�@���쐬����
		* @param width			�o�b�t�@�̉��T�C�Y
		* @param height			�o�b�t�@�̏c�T�C�Y
		* @param format			�o�b�t�@�̃t�H�[�}�b�g�@
		* @param sampleCount	�}���`�T���v���̐�
		* @dital				�t�H�[�}�b�g��TYPELESS�n�Ɍ���
		*/
		DepthTarget(int width, int height, DXGI_FORMAT format,int sampleCount);

	private:
		void createBuffer(int width, int height, DXGI_FORMAT format, int sampleCount = 1) override;
		DXGI_FORMAT createView(DXGI_FORMAT format, int sampleCount = 1);
		void createShaderResourceView(DXGI_FORMAT format)override;

	protected:
		//�[�x�`���ɐݒ肷�邽�߂̕ϐ�
		CComPtr<ID3D11DepthStencilView> m_pView;
	};

}