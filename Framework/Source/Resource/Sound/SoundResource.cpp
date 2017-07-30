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

		//セカンダリバッファを作成する
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
		// プライマリバッファの作成
		DSBUFFERDESC desc;
		desc.dwSize = sizeof(DSBUFFERDESC);
		desc.dwFlags = DSBCAPS_GLOBALFOCUS;
		desc.dwBufferBytes = decodeSize;
		desc.dwReserved = 0;
		desc.lpwfxFormat = &format;
		desc.guid3DAlgorithm = GUID_NULL;

		IDirectSoundBuffer *pPrimaryBuffer;
		bool isError = device->CreateSoundBuffer(&desc, &pPrimaryBuffer, NULL);
		assert(!isError && "プライマリバッファ作成失敗");

		IDirectSoundBuffer8* pSecondaryBuffer;
		isError = pPrimaryBuffer->QueryInterface(IID_IDirectSoundBuffer8, reinterpret_cast<LPVOID*>(&pSecondaryBuffer));
		assert(!isError && "セカンダリバッファの作成に失敗しました");

		//メンバに渡す
		m_SecondBuffer.Attach(pSecondaryBuffer);
		pPrimaryBuffer->Release();
	}

	void SoundResource::decode(util::OggLoader & loader)
	{
		//!書き込み先バッファ
		LPVOID pWriteBuffer;
		DWORD lockSize = 0;
		DWORD totalSize = 0;

		bool isError = m_SecondBuffer->Lock(0, 0, &pWriteBuffer, &lockSize, NULL, NULL, DSBLOCK_ENTIREBUFFER);
		assert(!isError && "バッファのロックに失敗しました");

		while (!loader.readSound(&totalSize, pWriteBuffer));

		isError = m_SecondBuffer->Unlock(pWriteBuffer, lockSize,NULL,0);
		assert(!isError && "バッファのアンロックに失敗しました");

	}

	CComPtr<IDirectSoundBuffer8> SoundResource::getSecoundBuffer(IDirectSoundBuffer * sb)
	{

		IDirectSoundBuffer8* pSecondaryBuffer;
		bool isError = sb->QueryInterface(IID_IDirectSoundBuffer8, reinterpret_cast<LPVOID*>(&pSecondaryBuffer));
		assert(!isError && "セカンダリバッファの作成に失敗しました");

		//メンバに渡す
		m_SecondBuffer.Attach(pSecondaryBuffer);
		return m_SecondBuffer;
	}

}