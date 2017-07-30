#pragma once
#include<list>
/**
* @file	    IRenderer.h
* @brief	�P�̕`��N���X�����ׂĎ������Ă���
* @authro	���{�D�P
* @date	2017/04/01
*/

namespace util {
	struct Transform;
}

namespace framework {

	class IRenderer
	{
	public:
		IRenderer() {};
		~IRenderer() {};


		/**
		* @brief					������
		*/
		virtual void init() {};

		/**
		* @brief					�`��R�[���o�b�N
		* @param drawTransList		�`��Ɏg���g�����X�t�H�[���̃��X�g
		*/
		virtual void draw(util::Transform* drawTransList) = 0;

		/**
		* @brief				�}�e���A���̐؂�ւ�
		* @param material		�؂�ւ���}�e���A���I�u�W�F�N�g�̖��O
		*/
		virtual void changeMaterial(const std::string& materialName) {};

	private:

	};
}