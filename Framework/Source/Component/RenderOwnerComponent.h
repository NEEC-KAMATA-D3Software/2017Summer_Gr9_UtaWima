#pragma once
#include<Source\Component\Render3DComponent.h>
#include<memory>
#include<list>
#include<functional>
#include<Source\Resource\Texture\CubeTexture.h>
#include<Source\Device\Render\Shader\IMaterial.h>

/**
* @file	RigidInstanceRenderComponent.h
* @brief	インスタンス描画オーナーコンポーネントクラス
* @dital	クライアントをインスタンス描画する
* @authro	高須優輝
* @date	2017/03/13
*/

namespace framework {
	class RigidInstanceRenderer;
	class CubeMapInstanceRenderer;
	class IInstanceRenderer;
	struct Model;
}

namespace util {
	struct Transform;
}

namespace component {

	class IInstanceRenderer;
	class RenderClientComponent;
	class AnimatorComponent;

	using DrawElement = std::pair<std::weak_ptr<framework::Entity>, std::weak_ptr<RenderClientComponent>>;
	using DrawContainer = std::list<DrawElement>;

	class RenderOwnerComponent : public framework::Render3DComponent
	{
	public:
		RenderOwnerComponent();
		~RenderOwnerComponent();

		/**
		* @brief			初期化
		*/
		void init()override;

		/**
		* @brief			1つのレンダーターゲットに描画
		*/
		virtual void draw() override;

		/**
		* @brief			キューブテクスチャ作成
		*/
		virtual void cubeMapCreate(std::list <std::weak_ptr<framework::Render3DComponent>> drawContainer) override;

		/**
		* @brief			キューブテクスチャに描画
		* @param
		*/
		virtual void cubeMapDraw(util::Transform* centerTransform)override;

		/**
		* @brief			パラメータセット
		* @param param		文字列パラメータ
		*/
		void setParam(const std::vector<std::string>& param)override;

		/**
		* @brief			モデルの名前取得
		*/
		std::string getModelName();

		/**
		* @brief			モデルのテクスチャの名前取得
		*/
		std::vector<std::string> getTexNames();

		/**
		* @brief			このレンダラーのクライアントをすべて取得
		*/
		std::vector<std::weak_ptr<RenderClientComponent>> getClients();

		/**
		* @brief			描画数取得
		*/
		int drawNum()override;

		void clear()override;

	private:
		/**
		* @brief			描画に不正なオブジェクトがないか調べるて問題トランスフォームだけ返す
		*/
		void  cheackEntity(std::function<void(DrawElement)> action);

		/**
		* @brief			有効なトランスフォーム取得
		*/
		std::list<util::Transform*> getActiveTransform();

		/**
		* @brief			有効な描画要素取得
		*/
		std::list<DrawElement> getActiveElement();

		/**
		* @brief			描画描画条件を満たしているかどうか
		*/
		bool isOK(const DrawElement& element);

		/**
		* @brief			弱参照が不正になものをリストから排除する
		*/
		void cheackDelete();

		void eachEntity(std::function<void(std::weak_ptr<framework::Entity>)> action);

	private:
		void skinningSetting(const std::vector<std::string>& param, std::shared_ptr<framework::Model> model);

	private:
		//!描画するモデルの名前
		std::string m_ModelName;
		//!描画するモデルのテクスチャの名前
		std::vector<std::string> m_TextureNames;
		//!通常のジオメトリレンダラ
		std::shared_ptr<framework::IInstanceRenderer> m_Renderer;
		//!6個のビューに描画されるレンダラ
		std::shared_ptr<framework::CubeMapInstanceRenderer> m_CubeMapRenderer;
		//!描画するトランスフォームリスト
		DrawContainer m_DrawObject;
		//!マテリアルのパラメータ
		framework::MaterialParam m_MaterialParam;
		//!クライアントのアニメーター
		std::vector<std::weak_ptr<AnimatorComponent>> m_AnimatorContainer;
		//テスト
		std::shared_ptr<framework::CubeTexture> m_pCubTexture;
	};



}