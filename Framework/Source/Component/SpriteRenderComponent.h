#pragma once
#include<memory>
#include<Source\Component\Render2DComponent.h>

namespace framework {
	class SpriteRenderer;
	class Texture2D;
}

namespace component {

	class SpriteRenderComponent : public framework::Render2DComponent
	{
	public:
		SpriteRenderComponent();
		~SpriteRenderComponent();

		void init()override;

		void draw();

		/**
		* @brief		パラメータをセット
		* @param param  文字列パラメータ
		*/
		virtual void setParam(const std::vector<std::string>& param);

		void setAlpha(float alpha);

		std::string getSpriteName();

		const util::Vec2 getSize();

	private:
		std::unique_ptr<framework::SpriteRenderer> m_pRenderer;
		std::shared_ptr<framework::Texture2D> m_pTexture;
		std::string m_SpriteName;
		util::Vec2 m_Size;
	};


}