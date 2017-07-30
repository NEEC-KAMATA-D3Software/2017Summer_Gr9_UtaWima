#pragma once
#include"DepthTarget.h"

namespace framework {

	class MSAADepthTarget : public DepthTarget
	{
	public:
		MSAADepthTarget(int width, int height, DXGI_FORMAT format, int sampleCount);
		~MSAADepthTarget();

		/**
		* @brief		�����\�[�X��Ԃ�
		* @return		ID3D11Texture2D*
		*/
		ID3D11Texture2D* getBuffer()override;

		/**
		* @brief		�}���`�T���v����Texture2D�ɉ���
		*/
		void resolveTexture();

	private:
		void createResolveTexture2D(int width, int height, DXGI_FORMAT format);

	private:
		//�}���`�T���v���p�ɕϊ���̃e�N�X�`���QD
		CComPtr<ID3D11Texture2D> m_pResolveTexture;
		DXGI_FORMAT m_Format;
	};



}