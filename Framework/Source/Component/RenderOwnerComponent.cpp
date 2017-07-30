#include"RenderOwnerComponent.h"
#include<Source\Device\Render\Renderer\3D\Instance\RigidInstanceRenderer.h>
#include<Source\Device\Render\Renderer\3D\Instance\SkinningInstanceRenderer.h>
#include<Source\Entity\Entity.h>
#include<Source\Component\RenderClientComponent.h>
#include<algorithm>
#include<Source\Util\WrapFunc.h>
#include<Source\Device\Render\Renderer\3D\Instance\CubeMapInstanceRenderer.h>
#include<Source\Util\Render\RenderTargetStack.h>
#include<Source\Resource\Texture\CubeReanderTarget.h>
#include<Source\Resource\ResourceManager.h>
#include<Source\Device\Render\Renderer\3D\Instance\SkinningInstanceRenderer.h>
#include<Source\Device\Input\Input.h>

namespace component {

	RenderOwnerComponent::RenderOwnerComponent()
	{
	}

	RenderOwnerComponent::~RenderOwnerComponent()
	{
	}

	void RenderOwnerComponent::init()
	{
		m_Renderer->init();
		m_Renderer->setMaterialParam(m_MaterialParam);
		m_CubeMapRenderer->init();

		eachEntity([&](std::weak_ptr<framework::Entity> findEntity) {
			//あらかじめエンティティーとコンポーネントを取得しておく
			auto component = findEntity.lock()->getComponent<RenderClientComponent>();
			if (component.lock()->getModelName() != m_ModelName) return;
			m_DrawObject.emplace_back(std::make_pair(findEntity, component));
		});


		auto cubeTexture = framework::ResourceManager::getInstance()->getTextureCube("Environment");
		m_pCubTexture = cubeTexture;

		for (auto element : m_DrawObject) {
			auto anim = element.second.lock()->getAnimator();
			m_AnimatorContainer.emplace_back(anim);
		}

		auto skinningRenderer = std::dynamic_pointer_cast<framework::SkinningInstanceRenderer>(m_Renderer);
		if (skinningRenderer.get() != nullptr) {
			skinningRenderer->setAnimator(m_AnimatorContainer);
		}
	}

	void RenderOwnerComponent::draw()
	{
		//削除対象削除
		cheackDelete();
		auto&& drawList = getActiveTransform();
		auto activeElement = getActiveElement();
		std::vector<std::shared_ptr<framework::CubeRenderTrget>> cubTextures;
		cubTextures.reserve(activeElement.size());
		//for (auto& element : activeElement) {
		//	cubTextures.emplace_back(element.second.lock()->getCubeMap());
		//	auto map = element.second.lock()->getCubeMap();
		//	if (i == 0) {
		//		map->clear(util::Vec4(0,0,1,1));
		//	}
		//	if (i == 1) {
		//		map->clear(util::Vec4(0, 0, 1, 1));
		//	}
		//	if (i == 2) {
		//		map->clear(util::Vec4(1, 0, 1, 1));
		//	}

		//	i++;
		//}
		//m_Renderer->setCubMap(cubTextures);
		m_Renderer->setCubMap(drawList.size(), m_pCubTexture);

		m_Renderer->draw(drawList);
	}

	void RenderOwnerComponent::cubeMapCreate(std::list <std::weak_ptr<framework::Render3DComponent>> drawContainer)
	{
		auto elementList = getActiveElement();
		for (auto& elemet : elementList) {
			elemet.second.lock()->cubMapWriteBegin();

			for (auto& drawTask : drawContainer) {
				drawTask.lock()->cubeMapDraw(elemet.first.lock()->getTransform());
			}

			elemet.second.lock()->cubMapWriteEnd();
		}
	}

	void RenderOwnerComponent::cubeMapDraw(util::Transform* centerTransform)
	{
		//削除対象削除
		cheackDelete();
		auto&& drawList = getActiveTransform();
		m_CubeMapRenderer->setCenterTransform(centerTransform);
		m_CubeMapRenderer->draw(drawList);
	}

