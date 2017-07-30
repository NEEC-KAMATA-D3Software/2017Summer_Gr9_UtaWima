#pragma once
#include<Source\Component\Component.h>
#include<list>

namespace util {

	class Transform;

}

namespace framework {


	class Render3DComponent : public framework::Component
	{
	public:
		Render3DComponent();
		~Render3DComponent();

		/**
		* @brief		１つのレンダーターゲットに描画
		*/
		virtual void init();

		virtual void setup() {}

		/**
		* @brief		Taskリストに追加
		*/
		virtual void active() override;

		/**
		* @brief		Taskリストから削除
		*/
		virtual void deActive() override;

		/**
		* @brief		１つのレンダーターゲットに描画
		*/
		virtual void draw() = 0;

		/**
		* @brief				6つのターゲットに描画
		* @param drawContainer	キューブマップに書き込む内容
		*/
		virtual void cubeMapCreate(std::list <std::weak_ptr<Render3DComponent>> drawContainer) = 0;


		/**
		* @brief			キューブテクスチャに描画
		*/
		virtual void cubeMapDraw(util::Transform* centerTransform) = 0;

		/**
		* @brief			描画数取得
		*/
		virtual int drawNum() = 0;
	private:

	};

}