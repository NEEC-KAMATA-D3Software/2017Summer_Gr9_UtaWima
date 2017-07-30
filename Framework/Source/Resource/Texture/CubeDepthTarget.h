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
		* @brief	  �r���[�̐ݒ�p�ϐ���Ԃ�
		* @return	�@ID3D11RenderTargetView*
		*/
		ID3D11DepthStencilView * getView();

		/**
		* @brief				�����_�[�^�[�Q�b�g���N���A����
		* @param clearColor		�N���A�F
		*/
		void clear();
	private:
		virtual void createBuffer(int width, int height, DXGI_FORMAT format, int sampleCount = 1) override;

		virtual void createShaderResourceView(DXGI_FORMAT format) override;

		DXGI_FORMAT createView(DXGI_FORMAT format);

	private:
		//�[�x�`���ɐݒ肷�邽�߂̕ϐ�
		CComPtr<ID3D11DepthStencilView> m_pView;

	};


}
