#include"SoundPlayer.h"
#include<assert.h>

namespace framework {

	SoundPlayer::SoundPlayer()
	{
		IDirectSound8 *pDevice;

		DirectSoundCreate8(NULL, &pDevice, NULL);

		//ƒXƒ}[ƒgƒ|ƒCƒ“ƒ^‚É“n‚·
		m_pSoundDevice.Attach(pDevice);
	}

	SoundPlayer::~SoundPlayer()
	{
	}

	void SoundPlayer::init(HWND hWnd)
	{
		m_pSoundDevice->SetCooperativeLevel(hWnd, DSSCL_NORMAL);
	}

	void SoundPlayer::playSound(const std::string & key)
	{
		assert(m_ResourceContainer.find(key) != m_ResourceContainer.end() && "“o˜^‚³‚ê‚Ä‚¢‚Ü‚¹‚ñ");
		m_ResourceContainer[key]->play();
	}

	void SoundPlayer::importResource(const std::string & key, const std::string & filePath)
	{
		assert(m_ResourceContainer.find(key) == m_ResourceContainer.end() && "‚·‚Å‚É“o˜^‚³‚ê‚Ä‚¢‚Ü‚·");
		m_ResourceContainer[key] = std::make_shared<SoundResource>(m_pSoundDevice,filePath);
	}

	void SoundPlayer::clear()
	{
		m_ResourceContainer.clear();
	}

	bool SoundPlayer::isPlaying(const std::string& key)
	{
		assert(m_ResourceContainer.find(key) != m_ResourceContainer.end() && "“o˜^‚³‚ê‚Ä‚¢‚Ü‚¹‚ñ");
		return  !m_ResourceContainer[key]->isEnd();
	}

}

