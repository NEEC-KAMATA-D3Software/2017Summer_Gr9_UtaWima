#pragma once
#include<dsound.h>
#include<atlbase.h>
#include<string>

namespace util {
	class OggLoader;
}

namespace framework {


	class SoundResource
	{
	public:
		SoundResource(IDirectSound8* device,const std::string& filePath);
		~SoundResource();

		/**
		* @brief		  音源再生
		*/
		void play();

		/**
		* @brief		 　音源ストップ
		*/
		void stop();

		bool isEnd();

	private:
		/**
		* @brief		  セカンダリバッファ作成
		*/
		void createSecondaryBffer(IDirectSound8* device,const DWORD decodeSize ,WAVEFORMATEX& fomat);
	
		/**
		* @brief		  データをデコード
		*/
		void decode(util::OggLoader& loader);

		CComPtr<IDirectSoundBuffer8> getSecoundBuffer(IDirectSoundBuffer *sb);

	private:
		//!セカンダリバッファ
		CComPtr<IDirectSoundBuffer8> m_SecondBuffer;

		IDirectSound8* m_pDevice;
		
	};

}