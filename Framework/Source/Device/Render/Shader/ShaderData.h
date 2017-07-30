#pragma once

#include<D3DX11.h>
#include<atlbase.h>
namespace framework {

	enum class ShaderType
	{
		Vertex,
		Geometry,
		Pix,
		Hal,
		Domain,
	};

	template<ShaderType T>
	struct ShaderData;

	template<>
	struct ShaderData<ShaderType::Vertex>
	{
		//!�p�C�v���C���ɃZ�b�g�����߂̕ϐ�
		CComPtr<ID3D11VertexShader>		shaderAccess;
		//!�o�C�g�f�[�^(���̓��C�A�E�g�Ȃǂŗ��p)
		CComPtr<ID3DBlob>				byteData;
	};

	template<>
	struct ShaderData<ShaderType::Pix>
	{
		//!�p�C�v���C���ɃZ�b�g�����߂̕ϐ�
		CComPtr<ID3D11PixelShader>		shaderAccess;
		//!�o�C�g�f�[�^(���̓��C�A�E�g�Ȃǂŗ��p)
		CComPtr<ID3DBlob>				byteData;
	};

	template<>
	struct ShaderData<ShaderType::Geometry>
	{
		//!�p�C�v���C���ɃZ�b�g�����߂̕ϐ�
		CComPtr<ID3D11GeometryShader>		shaderAccess;
		//!�o�C�g�f�[�^(���̓��C�A�E�g�Ȃǂŗ��p)
		CComPtr<ID3DBlob>				byteData;
	};
}