#pragma once
#include<Source\Component\Component.h>
#include<Source\State\State.h>
#include<list>
#include<deque>

namespace framework {

	class PhysicsComponent : public Component
	{
	public:
		PhysicsComponent();

		~PhysicsComponent();

		/**
		* @brief		このコンポーントをアクティブにする
		*/
		virtual void active();

		/**
		* @brief		このコンポーントをディアクティブにする
		*/
		virtual void deActive();


		void init()override;

		/**
		* @brief			フラグの更新のみを行う
		*/
		void collisionUpdate();

		/**
		* @brief			何かしらとヒットしているかどうか
		*/
		bool isHit();

		/**
		* @brief			指定したオブジェクトとヒットしているかどうか
		* @param entity		あたっているか確かめたいオブジェクト
		*/
		bool isHit(std::weak_ptr<Entity> entity);

		void hitCheck(std::weak_ptr<PhysicsComponent> other);

		/**
		* @brief			ヒットオブジェクトに対してのデリゲート
		* @param hitFlag	このフラグがtrueの時はhitしてるものに対して行われる falseはその逆
		* @param action		操作する関数ポインタ
		*/
		void hitObjAction(bool hitFlag,std::function<void(std::weak_ptr<Entity>)> action);

		/**
		* @brief			判定を行うコールバック
		*/
		virtual bool hitCheckCallBack(std::weak_ptr<PhysicsComponent> other) = 0;

		/**
		* @brief			ヒットコンテナに非アクティブなものがあったら削除する
		*/
		void checkDelete();

	protected:
		/**
		* @brief			自分の型かチェック
		*/
		template<typename T>
		bool isMold(std::weak_ptr<PhysicsComponent> other,std::shared_ptr<T>* result) {
			if (other.expired()) {
				return false;
			}
			auto circleComp = std::dynamic_pointer_cast<T>(other.lock());
			*result = circleComp;
			//チェックした相手が自分と同じでなければ処理しない
			if (!circleComp.get()) {
				return false;
			}
			return true;
		}

	protected:
		using HitPair = std::pair<std::weak_ptr<PhysicsComponent>, bool>;
		using HitContainer = std::list<HitPair>;

		HitContainer m_HitContainer;
	private:
		//本体と相手
		using CallBackContainer = std::deque<std::pair<std::weak_ptr<Entity>, std::weak_ptr<Entity>>>;
		
		CallBackContainer onCollitionContainer;
		CallBackContainer onStayContainer;
		CallBackContainer onExitContainer;
	};



}