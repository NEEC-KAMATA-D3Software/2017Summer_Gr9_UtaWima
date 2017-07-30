#pragma once
#include"CSVLoader.h"
#include<string>
#include<vector>
#include<Source\Resource\Model\Model.h>

/**
* @file	ModelInfoLoader.h
* @brief	�Ǝ��K�i���f����ǂݍ��ރN���X
* @author	���{�D�P
* @date	2017/3/30
*/


namespace util {

	class ModelInfoLoader
	{
	public:
		/**
		* @brief		  �R���X�g���N�^�@
		* @param fileName ModelInfo.csv�̃t�@�C���p�X
		*/
		ModelInfoLoader(const std::string& fileName);
		~ModelInfoLoader();

		/**
		* @brief		  �g���q�����f���̖��O�擾
		*/
		std::string getModelNameToFBX();

		/**
		* @brief		  �g���q�Ȃ����f���̖��O�擾
		*/
		std::string getModelName();

		/**
		* @brief		  �g���q�t���e�N�X�`���̖��O���擾
		* @param location ���Ԗڂ̃e�N�X�`�����擾���邩
		*/
		std::string getTextureNameToPng(int location);


		/**
		* @brief		  �g���q�Ȃ��e�N�X�`���̖��O���擾
		* @param location ���Ԗڂ̃e�N�X�`�����擾���邩
		*/
		std::string getTextureName(int location);

		/**
		* @brief		  �e�N�X�`���̖����擾
		*/
		int getTextureNum();

		/**
		* @brief		 �ǂݎ�����f�[�^��vector�z��ŕԂ�
		*/
		std::vector<std::string> getVectorResource();

		/**
		* @brief		 ���f���̃^�C�v�擾
		*/
		framework::ModelType getType();

		std::vector<std::string> getMotionPath();

	private:
		/**
		* @brief		 �}�e���A���̃p�����[�^�ǂݍ���
		*/
		void paramLoad(std::vector<std::vector<std::string>>& param);

		/**
		* @brief		 ���f���^�C�v�ǂݍ���
		*/
		void typeLoad(std::vector<std::vector<std::string>>& param);

		/**
		* @brief		 ���[�V�����̃p�X�ǂݍ���
		*/
		void motionLoad(std::vector<std::vector<std::string>>& param);

	private:
		CSVLoader m_Loader;
		std::string m_ModelName;
		std::vector<std::string> m_TextureName;
		std::vector<std::string> m_MotionPath;
		std::vector<std::string> m_Data;
		std::string tyep;

	};


}
