#include "EventManager.h"

#include "Application/GameTimer.h"
#include "Debugging/Debug.h"

#include <sstream>

EventManager* EventManager::_spInstance = nullptr;
bool EventManager::_sbInitialized = false;

EventManager::EventManager(bool isGlobal /* = true */) : _activeQueue(0), _bGlobal(isGlobal)
{
	// if we want to have more than one event manager then this needs to go
	// and we should get a bool in the constructor stating if this is the global event manager
	if (isGlobal)
	{
		assert(_spInstance == nullptr);
		_spInstance = this;
		_sbInitialized = true;
	}
}

EventManager::~EventManager()
{
	if (_bGlobal)
	{
		assert(_spInstance == this);
		_spInstance = nullptr;
		_sbInitialized = false;
	}
}

bool EventManager::AddListener(EventListener* eventListener, EventID eventID)
{
	ListenersList& listeners = _listenersMap[eventID];
	auto it = listeners.begin();
	auto end = listeners.end();

	for (; it != end; it++)
	{
		EventListener* listener = *it;
		if (listener == eventListener)
		{
			return false;
		}
	}

	listeners.push_back(eventListener);
	return true;
}

bool EventManager::RemoveListener(EventListener* eventListener, EventID eventID)
{
	auto findIt = _listenersMap.find(eventID);
	if (findIt == _listenersMap.end())
		return false;

	ListenersList& listeners = findIt->second;
	for (auto it = listeners.begin(); it != listeners.end(); it++)
	{
		EventListener* listener = *it;
		if (listener == eventListener)
		{
			listeners.erase(it);
			return true;
		}
	}

	return false;
}

bool EventManager::QueueEvent(StrongEventDataPtr eventData)
{
	PROFILE("EventManager::QueueEvent");

	assert(_activeQueue >= 0);
	assert(_activeQueue < 2);

	EventID evID = eventData->GetID();

#if defined(DEBUG) || defined(_DEBUG)
	std::stringstream stream;
	stream << "Queuing Event : " << eventData->GetName() << " On Active Queue #" << _activeQueue << std::endl;
	stream << "Event Data : " << eventData->ToString();
	LOG_M(stream.str());
#endif

	auto findIt = _listenersMap.find(evID);
	if (findIt != _listenersMap.end())
	{
		_eventQueues[_activeQueue].push_back(eventData);
		return true;
	}

	return false;
}

bool EventManager::TriggerEvent(StrongEventDataPtr eventData)
{
	PROFILE("EventManager::TriggerEvent");

	assert(_activeQueue >= 0);
	assert(_activeQueue < 2);

	bool goingGood = false;

	auto findIt = _listenersMap.find(eventData->GetID());
	if (findIt != _listenersMap.end())
	{
		const ListenersList& listeners = findIt->second;
		for (auto it = listeners.begin();
			 it != listeners.end(); ++it)
		{
			EventListener* l = *it;
			auto handled = l->HandleEvent(eventData);
			goingGood = true;

			if (handled)
				break;
		}
	}

	return goingGood;
}

void EventManager::OnUpdate(const GameTimer& gameTimer)
{
	PROFILE("EventManager::OnUpdate");

	int currQueue = _activeQueue;
	_activeQueue = (++_activeQueue) % 2;
	
	assert(_eventQueues[_activeQueue].empty());

	while (!_eventQueues[currQueue].empty())
	{
		StrongEventDataPtr currEventData = _eventQueues[currQueue].front();
		_eventQueues[currQueue].pop_front();

		EventID evID = currEventData->GetID();
		auto findIt = _listenersMap.find(evID);
		if (findIt != _listenersMap.end())
		{
			const ListenersList& listeners = findIt->second;
			for (auto it = listeners.begin();
				 it != listeners.end(); it++)
			{
				EventListener* l = *it;
				auto handled = l->HandleEvent(currEventData);
				if (handled)
					break;
			}
		}
	}
}
