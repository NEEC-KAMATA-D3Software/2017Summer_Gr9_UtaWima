
#pragma once

/**
* @file	RenderTarget.h
* @brief	�`���N���X �}���`�T���v���ɂ͑Ή����Ȃ�
* @authro	���{�D�P
* @date	2017/02/23
*/

#include<Source\Device\Render\DirectXInstance.h>
#include"ITarget.h"
#include<Source\Util\Type.h>

namespace framework {

	class SpriteRenderer;

	class RenderTarget : public ITarget
	{
	public:
		/**
		* @brief		��̃e�N�X�`���𐶐����Ă�������Ƃɂ���
		* @param width	�^�[�Q�b�g�̉���
		* @param height	�^�[�Q�b�g�̏c��
		* @param format	�e�N�X�`���t�H�[�}�b�g
		*/
		RenderTarget(int width, int height, DXGI_FORMAT format);

		/**
		* @brief			�����̃��\�[�X�����ƂɃr���[���쐬����@
		* @param resource	�����\�[�X�̃|�C���^
		* @detail			���������̃��\�[�X�͓����Ń|�C���^�Ǘ������
		*/
		RenderTarget(CComPtr<ID3D11Texture2D> resource, DXGI_FORMAT format);
		~RenderTarget();


		/**
		* @brief	  �r���[�̐ݒ�p�ϐ���Ԃ�
		* @return	�@ID3D11RenderTargetView*
		*/
		virtual ID3D11RenderTargetView * const* getView();

		/**
		* @brief				�����_�[�^�[�Q�b�g���N���A����
		* @param clearColor		�N���A�F
		*/
		void clear(const util::Vec4& clearColor);


		util::Vec2 getSize();


		void draw(const util::Vec2& drawPos);

		void draw(util::Transform* trans);

		void copy(const RenderTarget& other);

	protected:
		/**
		* @brief				�p�����Resolve���������Ă��炤���߂̃R���X�g���N�^
		* @param width			�^�[�Q�b�g�̉���
		* @param height			�^�[�Q�b�g�̏c��
		* @param format			�e�N�X�`���t�H�[�}�b�g
		* @param sampleCount	�}���`�T���v���̐�
		*/
		RenderTarget(int width, int height, DXGI_FORMAT format, int sampleCount);

		/**
		* @brief				�����_���쐬
		*/
		virtual void createSpriteRenderer(int width, int height);

	private:
		void createBuffer(int width, int height, DXGI_FORMAT format, int sampleCount = 1) override;
		void createView(DXGI_FORMAT format, int sampleCount = 1);
		void createShaderResourceView(DXGI_FORMAT format)override;

	protected:
		//�`���ɐݒ肷�邽�߂̕ϐ�
		CComPtr<ID3D11RenderTargetView> m_pView;
		util::Vec2 m_Size;
		std::unique_ptr<SpriteRenderer> m_pRenderer;

	};

}