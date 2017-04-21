#pragma once

#include <string>
#include "Actors/ActorsDefenitions.h"
#include <Utilities/UniqueObject.h>
#include <Serialization/PropertyBag.h>

class TiXmlElement;
class GameTimer;

class IComponent : public UniqueObject, public PropertyBag
{
public:
	virtual ~IComponent() {}

	// initialize the component properties with default values
	virtual void Initialize() = 0;
	// initialize the component properties with values from the XML element
	virtual void Initialize(TiXmlElement* xmlData) = 0;
	// update the component each frame
	virtual void OnUpdate(const GameTimer& gameTimer) = 0;

	// register this component properties
	virtual void RegisterProperties() = 0;

	// return the type id for this component type
	virtual ComponentTypeId GetTypeID() const = 0;
	// return this component type name
	virtual const std::string& GetName() const = 0;

	// returns true if the component is enabled, false otherwise
	virtual bool IsEnabled() const = 0;
	// sets the enabled state for this component
	virtual void SetEnabled(bool enabled) = 0;

	// sets the parent actor for this component
	virtual void SetOwner(StrongActorPtr owner) = 0;
	// return the parent actor for this component
	virtual WeakActorPtr GetOwner() const = 0;
	// serialize the component properties into an XML element
	virtual TiXmlElement* ToXML() const = 0;
};