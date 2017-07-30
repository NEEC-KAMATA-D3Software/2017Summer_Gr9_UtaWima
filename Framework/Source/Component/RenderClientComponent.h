#pragma once
#include<Source\Component\Component.h>
#include<string>
#include<memory>
#include<Source\Component\Animation\AnimatorComponent.h>

/**
* @file	RigidInstanceRenderComponent.h
* @brief	インスタンス描画オーナーコンポーネントクラス
* @dital	このコンポーネントを実装しているオブジェクトはインスタンス描画される
* @authro	高須優輝
* @date	2017/03/13
*/

namespace framework {
	class CubeRenderTrget;
	class CubeDepthTarget;
}

namespace component {

	class RenderOwnerComponent;

	class RenderClientComponent:public framework::Component
	{
	public:
		RenderClientComponent();
		~RenderClientComponent();

		const std::string& getModelName();

		/**
		* @brief		初期化
		* @Detail		すべてのコンポーネントが一斉に呼ばれる
		*/
		virtual void init()override;

		/** 
		* @brief			パラメータセット
		* @param param		文字列パラメータ
		*/
		void setParam(const std::vector<std::string>& param)override;

		/**
		* @brief			このクライアントのキューブマップの書き込みを開始する
		*/
		void cubMapWriteBegin();

		/**
		* @brief			このクライアントのキューブマップの書き込みを終了
		*/
		void cubMapWriteEnd();

		std::shared_ptr<framework::CubeRenderTrget> getCubeMap();

		/**
		* @brief			このクライアントのキューブマップの書き込みを終了
		*/
		std::weak_ptr<AnimatorComponent> getAnimator();

	private:
		std::weak_ptr<component::RenderOwnerComponent> findModelOwner();

	private:
		std::shared_ptr<framework::CubeRenderTrget> m_pCubeTarget;
		std::shared_ptr<framework::CubeDepthTarget> m_pCubeDepthTarget;
		std::weak_ptr<AnimatorComponent> m_pAnimator;

		//!描画するモデルの名前
		std::string m_ModelName;
	};



}