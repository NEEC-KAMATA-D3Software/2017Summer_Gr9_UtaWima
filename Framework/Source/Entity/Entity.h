#pragma once
#include<string>
#include"../Util/Math/Transform.h"
#include<string>
#include<list>
#include<algorithm>
#include<memory>
#include<iterator>
#include<vector>
#include<Source\Entity\ComponentStateManager.h>
#include<Source\Util\Template\Event.h>


/**
* @file	Entity.h
* @brief	ゲームオブジェクトを現すクラス
* @authro	高須優輝
* @date	2017/02/23
*/

namespace framework {

	class Component;
	class Entity
	{
	public:
		using EntityContainer = std::list<std::shared_ptr<Entity>>;
		Entity(std::string name, std::string tag, util::Transform transform);
		~Entity();

		/**
		* @brief	トランスフォームの取得
		* @return	トランスフォーム
		*/
		util::Transform* getTransform();

		/**
		* @brief	登録されている名前取得
		*/
		const std::string getName();

		/**
		* @brief	登録されているタグ取得
		*/
		const std::string getTag();


		/**
		* @brief	アクティブにする
		*/
		void active();

		/**
		* @brief	非アクティブにする
		*/
		void deActive();

		/**
		* @brief	アクティブかどうか
		*/
		bool isActive();

		/**
		* @brief		衝突時コールバック関数
		* @param other	衝突した相手オブジェクト
		* @detail		ここで実装されているコンポーネントに衝突を通知する
		*/
		void onCollisionEnter(const std::weak_ptr<Entity>& other);

		/**
		* @brief		衝突している間のコールバック関数
		* @param other	衝突している相手オブジェクト
		*/
		void onCollisionStay(const std::weak_ptr<Entity>& other);

		/**
		* @brief		衝突から抜けたとき
		* @param other	衝突していた相手オブジェクト
		*/
		void onCollisionExit(const std::weak_ptr<Entity>& other);


		/**
		* @brief				コンポーネントの追加
		* @param component		追加するコンポーネント
		* @return				追加したコンポーネントの弱参照
		*/
		std::weak_ptr<Component> addComponent(std::shared_ptr<Component> component);

		/**
		* @brief				イベント追加
		* @param key			イベントの名前
		* @param eventFunc		イベントが発生したときに呼ばれる関数
		*/
		void addEvent(const std::string& key, std::function<void(void)> eventFunc);

		/**
		* @brief				イベント発生
		* @param key			イベントの名前
		*/
		void onEvent(const std::string& key);

		/**
		* @brief				持っているイベントをすべて削除
		*/
		void clearEvent();

		/**
		* @brief				親オブジェクトをセット
		*/
		void setParent(std::weak_ptr<Entity> parent);

		/**
		* @brief				親オブジェクトを取得
		*/
		std::weak_ptr<Entity> getPearent();


		/**
		* @brief				子オブジェクトを条件検索
		*/
		WeakEntity findChildIf(std::function<bool(WeakEntity child)> action);


		static void initEntity();

		/**
		* @brief				エンティティー作成
		*/
		static std::weak_ptr<Entity> createEntity(const std::string& name, const std::string & tag, util::Transform trans);

		/**
		* @brief				名前検索
		*/
		static std::weak_ptr<Entity> findGameObj(const std::string& name);

		/**
		* @brief				タグ検索
		*/
		static std::list<std::weak_ptr<Entity>> findGameObjWithTags(const std::string& tag);

		/**
		* @brief				エンティティー削除
		*/
		static void removeEntity(Entity* entity);

	public:

		/**
		* @brief	コンポーネントの追加
		* @detail 	指定した型のコンポーネントを生成し登録する
		*/
		template<typename T>
		std::weak_ptr<T> addComponent()
		{
			std::shared_ptr<T> component = std::make_shared<T>();
			m_ComponentList.emplace_back(component);
			auto entity = Entity::findGameObj(m_Name);
			component->componentCreated(entity);
			component->active();
			return component;
		}

