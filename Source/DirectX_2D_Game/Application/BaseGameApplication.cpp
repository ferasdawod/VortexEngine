#include "pch.h"

#include "BaseGameApplication.h"

#include "SettingsManager.h"
#include "Resources/ResCache.h"
#include "Performance/ProfilingManager.h"

#include "Resources/ResourceLoaders.h"

bool BaseGameApplication::Init()
{
	_pProfilingManager.reset(DBG_NEW ProfilingManager);

	_pEventManager.reset(DBG_NEW EventManager(true));
	HandleRegistering(true);

	_pSettingsManager.reset(DBG_NEW SettingsManager);
	_pSettingsManager->Initialize("Settings.xml");

	_pResourceCache.reset(DBG_NEW ResCache);
	std::shared_ptr<IResourceLoader> textureLoader(DBG_NEW Texture2DLoader);
	std::shared_ptr<IResourceLoader> meshLoader(DBG_NEW MeshLoader);

	_pResourceCache->RegisterLoader("*.png", textureLoader);
	_pResourceCache->RegisterLoader("*.jpg", textureLoader);
	_pResourceCache->RegisterLoader("*.dds", textureLoader);

	_pResourceCache->RegisterLoader("*.obj", meshLoader);
	_pResourceCache->RegisterLoader("*.fbx", meshLoader);
	_pResourceCache->RegisterLoader("*.blend", meshLoader);
	_pResourceCache->RegisterLoader("*.mesh", meshLoader);

	return true;
}

void BaseGameApplication::OnShutDown()
{
	// no need to register since the settings manager is getting destroyed with us
	// HandleRegistering(false);
	
	std::stringstream msg;
	msg << "Shutting Down The Game" << std::endl;
	msg << "Total Game Time = " << _gameTimer.GameTime() << " Seconds";
	LOG_M(msg.str());

	_pProfilingManager->DumpInfo();

	StrongEventDataPtr saveEvent(DBG_NEW Event_SavingSettings(_pSettingsManager));
	_pEventManager->TriggerEvent(saveEvent);
	if (!_pSettingsManager->SaveToFile())
		LOG_M("Failed to save the settings to file");
	
	StrongEventDataPtr closeEvent(DBG_NEW Event_ApplicationExiting());
	_pEventManager->TriggerEvent(closeEvent);
}

void BaseGameApplication::OnResize(int newWidth, int newHeight)
{
	if (EventManager::Initialized())
	{
		std::shared_ptr<Event_WindowResized> resizeEvent(DBG_NEW Event_WindowResized(newWidth, newHeight));
		EventManager::GetPtr()->QueueEvent(resizeEvent);
	}
}
