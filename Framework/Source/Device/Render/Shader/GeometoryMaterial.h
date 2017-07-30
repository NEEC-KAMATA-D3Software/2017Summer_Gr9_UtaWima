#pragma once

#include<Source\Device\Render\Shader\StandardMaterial.h>

namespace framework {

	class GeometoryMaterial : public StandardMaterial
	{
	public:
		GeometoryMaterial(ShaderData<ShaderType::Vertex> vs, CComPtr<ID3D11GeometryShader> gs, CComPtr<ID3D11PixelShader> ps);
		~GeometoryMaterial();

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
		CComPtr<ID3D11GeometryShader> m_pGeometryShader;

	};



}
