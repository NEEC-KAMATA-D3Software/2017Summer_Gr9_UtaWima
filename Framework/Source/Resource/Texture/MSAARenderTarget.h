#pragma once
#include"RenderTarget.h"
#include<Source\Util\Type.h>
#include<memory>

namespace framework {

	class MSAARenderTarget : public RenderTarget
	{
	public:
		MSAARenderTarget(int width, int height, DXGI_FORMAT format,int sampleCout);
		~MSAARenderTarget();


		/**
		* @brief		�����\�[�X��Ԃ�
		* @return		ID3D11Texture2D*
		*/
		ID3D11Texture2D* getBuffer()override;

		void resolveTexture();

	private:
		void createResolveTexture2D(int width, int height, DXGI_FORMAT format);

	private:
		//�}���`�T���v���p�ɕϊ���̃e�N�X�`���QD
		CComPtr<ID3D11Texture2D> m_pResolveTexture;
		DXGI_FORMAT m_Format;
	};



}