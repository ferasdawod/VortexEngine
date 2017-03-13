#pragma once

#include <memory>

#include <Engine/GameTimer.h>

class Logger;
class EventManager;
class SettingsManager;
class ResCache;
class ProfilingManager;
class ActorFactory;
class RenderManager;

namespace Core
{
	class Engine
	{
	public:
		Engine();
		~Engine();

		template <typename T>
		void Run();

	protected:
		bool Initialize();

	protected:
		GameTimer _gameTimer;

		std::shared_ptr<Logger> _pLogger;
		std::shared_ptr<EventManager>		_pEventManager;
		std::shared_ptr<ResCache>			_pResourceCache;
		std::shared_ptr<SettingsManager>	_pSettingsManager;
		std::shared_ptr<ProfilingManager>	_pProfilingManager;
		std::shared_ptr<ActorFactory>		_pActorFactory;
		std::shared_ptr<RenderManager>		_pRenderManager;
	};
}