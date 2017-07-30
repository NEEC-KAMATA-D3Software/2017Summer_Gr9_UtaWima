#pragma once
#include<Source\Application\Scene\SceneThread.h>
#include<Source\Component\ComponentBuilder.h>
#include<functional>
#include<list>

namespace framework {

	class Scene
	{
	public:
		Scene();
		~Scene();

		/**
		* @brief	シーンを構築する
		*/
		void construction();

		/**
		* @brief	リソース読み込み
		*/
		void resourceLoad();


		/**
		* @brief	シーンを変える
		*/
		void changeScene();

		/**
		* @brief	初期化
		*/
		void init();

		/**
		* @brief	リセットに必要な処理をする
		*/
		void reset();

		/**
		* @brief	シーンをクリア
		*/
		void clear();

		void setScenePath(const std::string& path);

	public:
		//!シーンフォルダのルートディレクトリのパス
		static const std::string m_LootPath;

	private:

		/**
		* @brief	システム上必須エンティティーとコンポーネントを作成する
		*/
		void createSystemObject();

		/**
		* @brief	ゲームオブジェクト作成
		*/
		void createEntity(const std::string & fileName);

		/**
		* @brief	コンポーネント作成
		*/
		void createComponent(const std::string & fileName);

		/**
		* @brief	次のシーンの名前読み込み
		*/
		void loadNextSceneName(const std::string & fileName);

	public:
		//!次のシーン名
		static  std::string	m_NextSceneName;

	private:
		ComponentBuilder m_Builder;
		//!初期化するためだけに保持
		std::vector<std::weak_ptr<Component>> m_CreatedComponent;
		std::list<std::function<void()>> m_EntityActiveFunc;
		SceneThread::CreateList m_CreateList;
		std::string m_CurrentScenePath;
	};
}