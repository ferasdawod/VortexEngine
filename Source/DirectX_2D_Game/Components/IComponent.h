#pragma once

#include <string>
#include <memory>
#include "Actors/ActorsDefenitions.h"

class TiXmlElement;
class GameTimer;

class IComponent
{
public:
	static const ComponentID INVALID_ID;

public:
	virtual ~IComponent() {}

public:
	virtual void							Initialize() = 0;
	virtual void							Initialize(TiXmlElement* xmlData) = 0;
	virtual void							OnUpdate(const GameTimer& gameTimer) = 0;

	virtual ComponentID						GetTypeID() const = 0;
	virtual ComponentID						GetUniqueID() const = 0;
	virtual const std::string&				GetName() const = 0;

	virtual bool							IsEnabled() const = 0;
	virtual void							SetEnabled(bool enabled) = 0;

	virtual void							SetOwner(StrongActorPtr owner) = 0;
	virtual StrongActorPtr					GetOwner() const = 0;
	virtual TiXmlElement*					ToXML() const = 0;
};

__declspec(selectany) const ComponentID IComponent::INVALID_ID = static_cast<ComponentID>(-1);
