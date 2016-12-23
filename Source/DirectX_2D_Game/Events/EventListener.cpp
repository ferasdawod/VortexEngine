#include "pch.h"

#include "EventListener.h"

void EventListener::ToggleRegisteration(EventID id, bool isRegistering)
{
	if (isRegistering)
		EventManager::GetPtr()->AddListener(this, id);
	else
		EventManager::GetPtr()->RemoveListener(this, id);
}