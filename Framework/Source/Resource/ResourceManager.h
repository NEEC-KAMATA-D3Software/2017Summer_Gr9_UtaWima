#pragma once
#include<unordered_map>
#include<memory>
#include<Source\Resource\Texture\CubeTexture.h>
#include<Source\Device\Sound\SoundPlayer.h>

/**
* @file	    ResourceManager.h
* @brief	リソース管理クラス
* @dital	シングルトンでリソースを提供する
* @authro	高須優輝
* @date	2017/3/9
*/

namespace framework {

	struct Model;
	class Texture2D;
	class IMaterial;
	class Motion;

	class ResourceManager
	{
	public:
		~ResourceManager();

		static ResourceManager* getInstance();

		void importRigidModel(const std::string& key, const std::string& filePath);

		void importSkinningModel(const std::string& key, const std::string& filePath);

		void importAnimation(const std::string& key, const std::string& filePath);

		void importTexture2D(const std::string& key, const std::string& filePath);

		void importTexture2D(const std::string& key, std::shared_ptr<Texture2D> texture2D);

		void importTextureCube(const std::string& key, const std::string filePath[6]);

		void importStandardMaterial(const std::string& key, const std::string& filePath);

		void importGeometryMaterial(const std::string& key, const std::string& filePath);

		void importTessellationMaterial(const std::string& key, const std::string& filePath);

		void removeTexture(const std::string& key);

		/**
		* @brief	   サウンドデバイスをセット
		* @soundDevice 生成済みのサウンドデバイス
		* @deteil	   ウィンドウハンドルが必要なので生成したデバイスをセットする必要がある
		*/
		void setSoundDevice(std::shared_ptr<SoundPlayer> soundDevice);

		/**
		* @brief				サウンド読み込み
		* @param key			登録するキー
		* @param filePath	    ファイルのパス
		*/
		void importSound(const std::string& key,const std::string& filePath);

		std::shared_ptr<Model> getModel(const std::string& key);

		std::shared_ptr<Texture2D> getTexture2D(const std::string& key);

		std::shared_ptr<CubeTexture> getTextureCube(const std::string& key);

		std::shared_ptr<IMaterial> getMaterial(const std::string& key);

		std::shared_ptr<Motion> getMotion(const std::string& key);

		void playSound(const std::string& key);

		bool isPlaying(const std::string& key);

		void clear();
	private:
		//インスタンス生成禁止
		ResourceManager();

	private:
		static std::shared_ptr<ResourceManager> instance;

		//剛体モデル用コンテナ
		using ModelContainer = std::unordered_map<std::string,std::shared_ptr<Model>>;
		using MotionContainer = std::unordered_map<std::string, std::shared_ptr<Motion>>;
		using Texture2DContainer = std::unordered_map<std::string, std::shared_ptr<Texture2D>>;
		using TextureCubeContainer = std::unordered_map<std::string, std::shared_ptr<CubeTexture>>;
		using MaterialContainer = std::unordered_map<std::string, std::shared_ptr<IMaterial>>;

		ModelContainer m_ModelContainer;
		MotionContainer m_MotionContainer;
		Texture2DContainer m_Texture2DContainer;
		TextureCubeContainer m_TextureCubeContainer;
		MaterialContainer m_MaterialContainer;
		std::shared_ptr<SoundPlayer> m_pSoundDevice;

	};



}