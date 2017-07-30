#pragma once
#include"IInstanceRenderer.h"
#include<vector>
#include<memory>
#include<D3DX11.h>
#include<Source\Util\Render\DXFunc.h>
#include<Source\Resource\Model\Model.h>
#include<Source\Util\Type.h>
#include<Source\Application\Screen\Screen.h>

/**
* @file	    CubeInstanceRenderer.h
* @brief	�L���[�u���C���X�^���X�`�悷��N���X
* @authro	���{�D�P
* @date	2017/04/04
*/

namespace util {
	struct Transform;
}

namespace framework {

	class IMaterial;

	class CubeInstanceRenderer : public IInstanceRenderer
	{
	public:

		//�X�V�p�x�̍����o�b�t�@
		struct AlwaysBuffer
		{
			XMMATRIX view;
			//!�ő�`�搔�̓V�F�[�_�[�ɋL�q����Ă���100
			XMMATRIX world[MAX_DRAW_NUM];
			//!�@����]�p�̍s��
			XMMATRIX rotate[MAX_DRAW_NUM];
		};

		//�X�V�p�x�̍����o�b�t�@
		struct AlwaysBuffer2
		{
			util::Vec4 color[MAX_DRAW_NUM];
		};

		//���܂�X�V���Ȃ��o�b�t�@
		struct SometimeBuffer
		{
			XMMATRIX proj;
		};

		struct BoxLayout {
			util::Vec3 position;
			util::Vec3 normal;
		};

		CubeInstanceRenderer();
		~CubeInstanceRenderer();

		/**
		* @brief					������
		*/
		void init();

		/**
		* @brief					�`������C���[�t���[���ɂ���
		*/
		void onWire();

		/**
		* @brief					�`����|���S���ɂ���
		*/
		void onPolygon();

		void setColor(std::vector<util::Vec3> colors);

	private:
		// IInstanceRenderer ����Čp������܂���
		virtual void instanceAction(int instanceNum, util::Transform * trans) override;

		// IInstanceRenderer ����Čp������܂���
		virtual void setPipeline(int drawNum) override;

	private:
		RigidModel m_Model;
		CComPtr<ID3D11InputLayout> m_pInputLayout;
		//!�X�V�p�x�������o�b�t�@
		CComPtr<ID3D11Buffer> m_pAlwaysBuffer;
		CComPtr<ID3D11Buffer> m_pAlwaysBuffer2;
		//!�X�V�p�x�̒Ⴂ�o�b�t�@
		CComPtr<ID3D11Buffer> m_pSometimeBuffer;
		std::shared_ptr<IMaterial> m_pMaterial;
		//!�X�V�p�ϐ�
		AlwaysBuffer m_AlwaysBuffer;
		AlwaysBuffer2 m_AlwaysBuffer2;

		std::vector<util::Vec3> m_Colors;

		bool isWire = true;

		util::Mat4 m_Projection;

	};

}