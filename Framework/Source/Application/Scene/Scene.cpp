#include"Scene.h"
#include<Source\Util\IO\CSVLoader.h>
#include<Source\Entity\Entity.h>
#include<Source\Util\WrapFunc.h>
#include<Source\Resource\ResourceManager.h>
#include<Source\Task\TaskManager.h>
#include<Source\Util\WrapFunc.h>
#include<Source\Component\ComponentInclude.h>
#include<Source\Component\BoxRenderOwnerComponent.h>
#include<memory>
#include<Source\Application\Application.h>
#include<Source\Device\Render\Renderer\3D\Instance\SkinningInstanceRenderer.h>
#include<Source\Component\Player\AttackComponent.h>
#include<Source\Component\Scene\Title\Menu.h>
#include<Source\Component\SceneChangeComponent.h>
#include<Source\Util\Math\Math.h>

namespace framework {

	const std::string Scene::m_LootPath = "Resource/Scene/";
	std::string	Scene::m_NextSceneName = "";

	Scene::Scene()
	{
		//テキストから生成するコンポーネントを登録
		m_Builder.registComponent("Test", ComponentBuilder::componentRegister<component::Test>());
		m_Builder.registComponent("RenderClientComponent", ComponentBuilder::componentRegister<component::RenderClientComponent>());
		m_Builder.registComponent("RenderOwnerComponent", ComponentBuilder::componentRegister<component::RenderOwnerComponent>());
		m_Builder.registComponent("SpriteRenderComponent", ComponentBuilder::componentRegister<component::SpriteRenderComponent>());
		m_Builder.registComponent("CameraContoroller", ComponentBuilder::componentRegister<component::CameraContoroller>());
		m_Builder.registComponent("DirectionalLightComponent", ComponentBuilder::componentRegister<component::DirectionalLightComponent>());
		m_Builder.registComponent("DebugContoroller", ComponentBuilder::componentRegister<component::DebugContoroller>());
		m_Builder.registComponent("PointLightComponent", ComponentBuilder::componentRegister<component::PointLightComponent>());
		m_Builder.registComponent("BoxRenderClientComponent", ComponentBuilder::componentRegister<component::BoxRenderClientComponent>());
		m_Builder.registComponent("DebugPointLightController", ComponentBuilder::componentRegister<component::DebugPointLightController>());
		m_Builder.registComponent("DebugParticl", ComponentBuilder::componentRegister<component::DebugParticl>());
		m_Builder.registComponent("CameraComponent", ComponentBuilder::componentRegister<component::CameraComponent>());
		m_Builder.registComponent("AnimatorComponent", ComponentBuilder::componentRegister<component::AnimatorComponent>());
		m_Builder.registComponent("PlayerComponent", ComponentBuilder::componentRegister<component::PlayerComponent>());
		m_Builder.registComponent("BoxCollider", ComponentBuilder::componentRegister<component::BoxCollider>());
		m_Builder.registComponent("CircleCollider", ComponentBuilder::componentRegister<component::CircleCollider>());
		m_Builder.registComponent("BloomEffect", ComponentBuilder::componentRegister<component::BloomEffect>());
		m_Builder.registComponent("CameraMove", ComponentBuilder::componentRegister<component::CameraMove>());
		m_Builder.registComponent("StageMotion", ComponentBuilder::componentRegister<component::StageMotion>());
		m_Builder.registComponent("RigidBody", ComponentBuilder::componentRegister<component::RigidBody>());
		m_Builder.registComponent("AttackComponent", ComponentBuilder::componentRegister<component::AttackComponent>());
		m_Builder.registComponent("TitleCharacter", ComponentBuilder::componentRegister<component::TitleCharacter>());
		m_Builder.registComponent("Menu", ComponentBuilder::componentRegister<component::Menu>());
		m_Builder.registComponent("Buttom", ComponentBuilder::componentRegister<component::Buttom>());
		m_Builder.registComponent("BlurEffect", ComponentBuilder::componentRegister<component::BlurEffect>());
		m_Builder.registComponent("FadeComponent", ComponentBuilder::componentRegister<component::FadeComponent>());
		m_Builder.registComponent("ReadyComponent", ComponentBuilder::componentRegister<component::ReadyComponent>());
		m_Builder.registComponent("RulesComponent", ComponentBuilder::componentRegister<component::RulesComponent>());
		m_Builder.registComponent("RankSortComponent", ComponentBuilder::componentRegister<component::RankSortComponent>());
		m_Builder.registComponent("NextButton", ComponentBuilder::componentRegister<component::NextButton>());
			

		
		
	}

	Scene::~Scene()
	{

	}

	void Scene::construction()
	{
		//システムオブジェクト作成
		createSystemObject();

		createEntity(m_CurrentScenePath);
		createComponent(m_CurrentScenePath);


		loadNextSceneName(m_CurrentScenePath);

		//コンポーネントをハードコーディングでセット
		util::foreach(m_CreatedComponent.size(), [&](int i) {
			m_CreatedComponent[i].lock()->componentSet(&m_CreatedComponent);
		});

	}

	void Scene::changeScene()
	{
		std::string next =  m_NextSceneName;
		setScenePath(next);
		clear();
		loadMaterial();
		resourceLoad();
		construction();
		init();
		framework::TaskManager::getInstance()->init();
	}


