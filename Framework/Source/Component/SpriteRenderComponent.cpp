#include"SpriteRenderComponent.h"
#include<Source\Device\Render\Renderer\2D\Single\SpriteRenderer.h>
#include<Source\Application\Screen\Screen.h>
#include<Source\Resource\Texture\Texture2D.h>
#include<Source\Resource\ResourceManager.h>
#include<Source\Entity\Entity.h>

namespace component {

	SpriteRenderComponent::SpriteRenderComponent()
	{
	}

	SpriteRenderComponent::~SpriteRenderComponent()
	{
	}

	void SpriteRenderComponent::init()
	{
		m_pRenderer = std::make_unique<framework::SpriteRenderer>();
		m_pTexture = framework::ResourceManager::getInstance()->getTexture2D(m_SpriteName);
		m_Size = m_pTexture->getSize();
		m_pRenderer->setSize(m_Size);
		m_pRenderer->setTexture(m_pTexture);
	}

	void SpriteRenderComponent::draw()
	{
		auto trans = m_Entity.lock()->getTransform();
		util::Transform drawTrans = *trans;
		drawTrans.m_Position = trans->m_Position + util::Vec3(m_Size.x / 2, m_Size.y / 2, 0);
		m_pRenderer->draw(&drawTrans);
	}

	void SpriteRenderComponent::setParam(const std::vector<std::string>& param)
	{
		//パラメータがあるかチェック
		paramCheck(param);
		m_SpriteName = param[0];

		m_CallOrder = 0;

		if (param.size() < 2)return;
		if (param[1] == "")return;

		m_CallOrder = atof(param[1].c_str());
	}

	void SpriteRenderComponent::setAlpha(float alpha)
	{
		m_pRenderer->setAlpha(alpha);
	}


	std::string SpriteRenderComponent::getSpriteName()
	{
		return m_SpriteName;
	}

	const util::Vec2 SpriteRenderComponent::getSize()
	{
		return m_Size;
	}

}