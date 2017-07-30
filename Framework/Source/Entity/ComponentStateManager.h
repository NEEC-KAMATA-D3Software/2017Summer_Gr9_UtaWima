#pragma once
#include<unordered_map>
#include<Source\Component\Component.h>
#include<memory>


/**
* @file	ComponentStateManager.h
* @brief	コンポーネントの状態を記憶するクラス
* @authro	高須優輝
* @date	2017/02/23
*/

namespace framework {

	enum class ComponentState
	{
		deActive = 0,
		active = 1,
		notFind,
	};

	class ComponentStateManager
	{
	public:
		ComponentStateManager();
		~ComponentStateManager();

		/**
		* @brief			状態を保持させるコンポーネント登録
		* @param component  状態を保持させるコンポーネントの弱参照
		*/
		void registComponent(std::weak_ptr<Component> component);

		/**
		* @brief			状態を保持させたコンポーネント破棄と状態取得
		* @param component  状態を保持させたコンポーネントの弱参照
		* @return			状態
		*/
		ComponentState popComponentState(std::weak_ptr<Component> component);

		void clear();

	private:
		//コンポーネント状態キュー
		std::unordered_map<Component*,bool> m_ComponentMemory;
	};


}