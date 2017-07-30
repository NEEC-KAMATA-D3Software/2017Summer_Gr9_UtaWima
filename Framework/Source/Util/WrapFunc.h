#pragma once
#include<functional>
#include<Source\Util\Type.h>
#include<string>
#include<vector>
#include<stdio.h>
#include<assert.h>
#include<algorithm>
#include<iterator>

/**
* @file	WrapFunc.h
* @brief	���b�v�֐��܂Ƃ�
* @authro	���{�D�P
* @date	2017/03/14
*/


namespace util {

	/**
	* @brief			for���̃��b�v
	* @param eachNum	����J��Ԃ���
	* @param action 	�w�肵���񐔕����s����֐�(i�͌��݂̉�)
	*/
	template<typename T = int>
	void foreach(int eachNum, const std::function<void(int i)>& action)
	{
		for (T i = 0; i != eachNum; ++i) {
			action(i);
		}

	}

	/**
	* @brief			for���̃��b�v
	* @param eachNum	����J��Ԃ���
	* @param action 	�w�肵���񐔕����s����֐�(i�͌��݂̉�)
	* @param escapNum 	�w�肵�����[�v��continue����(�����w��\)
	*/
	template<typename T = int>
	void foreach(int eachNum, const std::function<void(int i)>& action,int escapNum, ...)
	{
		for (T i = 0; i != eachNum; ++i) {
			if (i == escapNum)continue;
			action(i);
		}
	}

	template<typename T>
	struct atof;

	template<>
	struct atof<util::Vec2> {
		/**
		* @brief			vector<string>��float�^�ɒ���
		* @param str		�ϊ�����string vector�z��
		* @param offset 	vector�̂ǂ̃C���f�b�N�X����ϊ����邩�̃I�t�Z�b�g(�f�t�H���g = 0)
		*/
		atof(std::vector<std::string> str, int offset = 0) {
			assert(str.size() > 1 && "Vector�̃T�C�Y���s�����Ă��܂�");
			assert(str.size() > 1 + offset && "offset���T�C�Y�𒴂��Ă��܂�");
			reault = util::Vec2(std::atof(str[0 + offset].c_str()),
				std::atof(str[1 + offset].c_str()));
		}
		util::Vec2 reault;
	};

	template<>
	struct atof<util::Vec3> {
		/**
		* @brief			vector<string>��float�^�ɒ���
		* @param str		�ϊ�����string vector�z��
		* @param offset 	vector�̂ǂ̃C���f�b�N�X����ϊ����邩�̃I�t�Z�b�g(�f�t�H���g = 0)
		*/
		atof(std::vector<std::string> str, int offset = 0) {
			assert(str.size() > 2 && "Vector�̃T�C�Y���s�����Ă��܂�");
			assert(str.size() > 2 + offset && "offset���T�C�Y�𒴂��Ă��܂�");

			reault = util::Vec3(std::atof(str[0 + offset].c_str()),
				std::atof(str[1 + offset].c_str()),
				std::atof(str[2 + offset].c_str()));
		}
		util::Vec3 reault;
	};

	template<>
	struct atof<util::Vec4> {
		/**
		* @brief			vector<string>��float�^�ɒ���
		* @param str		�ϊ�����string vector�z��
		* @param offset 	vector�̂ǂ̃C���f�b�N�X����ϊ����邩�̃I�t�Z�b�g(�f�t�H���g = 0)
		*/
		atof(std::vector<std::string> str,int offset = 0) {
			assert(str.size() > 3 && "Vector�̃T�C�Y���s�����Ă��܂�");
			assert(str.size() > 3 + offset && "offset���T�C�Y�𒴂��Ă��܂�");

			reault = util::Vec4(std::atof(str[0 + offset].c_str()),
								std::atof(str[1 + offset].c_str()), 
								std::atof(str[2 + offset].c_str()), 
								std::atof(str[3 + offset].c_str()));
		}
		util::Vec4 reault;
	};	


	std::wstring StringToWString(const std::string& src);
}