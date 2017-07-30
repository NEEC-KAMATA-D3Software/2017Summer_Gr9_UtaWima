#pragma once

#include"../../Device/Render/DirectXInstance.h"
#include<string>
#include"../../Device/Render/Shader/ShaderData.h"
#include<d3d11shader.h>
#include<assert.h>
#include<iostream>
#include"../../Device/Render/Shader/ShaderData.h"
#include<Source\Util\Type.h>
#include<functional>

/**
* @file	DXFunc.h
* @brief	�悭�g���֐����ȈՂɎg����悤�Ƀ��b�v�����֐��܂Ƃ�
* @authro	���{�D�P
* @date	2017/02/23
*/
namespace util {

	enum class SamplerType
	{
		Clamp,
		Wrap,
	};

	/**
	* @brief		�f�o�C�X�擾
	*/
	ID3D11Device * getDevice();
	/**
	* @brief		�R���e�L�X�g�擾
	*/
	ID3D11DeviceContext *getContext();
	/**
	* @brief		�X���b�v�`�F�C���擾
	*/
	IDXGISwapChain *getSwapChain();

	/**
	* @brief		�o�b�N�o�b�t�@�擾
	* @return		�o�b�N�o�b�t�@�[�փA�N�Z�X���邽�߂̎Q��
	*/
	CComPtr<ID3D11Texture2D> getBackBuffer();

	void setSingleViewPort(int width, int height);

	/**
	* @brief				���_�o�b�t�@���쐬����
	* @param bufferSize		�o�b�t�@�[�̃T�C�Y
	* @param data			���_�o�b�t�@�����f�[�^
	* @return				���_�o�b�t�@
	*/
	CComPtr<ID3D11Buffer> createVertexBuffer(UINT bufferSize, void* data);

	/**
	* @brief				�C���f�b�N�X�o�b�t�@���쐬����
	* @param bufferSize		�o�b�t�@�[�̃T�C�Y
	* @param data			�C���f�b�N�X�o�b�t�@�����f�[�^
	* @return				�C���f�b�N�X�o�b�t�@
	*/
	CComPtr<ID3D11Buffer> createIndexBuffer(UINT bufferSize, void* data);

	/**
	* @brief				�萔�o�b�t�@�o�b�t�@���쐬����
	* @param bufferSize		�o�b�t�@�[�̃T�C�Y
	* @param data			�f�[�^�̏����l(�Ȃ��Ă��悢)
	* @return				�萔�o�b�t�@
	* @param data			�o�b�t�@�[�̏����l(�Ȃ��Ă��ǂ�)
	*/
	CComPtr<ID3D11Buffer> createConstantBuffer(UINT bufferSize, void* data = nullptr);


	/**
	* @brief				�V�F�[�_�[���R���p�C������
	* @param fileName		�V�F�[�_�t�@�C���܂ł̃p�X
	* @param entryName		�V�F�[�_�[�̃G���g���[�|�C���g�̖��O
	* @param compile		�R���p�C���̎��
	* @return				�V�F�[�_�̃o�C�g�R�[�h
	*/
	CComPtr<ID3DBlob> compileShader(std::string fileName, std::string entryName, std::string compileVersion, const std::string & includePath = "");

	/**
	* @brief				���̓��C�A�E�g���쐬����
	* @param layout			�쐬���郌�C�A�E�g
	* @param layoutCount	���C�A�E�g�̐�
	* @param vertexBlob		���_�V�F�[�_�̃o�C�i���f�[�^
	* @return				IA�C���^�[�t�F�[�X
	*/
	CComPtr<ID3D11InputLayout> createIA(const D3D11_INPUT_ELEMENT_DESC* layout, int layoutCount, ID3DBlob* vertexBlob);

	/**
	* @brief				���̓��C�A�E�g���쐬����
	* @param layout			�쐬���郌�C�A�E�g
	* @param layoutCount	���C�A�E�g�̐�
	* @param data			�o�C�g�f�[�^
	* @param size			�o�C�g�f�[�^�̃T�C�Y
	* @return				IA�C���^�[�t�F�[�X
	*/
	CComPtr<ID3D11InputLayout> createIA(const D3D11_INPUT_ELEMENT_DESC* layout, int layoutCount, void* data, int size);

	/**
	* @brief					�悭�g���ݒ�Ń��X�^���C�U���쐬����
	* @param activeMultiSample	�}���`�T���v����L���ɂ��邩�ǂ��� true = �L�� false = ����
	* @param isCullFace			�t�F�[�X���J�����O���邩�ǂ���     true = �J�����O�L�� fakse = �J�����O����
	* @return					���X�^���C�U�C���^�[�t�F�[�X
	*/
	CComPtr<ID3D11RasterizerState> createRasterizerSate(bool activeMultiSample,bool isCullFace = true);

