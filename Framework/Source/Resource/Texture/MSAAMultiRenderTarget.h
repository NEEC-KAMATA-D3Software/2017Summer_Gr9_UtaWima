#pragma once

#include<Source\Resource\Texture\MSAARenderTarget.h>
#include<memory>
#include<vector>
#include<D3DX11.h>
#include<Source\Util\Type.h>
#include<Source\Resource\Texture\IMultiRenderTarget.h>

namespace framework {

	class MSAARenderTarget;

	class MSAAMultiRenderTarget : public IMultiRenderTarget
	{
	public:
		MSAAMultiRenderTarget(int numTexture,int width,int height,DXGI_FORMAT format,int sampleCount);
		~MSAAMultiRenderTarget();

		/**
		* @brief	  �P�̂̃^�[�Q���Ƃ�Ԃ�
		* @return	�@ID3D11RenderTargetView*const*
		*/
		ID3D11RenderTargetView * const* getView(int location) override;

		/**
		* @brief	  �}���`�^�[�Q�b�g�̔z��̐擪�A�h���X��Ԃ�
		* @return	�@ID3D11RenderTargetView*const*
		*/
		ID3D11RenderTargetView * const* getView() override;

		/**
		* @brief	  �w��̃��P�[�V�����̃V�F�[�_�[���\�[�X�r���[��Ԃ�
		* @return	�@ID3D11ShaderResourceView*
		*/
		CComPtr<ID3D11ShaderResourceView> getShaderView(int location) override;

		/**
		* @brief	  �o�b�t�@�̐��擾
		* @return	�@�o�b�t�@�̐�
		*/
		int getBufferNum() override;


		/**
		* @brief		�o�b�t�@�̃N���A�[
		* @param color	�N���A�Ɏg���F
		*/
		void clear(const util::Vec4& color) override;

		util::Vec2 getSize() override;
		

		void resolveTexutre();

		/**
		* @brief			  �����_�[�^�[�Q�b�g�̕`��
		* @param drawLocation �`�悷��^�[�Q�b�g�̃��P�[�V����
		*/
		void draw(int drawLoacation);

	private:
		std::vector<std::unique_ptr<MSAARenderTarget>> m_Targets;
		std::unique_ptr<ID3D11RenderTargetView*[]> m_pResult;
	};
}
