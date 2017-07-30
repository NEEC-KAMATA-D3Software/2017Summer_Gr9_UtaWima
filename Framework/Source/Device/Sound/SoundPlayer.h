#pragma once
#include<dsound.h>
#include<atlbase.h>
#include<Windows.h>
#include<unordered_map>
#include<Source\Resource\Sound\SoundResource.h>
#include<memory>

namespace framework {

	class SoundPlayer
	{
	public:
		SoundPlayer();
		~SoundPlayer();

		/**
		* @brief	サウンドデバイスの初期化
		*/
		void init(HWND hWnd);

		/**
		* @brief	再生するサウンドを切り替え
		*/
		void playSound(const std::string& key);

		/**
		* @brief	サウンド読み込み
		*/
		void importResource(const std::string& key, const std::string& filePath);

		/**
		* @brief	クリア
		*/
		void clear();

		bool isPlaying(const std::string& key);

	private:
		//!サウンドデバイス
		CComPtr<IDirectSound8> m_pSoundDevice;

		//!サウンドリソース
		std::unordered_map<std::string, std::shared_ptr<SoundResource>> m_ResourceContainer;


	};



}