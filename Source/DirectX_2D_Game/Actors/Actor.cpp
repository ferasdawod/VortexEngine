#include "pch.h"

#include "Actor.h"


Actor::Actor(const std::string& name) : _name(name), _enabled(true)
{
	_name.reserve(255);
}

Actor::~Actor()
{
	Assert(_components.empty(), "The components list must be empty");
}

void Actor::OnUpdate(float deltaTime)
{
	if (!_enabled)
	{
		LOG_W("disabled actor is being updated");
		return;
	}

	for (auto itr = _components.cbegin(); itr != _components.cend(); ++itr)
	{
		auto component = *itr;
		if (component->IsEnabled())
			component->OnUpdate(deltaTime);
	}
}

void Actor::Destroy()
{
	_components.clear();
	_id.Invalidate();
}

bool Actor::AddComponent(StrongComponentPtr component)
{
	// can't add this component if it already has a parent
	if (component->GetOwner().lock())
	{
		LOG_W("attempting to add a component which belongs to another actor");
		return false;
	}

	auto findIt = std::find_if(_components.begin(), _components.end(),
		[&](StrongComponentPtr ptr)
	{
		return ptr->GetId() == component->GetId();
	});

	if (findIt != _components.end())
		return false;

	component->SetOwner(shared_from_this());
	_components.push_back(component);
	component->Initialize();
	
	return true;
}

bool Actor::RemoveComponent(const ObjectId& uniqueComponentID)
{
	auto findIt = std::find_if(_components.begin(), _components.end(), 
		[&](StrongComponentPtr ptr)
	{
		return ptr->GetId() == uniqueComponentID;
	});

	if (findIt != _components.end())
	{
		_components.erase(findIt);
		return true;
	}
		
	return false;
}

bool Actor::RemoveComponent(StrongComponentPtr component)
{
	return RemoveComponent(component->GetId());
}

TiXmlElement* Actor::ToXML() const
{
	TiXmlElement* actorElement = DBG_NEW TiXmlElement("Actor");
	actorElement->SetAttribute("Name", _name.c_str());

	TiXmlElement* enabledE = XmlHelper::ToXml("Enabled", _enabled);

	TiXmlElement* componentsElement = DBG_NEW TiXmlElement("Components");
	componentsElement->SetDoubleAttribute("Count", _components.size());

	// add the transform to the top of the 
	auto trans = GetComponent<Transform>().lock();
	componentsElement->LinkEndChild(trans->ToXML());
	
	for (auto it = _components.begin(); it != _components.end(); ++it)
	{
		StrongComponentPtr component = *it;

		// we already added the transform component
		if (component->GetTypeID() == Transform::kComponentID)
			continue;

		auto componentElement = component->ToXML();
		componentsElement->LinkEndChild(componentElement);
	}

	actorElement->LinkEndChild(enabledE);
	actorElement->LinkEndChild(componentsElement);
	return actorElement;
}

std::weak_ptr<Transform> Actor::GetTransform()
{
	if (_pWeakTransformPtr.expired())
		_pWeakTransformPtr = GetComponent<Transform>();

	return _pWeakTransformPtr;
}
