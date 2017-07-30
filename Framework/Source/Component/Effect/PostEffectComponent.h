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
		* @brief		ポストエフェクトで必要なテクスチャを選択できる
		* @param texContainer	テクスチャコンテナ
		*/
		virtual void selectTexture(framework::TextureContainer& texContainer) {};

		/**
		* @brief		ポストエフェクトをつける
		* @param mainTarget	作り上げてる描画結果
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
		//!低いほど先に描画される
		unsigned int m_CallOrder;
		//!シェーダービュー
		std::vector<ID3D11ShaderResourceView*> m_pShaderView;

	};

}