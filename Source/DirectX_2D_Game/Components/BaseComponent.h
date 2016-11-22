#pragma once

#include "IComponent.h"
#include "Actors/Actor.h"
#include "Utilities/Utils.h"
#include "Utilities/Macros.h"

class Transform;

#define EXPOSE_ID(x) virtual ComponentID GetTypeID() const override { return (x); }

class BaseComponent : public IComponent
{
public:
	BaseComponent(const std::string& componentName);
	virtual ~BaseComponent() {}

	virtual void				Initialize() = 0;
	virtual void				Initialize(TiXmlElement* xmlData);

	virtual void				OnUpdate(const GameTimer& gameTimer) = 0;
	virtual TiXmlElement*		ToXML() const override;

	virtual ComponentID			GetTypeID() const override = 0;
	std::shared_ptr<Transform>	GetTransform() const { return _Owner->GetTransform(); }

	DECLARE_PROPERTY_READ_ONLY(ComponentID, UniqueID);
	DECLARE_STRING_PROPERTY_READ_ONLY(Name);
	DECLARE_PROPERTY(StrongActorPtr, Owner);

public:
	bool			IsEnabled() const override final { return _Enabled; }
	void			SetEnabled(bool val) override final { _Enabled = val; }
protected:
	bool			_Enabled;

private:
	static ComponentID _nIdCounter;
	static ComponentID	GeneraateUniqueID() { ++_nIdCounter; return _nIdCounter; }
private:
	static ComponentID	UniqueIDCounter() { return _nIdCounter; }
	static void			UniqueIDCounter(ComponentID id) { _nIdCounter = id; }
};