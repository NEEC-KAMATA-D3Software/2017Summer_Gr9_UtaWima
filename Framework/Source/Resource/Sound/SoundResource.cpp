#include"SoundResource.h"
#include<Source\Util\IO\OggLoader.h>
#include<assert.h>

namespace framework {

	SoundResource::SoundResource(IDirectSound8* device, const std::string& filePath)
		:m_pDevice(device)
	{
		util::OggLoader loader(filePath);

		auto waveFormat = loader.getFormat();

		const DWORD decodeSize = loader.getDecodeSize();

		//�Z�J���_���o�b�t�@���쐬����
		createSecondaryBffer(device, decodeSize, waveFormat);

		decode(loader);
	}

	SoundResource::~SoundResource()
	{
	}

	void SoundResource::play()
	{
		if (isEnd()) {
			m_SecondBuffer->Play(0, 0, 0);
		}
		else {
			IDirectSoundBuffer *sSound;
			m_pDevice->DuplicateSoundBuffer(m_SecondBuffer.p, &sSound);

			auto copy = getSecoundBuffer(sSound);
			sSound->Release();
			copy->Play(0,0,0);
		}
	}

	void SoundResource::stop()
	{
		m_SecondBuffer->Stop();
	}

	bool SoundResource::isEnd()
	{
		DWORD status = 0;

		m_SecondBuffer->GetStatus(&status);

		return (status & DSBSTATUS_PLAYING) != 1;
	}

	void SoundResource::createSecondaryBffer(IDirectSound8 * device, const DWORD decodeSize, WAVEFORMATEX& format)
	{
		// �v���C�}���o�b�t�@�̍쐬
		DSBUFFERDESC desc;
		desc.dwSize = sizeof(DSBUFFERDESC);
		desc.dwFlags = DSBCAPS_GLOBALFOCUS;
		desc.dwBufferBytes = decodeSize;
		desc.dwReserved = 0;
		desc.lpwfxFormat = &format;
		desc.guid3DAlgorithm = GUID_NULL;

		IDirectSoundBuffer *pPrimaryBuffer;
		bool isError = device->CreateSoundBuffer(&desc, &pPrimaryBuffer, NULL);
		assert(!isError && "�v���C�}���o�b�t�@�쐬���s");

		IDirectSoundBuffer8* pSecondaryBuffer;
		isError = pPrimaryBuffer->QueryInterface(IID_IDirectSoundBuffer8, reinterpret_cast<LPVOID*>(&pSecondaryBuffer));
		assert(!isError && "�Z�J���_���o�b�t�@�̍쐬�Ɏ��s���܂���");

		//�����o�ɓn��
		m_SecondBuffer.Attach(pSecondaryBuffer);
		pPrimaryBuffer->Release();
	}

	void SoundResource::decode(util::OggLoader & loader)
	{
		//!�������ݐ�o�b�t�@
		LPVOID pWriteBuffer;
		DWORD lockSize = 0;
		DWORD totalSize = 0;

		bool isError = m_SecondBuffer->Lock(0, 0, &pWriteBuffer, &lockSize, NULL, NULL, DSBLOCK_ENTIREBUFFER);
		assert(!isError && "�o�b�t�@�̃��b�N�Ɏ��s���܂���");

		while (!loader.readSound(&totalSize, pWriteBuffer));

		isError = m_SecondBuffer->Unlock(pWriteBuffer, lockSize,NULL,0);
		assert(!isError && "�o�b�t�@�̃A�����b�N�Ɏ��s���܂���");

	}

	CComPtr<IDirectSoundBuffer8> SoundResource::getSecoundBuffer(IDirectSoundBuffer * sb)
	{

		IDirectSoundBuffer8* pSecondaryBuffer;
		bool isError = sb->QueryInterface(IID_IDirectSoundBuffer8, reinterpret_cast<LPVOID*>(&pSecondaryBuffer));
		assert(!isError && "�Z�J���_���o�b�t�@�̍쐬�Ɏ��s���܂���");

		//�����o�ɓn��
		m_SecondBuffer.Attach(pSecondaryBuffer);
		return m_SecondBuffer;
	}

}