		/**
		* @brief	コンポーネントを一つ取得
		* @detail	複数あった場合は最初の一つを取得
		*/
		template<typename T>
		std::weak_ptr<T> getComponent()
		{
			//	当てはまるコンポーネントがあるか検索
			auto& it = std::find_if(std::begin(m_ComponentList), std::end(m_ComponentList),
				[&](std::shared_ptr<Component> component) {
				return dynamic_cast<T*>(component.get()) != nullptr;
			});

			if (it == std::end(m_ComponentList)) return std::weak_ptr<T>();

			//	ダウンキャストして返す
			return std::move(std::static_pointer_cast<T>(*it));
		}

		/**
		* @brief	コンポーネントを一つ取得
		* @detail	複数あった場合は最初の一つを取得
		*/
		template<typename T>
		std::weak_ptr<T> getComponent(Component *pom)
		{
			//	当てはまるコンポーネントがあるか検索
			auto& it = std::find_if(std::begin(m_ComponentList), std::end(m_ComponentList),
				[&](std::shared_ptr<Component> component) {
				return dynamic_cast<T*>(component.get()) == pom;
			});

			if (it == std::end(m_ComponentList)) return std::weak_ptr<T>();

			//	ダウンキャストして返す
			return std::move(std::static_pointer_cast<T>(*it));
		}

		/**
		* @brief	コンポーネントを持つものをすべて取得
		*/
		template<typename T>
		std::list<std::weak_ptr<T>> getComponents()
		{
			std::list<std::shared_ptr<Component>> copyList;
			//	当てはまるコンポーネントがあるか検索
			std::copy_if(m_ComponentList.begin(), m_ComponentList.end(), std::back_inserter(copyList),
				[&](std::shared_ptr<Component> component) {
				return dynamic_cast<T*>(component.get()) != nullptr;
			});

			//!最終的にキャストして返すための変数
			std::list<std::weak_ptr<T>> castList;

			for (auto component : copyList) {
				castList.emplace_back(std::static_pointer_cast<T>(component));
			}

			return castList;
		}

		/**
		* @brief	staticメンバー変数のクリア呼び出し
		*/
		static void clear();

		/**
		* @brief		指定したコンポーネントを持っているエンティティーを探す
		* @return       見つけたエンティティの入ったリスト
		*/
		template<typename T>
		static std::list<std::weak_ptr<Entity>> findHaveComponentEntity() {
			//!コピーした要素を入れる空のリスト
			std::list<std::weak_ptr<Entity>> copyList;

			std::copy_if(std::begin(m_EntityContainer), std::end(m_EntityContainer), std::back_inserter(copyList),
				[](std::shared_ptr<Entity> entity) {
				return entity->getComponent<T>().lock().get() != nullptr;
			});

			return copyList;
		}
		static void allComponentClear();
		
	private:
		/**
		* @brief	コンポーネントは消さずにポジションだけ初期化する
		*/
		void init();

		/**
		* @brief	コンポーネントのみクリアー
		*/
		void componentClear();

		/**
		* @brief	子オブジェクト追加
		*/
		void addChild(WeakEntity childEntity);
	private:
		using ComponentContainer = std::list<std::shared_ptr<Component>>;

		//!オブジェクトの名前
		std::string m_Name;

		//!オブジェクトのタグ
		std::string m_Tag;

		//!座標変換
		util::Transform m_pTransform;

		//!初期座標
		util::Transform m_InitTransform;

		ComponentContainer m_ComponentList;

		util::Event<std::string> m_Event;

		//!アクティブフラグ　
		bool m_IsActive = true;

		//!自身のアクティブフラグ切り替えじにコンポーネントの状態を保持させる
		ComponentStateManager m_ComponentStateManager;

		//!親オブジェクト
		WeakEntity m_pParent;

		//!子オブジェクト
		std::vector<WeakEntity> m_Child;

		//!エンティティー保存場所
		static EntityContainer m_EntityContainer;
	};
}