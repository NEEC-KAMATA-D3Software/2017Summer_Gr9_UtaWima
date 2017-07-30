#pragma once
#include<Source\Component\Component.h>
#include<string>
#include<unordered_map>
#include<Source\Resource\Texture\RenderTarget.h>
#include<D3DX11.h>
#include<Source\Util\Type.h>

namespace framework {
	using TextureContainer = std::unordered_map<std::string, CComPtr<ID3D11ShaderResourceView>>;
}

namespace component {
 
	class PostEffectComponent : public framework::Component
	{
	public:
		PostEffectComponent() {};
		~PostEffectComponent() {};

		/**
		* @brief		�|�X�g�G�t�F�N�g�ŕK�v�ȃe�N�X�`����I���ł���
		* @param texContainer	�e�N�X�`���R���e�i
		*/
		virtual void selectTexture(framework::TextureContainer& texContainer) {};

		/**
		* @brief		�|�X�g�G�t�F�N�g������
		* @param mainTarget	���グ�Ă�`�挋��
		*/
		void doEffect(const util::Vec2& pos) {
			util::Transform trans(util::Vec3(pos.x,pos.y,0),util::Vec3(0,0,0),util::Vec3(1,1,1));
		
			effectDraw(&trans);
		};

		int getCallOrder() {
			return m_CallOrder;
		}

	private:
		virtual void effectDraw(util::Transform* trans) = 0;

	protected:
		//!�Ⴂ�قǐ�ɕ`�悳���
		unsigned int m_CallOrder;
		//!�V�F�[�_�[�r���[
		std::vector<ID3D11ShaderResourceView*> m_pShaderView;

	};

}