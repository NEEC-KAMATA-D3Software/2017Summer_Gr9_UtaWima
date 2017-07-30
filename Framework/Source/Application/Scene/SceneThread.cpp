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
		//���f���Ƃ͊֌W�̂Ȃ��e�N�X�`����ǂݍ���
		util::Thread textureThread([&]() {
			loadTexture();
			loadTextureCube();
		});


		util::Thread soundThread([&]() {loadSound(); });

		textureThread.start();
		soundThread.start();

		//���C���X���b�h�Ń��f���ǂݍ���
		loadInfo();

		textureThread.join();
		soundThread.join();

		m_IsEnd = true;
	}
	void SceneThread::loadInfo()
	{
		//�����ɕ����X���b�h����A�N�Z�X���Ȃ��悤�ɐ���
		util::CriticalSection cs;

		util::CSVLoader m_ModelCSVLoader(m_FileName + "/Model.csv");

		auto loadList = m_ModelCSVLoader.load();

		//CSV�̃R�����g�s�폜
		loadList.erase(loadList.begin());

		util::foreach(loadList.size(), [&](int i) {
			util::ModelInfoLoader infoLoader(loadList[i][1] + "/ModelInfo.csv");

			//���f���̓ǂݍ���
			loadModel(infoLoader, loadList, i);
			//���[�V�����t�@�C���ǂݍ���
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
		//�����ɕ����X���b�h����A�N�Z�X���Ȃ��悤�ɐ���
		util::CriticalSection cs;
		util::CSVLoader textureCSVLoader(m_FileName + "/Texture.csv");
		auto data = textureCSVLoader.load();

		//CSV�̃R�����g�s�폜
		data.erase(data.begin());

		for (auto& list : data) {
			ResourceManager::getInstance()->importTexture2D(list[0], list[1]);
		}
	}

	void SceneThread::loadTextureCube()
	{
		//�����ɕ����X���b�h����A�N�Z�X���Ȃ��悤�ɐ���
		util::CriticalSection cs;
		util::CSVLoader textureCSVLoader(m_FileName + "/CubeTexture.csv");
		auto data = textureCSVLoader.load();

		//CSV�̃R�����g�s�폜
		data.erase(data.begin());

		for (auto& list : data) {
			std::string texPath[6];
			util::foreach(6, [&](int i) {
				texPath[i] = list[i + 1];//key���΂��ăR�s�[����
			});
			ResourceManager::getInstance()->importTextureCube(list[0], texPath);
		}
	}

	void SceneThread::loadSound()
	{
		//�����ɕ����X���b�h����A�N�Z�X���Ȃ��悤�ɐ���
		util::CriticalSection cs;
		util::CSVLoader textureCSVLoader(m_FileName + "/Sound.csv");
		auto data = textureCSVLoader.load();

		//CSV�̃R�����g�s�폜
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
			//Key�̓��f���t�H���_�̖��O�Ɠ���
			ResourceManager::getInstance()->importRigidModel(loadList[location][0], loadList[location][1] + "/" + infoLoader.getModelNameToFBX());
		}
		else if (infoLoader.getType() == ModelType::skin) {
			//Key�̓��f���t�H���_�̖��O�Ɠ���
			ResourceManager::getInstance()->importSkinningModel(loadList[location][0], loadList[location][1] + "/" + infoLoader.getModelNameToFBX());
		}

		//�e�N�X�`���̐������
		util::foreach(infoLoader.getTextureNum(), [&](int j) {
			std::string texName = infoLoader.getTextureName(j);
			if (texName == "")return;//�e�N�X�`���̖��O��������Ώ������Ȃ�
									 //�e�N�X�`���̖��O�œo�^�����
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