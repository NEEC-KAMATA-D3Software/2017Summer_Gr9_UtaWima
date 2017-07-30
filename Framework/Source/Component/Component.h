#pragma once

#include<memory>
#include<vector>
#include<string>
#include<functional>

/**
* @file	Component.h
* @brief	すべてのコンポーネントが継承されるクラス
* @authro	高須優輝
* @date	2017/02/23
*/

namespace framework {

	class Entity;

	using WeakEntity = std::weak_ptr<Entity>;

	class Component
	{
	public:
		Component();
		virtual ~Component();

		/**
		* @brief		ゲームオブジェクトに接続された瞬間
		*/
		virtual void onConect();

		/**
		* @brief						自分が実装されているEntityにコンポーネントを追加する
		* @param 	componentInitalizer	このvectorに格納されたコンポーネントは一斉に初期化される
		*/
		virtual void componentSet(std::vector<std::weak_ptr<Component>>* componentInitalizer);

		/**
		* @brief		初期化
		* @Detail		すべてのコンポーネントが一斉に呼ばれる
		*/
		virtual void init();

		/**
		* @brief		このコンポーントをアクティブにする
		*/
		virtual void active();

		/**
		* @brief		このコンポーントをディアクティブにする
		*/
		virtual void deActive();

		/**
		* @brief		アクティブかどうか
		*/
		bool isActive();

		/**
		* @brief		衝突したときの処理
		* @param other	衝突した相手オブジェクト
		*/
		virtual void onCollisionEnter(const std::weak_ptr<Entity>& other);

		/**
		* @brief		衝突している間の処理
		* @param other	衝突している相手オブジェクト
		*/
		virtual void onCollisionStay(const std::weak_ptr<Entity>& other);

		/**
		* @brief		衝突から抜けた時の処理
		* @param other	衝突していた相手オブジェクト
		*/
		virtual void onCollisionExit(const std::weak_ptr<Entity>& other);

		/**
		* @brief		自分が実装されているゲームオブジェクトをセットする
		*/
		void setGameObj(std::weak_ptr<Entity> entity);

		/**
		* @brief		自分が実装されているゲームオブジェクトを取得する
		*/
		std::weak_ptr<Entity> getGameObj();

		/**
		* @brief		パラメータをセット
		* @param param  文字列パラメータ
		*/
		virtual void setParam(const std::vector<std::string>& param);

		/**
		* @brief		コンポーネント生成した後に必要な関数まとめ
		* @param entity 自分が実装されているEntity
		*/
		void componentCreated(std::weak_ptr<Entity> entity);

		/**
		* @brief		開放処理
		*/
		virtual void remove();

		/**
		* @brief		呼び出し値取得
		*/
		int getCallOrder();

		/**
		* @brief		クリアー
		*/
		virtual void clear();

	protected:
		void paramCheck(const std::vector<std::string>& param);

		template<typename T>
		static void haveComponentEntityLoop(std::function<void(WeakEntity)> entityAction)
		{
			//カメラコンポーネントを持っているエンティティーをすべて探す
			auto entityList = framework::Entity::findHaveComponentEntity<T>();
			for (auto& entity : entityList) {
				entityAction(entity);
			}
		}

	protected:
		//!自分が実装されているゲームオブジェクト
		std::weak_ptr<Entity> m_Entity;
		//!処理をするかどうかかどうか
		bool m_isActive = true;
		//!数値が低いほど早く実行される
		float m_CallOrder = 0;
	};


}