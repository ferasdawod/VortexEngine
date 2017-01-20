#pragma once

#include "IEventData.h"
#include <vector>

class EventListener
{
public:
	virtual ~EventListener() {}

	virtual bool	HandleEvent(StrongEventDataPtr eventData) = 0;
	virtual void	HandleRegistering(bool isRegistering) = 0;

protected:
	void			ToggleRegisteration(EventID id, bool isRegistering);
};