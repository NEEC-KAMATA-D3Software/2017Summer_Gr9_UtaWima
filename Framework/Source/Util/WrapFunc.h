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
* @brief	ラップ関数まとめ
* @authro	高須優輝
* @date	2017/03/14
*/


namespace util {

	/**
	* @brief			for分のラップ
	* @param eachNum	何回繰り返すか
	* @param action 	指定した回数文実行する関数(iは現在の回数)
	*/
	template<typename T = int>
	void foreach(int eachNum, const std::function<void(int i)>& action)
	{
		for (T i = 0; i != eachNum; ++i) {
			action(i);
		}

	}

	/**
	* @brief			for分のラップ
	* @param eachNum	何回繰り返すか
	* @param action 	指定した回数文実行する関数(iは現在の回数)
	* @param escapNum 	指定したループをcontinueする(複数指定可能)
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
		* @brief			vector<string>をfloat型に直す
		* @param str		変換するstring vector配列
		* @param offset 	vectorのどのインデックスから変換するかのオフセット(デフォルト = 0)
		*/
		atof(std::vector<std::string> str, int offset = 0) {
			assert(str.size() > 1 && "Vectorのサイズが不足しています");
			assert(str.size() > 1 + offset && "offsetがサイズを超えています");
			reault = util::Vec2(std::atof(str[0 + offset].c_str()),
				std::atof(str[1 + offset].c_str()));
		}
		util::Vec2 reault;
	};

	template<>
	struct atof<util::Vec3> {
		/**
		* @brief			vector<string>をfloat型に直す
		* @param str		変換するstring vector配列
		* @param offset 	vectorのどのインデックスから変換するかのオフセット(デフォルト = 0)
		*/
		atof(std::vector<std::string> str, int offset = 0) {
			assert(str.size() > 2 && "Vectorのサイズが不足しています");
			assert(str.size() > 2 + offset && "offsetがサイズを超えています");

			reault = util::Vec3(std::atof(str[0 + offset].c_str()),
				std::atof(str[1 + offset].c_str()),
				std::atof(str[2 + offset].c_str()));
		}
		util::Vec3 reault;
	};

	template<>
	struct atof<util::Vec4> {
		/**
		* @brief			vector<string>をfloat型に直す
		* @param str		変換するstring vector配列
		* @param offset 	vectorのどのインデックスから変換するかのオフセット(デフォルト = 0)
		*/
		atof(std::vector<std::string> str,int offset = 0) {
			assert(str.size() > 3 && "Vectorのサイズが不足しています");
			assert(str.size() > 3 + offset && "offsetがサイズを超えています");

			reault = util::Vec4(std::atof(str[0 + offset].c_str()),
								std::atof(str[1 + offset].c_str()), 
								std::atof(str[2 + offset].c_str()), 
								std::atof(str[3 + offset].c_str()));
		}
		util::Vec4 reault;
	};	


	std::wstring StringToWString(const std::string& src);
}