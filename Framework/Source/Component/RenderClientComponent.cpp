#include"RenderClientComponent.h"
#include"RenderOwnerComponent.h"
#include<Source\Entity\Entity.h>
#include<Source\Component\CubeMapCreateComponent.h>
#include<Source\Resource\Texture\CubeReanderTarget.h>
#include<Source\Resource\Texture\CubeDepthTarget.h>
#include<Source\Util\Render\DXFunc.h>
#include<Source\Application\Screen\Screen.h>
#include<algorithm>

namespace component {


	RenderClientComponent::RenderClientComponent()
	{
		m_pCubeTarget = std::make_shared<framework::CubeRenderTrget>(256, 256, DXGI_FORMAT_R16G16B16A16_FLOAT);
		m_pCubeDepthTarget = std::make_shared<framework::CubeDepthTarget>(256, 256, DXGI_FORMAT_R32_TYPELESS);
	}

	RenderClientComponent::~RenderClientComponent()
	{
	}

	const std::string & RenderClientComponent::getModelName()
	{
		return m_ModelName;
	}


	void RenderClientComponent::init()
	{
		bool isSkin = false;
#ifdef _MDEBUG
		auto renderer = framework::Entity::findGameObj("Renderer");
		auto owners = renderer.lock()->getComponents<RenderOwnerComponent>();

		auto find = std::find_if(owners.begin(), owners.end(), [&](std::weak_ptr<RenderOwnerComponent> owner) {
			return owner.lock()->getModelName() == m_ModelName;
		});
		assert(find != owners.end() && "モデルがcsvに登録されていません");
		auto model = framework::ResourceManager::getInstance()->getModel(m_ModelName);
		isSkin = model->type == framework::ModelType::skin;
#endif
	
		m_pAnimator = m_Entity.lock()->getComponent<AnimatorComponent>();
		if(isSkin)
			assert(!m_pAnimator.expired() && "アニメーターが必要です");
	}

	void RenderClientComponent::setParam(const std::vector<std::string>& param)
	{
		//描画するモデルの名前
		m_ModelName = param[0];
	}

	void RenderClientComponent::cubMapWriteBegin()
	{
		util::setSingleViewPort(m_pCubeTarget->getSize().x, m_pCubeTarget->getSize().y);
		util::getContext()->OMSetRenderTargets(1, m_pCubeTarget->getView(), m_pCubeDepthTarget->getView());
		m_pCubeTarget->clear(util::Vec4(0.0f, 0.0f, 0.0f, 1.0f));
		m_pCubeDepthTarget->clear();
	}

	void RenderClientComponent::cubMapWriteEnd()
	{
		util::setSingleViewPort(framework::Screen::PIXEL_WIDTH, framework::Screen::PIXEL_WIDTH);
		util::getContext()->OMSetRenderTargets(0, NULL ,NULL);
	}

	std::shared_ptr<framework::CubeRenderTrget> RenderClientComponent::getCubeMap()
	{
		return m_pCubeTarget;
	}

	std::weak_ptr<AnimatorComponent> RenderClientComponent::getAnimator()
	{
		return m_pAnimator;
	}

	std::weak_ptr<component::RenderOwnerComponent> RenderClientComponent::findModelOwner()
	{
		auto renderer = framework::Entity::findGameObj("Renderer");
		auto renderComponentList = renderer.lock()->getComponents<component::RenderOwnerComponent>();
		//自分のオーナーを探す
		auto findComponent = std::find_if(renderComponentList.begin(), renderComponentList.end(), [&](std::weak_ptr<component::RenderOwnerComponent> component) {
			return component.lock()->getModelName() == m_ModelName;
		});

		return *findComponent;
	}

}