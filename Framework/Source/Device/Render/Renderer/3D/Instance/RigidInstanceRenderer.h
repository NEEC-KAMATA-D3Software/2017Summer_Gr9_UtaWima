#pragma once
#include<Source\Util\Render\DXFunc.h>
#include<Source\Resource\Model\Model.h>
#include<Source\Util\Type.h>
#include<list>
#include<vector>
#include<Source\Device\Render\Renderer\3D\Instance\IInstanceRenderer.h>
#include<Source\Resource\Texture\CubeTexture.h>

/**
* @file	    RigidRenderer.h
* @brief	���̃C���X�^���X�`��N���X
* @detail   �e�N�X�`���̃��W�X�^0~9�܂łƂ��A10�Ԗڂɂ̓L���[�u�e�N�X�`��������
* @authro	���{�D�P
* @date	2017/03/13
*/



namespace util {
	struct Transform;
}

namespace framework {

	struct MaterialParam;
	class IMaterial;
	class Texture2D;
	class CubeRenderTrget;
	class CubeDepthTarget;

	class RigidInstanceRenderer : public IInstanceRenderer
	{
	public:
		/**
		* @brief		�R�X�g���N�^
		* @param modelName     ���f����Key
		* @param textureNames  �e�N�X�`���̖��O�̓�����vector
		*/
		RigidInstanceRenderer(std::shared_ptr<Model> model, std::vector <std::string> textureNames);
		~RigidInstanceRenderer();

		//�X�V�p�x�̍����o�b�t�@
		struct AlwaysBuffer
		{
			XMMATRIX view;
			//!�ő�`�搔�̓V�F�[�_�[�ɋL�q����Ă���100
			XMMATRIX world[MAX_DRAW_NUM];
			//!�@����]�p�̍s��
			XMMATRIX rotate[MAX_DRAW_NUM];
			//!�V���h�E�}�b�s���O�p�s��
			XMMATRIX shadowViewMat;
			util::Vec3 eyePos;
			float damy;
		};

		struct MaterialBuffer {
			float height;
			float specular;
			//!���C�e�B���O���邩�ǂ���
			float isNotLighting;
			float damy;
		};

		//���܂�X�V���Ȃ��o�b�t�@
		struct SometimeBuffer
		{
			XMMATRIX proj;
		};

		/**
		* @brief					������
		*/
		void init() override;

		void setCubMap(const std::vector<std::shared_ptr<framework::CubeRenderTrget>>& cubMap)override;

		/**
		* @brief				�P�̃L���[�u�}�b�v�����ׂĂɔ��f������
		* @param instanceNum�@�@�`�悷��C���X�^���X�̐�
		* @param cubMap			���f���������L���[�u�e�N�X�`��
		*/
		void setCubMap(int instanceNum, const std::shared_ptr<framework::CubeTexture>& cubMap)override;


		/**
		* @brief				�}�e���A���̃p�����[�^�Z�b�g
		* @param param			�}�e���A���p�����[�^
		*/
		void setMaterialParam(MaterialParam& param)override;

		void clear();

	private:
		/**
		* @brief				�C���X�^���X���Ƃɏ������o�����߂̃R�[���o�b�N
		* @param instanceNum	���Ԗڂ̃C���X�^���X��
		* @param trans			���̃��[�v�`�悷��g�����X�t�H�[��
		*/
		virtual void instanceAction(int instanceNum, util::Transform*  trans);
		/**
		* @brief			�p�C�v���C���̐ݒ�
		*/
		void setPipeline(int drawNum)override;

		void setTexture();

	private:
		std::vector <std::string> m_TextureNames;
		//!�V�F�[�_�[�Ƀe�N�X�`����z��œn�����߂̕ϐ�
		std::unique_ptr<ID3D11ShaderResourceView*[]> m_pShaderResource;
		std::shared_ptr<Model> m_Model;
		std::vector<std::shared_ptr<Texture2D>> m_Textures;
		CComPtr<ID3D11InputLayout> m_pInputLayout;
		//!�X�V�p�x�������o�b�t�@
		CComPtr<ID3D11Buffer> m_pAlwaysBuffer;
		//!�X�V�p�x�̍����o�b�t�@
		CComPtr<ID3D11Buffer> m_pMaterialBuffer;
		//!�X�V�p�x�̒Ⴂ�o�b�t�@
		CComPtr<ID3D11Buffer> m_pSometimeBuffer;
		std::shared_ptr<IMaterial> m_pMaterial;
		//!�X�V�p�ϐ�
		AlwaysBuffer m_AlwaysBuffer;
		MaterialBuffer m_MaterialBuffer;
		std::pair<int, std::unique_ptr<ID3D11ShaderResourceView*[]>> m_pCubMaps;
		util::Mat4 m_Projection;
		int m_TextureNum = 0;
	};




}
