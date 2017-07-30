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
		* @brief		  �����Đ�
		*/
		void play();

		/**
		* @brief		 �@�����X�g�b�v
		*/
		void stop();

		bool isEnd();

	private:
		/**
		* @brief		  �Z�J���_���o�b�t�@�쐬
		*/
		void createSecondaryBffer(IDirectSound8* device,const DWORD decodeSize ,WAVEFORMATEX& fomat);
	
		/**
		* @brief		  �f�[�^���f�R�[�h
		*/
		void decode(util::OggLoader& loader);

		CComPtr<IDirectSoundBuffer8> getSecoundBuffer(IDirectSoundBuffer *sb);

	private:
		//!�Z�J���_���o�b�t�@
		CComPtr<IDirectSoundBuffer8> m_SecondBuffer;

		IDirectSound8* m_pDevice;
		
	};

}