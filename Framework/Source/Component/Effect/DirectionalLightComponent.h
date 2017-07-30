#pragma once

#include<Source\Component\Component.h>
#include<functional>
#include<memory>
#include<Source\Util\Type.h>

/**
* @file	DirectionalLightComponent.h
* @brief	エンティティーをディレクショナルライトにするコンポーネント
* @authro	高須優輝
* @date	2017/04/12
*/

namespace framework {
	class MSAAMultiRenderTarget;
}

namespace component {

	class CameraComponent;

	class DirectionalLightComponent : public framework::Component
	{
	public:
		DirectionalLightComponent();
		~DirectionalLightComponent();


		/**
		* @brief		このコンポーントをアクティブにする
		*/
		virtual void active()override;

		/**
		* @brief		このコンポーントをディアクティブにする
		*/
		virtual void deActive()override;

		/**
		* @brief						自分が実装されているEntityにコンポーネントを追加する
		* @param 	componentInitalizer	このvectorに格納されたコンポーネントは一斉に初期化される
		*/
		virtual void componentSet(std::vector<std::weak_ptr<Component>>* componentInitalizer) override;

		/**
		* @brief		ライトのポジションを取得
		*/
		static util::Vec3 getLightPosition();

		/**
		* @brief		ライトのカラー取得
		*/
		static util::Vec3 getLightColor();

		/**
		* @brief		ライトからのカメラを取得
		*/
		static std::weak_ptr<CameraComponent> getCamera();

		/**
		* @brief		ライトからのカメラを取得
		*/
		//static util::Transform* getTransform();

		

		/**
		* @brief		パラメータをセット
		* @param param  文字列パラメータ
		*/
		virtual void setParam(const std::vector<std::string>& param);
	private:

		/**
		* @brief		DirectionalLightComponentを持っているエンティティー取得
		*/
		static void haveDirectionalLightEntityLoop(std::function<void(framework::WeakEntity)> entityAction);

		/**
		* @brief		自分の色取得用
		*/
		util::Vec3 getMColor();
	private:
		//!１つしか存在しないライト
		static std::weak_ptr<DirectionalLightComponent> m_DerectionalLight;

		std::weak_ptr<CameraComponent> m_pCamera;

		util::Vec3 m_Color;
	};




}
