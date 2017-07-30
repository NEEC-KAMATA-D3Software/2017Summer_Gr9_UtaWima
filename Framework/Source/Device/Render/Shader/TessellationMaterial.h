#pragma once
#include"StandardMaterial.h"
#include<D3DX11.h>


namespace framework {

	class TessellationMaterial : public StandardMaterial
	{
	public:
		TessellationMaterial(ShaderData<ShaderType::Vertex> vs, CComPtr<ID3D11PixelShader> ps, CComPtr<ID3D11HullShader> hs, CComPtr<ID3D11DomainShader> ds);
		~TessellationMaterial();


		/**
		* @brief				�V�F�[�_�[���p�C�v���C���ɃZ�b�g���ėL���ɂ���
		*/
		virtual void active() override;

		/**
		* @brief				�V�F�[�_�[�𖳌��ɂ���
		*/
		virtual void deActive() override;

		/**
		* @brief				�萔���Z�b�g����
		* @param buffer			ID3D11Buffer�z��̃A�h���X
		* @param bufferNum		buffer�̔z��̗v�f��
		*/
		virtual void setConstantBuffer(ID3D11Buffer *const* buffer, UINT bufferNum) override;

	private:
		CComPtr<ID3D11HullShader> m_pHullShader;
		CComPtr<ID3D11DomainShader> m_pDomainShader;
	};


}