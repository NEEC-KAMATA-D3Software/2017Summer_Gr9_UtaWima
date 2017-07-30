#pragma once

/**
* @file	ITarget.h
* @brief	�`��撊�ۃN���X
* @authro	���{�D�P
* @date	2017/02/23
*/

#include<Source\Device\Render\DirectXInstance.h>
#include<memory>

namespace framework {

	class ITarget
	{
	public:
		ITarget() {

		}

		~ITarget() {

		}

		/**
		* @brief		�����\�[�X��Ԃ�
		* @return		ID3D11Texture2D*
		*/
		virtual ID3D11Texture2D* getBuffer() {
			return m_pBuffer.p;
		}

		/**
		* @brief		�V�F�[�_�[�ɓn�����߂̕ϐ�
		* @return		ID3D11ShaderResourceView*
		*/
		virtual CComPtr<ID3D11ShaderResourceView> getShaderView() {
			return m_pShaderView;
		}

	protected:
		//�e�o�b�t�@�𐶐����郁�\�b�h
		virtual void createBuffer(int width, int height, DXGI_FORMAT format,int sampleCount = 1) = 0;
		virtual void createShaderResourceView(DXGI_FORMAT format) = 0;

	protected:

		//!�e�N�X�`���Ƃ��Ĉ����ϐ�
		CComPtr<ID3D11Texture2D> m_pBuffer;
		//!�V�F�[�_�[�A�N�Z�X�p�ϐ�
		CComPtr<ID3D11ShaderResourceView> m_pShaderView;


	};

}