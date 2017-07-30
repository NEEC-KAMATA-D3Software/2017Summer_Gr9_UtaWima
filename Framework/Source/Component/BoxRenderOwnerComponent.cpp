#include"BoxRenderOwnerComponent.h"
#include<Source\Util\Event\WindowEvent.h>
#include<../DXApp\resource.h>

namespace component {
	BoxRenderOwnerComponent::BoxRenderOwnerComponent()
		:m_CubeRenderer()
	{
	}

	BoxRenderOwnerComponent::~BoxRenderOwnerComponent()
	{
	}

	void BoxRenderOwnerComponent::init()
	{
		m_DrawObject.clear();

		m_CubeRenderer.init();

		eachEntity([&](std::weak_ptr<framework::Entity> findEntity) {
			//あらかじめエンティティーとコンポーネントを取得しておく
			auto component = findEntity.lock()->getComponent<BoxRenderClientComponent>();
			m_DrawObject.emplace_back(std::make_pair(findEntity, component));
		});

		util::WindowEvent::getInstance()->addEvent(ID_POLYGON, [&]() {
			m_CubeRenderer.onPolygon();
		});
		util::WindowEvent::getInstance()->addEvent(ID_WIER, [&]() {
			m_CubeRenderer.onWire();
		});
	}

	void BoxRenderOwnerComponent::draw()
	{
		cheackDelete();
		auto activeList = getActiveElement();
		std::vector<util::Vec3> colors;
		for (auto& client : activeList) {
			colors.emplace_back(client.second.lock()->getColor());
		}
		auto&& drawList = getActiveTransform();
		m_CubeRenderer.setColor(colors);
		m_CubeRenderer.draw(drawList);
	}

	void BoxRenderOwnerComponent::cubeMapCreate(std::list<std::weak_ptr<Render3DComponent>> BoxContainer)
	{
	}

	void BoxRenderOwnerComponent::cubeMapDraw(util::Transform * centerTransform)
	{
	}

	int BoxRenderOwnerComponent::drawNum()
	{
		auto element = getActiveElement();
		return element.size();
	}

	void BoxRenderOwnerComponent::setParam(const std::vector<std::string>& param)
	{
	}

	void BoxRenderOwnerComponent::cheackEntity(std::function<void(BoxElement)> action)
	{
		std::list<util::Transform*> drawTrans;

		//!条件に合ったものをコピーするための変数
		BoxContainer copy;

		//描画条件を満たしたものを抽出
		std::copy_if(std::begin(m_DrawObject), std::end(m_DrawObject), std::back_inserter(copy),
			[&](BoxElement element) {
			return isOK(element);
		});

		for (auto& element : copy) {
			action(element);
		}
	}

	std::list<util::Transform*> BoxRenderOwnerComponent::getActiveTransform()
	{
		std::list<util::Transform*> drawTrans;
		cheackEntity([&](BoxElement element) {
			drawTrans.emplace_back(element.second.lock()->getTransform());
		});

		return drawTrans;
	}

	std::list<BoxElement> BoxRenderOwnerComponent::getActiveElement()
	{
		std::list<BoxElement> activeElement;
		cheackEntity([&](BoxElement element) {
			activeElement.emplace_back(element);
		});
		return activeElement;
	}

	bool BoxRenderOwnerComponent::isOK(const BoxElement & element)
	{
		//描画コンポーネントがアクティブである
		if (element.second.lock()->isActive()) {
			return true;
		}

		return false;
	}

	void BoxRenderOwnerComponent::cheackDelete()
	{
		//エンティティーまたはコンポーネントのポインタがなくなっていればリストから削除
		auto deleteList = std::remove_if(m_DrawObject.begin(), m_DrawObject.end(), [&](BoxElement element) {
			return element.first.expired() || element.second.expired();
		});

		//要素削除
		m_DrawObject.erase(deleteList, m_DrawObject.end());
	}

	void BoxRenderOwnerComponent::eachEntity(std::function<void(std::weak_ptr<framework::Entity>)> action)
	{
		auto entityList = framework::Entity::findHaveComponentEntity<BoxRenderClientComponent>();
		for (auto& entity : entityList) {
			action(entity);
		}
	}

}