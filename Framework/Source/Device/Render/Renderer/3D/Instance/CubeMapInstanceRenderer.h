#pragma once
#include<Source\Util\Render\DXFunc.h>
#include<Source\Resource\Model\Model.h>
#include<Source\Util\Type.h>
#include<list>
#include<vector>
#include<Source\Device\Render\Renderer\3D\Instance\IInstanceRenderer.h>

/**
* @file	    CubeMapRenderer.h
* @brief	���̃C���X�^���X���L���[�u�}�b�v�ɕ`�悷��N���X
* @authro	���{�D�P
* @date	2017/03/13
*/

#define VIEW_NUM 6

namespace util {
	struct Transform;
}

namespace framework {
	class IInstanceRenderer;
	class IMaterial;
	class Camera;

	class CubeMapInstanceRenderer : public IInstanceRenderer
	{
	public:

		//�X�V�p�x�̍����o�b�t�@
		struct AlwaysBuffer
		{
			XMMATRIX view[VIEW_NUM];
			//!�ő�`�搔�̓V�F�[�_�[�ɋL�q����Ă���100
			XMMATRIX world[MAX_DRAW_NUM];
			//!�@����]�p�̍s��
			XMMATRIX rotate[MAX_DRAW_NUM];
		};

		//���܂�X�V���Ȃ��o�b�t�@
		struct SometimeBuffer
		{
			XMMATRIX proj;
		};

		CubeMapInstanceRenderer(std::string modelName, std::vector <std::string> textureNames);
		~CubeMapInstanceRenderer();

		/**
		* @brief					������
		*/
		void init();

		/**
		* @brief					�L���[�u�}�b�v�̒��S���W���Z�b�g
		* @param centerPosition		�Z���^�[�|�W�V�����̓������g�����X�t�H�[��
		*/
		void setCenterTransform(util::Transform* centerPosition);

	private:

		// IInstanceRenderer ����Čp������܂���
		virtual void instanceAction(int instanceNum, util::Transform * trans) override;

		/**
		* @brief			�p�C�v���C���̐ݒ�
		*/
		void setPipeline(int drawNum);

		/**
		* @brief			�L���[�u�}�b�v�p�J�����̃A�b�v�f�[�g
		*/
		void cameraUpdate();

	private:
		std::string m_ModelName;
		std::vector <std::string> m_TextureNames;
		//!�V�F�[�_�[�Ƀe�N�X�`����z��œn�����߂̕ϐ�
		std::unique_ptr<ID3D11ShaderResourceView*[]> m_pShaderResource;
		std::shared_ptr<Model> m_Model;
		std::vector<std::shared_ptr<Texture2D>> m_Textures;
		CComPtr<ID3D11InputLayout> m_pInputLayout;
		//!�X�V�p�x�������o�b�t�@
		CComPtr<ID3D11Buffer> m_pAlwaysBuffer;
		//!�X�V�p�x�̒Ⴂ�o�b�t�@
		CComPtr<ID3D11Buffer> m_pSometimeBuffer;
		std::shared_ptr<IMaterial> m_pMaterial;
		//!�X�V�p�ϐ�
		AlwaysBuffer m_AlwaysBuffer;
		//!�L���[�u�}�b�v�p�̃J����
		std::unique_ptr<util::Mat4[]> m_ViewMat;
		//�L���[�u�}�b�v�̒��S�|�W�V����
		util::Transform* m_pCenterPosition;
		util::Mat4 m_Projection;

	};



}