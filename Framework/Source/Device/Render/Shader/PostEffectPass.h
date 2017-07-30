#pragma once
#include<list>
#include<memory>
#include"Source\Resource\Texture\RenderTarget.h"
#include<D3DX11.h>
#include<atlbase.h>
#include<vector>

namespace component {
	class PostEffectComponent;
}

namespace framework {

	class MultiRenderTarget;
	class MultiRenderTarget;

	class PostEffectPass
	{
	private:
		using EffectContainer = std::list<std::weak_ptr<component::PostEffectComponent>>;
	public:
		PostEffectPass(EffectContainer effects, std::shared_ptr<framework::MultiRenderTarget> gBuffer, std::shared_ptr<framework::MultiRenderTarget> lBuffer);
		~PostEffectPass();

		//テクスチャに書き込む処理
		void rendering();

		//最終結果の描画
		void draw();

	private:
		using TextureContainer = std::unordered_map<std::string, CComPtr<ID3D11ShaderResourceView>>;
		TextureContainer m_TextureContainer;
		EffectContainer m_EffectContainer;
		std::vector<std::shared_ptr<RenderTarget>> m_SwapTarget;
		std::shared_ptr<framework::MultiRenderTarget> m_pGBuffer;
		std::shared_ptr<framework::MultiRenderTarget> m_pLBuffer;
	};


}