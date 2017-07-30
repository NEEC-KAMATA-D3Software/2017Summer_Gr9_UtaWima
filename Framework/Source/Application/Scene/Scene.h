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
		* @brief	�V�[�����\�z����
		*/
		void construction();

		/**
		* @brief	���\�[�X�ǂݍ���
		*/
		void resourceLoad();


		/**
		* @brief	�V�[����ς���
		*/
		void changeScene();

		/**
		* @brief	������
		*/
		void init();

		/**
		* @brief	���Z�b�g�ɕK�v�ȏ���������
		*/
		void reset();

		/**
		* @brief	�V�[�����N���A
		*/
		void clear();

		void setScenePath(const std::string& path);

	public:
		//!�V�[���t�H���_�̃��[�g�f�B���N�g���̃p�X
		static const std::string m_LootPath;

	private:

		/**
		* @brief	�V�X�e����K�{�G���e�B�e�B�[�ƃR���|�[�l���g���쐬����
		*/
		void createSystemObject();

		/**
		* @brief	�Q�[���I�u�W�F�N�g�쐬
		*/
		void createEntity(const std::string & fileName);

		/**
		* @brief	�R���|�[�l���g�쐬
		*/
		void createComponent(const std::string & fileName);

		/**
		* @brief	���̃V�[���̖��O�ǂݍ���
		*/
		void loadNextSceneName(const std::string & fileName);

	public:
		//!���̃V�[����
		static  std::string	m_NextSceneName;

	private:
		ComponentBuilder m_Builder;
		//!���������邽�߂����ɕێ�
		std::vector<std::weak_ptr<Component>> m_CreatedComponent;
		std::list<std::function<void()>> m_EntityActiveFunc;
		SceneThread::CreateList m_CreateList;
		std::string m_CurrentScenePath;
	};
}