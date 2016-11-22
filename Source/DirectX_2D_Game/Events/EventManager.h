#pragma once

#include <queue>
#include <unordered_map>
#include <list>
#include <assert.h>

#include "IEventData.h"
#include "EventListener.h"

using EventsQueue = std::list<StrongEventDataPtr>;
using ListenersList = std::list<EventListener*>;
using EventListenersMap = std::unordered_map<EventID, ListenersList>;
class GameTimer;

class EventManager
{
public:
	EventManager(bool isGlobal);
	virtual ~EventManager();

	bool			AddListener(EventListener* eventListener, EventID eventID);
	bool			RemoveListener(EventListener* eventListener, EventID eventID);

	bool			QueueEvent(StrongEventDataPtr eventData);
	bool			TriggerEvent(StrongEventDataPtr eventData);

	void			OnUpdate(const GameTimer& gameTimer);

	static EventManager&				Get() { assert(_spInstance != nullptr); return *_spInstance; }
	static EventManager*				GetPtr() { assert(_spInstance != nullptr); return _spInstance; }
	static bool							Initialized() { return _sbInitialized; }

private:
	static EventManager*				_spInstance;
	static bool							_sbInitialized;

private:
	EventsQueue							_eventQueues[2];
	int									_activeQueue;

	EventListenersMap					_listenersMap;

	bool								_bGlobal;
};