	void Scene::init()
	{
		Entity::initEntity();

		for (auto& component : m_CreatedComponent) {
			component.lock()->init();
		}
	}

	void Scene::reset()
	{
		Entity::allComponentClear();
		init();
	}

	void Scene::createSystemObject()
	{
		//システムで必須のオブジェクトを作成しておく
		auto entity = framework::Entity::createEntity("Renderer","System", util::Transform());
		auto component = entity.lock()->addComponent(std::make_shared<component::BoxRenderOwnerComponent>());
		component.lock()->componentCreated(entity);
		component.lock()->active();
		m_CreatedComponent.emplace_back(component);

		entity = framework::Entity::createEntity("GlobalEvent", "System", util::Transform());
		entity.lock()->addEvent("ChangeScene", [&]() {
			changeScene();
		});

		m_CreatedComponent.emplace_back(entity.lock()->addComponent<component::SceneChangeComponent>());
	}

	void Scene::resourceLoad()
	{
	
		//リソース読み込み
		SceneThread sceneThread(m_CurrentScenePath);
		sceneThread.start();

		util::SetMultiThreadFlag(TRUE);

		util::Timer timer(60 * 5);
		timer.init();

		while (!sceneThread.isEnd())
		{
			timer.update();

			if (timer.isEnd())
				timer.init();

			float scale = util::bezierCurve(timer.rate() , 1.0f,1.5f,1.0f);

			util::LoadAndDraw("Resource/Texture/Loading.png", util::Vec2(1050, 100), util::Vec2(scale, scale));
		}

		sceneThread.join();
		util::SetMultiThreadFlag(FALSE);


		m_CreateList = sceneThread.getCreateList();
		//auto createdComponent = sceneThread.getCreatedComponent();
	//	std::copy(createdComponent.begin(), createdComponent.end(), std::back_inserter(m_CreatedComponent));
	}

	void Scene::createEntity(const std::string & fileName)
	{
		util::CSVLoader entityCSVLoader(fileName + "/Entity.csv");
		auto entityList = entityCSVLoader.load();

		//CSVのコメント行削除
		entityList.erase(entityList.begin());


		//!一時的にエンティティを保持するためのコンテナ
		std::list<std::pair<std::string, std::weak_ptr<Entity>>> entityTempContainer;

		for (auto& list : entityList) {
			auto transform = util::Transform(util::atof<util::Vec3>(list, 3).reault,
				util::atof<util::Vec3>(list, 6).reault,
				util::atof<util::Vec3>(list, 9).reault);
			auto createdEntity = Entity::createEntity(list[1], list[2], transform);
			std::function<void()> func = (bool)std::atoi(list[0].c_str()) ? std::bind(&Entity::active, createdEntity.lock().get()) : std::bind(&Entity::deActive, createdEntity.lock().get());
			m_EntityActiveFunc.emplace_back(func);
			if (list.size() >= 13) {
				if (list[12] != "")
					entityTempContainer.emplace_back(std::make_pair(list[12], createdEntity));
			}
		}

		for (auto entity : entityTempContainer) {
			entity.second.lock()->setParent(Entity::findGameObj(entity.first));
		}

	}

	void Scene::createComponent(const std::string & fileName)
	{
		util::CSVLoader componentCSVLoader(fileName + "/Component.csv");
		auto componentList = componentCSVLoader.load();

		//CSVのコメント行削除
		componentList.erase(componentList.begin());
		componentList.reserve(componentList.size());
		for (auto& list : componentList) {

			//コンポーネントの名前をもとに生成
			auto component = m_Builder.create(list[1]);
			//接続したいオブジェクトを名前でさがす(名前かぶりはない)
			auto entity = Entity::findGameObj(list[2]);
			assert(!entity.expired() && "接続しようとしたゲームオブジェクトが存在しません");
			entity.lock()->addComponent(component);//ゲームオブジェクトにコンポーネント付与
			component->componentCreated(entity);
			//0以外だったらアクティブ
			if (std::atoi(list[0].c_str()) != 0)component->active();
			else component->deActive();
			m_CreatedComponent.emplace_back(component);
			//パラメータまでの情報を削除
			list.erase(list.begin());
			list.erase(list.begin());
			list.erase(list.begin());
			//パラメータがあればセットする
			if (!list.empty())
				component->setParam(list);
		}

		for (auto crete : m_CreateList) {
			auto&& entity =  Entity::findGameObj(crete.entityName);
			auto component =  m_Builder.create(crete.componentName);
			entity.lock()->addComponent(component);
			component->componentCreated(entity);
			component->active();
			m_CreatedComponent.emplace_back(component);
			component->setParam(crete.param);
		}


		//コンポーネントが作られたあとにエンティティのアクティブフラグを切り替えて
		//エンティティー優先にする
		for (auto func : m_EntityActiveFunc) {
			func();
		}

	}

	void Scene::loadNextSceneName(const std::string & fileName)
	{
		util::CSVLoader loader(fileName + "/NextScene.csv");
		auto data = loader.load();
		data.erase(data.begin());
		m_NextSceneName = data[0][0];
	}

	void Scene::clear()
	{
		framework::TaskManager::getInstance()->clear();
		ResourceManager::getInstance()->clear();
		Entity::clear();
		m_EntityActiveFunc.clear();
		m_CreatedComponent.clear();
	}

	void Scene::setScenePath(const std::string & path)
	{
		m_CurrentScenePath = m_LootPath + path;
	}

}