#pragma once
#include<list>
/**
* @file	    IRenderer.h
* @brief	単体描画クラスがすべて実装している
* @authro	高須優輝
* @date	2017/04/01
*/

namespace util {
	struct Transform;
}

namespace framework {

	class IRenderer
	{
	public:
		IRenderer() {};
		~IRenderer() {};


		/**
		* @brief					初期化
		*/
		virtual void init() {};

		/**
		* @brief					描画コールバック
		* @param drawTransList		描画に使うトランスフォームのリスト
		*/
		virtual void draw(util::Transform* drawTransList) = 0;

		/**
		* @brief				マテリアルの切り替え
		* @param material		切り替えるマテリアルオブジェクトの名前
		*/
		virtual void changeMaterial(const std::string& materialName) {};

	private:

	};
}