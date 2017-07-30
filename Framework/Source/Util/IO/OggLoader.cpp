#include"OggLoader.h"
#include<Include\VORBIS\vorbis\codec.h>
#include<assert.h>
#include<Source\Util\Win\WinFunc.h>

namespace util {

	OggLoader::OggLoader(const std::string& filePath)
	{
		//�J��
		bool isError = ov_fopen(filePath.c_str(), &m_OggFile);
		assert(!isError && "ogg�ǂݍ��ݎ��s���܂���");
		

		createWaveFormat(&m_Format);
	}

	OggLoader::~OggLoader()
	{
		//����
		ov_clear(&m_OggFile);
	}

	const WAVEFORMATEX OggLoader::getFormat() const
	{
		return m_Format;
	}

	const DWORD OggLoader::getDecodeSize() const
	{
		return m_DecodeSize;
	}

	bool OggLoader::readSound(DWORD* totalSize,LPVOID pWriteBuffer)
	{
		int bitstream;
		long readSize = ov_read(
			&m_OggFile,
			static_cast<char*>(pWriteBuffer) + *totalSize,
			m_DecodeSize - *totalSize,
			0,
			2,
			1,
			&bitstream);

		assert(readSize >= 0 && "readSize���}�C�i�X�ɂȂ�܂���");

		*totalSize += readSize;
		
		return readSize == 0;
	}

	void OggLoader::createWaveFormat(WAVEFORMATEX * formatResult)
	{
		vorbis_info *vi = nullptr;
		getInterface(&vi);

		formatResult->wFormatTag = WAVE_FORMAT_PCM;
		formatResult->nChannels = vi->channels;
		formatResult->nSamplesPerSec = vi->rate;
		formatResult->nAvgBytesPerSec = vi->rate * vi->channels * 2;
		formatResult->nBlockAlign = vi->channels * 2;
		formatResult->wBitsPerSample = 16;
		formatResult->cbSize = sizeof(WAVEFORMATEX);

		m_DecodeSize = static_cast<DWORD>(ov_pcm_total(&m_OggFile, -1)) * vi->channels * 2;
	}

	void OggLoader::getInterface(vorbis_info** result)
	{
		try {
			//�C���^�[�t�F�[�X���擾
			*result = ov_info(&m_OggFile, -1);

			if (*result == nullptr) {
				throw std::runtime_error("Ogg�t�@�C���̏��擾�Ɏ��s���܂���");
			}
		}
		catch (std::exception& e) {
			util::WinFunc::DrawMessageBox(NULL, "�G���[", "ogg�̏��擾���s");
		}
	}


}