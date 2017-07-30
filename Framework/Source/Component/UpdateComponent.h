#pragma once
#include"Component.h"

/**
* @file	UpdateComponent.h
* @brief	更新対象のコンポーネント
* @authro	高須優輝
* @date	2017/02/23
*/

namespace framework {

	class UpdateComponent : public Component
	{
	public:
		UpdateComponent();
		~UpdateComponent();

		/**
		* @brief		初期化
		*/
		virtual void init();

		virtual void setup() {}

		/**
		* @brief		更新
		*/
		virtual void update() = 0;

		/**
		* @brief		アクティブにする
		*/
		virtual void active() override;

		/**
		* @brief		ディアクティブにする
		*/
		virtual void deActive() override;

	};



}