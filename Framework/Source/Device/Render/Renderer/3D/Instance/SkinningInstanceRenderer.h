#pragma once
#include<Source\Util\Render\DXFunc.h>
#include<Source\Resource\Model\Model.h>
#include<Source\Util\Type.h>
#include<list>
#include<vector>
#include<Source\Device\Render\Renderer\3D\Instance\IInstanceRenderer.h>
#include<Source\Resource\Texture\CubeTexture.h>
#include<memory>
#include<Source\Component\Animation\AnimatorComponent.h>

/**
* @file	    SkinningInstanceRenderer.h
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
	class Motion;
	class BoneTexture;

	
	class SkinningInstanceRenderer : public IInstanceRenderer
	{
	public:
		/**
		* @brief		�R�X�g���N�^
		* @param model     �`�悷�郂�f��
		* @param textureNames  �e�N�X�`���̖��O�̓�����vector
		*/
		SkinningInstanceRenderer(std::shared_ptr<Model> model, std::vector <std::string> textureNames, std::vector <std::string> motionNames);
		~SkinningInstanceRenderer();

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
			int damy;
		};

		//�X�V�p�x�̍����o�b�t�@
		struct SkinningBuffer
		{
			//!�{�[���s�� ���b�V���̐��@�{�[���̐�
			//XMMATRIX boneMatrix[BONE_NUM];
			util::Vec4 drawData[MAX_DRAW_NUM];//16�̔{���ɂ��邽�߂ɂQ����
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

		//�萔�o�b�t�@�n�f�[�^
		struct ConstantData
		{
			//!Always��GPU�ɓn�����߂̃o�b�t�@
			CComPtr<ID3D11Buffer> pAlwaysGPU;
			//!Material��GPU�ɓn�����߂̃o�b�t�@
			CComPtr<ID3D11Buffer> pMaterialGPU;
			//!Sometime��GPU�ɓn�����߂̃o�b�t�@
			CComPtr<ID3D11Buffer> pSometimeGPU;
			//!Skinning��GPU�ɓn�����߂̃o�b�t�@
			CComPtr<ID3D11Buffer> pSkinningGPU;

			//!�萔���f�[�^
			AlwaysBuffer alwaysBuffer;
			//!�萔���f�[�^
			MaterialBuffer materialBuffer;
			//!�萔���f�[�^
			SkinningBuffer skinningBuffer;
		};

		//���\�[�X�ǂݍ��݁@���蓖�ėp�̖��O
		struct LoadNames {
			std::vector <std::string> textureNames;
			std::vector <std::string> motionNames;
		};

		struct ResourceData
		{
			//!�A�j���[�V�����f�[�^
			std::unique_ptr<BoneTexture> pBoneTexure;
			//!���f���f�[�^
			std::shared_ptr<Model> pModel;
			//!�e�N�X�`���f�[�^
			std::vector<std::shared_ptr<Texture2D>> pTextures;
		};

		/**
		* @brief					������
		*/
		void init();

		void setCubMap(const std::vector<std::shared_ptr<framework::CubeRenderTrget>>& cubMap);

		/**
		* @brief				�P�̃L���[�u�}�b�v�����ׂĂɔ��f������
		* @param instanceNum�@�@�`�悷��C���X�^���X�̐�
		* @param cubMap			���f���������L���[�u�e�N�X�`��
		*/
		void setCubMap(int instanceNum, const std::shared_ptr<framework::CubeTexture>& cubMap);

		/**
		* @brief				�}�e���A���̃p�����[�^�Z�b�g
		* @param param			�}�e���A���p�����[�^
		*/
		void setMaterialParam(MaterialParam& param);

		/**
		* @brief				�C���X�^���X���Ƃ̃A�j���[�^�[�Z�b�g
		* @param animators		�`�悷��I�u�W�F�N�g�̃C���X�^���X���ƃA�j���[�^�[
		*/
		void setAnimator(std::vector<std::weak_ptr<component::AnimatorComponent>> animators);


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

		/**
		* @brief			�e�N�X�`���Z�b�g
		*/
		void setTexture();

		/**
		* @brief			�e�N�X�`���Ƀ{�[���s�����������
		* @deteil			���\�[�X�̓ǂݍ��܂�鏇�Ԃ̊֌W�ŃR���X�g���N�^�ł͂ł��Ȃ�
		*/
		void writeBoneTexture(std::weak_ptr<component::AnimatorComponent>);

		/**
		* @brief			�A�j���[�V�����̃t���[�����Z�b�g
		*/
		void frameSet(const int instanceID);

		void clear()override;

	private:
		//!�ǂݍ��݁@���蓖�ĂɎg�����O
		LoadNames m_LoadName;
		//!�萔�o�b�t�@�n�f�[�^
		ConstantData m_ConstantData;
		//!���\�[�X�f�[�^
		ResourceData m_ResourceData;
		//!�V�F�[�_�[�Ƀe�N�X�`����z��œn�����߂̕ϐ�
		std::unique_ptr<ID3D11ShaderResourceView*[]> m_pShaderResource;
		CComPtr<ID3D11InputLayout> m_pInputLayout;
	
		std::shared_ptr<IMaterial> m_pMaterial;
		std::pair<int, std::unique_ptr<ID3D11ShaderResourceView*[]>> m_pCubMaps;
		util::Mat4 m_Projection;

		//!�C���X�^���X���ƃA�j���[�^�[
		std::vector<std::weak_ptr<component::AnimatorComponent>> m_Animators;

		int m_TextureNum = 0;
	};




}
