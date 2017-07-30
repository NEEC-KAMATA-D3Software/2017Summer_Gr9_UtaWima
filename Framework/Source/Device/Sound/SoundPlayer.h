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
		* @brief	�T�E���h�f�o�C�X�̏�����
		*/
		void init(HWND hWnd);

		/**
		* @brief	�Đ�����T�E���h��؂�ւ�
		*/
		void playSound(const std::string& key);

		/**
		* @brief	�T�E���h�ǂݍ���
		*/
		void importResource(const std::string& key, const std::string& filePath);

		/**
		* @brief	�N���A
		*/
		void clear();

		bool isPlaying(const std::string& key);

	private:
		//!�T�E���h�f�o�C�X
		CComPtr<IDirectSound8> m_pSoundDevice;

		//!�T�E���h���\�[�X
		std::unordered_map<std::string, std::shared_ptr<SoundResource>> m_ResourceContainer;


	};



}