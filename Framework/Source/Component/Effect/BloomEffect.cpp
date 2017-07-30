#include"BloomEffect.h"
#include<Source\Application\Screen\Screen.h>

namespace component {

	BloomEffect::BloomEffect()
	{
	}

	BloomEffect::~BloomEffect()
	{
	}

	void BloomEffect::init()
	{
		m_pRenderer = std::make_unique<framework::PostEffectRenderer<BloomParam>>("Bloom");

		BloomParam d;
		d.pixSize.x = 1.0f / framework::Screen::PIXEL_WIDTH;
		d.pixSize.y = 1.0f / framework::Screen::PIXEL_HEIGHT;
		m_pRenderer->setBuffer(d);
		m_pRenderer->setSize(util::Vec2(framework::Screen::WINDOW_WIDTH, framework::Screen::WINDOW_HEIGHT));
	}

	void BloomEffect::selectTexture(framework::TextureContainer & texContainer)
	{
		//‚·‚Ø‚«‚ã‚ç[‚ğ‚Ú‚©‚µˆ—‚·‚é1
	//	m_GradationFilter.write(texContainer["Specular"],1);

		auto gFilter1 = std::make_unique<framework::GradationFilter>(std::make_shared<framework::Texture2D>(framework::Screen::WINDOW_WIDTH,
			framework::Screen::WINDOW_HEIGHT,
			texContainer["Specular"]),4);
		gFilter1->write();

		auto gFilter2 = std::make_unique<framework::GradationFilter>(std::make_shared<framework::Texture2D>(framework::Screen::WINDOW_WIDTH,
			framework::Screen::WINDOW_HEIGHT,
			gFilter1->getShaderView()), 8);
		gFilter2->write();

		auto gFilter3 = std::make_unique<framework::GradationFilter>(std::make_shared<framework::Texture2D>(framework::Screen::WINDOW_WIDTH,
			framework::Screen::WINDOW_HEIGHT,
			gFilter2->getShaderView()), 16);
		gFilter3->write();

		util::setSingleViewPort(framework::Screen::PIXEL_WIDTH, framework::Screen::PIXEL_HEIGHT);
		std::vector<std::shared_ptr<framework::Texture2D>> textures;
		textures.emplace_back(std::make_shared<framework::Texture2D>(framework::Screen::PIXEL_WIDTH,
			framework::Screen::PIXEL_WIDTH, texContainer["Main"]) );
		textures.emplace_back(std::make_shared<framework::Texture2D>(framework::Screen::PIXEL_WIDTH,
			framework::Screen::PIXEL_WIDTH, gFilter3->getShaderView()));


		m_pRenderer->setTexture(textures);
	}

	void BloomEffect::effectDraw(util::Transform * trans)
	{
		m_pRenderer->draw(trans);
	}

}