	void RenderOwnerComponent::setParam(const std::vector<std::string>& param)
	{
		paramCheck(param);

		const int textureNum = std::atoi(param[1].c_str());
		util::foreach(textureNum, [&](int i) {
			//モデルの名前は飛ばして読み込む
			m_TextureNames.emplace_back(param[i + 2]);
		});

		//モデルのタイプを見てレンダラを切り替える
		auto model = framework::ResourceManager::getInstance()->getModel(param[0]);
		if (model->type == framework::ModelType::rigid)
			m_Renderer = std::make_shared<framework::RigidInstanceRenderer>(model, m_TextureNames);
		else if (model->type == framework::ModelType::skin) {
			skinningSetting(param, model);
		}

		m_CubeMapRenderer = std::make_shared<framework::CubeMapInstanceRenderer>(param[0], m_TextureNames);
		m_ModelName = param[0];

		//!テクスチャーまでのオフセット（パラメータの最初の２つはモデルの名前とテクスチャの枚数）
		const int textureOffset = 2;

		m_MaterialParam.height = std::atof(param[textureNum + textureOffset].c_str());
		//!次のマテリアルパラメータ読み込み
		m_MaterialParam.specular = std::atof(param[textureNum + textureOffset + 1].c_str());
		//!次のマテリアルパラメータ読み込み
		m_MaterialParam.isNotLighting = std::atof(param[textureNum + textureOffset + 2].c_str());
	}

	std::string RenderOwnerComponent::getModelName()
	{
		return m_ModelName;
	}

	std::vector<std::string> RenderOwnerComponent::getTexNames()
	{
		return m_TextureNames;
	}

	std::vector<std::weak_ptr<RenderClientComponent>> RenderOwnerComponent::getClients()
	{
		//詰め替える
		std::vector<std::weak_ptr<RenderClientComponent>> result;
		for (auto& obj : m_DrawObject) {
			result.emplace_back(obj.second);
		}

		return result;
	}

	int RenderOwnerComponent::drawNum()
	{
		auto element = getActiveElement();
		return element.size();
	}

	void RenderOwnerComponent::clear()
	{
		m_TextureNames.clear();
		m_Renderer->clear();
		m_AnimatorContainer.clear();
	}

	void  RenderOwnerComponent::cheackEntity(std::function<void(DrawElement)> action)
	{
		std::list<util::Transform*> drawTrans;

		//!条件に合ったものをコピーするための変数
		DrawContainer copy;

		//描画条件を満たしたものを抽出
		std::copy_if(std::begin(m_DrawObject), std::end(m_DrawObject), std::back_inserter(copy),
			[&](DrawElement element) {
			return isOK(element);
		});

		for (auto& element : copy) {
			action(element);
		}
	}

	std::list<util::Transform*> RenderOwnerComponent::getActiveTransform()
	{
		std::list<util::Transform*> drawTrans;
		cheackEntity([&](DrawElement element) {
			drawTrans.emplace_back(element.first.lock()->getTransform());

		});

		return drawTrans;
	}

	std::list<DrawElement> RenderOwnerComponent::getActiveElement()
	{
		std::list<DrawElement> activeElement;
		cheackEntity([&](DrawElement element) {
			activeElement.emplace_back(element);
		});
		return activeElement;
	}

	bool RenderOwnerComponent::isOK(const DrawElement & element)
	{
		//描画コンポーネントがアクティブである
		if (element.second.lock()->isActive()) {
			return true;
		}

		return false;
	}

	void RenderOwnerComponent::cheackDelete()
	{
		//エンティティーまたはコンポーネントのポインタがなくなっていればリストから削除
		auto deleteList = std::remove_if(m_DrawObject.begin(), m_DrawObject.end(), [&](DrawElement element) {
			return element.first.expired() || element.second.expired();
		});

		//要素削除
		m_DrawObject.erase(deleteList, m_DrawObject.end());
	}

	void RenderOwnerComponent::eachEntity(std::function<void(std::weak_ptr<framework::Entity>)> action)
	{
		auto entityList = framework::Entity::findHaveComponentEntity<RenderClientComponent>();
		for (auto& entity : entityList) {
			action(entity);
		}
	}

	void RenderOwnerComponent::skinningSetting(const std::vector<std::string>& param, std::shared_ptr<framework::Model> model)
	{
		std::vector<std::string> motionNames;

		if (param.size() < 11)return;

		//モーションの名前取得
		util::foreach(param.size() - 10, [&](int i) {
			//モーションと同じ名前でモーションを登録する
			motionNames.emplace_back(param[i + 10]);
		});

		m_Renderer = std::make_shared<framework::SkinningInstanceRenderer>(model, m_TextureNames, motionNames);
	}


}