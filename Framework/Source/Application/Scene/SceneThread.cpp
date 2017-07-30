#include"SceneThread.h"
#include<Source\Resource\ResourceManager.h>
#include<Source\Util\WrapFunc.h>
#include<Source\Entity\Entity.h>
#include<Source\Util\Thread\CriticalSection.h>
#include<Source\Util\IO\ModelInfoLoader.h>
#include<Source\Component\RenderOwnerComponent.h>
#include<Source\Util\Thread\CriticalSection.h>

namespace framework {


	SceneThread::SceneThread(const std::string& fileName)
		:m_FileName(fileName)
	{
		m_IsEnd = false;
	}

	SceneThread::~SceneThread()
	{
	}
	void SceneThread::run()
	{
		//モデルとは関係のないテクスチャを読み込む
		util::Thread textureThread([&]() {
			loadTexture();
			loadTextureCube();
		});


		util::Thread soundThread([&]() {loadSound(); });

		textureThread.start();
		soundThread.start();

		//メインスレッドでモデル読み込み
		loadInfo();

		textureThread.join();
		soundThread.join();

		m_IsEnd = true;
	}
	void SceneThread::loadInfo()
	{
		//同時に複数スレッドからアクセスしないように制限
		util::CriticalSection cs;

		util::CSVLoader m_ModelCSVLoader(m_FileName + "/Model.csv");

		auto loadList = m_ModelCSVLoader.load();

		//CSVのコメント行削除
		loadList.erase(loadList.begin());

		util::foreach(loadList.size(), [&](int i) {
			util::ModelInfoLoader infoLoader(loadList[i][1] + "/ModelInfo.csv");

			//モデルの読み込み
			loadModel(infoLoader, loadList, i);
			//モーションファイル読み込み
			loadMotion(infoLoader, loadList[i][1]);

			CreateInfo info;
			info.entityName = "Renderer";
			info.componentName = "RenderOwnerComponent";
			info.param = infoLoader.getVectorResource();

			m_CreateList.emplace_back(info);
		});

	}
	void SceneThread::loadTexture()
	{
		//同時に複数スレッドからアクセスしないように制限
		util::CriticalSection cs;
		util::CSVLoader textureCSVLoader(m_FileName + "/Texture.csv");
		auto data = textureCSVLoader.load();

		//CSVのコメント行削除
		data.erase(data.begin());

		for (auto& list : data) {
			ResourceManager::getInstance()->importTexture2D(list[0], list[1]);
		}
	}

	void SceneThread::loadTextureCube()
	{
		//同時に複数スレッドからアクセスしないように制限
		util::CriticalSection cs;
		util::CSVLoader textureCSVLoader(m_FileName + "/CubeTexture.csv");
		auto data = textureCSVLoader.load();

		//CSVのコメント行削除
		data.erase(data.begin());

		for (auto& list : data) {
			std::string texPath[6];
			util::foreach(6, [&](int i) {
				texPath[i] = list[i + 1];//keyを飛ばしてコピーする
			});
			ResourceManager::getInstance()->importTextureCube(list[0], texPath);
		}
	}

	void SceneThread::loadSound()
	{
		//同時に複数スレッドからアクセスしないように制限
		util::CriticalSection cs;
		util::CSVLoader textureCSVLoader(m_FileName + "/Sound.csv");
		auto data = textureCSVLoader.load();

		//CSVのコメント行削除
		data.erase(data.begin());
		for (auto& list : data) {
			ResourceManager::getInstance()->importSound(list[0], list[1]);
		}
	}

	std::vector<std::weak_ptr<Component>> SceneThread::getCreatedComponent()
	{
		return m_ComponentList;
	}

	SceneThread::CreateList SceneThread::getCreateList()
	{
		return m_CreateList;
	}

	bool SceneThread::isEnd()
	{
		return m_IsEnd;
	}

	void SceneThread::loadModel(util::ModelInfoLoader & infoLoader, std::vector<std::vector<std::string>>& loadList, int location)
	{
		if (infoLoader.getType() == ModelType::rigid) {
			//Keyはモデルフォルダの名前と同一
			ResourceManager::getInstance()->importRigidModel(loadList[location][0], loadList[location][1] + "/" + infoLoader.getModelNameToFBX());
		}
		else if (infoLoader.getType() == ModelType::skin) {
			//Keyはモデルフォルダの名前と同一
			ResourceManager::getInstance()->importSkinningModel(loadList[location][0], loadList[location][1] + "/" + infoLoader.getModelNameToFBX());
		}

		//テクスチャの数分回る
		util::foreach(infoLoader.getTextureNum(), [&](int j) {
			std::string texName = infoLoader.getTextureName(j);
			if (texName == "")return;//テクスチャの名前が無ければ処理しない
									 //テクスチャの名前で登録される
			ResourceManager::getInstance()->importTexture2D(texName, loadList[location][1] + "/" + infoLoader.getTextureNameToPng(j));
		});
	}

	void SceneThread::loadMotion(util::ModelInfoLoader & loader, const std::string& path)
	{
		auto localPath = loader.getMotionPath();
		if (localPath.empty())return;

		for (auto p : localPath) {
			auto motionPath = path + "/Motion/" + p + ".fbx";

			ResourceManager::getInstance()->importAnimation(p, motionPath);
		}
	}

}