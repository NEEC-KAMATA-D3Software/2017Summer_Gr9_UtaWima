#pragma once
#include<Source\Util\Thread\Thread.h>
#include<string>
#include<vector>
#include<unordered_map>
#include<Source\Util\IO\CSVLoader.h>
#include<Source\Component\Component.h>

/**
* @file	SceneThread.h
* @brief	シーンを変更するスレッド
* @author	高須優輝
* @date	2017/3/18
*/

namespace util {
	class ModelInfoLoader;
}

namespace framework {

	class SceneThread : public util::Thread
	{
	public:
		struct CreateInfo
		{
			std::string componentName;
			std::string entityName;
			std::vector<std::string> param;
		};

		using  CreateList = std::vector<CreateInfo>;

		/**
		* @brief	コンストラクタ
		*/
		SceneThread(const std::string& fileName);

		/**
		* @brief	デストラクタ
		*/
		~SceneThread();

		/**
		* @brief	スレッドの実行
		*/
		virtual void run() override;

		/**
		* @brief	モデルの読み込み
		*/
		void loadInfo();

		/**
		* @brief	テクスチャの読み込み
		*/
		void loadTexture();

		/**
		* @brief	キューブテクスチャの読み込み
		*/
		void loadTextureCube();

		/**
		* @brief	サウンド読み込み
		*/
		void loadSound();

		/**
		* @brief	読み込み処理の途中で作成したコンポーネントを取得
		* @detail	作られたコンポーネントのinitは一斉に呼ばれるべきなので一度保持されている
		*/
		std::vector<std::weak_ptr<Component>> getCreatedComponent();

		/**
		* @brief	読み込み処理の途中で作成が必要になったコンポーネントの名前取得
		*/
		CreateList getCreateList();

		bool isEnd();

	private:
		/**
		* @brief	モデル読み込み
		*/
		void loadModel(util::ModelInfoLoader& infoLoader,std::vector<std::vector<std::string>>& loadList,int location);

		/**
		* @brief	モーション読み込み
		*/
		void loadMotion(util::ModelInfoLoader& infoLoader,const std::string& path);

	private:
		std::string m_FileName;
		//!作られたコンポーネントのinitは一斉に呼ばれるべきなので一度保持されている
		std::vector<std::weak_ptr<Component>> m_ComponentList;
		
		//!作成してほしいコンポーネントの名前
		CreateList m_CreateList;

		bool m_IsEnd;

	};


}