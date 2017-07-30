#pragma once
#include"IMaterial.h"
#include<atlbase.h>

namespace framework {

	class StandardMaterial : public IMaterial
	{
	public:
		StandardMaterial(ShaderData<ShaderType::Vertex> vs, CComPtr<ID3D11PixelShader> ps);
		~StandardMaterial();

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
		CComPtr<ID3D11PixelShader> m_pPixelShader;
	};
}
