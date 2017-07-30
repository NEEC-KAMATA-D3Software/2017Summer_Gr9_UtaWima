#pragma once

#include<Source\Component\UpdateComponent.h>
#include<Source\Util\Type.h>


namespace framework {
	class Entity;
}

namespace component {

	class BoxCollider;

	class RigidBody : public framework::UpdateComponent
	{
	public:
		enum class Direction
		{
			Up,
			Side,
			Under,
		};


		RigidBody();
		~RigidBody();

		/**
		* @brief		アクティブにする
		*/
		virtual void active() override;

		/**
		* @brief		ディアクティブにする
		*/
		virtual void deActive() override;

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
		* @brief		衝突したときの処理
		* @param other	衝突した相手オブジェクト
		*/
		virtual void onCollisionEnter(const std::weak_ptr<framework::Entity>& other);

		/**
		* @brief		衝突している間の処理
		* @param other	衝突している相手オブジェクト
		*/
		virtual void onCollisionStay(const std::weak_ptr<framework::Entity>& other);

		/**
		* @brief		衝突から抜けた時の処理
		* @param other	衝突していた相手オブジェクト
		*/
		virtual void onCollisionExit(const std::weak_ptr<framework::Entity>& other);


		// UpdateComponent を介して継承されました
		virtual void update() override;

	private:

		/**
		* @brief		コリジョンの外に追い出す
		*/
		void moveOut(const std::weak_ptr<framework::Entity>& other);
		
		/**
		* @brief		衝突した方向を判定する
		*/
		Direction DirDecision(util::Vec3& dir, const std::weak_ptr<framework::Entity>& other);


	private:
		std::weak_ptr<BoxCollider> m_pBocCollider;
		util::Vec3 m_UpVec = util::Vec3(0,1,0);
		float m_Gravity;
		const float m_G = 0.98;
	};


}