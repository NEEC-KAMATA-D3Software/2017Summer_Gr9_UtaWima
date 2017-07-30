#pragma once
#include"CSVLoader.h"
#include<string>
#include<vector>
#include<Source\Resource\Model\Model.h>

/**
* @file	ModelInfoLoader.h
* @brief	独自規格モデルを読み込むクラス
* @author	高須優輝
* @date	2017/3/30
*/


namespace util {

	class ModelInfoLoader
	{
	public:
		/**
		* @brief		  コンストラクタ　
		* @param fileName ModelInfo.csvのファイルパス
		*/
		ModelInfoLoader(const std::string& fileName);
		~ModelInfoLoader();

		/**
		* @brief		  拡張子つきモデルの名前取得
		*/
		std::string getModelNameToFBX();

		/**
		* @brief		  拡張子なしモデルの名前取得
		*/
		std::string getModelName();

		/**
		* @brief		  拡張子付きテクスチャの名前を取得
		* @param location 何番目のテクスチャを取得するか
		*/
		std::string getTextureNameToPng(int location);


		/**
		* @brief		  拡張子なしテクスチャの名前を取得
		* @param location 何番目のテクスチャを取得するか
		*/
		std::string getTextureName(int location);

		/**
		* @brief		  テクスチャの枚数取得
		*/
		int getTextureNum();

		/**
		* @brief		 読み取ったデータをvector配列で返す
		*/
		std::vector<std::string> getVectorResource();

		/**
		* @brief		 モデルのタイプ取得
		*/
		framework::ModelType getType();

		std::vector<std::string> getMotionPath();

	private:
		/**
		* @brief		 マテリアルのパラメータ読み込み
		*/
		void paramLoad(std::vector<std::vector<std::string>>& param);

		/**
		* @brief		 モデルタイプ読み込み
		*/
		void typeLoad(std::vector<std::vector<std::string>>& param);

		/**
		* @brief		 モーションのパス読み込み
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
