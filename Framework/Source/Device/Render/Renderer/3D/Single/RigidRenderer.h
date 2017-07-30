#pragma once
#include<Source\Util\Render\DXFunc.h>
#include<Source\Resource\Model\Model.h>
#include<Source\Util\Type.h>
#include<string>
#include"IRenderer.h"
/**
* @file	    RigidRenderer.h
* @brief	���̕`��N���X
* @authro	���{�D�P
* @date	2017/03/7
*/

namespace util {
	struct Transform;
}

namespace framework {

	class IMaterial;
	class Texture2D;


	class RigidRenderer : public IRenderer
	{
	public:
		//�X�V�p�x�̍����o�b�t�@
		struct AlwaysBuffer {
			XMMATRIX view;
			XMMATRIX world;
		};

		//�X�V�p�x�̒Ⴂ�o�b�t�@
		struct SometimeBuffer
		{
			XMMATRIX proj;
		};

		/**
		* @brief			�R���X�g���N�^
		* @param material		�`��Ɏg���}�e���A���I�u�W�F�N�g
		*/
		RigidRenderer(const std::string& modelName);
		~RigidRenderer();

		void init()override;

		/**
		* @brief			�`��R�[���o�b�N
		* @param trans		�`��Ɏg���g�����X�t�H�[��
		*/
		void draw(util::Transform * trans)override;

	private:
		/**
		* @brief			�萔�o�b�t�@�̍X�V
		*/
		void constBufferUpdate();

	private:
		Model m_Model;
		std::shared_ptr<Texture2D> texture;
		CComPtr<ID3D11InputLayout> m_pInputLayout;
		//!�X�V�p�x�������o�b�t�@
		CComPtr<ID3D11Buffer> m_pAlwaysBuffer;
		//!�X�V�p�x�̒Ⴂ�o�b�t�@
		CComPtr<ID3D11Buffer> m_pSometimeBuffer;
		std::shared_ptr<IMaterial> m_pMaterial;

	};



}