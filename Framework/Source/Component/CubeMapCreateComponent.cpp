#include"CubeMapCreateComponent.h"
#include<Source\Device\Render\Renderer\3D\Instance\CubeMapInstanceRenderer.h>
#include<Source\Entity\Entity.h>
#include<algorithm>
#include<Source\Util\Math\Transform.h>
#include<Source\Util\Render\DXFunc.h>
#include<Source\Resource\Texture\CubeReanderTarget.h>
#include<Source\Resource\Texture\CubeDepthTarget.h>
#include<Source\Component\RenderClientComponent.h>
#include<Source\Component\RenderOwnerComponent.h>
#include<Source\Util\WrapFunc.h>

namespace component {
	CubeMapCreateComponent::CubeMapCreateComponent()
	{
	}

	CubeMapCreateComponent::~CubeMapCreateComponent()
	{
	}

	void CubeMapCreateComponent::init()
	{
		m_pCubMapRenderer->init();

		auto entityList = framework::Entity::findHaveComponentEntity<RenderClientComponent>();

		//あらかじめコンポーネントも取得しておく
		for (auto entity : entityList) {
			auto client = entity.lock()->getComponent<RenderClientComponent>();
			m_EntityList.emplace_back(std::make_pair(entity, client));
		}

		m_pCubeTarget = std::make_shared<framework::CubeRenderTrget>(256, 256, DXGI_FORMAT_R16G16B16A16_FLOAT);
		m_pCubeDepth = std::make_shared<framework::CubeDepthTarget>(256, 256, DXGI_FORMAT_R32_TYPELESS);
	}


	void CubeMapCreateComponent::draw()
	{
		util::getContext()->OMSetRenderTargets(1, m_pCubeTarget->getView(), m_pCubeDepth->getView());
		m_pCubeTarget->clear(util::Vec4(0.12, 0.2, 0.14, 1));
		m_pCubeDepth->clear();

		//有効な状態なものだけをリストアップ
		auto&& transList = findActiveObject();
		m_pCubMapRenderer->draw(transList);

		util::getContext()->OMSetRenderTargets(0, NULL, NULL);

	}

	void CubeMapCreateComponent::setParam(const std::vector<std::string>& param)
	{
		//モデルの名前を飛ばしてテクスチャの名前を格納
		std::vector<std::string> texNames;
		util::foreach(param.size() - 1, [&](int i) {
			texNames.emplace_back(param[i + 1]);
		});
		m_pCubMapRenderer = std::make_unique<framework::CubeMapInstanceRenderer>(param[0], texNames);
	}

	std::shared_ptr<framework::CubeRenderTrget> CubeMapCreateComponent::getCubMap()
	{
		return std::shared_ptr<framework::CubeRenderTrget>();
	}

	std::list<util::Transform*> CubeMapCreateComponent::findActiveObject()
	{
		EntityList copyList;
		std::copy_if(m_EntityList.begin(), m_EntityList.end(), std::back_inserter(copyList),
			[&](EntityAndComponent object) {
			return isCreate(object);
		});

		//有効状態のエンティティーからトランスフォームを取得する
		std::list<util::Transform*> result;
		for (auto object : copyList) {
			result.emplace_back(object.first.lock()->getTransform());
		}
		return result;
	}

	bool CubeMapCreateComponent::isCreate(EntityAndComponent object)
	{
		//TODO Entityがアクティブかどうか確認するのを後で追加する
		//object.first.lock()->
		if (object.second.lock()->isActive()) {
			return true;
		}
		return false;
	}

}