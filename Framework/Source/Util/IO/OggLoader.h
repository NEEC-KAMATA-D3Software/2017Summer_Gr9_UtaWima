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
		* @brief		�t�H�[�}�b�g�擾
		*/
		const WAVEFORMATEX getFormat()const;

		/**
		* @brief		�f�R�[�h�T�C�Y�擾
		*/
		const DWORD getDecodeSize()const;

		/**
		* @brief			  �������f�R�[�h�T�C�Y���Ƃɓǂݍ���
		* @param totalSize    �g�[�^���̃T�C�Y
		* @param pWriteBuffer �������݃o�b�t�@
		* @param �ǂݍ��݂��I��������ǂ��� true �ǂݏI�� false �ǂݍ��ݏI����ĂȂ�
		*/
		bool readSound(DWORD* totalSize, LPVOID pWriteBuffer);

	private:
		/**
		* @brief		wave�t�H�[�}�b�g���쐬����
		*/
		void createWaveFormat(WAVEFORMATEX* formatResult);
	
		/**
		* @brief		���擾�p�C���^�[�t�F�[�X�擾
		*/
		void getInterface(vorbis_info**result);

	private:
		//!�J����ogg�̃f�[�^
		OggVorbis_File m_OggFile;
		//!�t�H�[�}�b�g�\����
		WAVEFORMATEX m_Format;
		//!�f�R�[�h��̃T�C�Y
		DWORD m_DecodeSize;
	};



}