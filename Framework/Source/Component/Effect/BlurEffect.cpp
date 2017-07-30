#include"BlurEffect.h"
#include<Source\Device\Render\Shader\GradationFilter.h>


namespace component {

	BlurEffect::BlurEffect()
	{
	}

	BlurEffect::~BlurEffect()
	{
	}

	void BlurEffect::init()
	{
		m_pRenderer = std::make_unique<framework::PostEffectRenderer<Blur>>("Blur");

		Blur d;
		d.pixSize.x = 1.0f / framework::Screen::PIXEL_WIDTH;
		d.pixSize.y = 1.0f / framework::Screen::PIXEL_HEIGHT;
		m_pRenderer->setBuffer(d);
		m_pRenderer->setSize(util::Vec2(framework::Screen::WINDOW_WIDTH, framework::Screen::WINDOW_HEIGHT));
	}

	void BlurEffect::selectTexture(framework::TextureContainer & texContainer)
	{

		auto gFilter1 = std::make_unique<framework::GradationFilter>(std::make_shared<framework::Texture2D>(framework::Screen::WINDOW_WIDTH,
			framework::Screen::WINDOW_HEIGHT,
			texContainer["Velocity"]), 4);
		gFilter1->write();

		auto gFilter2 = std::make_unique<framework::GradationFilter>(std::make_shared<framework::Texture2D>(framework::Screen::WINDOW_WIDTH,
			framework::Screen::WINDOW_HEIGHT,
			gFilter1->getShaderView()), 8);
		gFilter2->write();


		util::setSingleViewPort(framework::Screen::PIXEL_WIDTH, framework::Screen::PIXEL_HEIGHT);
		std::vector<std::shared_ptr<framework::Texture2D>> textures;
		textures.emplace_back(std::make_shared<framework::Texture2D>(framework::Screen::PIXEL_WIDTH,
			framework::Screen::PIXEL_WIDTH, texContainer["Main"]));
		textures.emplace_back(std::make_shared<framework::Texture2D>(framework::Screen::PIXEL_WIDTH,
			framework::Screen::PIXEL_WIDTH, gFilter2->getShaderView()));

		m_pRenderer->setTexture(textures);
	}

	void BlurEffect::effectDraw(util::Transform * trans)
	{
		m_pRenderer->draw(trans);
	}

}