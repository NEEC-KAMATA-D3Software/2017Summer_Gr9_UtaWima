#pragma once
#include<Framework.h>
namespace framework {
	class Entity;
}

namespace component {
	class CircleCollider;

	class AttackComponent : public framework::UpdateComponent
	{
	public:
		AttackComponent();
		~AttackComponent();

		void init()override;

		/**
		* @brief						自分が実装されているEntityにコンポーネントを追加する
		* @param 	componentInitalizer	このvectorに格納されたコンポーネントは一斉に初期化される
		*/
		virtual void componentSet(std::vector<std::weak_ptr<Component>>* componentInitalizer)override;

		// UpdateComponent を介して継承されました
		virtual void update() override;

		/**
		* @brief		パラメータをセット
		* @param param  文字列パラメータ
		*/
		virtual void setParam(const std::vector<std::string>& param);


		/**
		* @brief		エフェクトが終了したか
		*/
		bool isEffectEnd();

	private:
		void addEvent();

	private:
		std::weak_ptr<framework::Entity> m_AttackEntity;
		std::weak_ptr<component::CircleCollider> m_Collider;
		//!攻撃判定のずらす量
		util::Vec3 m_Shift;
		float m_Radius = 50;
	};



}
