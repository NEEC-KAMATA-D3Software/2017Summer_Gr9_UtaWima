#pragma once
#include"Component.h"
#include<memory>
#include<list>
#include<utility>

/**
* @file		CubeMapCreateOwnerComponent.h
* @brief	Rendererクライアントを検索してキューブマップを作成する
* @author	高須優輝
* @date	2017/3/19
*/

namespace util {
	struct Transform;
}

namespace framework {
	class CubeMapInstanceRenderer;
	class Entity;
	class CubeRenderTrget;
	class CubeDepthTarget;
}

namespace component {
	class RenderClientComponent;

	using EntityAndComponent = std::pair<std::weak_ptr<framework::Entity>, std::weak_ptr<RenderClientComponent>>;
	using EntityList = std::list<EntityAndComponent>;

	class CubeMapCreateComponent : public framework::Component
	{
	public:
		CubeMapCreateComponent();
		~CubeMapCreateComponent();
		/**
		* @brief		初期化
		* @Detail		すべてのコンポーネントが一斉に呼ばれる
		*/
		virtual void init()override;

		virtual void draw();

		/**
		* @brief		パラメータをセット
		* @param param  文字列パラメータ
		*/
		virtual void setParam(const std::vector<std::string>& param)override;

		/**
		* @brief		キューブマップ取得
		*/
		std::shared_ptr<framework::CubeRenderTrget> getCubMap();

	private:
		/**
		* @brief		有効な状態のエンティティーを選別して返す
		*/
		std::list<util::Transform*> findActiveObject();

		/**
		* @brief		エンティティーの状態を確認してど作成する状態ならtrueを変えず
		*/
		bool isCreate(EntityAndComponent object);

	private:
		std::shared_ptr<framework::CubeRenderTrget> m_pCubeTarget;
		std::shared_ptr<framework::CubeDepthTarget> m_pCubeDepth;
		std::unique_ptr<framework::CubeMapInstanceRenderer> m_pCubMapRenderer;
		EntityList m_EntityList;
		

	};




}
