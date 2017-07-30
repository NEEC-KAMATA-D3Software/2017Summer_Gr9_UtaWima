#pragma once
#include"Render3DComponent.h"
#include<Source\Device\Render\Renderer\3D\Instance\CubeInstanceRenderer.h>
#include<Source\Component\BoxRenderClientComponent.h>
#include<memory>
#include<Source\Entity\Entity.h>

namespace component {

	using BoxElement = std::pair<std::weak_ptr<framework::Entity>, std::weak_ptr<BoxRenderClientComponent>>;
	using BoxContainer = std::list<BoxElement>;

	class BoxRenderOwnerComponent : public framework::Render3DComponent
	{
	public:
		BoxRenderOwnerComponent();
		~BoxRenderOwnerComponent();

		/**
		* @brief		１つのレンダーターゲットに描画
		*/
		virtual void init();

		// Render3DComponent を介して継承されました
		virtual void draw() override;

		virtual void cubeMapCreate(std::list<std::weak_ptr<Render3DComponent>> BoxContainer) override;

		virtual void cubeMapDraw(util::Transform * centerTransform) override;

		virtual int drawNum() override;

		/**
		* @brief		パラメータをセット
		* @param param  文字列パラメータ
		*/
		virtual void setParam(const std::vector<std::string>& param);

	private:

		/**
		* @brief			描画に不正なオブジェクトがないか調べるて問題トランスフォームだけ返す
		*/
		void  cheackEntity(std::function<void(BoxElement)> action);

		/**
		* @brief			有効なトランスフォーム取得
		*/
		std::list<util::Transform*> getActiveTransform();

		/**
		* @brief			有効な描画要素取得
		*/
		std::list<BoxElement> getActiveElement();

		/**
		* @brief			描画描画条件を満たしているかどうか
		*/
		bool isOK(const BoxElement& element);

		/**
		* @brief			弱参照が不正になものをリストから排除する
		*/
		void cheackDelete();

		void eachEntity(std::function<void(std::weak_ptr<framework::Entity>)> action);

	private:
		framework::CubeInstanceRenderer m_CubeRenderer;

		//!描画するトランスフォームリスト
		BoxContainer m_DrawObject;

	};



}