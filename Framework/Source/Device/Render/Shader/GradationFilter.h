#pragma once
#include<Source\Resource\Texture\RenderTarget.h>
#include<Source\Device\Render\Renderer\2D\Single\PostEffectRenderer.h>
#include<Source\Util\Type.h>
#include<memory>
#include<atlbase.h>

namespace framework {

	class Texture2D;

	class GradationFilter
	{
	public:
		GradationFilter(std::shared_ptr<Texture2D> tex2D);
		GradationFilter(std::shared_ptr<Texture2D> tex2D, float scale);
		~GradationFilter();

		/**
		* @brief				�����e�N�X�`���ɏ�������
		* @param source			�ڂ������e�N�X�`���̃V�F�[�_�[view
		* @param viewNum	    ���݃Z�b�g����Ă��郌���_�[�^�[�Q�b�g�̐�
		*/
		void write(int viewNum);

		/**
		* @brief				�`��
		*/
		void write();
		
		/**
		* @brief				�V�F�[�_�[view�擾
		*/
		ID3D11ShaderResourceView* getShaderView();

		/**
		* @brief				�`��
		*/
		void draw();

	private:
		//�O���f�[�V�����t�B���^�̒萔�o�b�t�@
		struct GradationConstant {
			XMMATRIX world;
			//!�s�N�Z���̃T�C�Y
			util::Vec2 pixSize;
			util::Vec2 damy;
		};


		std::unique_ptr<RenderTarget> m_Target;
		PostEffectRenderer<GradationConstant> renderer;
		std::shared_ptr<Texture2D> m_pTexture2D;
		GradationConstant m_Param;
		float m_Scale;

	};

}
