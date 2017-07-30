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
		* @brief				内部テクスチャに書き込み
		* @param source			ぼかす元テクスチャのシェーダーview
		* @param viewNum	    現在セットされているレンダーターゲットの数
		*/
		void write(int viewNum);

		/**
		* @brief				描画
		*/
		void write();
		
		/**
		* @brief				シェーダーview取得
		*/
		ID3D11ShaderResourceView* getShaderView();

		/**
		* @brief				描画
		*/
		void draw();

	private:
		//グラデーションフィルタの定数バッファ
		struct GradationConstant {
			XMMATRIX world;
			//!ピクセルのサイズ
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
