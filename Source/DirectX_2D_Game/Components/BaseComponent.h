#pragma once

#include "IComponent.h"
#include "Actors/Actor.h"
#include "Utilities/Utils.h"
#include "Utilities/Macros.h"

class Transform;

#define EXPOSE_ID(x) virtual ComponentTypeId GetTypeID() const override { return (x); }

class BaseComponent : public IComponent
{
public:
	explicit BaseComponent(const std::string& componentName);
	virtual ~BaseComponent() {}

	void Initialize() override = 0;
	void Initialize(TiXmlElement* xmlData) override;

	void OnUpdate(const GameTimer& gameTimer) override = 0;
	TiXmlElement* ToXML() const override;

	ComponentTypeId GetTypeID() const override = 0;
	std::weak_ptr<Transform> GetTransform() const { return _pOwner->GetTransform(); }

	const std::string& GetName() const override { return _name; }
	void SetName(const std::string& newName) { _name = newName; }
	
	WeakActorPtr GetOwner() const override { return WeakActorPtr(_pOwner); }
	void SetOwner(StrongActorPtr owner) override { _pOwner = owner; }

	bool			IsEnabled() const override final { return _enabled; }
	void			SetEnabled(bool val) override final { _enabled = val; }

protected:
	std::string _name;
	StrongActorPtr _pOwner;
	bool			_enabled;
};