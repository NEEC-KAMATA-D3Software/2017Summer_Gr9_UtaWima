#pragma once
#include<Source\Util\Thread\Thread.h>
#include<string>
#include<vector>
#include<unordered_map>
#include<Source\Util\IO\CSVLoader.h>
#include<Source\Component\Component.h>

/**
* @file	SceneThread.h
* @brief	�V�[����ύX����X���b�h
* @author	���{�D�P
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
		* @brief	�R���X�g���N�^
		*/
		SceneThread(const std::string& fileName);

		/**
		* @brief	�f�X�g���N�^
		*/
		~SceneThread();

		/**
		* @brief	�X���b�h�̎��s
		*/
		virtual void run() override;

		/**
		* @brief	���f���̓ǂݍ���
		*/
		void loadInfo();

		/**
		* @brief	�e�N�X�`���̓ǂݍ���
		*/
		void loadTexture();

		/**
		* @brief	�L���[�u�e�N�X�`���̓ǂݍ���
		*/
		void loadTextureCube();

		/**
		* @brief	�T�E���h�ǂݍ���
		*/
		void loadSound();

		/**
		* @brief	�ǂݍ��ݏ����̓r���ō쐬�����R���|�[�l���g���擾
		* @detail	���ꂽ�R���|�[�l���g��init�͈�ĂɌĂ΂��ׂ��Ȃ̂ň�x�ێ�����Ă���
		*/
		std::vector<std::weak_ptr<Component>> getCreatedComponent();

		/**
		* @brief	�ǂݍ��ݏ����̓r���ō쐬���K�v�ɂȂ����R���|�[�l���g�̖��O�擾
		*/
		CreateList getCreateList();

		bool isEnd();

	private:
		/**
		* @brief	���f���ǂݍ���
		*/
		void loadModel(util::ModelInfoLoader& infoLoader,std::vector<std::vector<std::string>>& loadList,int location);

		/**
		* @brief	���[�V�����ǂݍ���
		*/
		void loadMotion(util::ModelInfoLoader& infoLoader,const std::string& path);

	private:
		std::string m_FileName;
		//!���ꂽ�R���|�[�l���g��init�͈�ĂɌĂ΂��ׂ��Ȃ̂ň�x�ێ�����Ă���
		std::vector<std::weak_ptr<Component>> m_ComponentList;
		
		//!�쐬���Ăق����R���|�[�l���g�̖��O
		CreateList m_CreateList;

		bool m_IsEnd;

	};


}