#pragma once
#include"PhysicsComponent.h"
#include<Source\Util\Type.h>
#include<Source\Entity\Entity.h>

namespace component {

	class BoxRenderClientComponent;

	class CircleCollider : public framework::PhysicsComponent
	{
	public:
		CircleCollider();
		~CircleCollider();

		void active()override;

		void deActive()override;

		/**
		* @brief						自分が実装されているEntityにコンポーネントを追加する
		* @param 	componentInitalizer	このvectorに格納されたコンポーネントは一斉に初期化される
		*/
		virtual void componentSet(std::vector<std::weak_ptr<Component>>* componentInitalizer);

		/**
		* @brief		パラメータをセット
		* @param param  文字列パラメータ
		*/
		virtual void setParam(const std::vector<std::string>& param)override;

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

	private:
		//!きゅうの半径
		int m_Radius;
		util::Vec3 m_Offset;
		std::weak_ptr<BoxRenderClientComponent> m_BoxRender;

		//!どれくらい入り込んでいるか
		float m_EnterAmount;
	};



}