#include "pch.h"


#include "AudioManager.h"
#include <irrKlang/include/irrKlang.h>


Core::AudioManager::AudioManager()
{
}


Core::AudioManager::~AudioManager()
{
	_pSoundEngine->drop();
}

bool Core::AudioManager::Initialize()
{
	_pSoundEngine = irrklang::createIrrKlangDevice();
	if (!_pSoundEngine)
	{
		LOG_E("Failed to create the sound device", 0);
		return false;
	}

	return true;
}

void Core::AudioManager::Play2D(const std::string& audioFilePath)
{
	_pSoundEngine->play2D(audioFilePath.c_str());
}