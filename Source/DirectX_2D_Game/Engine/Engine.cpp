#include "pch.h"

#include "Engine.h"

#include "Resources/ResCache.h"
#include "Performance/ProfilingManager.h"
#include <Application/SettingsManager.h>


namespace Core
{
	Engine::Engine()
	{
	}


	Engine::~Engine()
	{
	}


	template<typename T>
	void Engine::Run()
	{
	}


	bool Engine::Initialize()
	{
		_pProfilingManager.reset(DBG_NEW ProfilingManager);
		
		_pEventManager.reset(DBG_NEW EventManager(true));
		HandleRegistering(true);

		_pSettingsManager.reset(DBG_NEW SettingsManager);
		_pSettingsManager->Initialize("Settings.xml");

		_pResourceCache.reset(DBG_NEW ResCache);
		_pResourceCache->RegisterDefaultLoaders();

		return true;
	}
}
