#pragma once
#include<D3DX11.h>

/**
* @file	RenderTargetStack.h
* @brief	DirectX�̃p�C�v���C����RenderTarget���ꎞ�I�ɑޔ�������N���X
* @authro	���{�D�P
* @date	2017/04/13
*/

namespace util {


	class RenderTargetStack
	{
	public:
		/**
		* @brief			�R���X�g���N�^
		* @param numView	���݃p�C�v���C���ɂ���^�[�Q�b�g�̐�
		* @deteil			�����_�[�^�[�Q�b�g���ꎞ�ޔ�
		*/
		RenderTargetStack(int numView);

		/**
		* @brief			�f�X�g���N�^
		* @deteil			�����_�[�^�[�Q�b�g�����Ƃɖ߂�
		*/
		~RenderTargetStack();

	private:
		int m_numView = 0;
		ID3D11RenderTargetView* m_TargetStack = nullptr;
		ID3D11DepthStencilView* m_DepthStack = nullptr;
	};



}