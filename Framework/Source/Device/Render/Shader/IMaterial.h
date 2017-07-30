#pragma once

#include<D3DX11.h>
#include"../../../Util/Render/DXFunc.h"
#include<Source\Device\Render\Shader\ShaderData.h>

/**
* @file	IMaterial.h
* @brief	�}�e���A���̃C���^�[�t�F�[�X
* @authro	���{�D�P
* @date	2017/03/7
*/

namespace framework {

	class Texture2D;

	struct MaterialParam {
		MaterialParam()
			:specular(0),
			height(0)
		{
		}

		float specular;
		float height;
		//!���C�e�B���O���邩�ǂ���
		bool isNotLighting;
	};

	class IMaterial
	{
	public:
		IMaterial(ShaderData<ShaderType::Vertex> vertexShaderData)
			:m_pVertexShader(vertexShaderData)
		{};
		~IMaterial() {};

		/**
		* @brief		�V�F�[�_��L���ɂ���
		*/
		virtual void active() = 0;

		/**
		* @brief		�V�F�[�_�𖳌��ɂ���
		*/
		virtual void deActive() = 0;

		/**
		* @brief				�萔���Z�b�g����
		* @param buffer			ID3D11Buffer�z��̃A�h���X
		* @param bufferNum		buffer�̔z��̗v�f��
		*/
		virtual void setConstantBuffer(ID3D11Buffer*const* buffer,UINT bufferNum) = 0;

		/**
		* @brief				�e�N�X�`�����Z�b�g����
		* @param baindNum		���Ԗڂ���Z�b�g���邩
		* @param texNum			���e�N�X�`�����Z�b�g���邩
		* @param texture		�Z�b�g����e�N�X�`��
		*/
		virtual void setTexture(int startSlot,int texNum, ID3D11ShaderResourceView* const* texture);

		/**
		* @brief				�T���v�����Z�b�g����
		* @param baindNum		���Ԗڂ���Z�b�g���邩
		* @param samplerNum		���T���v�����Z�b�g���邩
		* @param sampler		�Z�b�g����X�e�[�g�I�u�W�F�N�g
		*/
		virtual void setSamplerState(int startSlot, int samplerNum, ID3D11SamplerState*const* sampler);

		/**
		* @brief				���_�V�F�[�_�̃o�C�i���擾
		*/
		ID3DBlob* getVertexByte();

	protected:
		ShaderData<ShaderType::Vertex> m_pVertexShader;
	};
}