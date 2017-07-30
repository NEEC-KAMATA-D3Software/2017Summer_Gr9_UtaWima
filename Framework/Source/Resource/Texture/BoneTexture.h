#pragma once
#include<Source\Util\Render\DXFunc.h>
#include<functional>

/**
* @file	    BoneTexture.h
* @brief	�{�[���e�N�X�`���N���X
* @authro	���{�D�P
* @date	2017/05/12
*/


namespace framework {

	class BoneTexture
	{
	public:
		/**
		* @brief				�R���X�g���N�^
		* @param boneNum		�{�[���̐�
		* @param frameNum		�A�j���[�V�����̃t���[����
		*/
		BoneTexture(int boneNum, int frameNum);
		~BoneTexture();

		void copyTexture(BoneTexture* other,int insertSlot,int sourceSloat);

		/**
		* @brief				�������݊J�n
		*/
		void begin();

		/**
		* @brief				�������ݏI��
		*/
		void end();


		/**
		* @brief				�}�b�v���̂��̂��R�s�[����
		*/
		void begonToCopy(D3D11_MAPPED_SUBRESOURCE* result);

		/**
		* @brief				�}�b�v���̂��̂ɃR�s�[����
		*/
		void begonToSet(D3D11_MAPPED_SUBRESOURCE* data);

		/**
		* @brief				begin()���Ă΂�Ă����Ԃł̂ݏ������݂ł���
		* @param instanceID		�������ރC���X�^���XID
		* @param action			�������݃A�N�V����
		*/
		void beginToBuffer(int boneSloat, int frameSloat, std::function<void(float* r, float* g, float* b, float* a)> action);

		/**
		* @brief			�V�F�[�_�[�r���[�擾
		*/
		ID3D11ShaderResourceView* getShaderView();

		/**
		* @brief			�T�C�Y�擾
		*/
		util::Vec2 getSize();

		/**
		* @brief			�s�N�Z���T�C�Y�擾
		*/
		int getPixBufferSize();

	private:
		void createTexture2D(int boneNum, int frameNum);

		void createShaderResourceView();

		void writeSubResource(D3D11_MAPPED_SUBRESOURCE* pMapData, int x, int y, std::function<void(float* r, float* g, float* b, float* a)> action);

		void eachPix(std::function<void(int, int)> action);

		ID3D11Texture2D* getTexture2D();

	private:
		//!���f�[�^
		CComPtr<ID3D11Texture2D> m_pTexture2D;

		//!�V�F�[�_�[�r���[
		CComPtr<ID3D11ShaderResourceView> m_pShaderView;

		bool m_IsWriting = false;
		D3D11_MAPPED_SUBRESOURCE m_MappedResource;
		
		int m_Width, m_Height;
	};


}