	/**
	* @brief	�悭�g���ݒ�Ńf�v�X�X�e���V���X�e�[�g���쐬����
	* @return	�f�v�X�X�e���V���X�e�[�g�I�u�W�F�N�g�C���^�[�t�F�[�X
	* @dital	�X�e���V���e�X�g�͖����ɂȂ��Ă���
	*/
	CComPtr<ID3D11DepthStencilState> createDepthStencilState();

	/**
	* @brief				�悭�g���ݒ�Ńu�����h�e�[�g���쐬����
	* @param isBlendEnable  �A���t�@�u�����h�L���ɂ��邩�ǂ��� true = �L���@false = ����
	* @return				�u�����h�X�e�[�g�I�u�W�F�N�g
	* @dital				�A���t�@�u�����h���L���ɂȂ��Ă���ꍇ�����_�[�^�[�Q�b�g0�Ԗڂ̂݃A���t�@�u�����h�L��
	*/
	CComPtr<ID3D11BlendState> createBlendState(bool isBlendEnable = true, D3D11_BLEND_DESC* desc = nullptr);

	/**
	* @brief					�萔�o�b�t�@�̍X�V���s��
	* @param constantBuffer		�����������s���o�b�t�@
	* @param bufferStruct		�㏑������f�[�^
	*/
	template<typename T>
	void updateConstantBuffer(ID3D11Buffer* constantBuffer, const T& bufferStruct) {
		D3D11_MAPPED_SUBRESOURCE mapResource;
		HRESULT hr = getContext()->Map(
			constantBuffer,
			0,
			D3D11_MAP_WRITE_DISCARD,//�������݃A�N�Z�X
			0,
			&mapResource);

		if (FAILED(hr)) {
			assert(false && "�萔�o�b�t�@�������ݎ��s");
		}

		CopyMemory(mapResource.pData, &bufferStruct, sizeof(T));

		getContext()->Unmap(constantBuffer, 0);
	}

	/**
	* @brief					�T���v���[�X�e�[�g�I�u�W�F�N�g�쐬
	* @param samplerType		���E���̐ݒ�
	*/
	CComPtr<ID3D11SamplerState> createSamplerState(const SamplerType& samplerType);

	/**
	* @brief					�u�����h�X�e�[�g�̃Z�b�g
	* @param sate				�u�����h�X�e�[�g�I�u�W�F�N�g
	*/
	void setBlendState(ID3D11BlendState* state);

	/**
	* @brief					�V���h�E�}�b�v�p�s��쐬
	*/
	void shadowViewMat(util::Mat4& lightView,util::Mat4& lightProj,util::Mat4& world, util::Mat4* result);

	/**
	* @brief					�e�N�X�`���ɏ�������
	* @param  pTexture2D		�������܂��e�N�X�`��
	* @param  textureNum		�������܂��e�N�X�`����Array��
	*/
	void writeTexutre(ID3D11Texture2D* pTexture2D, std::function<void(D3D11_MAPPED_SUBRESOURCE*)> action, D3D11_MAP mapMode);

	/**
	* @brief					�e�N�X�`���ɏ�������
	* @param  pTexture2D		�������܂��e�N�X�`��
	* @param  location			���Ԗڂɏ������ނ��ǂ���
	* @param  pResource			�������݂�����e�N�X�`��
	* @detile�@CPU�A�N�Z�X�t���O���������ݕs�\��Ԃ̂������\ D3D11_MAP_DISCARD���w�肵���ꍇ�s�g����Ȃ��ꍇ������
	*/
	void writeArrayTexutre(ID3D11Texture2D* pTexture2D,int location , ID3D11Texture2D * pResource, D3D11_MAP mapMode);

	/**
	* @brief					�e�N�X�`���ɏ�������
	* @param  pTexture2D		�������܂��e�N�X�`��
	* @param  location			���Ԗڂɏ������ނ��ǂ���
	* @param  mappedResource	�R�s�[����}�b�v�f�[�^
	* @detile�@CPU�A�N�Z�X�t���O���������ݕs�\��Ԃ̂������\ D3D11_MAP_DISCARD���w�肵���ꍇ�s�g����Ȃ��ꍇ������
	*/
	void writeArrayTexutre(ID3D11Texture2D* pTexture2D, int location, D3D11_MAPPED_SUBRESOURCE* mappedResource);

	CComPtr<ID3D11Texture2D> loadTexture2D(const std::string& file);

	/**
	* @brief				�ǂݍ��݂ƕ`��
	* @param  filePath		�`�悷��e�N�X�`���̃p�X
	* @param  pos			�`��ʒu
	* @param  deteil		�`�悪�I��邽�тɓǂݍ��񂾃e�N�X�`���͔j�������
	*/
	void LoadAndDraw(const std::string& filePath,const util::Vec2& pos,const util::Vec2& scale);

	void SetMultiThreadFlag(bool isMultiThread);
}