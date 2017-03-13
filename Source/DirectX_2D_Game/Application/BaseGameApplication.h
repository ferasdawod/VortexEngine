#pragma once

#include "BaseApplication.h"
#include "Debugging/Logger.h"

#include "Events/EventListener.h"

#include <memory>

class EventManager;
class SettingsManager;
class ResCache;
class ProfilingManager;

class BaseGameApplication : public BaseApplication, public EventListener
{
public:
	BaseGameApplication() : _bIsPaused(false), _bIsResizing(false) {}
	virtual ~BaseGameApplication() = default;

	virtual bool Run(HINSTANCE hInstace) override = 0;

	void			Pause() { _gameTimer.Stop(); _bIsPaused = true; }
	void			Resume() { _gameTimer.Start(); _bIsPaused = false; }

	bool			IsPaused() const { return _bIsPaused; }

protected:
	virtual bool Init() override;
	virtual bool LoadResources() override { return true; }

	virtual void OnUpdate() override {}
	virtual void OnRender() override {}

	virtual void OnResize(int newWidth, int newHeight) override;

	virtual void OnShutDown() override;

	virtual bool HandleEvent(StrongEventDataPtr eventData) { return false; }
	virtual void HandleRegistering(bool isRegistering) {}

protected:
	Logger								_logger;
	GameTimer							_gameTimer;

	bool								_bIsPaused;
	bool								_bIsResizing;

	std::shared_ptr<EventManager>		_pEventManager;
	std::shared_ptr<ResCache>			_pResourceCache;
	std::shared_ptr<SettingsManager>	_pSettingsManager;
	std::shared_ptr<ProfilingManager>	_pProfilingManager;
};