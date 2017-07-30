#pragma once


#pragma once
#include<D3DX11.h>
#include<vector>
#include<memory>
#include<Source\Util\Type.h>
#include<atlbase.h>
#include<Source\Util\Type.h>

namespace framework {

	class RenderTarget;

	class IMultiRenderTarget
	{
	public:
		
		/**
		* @brief	  �P�̂̃^�[�Q���Ƃ�Ԃ�
		* @return	�@ID3D11RenderTargetView*const*
		*/
		virtual ID3D11RenderTargetView * const* getView(int location) = 0;

		/**
		* @brief	  �}���`�^�[�Q�b�g�̔z��̐擪�A�h���X��Ԃ�
		* @return	�@ID3D11RenderTargetView*const*
		*/
		virtual ID3D11RenderTargetView * const* getView() = 0;

		/**
		* @brief	  �w��̃��P�[�V�����̃V�F�[�_�[���\�[�X�r���[��Ԃ�
		* @return	�@ID3D11ShaderResourceView*
		*/
		virtual CComPtr<ID3D11ShaderResourceView> getShaderView(int location) = 0;
		/**
		* @brief	  �o�b�t�@�̐��擾
		* @return	�@�o�b�t�@�̐�
		*/
		virtual int getBufferNum() = 0;

		/**
		* @brief		�o�b�t�@�̃N���A�[
		* @param color	�N���A�Ɏg���F
		*/
		virtual void clear(const util::Vec4& color) = 0;

		virtual util::Vec2 getSize() = 0;

	};



}
