#include"SpriteRenderer.h"
#include<Source\Device\Render\Shader\StandardMaterial.h>
#include<Source\Resource\Texture\Texture2D.h>
#include<Source\Resource\ResourceManager.h>
#include<Source\Util\Math\Transform.h>
#include<Source\Util\WrapFunc.h>
#include<Source\Resource\Texture\MultiRenderTarget.h>
#include<Source\Application\Screen\Screen.h>

namespace framework {

	SpriteRenderer::SpriteRenderer()
		:ISpriteRenderer("SpriteMaterial")
	{
		m_Alpha = 1.0f;
	}

	SpriteRenderer::~SpriteRenderer()
	{
	}


	void SpriteRenderer::changeMaterial(const std::string & materialName)
	{
		m_pMaterial = ResourceManager::getInstance()->getMaterial(materialName);
	}

	void SpriteRenderer::setAlpha(float alpha)
	{
		m_Alpha = alpha;
	}

	void SpriteRenderer::begin()
	{
		m_pMaterial->active();
	}

	void SpriteRenderer::end()
	{
		m_pMaterial->deActive();
	}

	void SpriteRenderer::setPipeline()
	{
		AlwaysBuffer ab;

		ab.world = m_WoldMat.toXMMatrix();
		ab.alpha = m_Alpha;

		util::updateConstantBuffer<AlwaysBuffer>(m_pAlwaysBuffer.p, ab);

		SometimeBuffer sb;
		sb.projection = m_Projection.toXMMatrix();

		util::updateConstantBuffer<SometimeBuffer>(m_pSometimeBuffer.p, sb);
	}

}