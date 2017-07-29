#pragma once

#include <Utilities/Singleton.h>

namespace irrklang
{
	class ISoundEngine;
}

namespace Core
{
	class AudioManager : public Singleton<AudioManager>
	{
	public:
		AudioManager();
		virtual ~AudioManager();
	
		bool Initialize();

		void Play2D(const std::string& audioFilePath);


	protected:
		irrklang::ISoundEngine* _pSoundEngine;

	};
}