#pragma once

#include <memory>

#include <Engine/GameTimer.h>
#include <Events/EventListener.h>

class Logger;
class EventManager;
class SettingsManager;
class ResCache;
class ProfilingManager;
class ActorFactory;
class RenderManager;
class Level;
class InputDevice;

namespace Core
{
	class WindowsWindow;
	class Application;
	class GuiController;

	class Engine : EventListener
	{
	public:
		Engine();
		~Engine();

		void Run(std::unique_ptr<Application> application);

	protected:
		bool Initialize();
		void RegisterComponents();

		void Shutdown() const;

		void UpdateSystems(float deltaTime);
		void RunMainLoop();
		void Update(float deltaTime);

		void Render() const;

		// meta functions
		void Pause();
		void Resume();

		bool HandleEvent(StrongEventDataPtr eventData) override;
		void HandleRegistering(bool isRegistering) override;

	protected:
		GameTimer _gameTimer;
		bool _isPaused;
		bool _isRunning;
		int _updateRate;

		std::shared_ptr<Logger>				_pLogger;
		std::shared_ptr<EventManager>		_pEventManager;
		std::shared_ptr<WindowsWindow>		_pWindow;

		std::shared_ptr<ResCache>			_pResourceCache;
		std::shared_ptr<SettingsManager>	_pSettingsManager;
		std::shared_ptr<ProfilingManager>	_pProfilingManager;
		std::shared_ptr<ActorFactory>		_pActorFactory;

		std::shared_ptr<InputDevice>		_pInputDevice;
		std::unique_ptr<Application>		_pApplication;

		std::shared_ptr<Level>				_pLevel;
		std::shared_ptr<RenderManager>		_pRenderManager;

		std::shared_ptr<GuiController>			_pGuiController;
	};
}