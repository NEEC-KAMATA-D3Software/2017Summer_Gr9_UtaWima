#pragma once
#include"PhysicsComponent.h"
#include<Source\Util\Type.h>

namespace component {

	class BoxRenderClientComponent;

	class BoxCollider : public framework::PhysicsComponent
	{
	public:
		struct Rect;

		BoxCollider();
		~BoxCollider();

		/**
		* @brief						自分が実装されているEntityにコンポーネントを追加する
		* @param 	componentInitalizer	このvectorに格納されたコンポーネントは一斉に初期化される
		*/
		virtual void componentSet(std::vector<std::weak_ptr<Component>>* componentInitalizer);

		/**
		* @brief		パラメータをセット
		* @param param  文字列パラメータ
		*/
		virtual void setParam(const std::vector<std::string>& param);

		/**
		* @brief		衝突したときの処理
		* @param other	衝突した相手オブジェクト
		*/
		virtual void onCollisionEnter(const std::weak_ptr<framework::Entity>& other)override;

		/**
		* @brief		衝突している間の処理
		* @param other	衝突している相手オブジェクト
		*/
		virtual void onCollisionStay(const std::weak_ptr<framework::Entity>& other)override;

		/**
		* @brief		衝突から抜けた時の処理
		* @param other	衝突していた相手オブジェクト
		*/
		virtual void onCollisionExit(const std::weak_ptr<framework::Entity>& other)override;

		// PhysicsComponent を介して継承されました
		virtual bool hitCheckCallBack(std::weak_ptr<PhysicsComponent> other) override;

		/**
		* @brief		どれくらい入り込んでいるか
		*/
		const util::Vec3& getEnterAmount();

	private:
		/**
		* @brief		衝突しているかどうか
		*/
		bool isCollision(std::shared_ptr<BoxCollider>& other);

		void getRect(Rect* result);


	private:

		struct BoxCollisionDesc
		{
			//!トランスフォームのポジションからずらすためのオフセット
			util::Vec3 posOffset;
			//!各辺の長さ
			util::Vec3 edge;
		};

		struct Rect {
			float right, left, top, bottom, _near, _far;
		};

		/**
		* @brief		判定用矩形を作る
		*/
		void createRect(Rect* result, BoxCollider* obj);
	private:
		std::weak_ptr<BoxRenderClientComponent> m_BoxRender;
		BoxCollisionDesc m_Desc;
		//!どれくらい入り込んでいるか
		util::Vec3 m_EnterAmount;
	};



}