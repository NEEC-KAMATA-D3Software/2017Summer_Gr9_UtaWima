#pragma once
#include<Include\VORBIS\vorbis\vorbisfile.h>
#include <dsound.h>
#include<string>

namespace util {
	
	class OggLoader
	{
	public:
		OggLoader(const std::string& filePath);
		~OggLoader();

		/**
		* @brief		フォーマット取得
		*/
		const WAVEFORMATEX getFormat()const;

		/**
		* @brief		デコードサイズ取得
		*/
		const DWORD getDecodeSize()const;

		/**
		* @brief			  音源をデコードサイズごとに読み込み
		* @param totalSize    トータルのサイズ
		* @param pWriteBuffer 書き込みバッファ
		* @param 読み込みが終わったかどうか true 読み終了 false 読み込み終わってない
		*/
		bool readSound(DWORD* totalSize, LPVOID pWriteBuffer);

	private:
		/**
		* @brief		waveフォーマットを作成する
		*/
		void createWaveFormat(WAVEFORMATEX* formatResult);
	
		/**
		* @brief		情報取得用インターフェース取得
		*/
		void getInterface(vorbis_info**result);

	private:
		//!開いたoggのデータ
		OggVorbis_File m_OggFile;
		//!フォーマット構造体
		WAVEFORMATEX m_Format;
		//!デコード後のサイズ
		DWORD m_DecodeSize;
	};



}