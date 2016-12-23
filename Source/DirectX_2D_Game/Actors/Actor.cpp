#include "pch.h"

#include "Actor.h"

ActorID Actor::_nIdCounter = static_cast<ActorID>(-1);
const ActorID Actor::INVALID_ID = static_cast<ActorID>(-1);

Actor::Actor(const std::string& name)
: std::enable_shared_from_this<Actor>(), _Name(name), _UniqueID(GenerateUniqueID()), _Enabled(true)
{
}

Actor::~Actor()
{
	Assert(_components.empty(), "The components list must be empty");
}

void Actor::OnUpdate(const GameTimer& gameTimer)
{
	if (!_Enabled)
	{
		LOG_E("Someone is updating a disabled actor", 0);
		return;
	}

	auto it = _components.begin();
	auto end = _components.end();

	while (it != end)
	{
		it->second->OnUpdate(gameTimer);
		it++;
	}
}

void Actor::Destroy()
{
	_components.clear();
	_UniqueID = INVALID_ID;
}

bool Actor::AddComponent(StrongComponentPtr component)
{
	auto findIt = _components.find(component->GetUniqueID());
	if (findIt != _components.end())
		return false;

	component->SetOwner(shared_from_this());
	_components.emplace(component->GetUniqueID(), component);
	component->Initialize();
	
	return true;
}

bool Actor::RemoveComponent(ComponentID uniqueComponentID)
{
	auto findIt = _components.find(uniqueComponentID);
	if (findIt != _components.end())
	{
		_components.erase(findIt);
		return true;
	}
	else
	{
		return false;
	}
}

bool Actor::RemoveComponent(StrongComponentPtr component)
{
	return RemoveComponent(component->GetUniqueID());
}

TiXmlElement* Actor::ToXML() const
{
	TiXmlElement* actorElement = DBG_NEW TiXmlElement("Actor");
	actorElement->SetAttribute("Name", _Name.c_str());

	TiXmlElement* enabledE = XmlHelper::ToXml("Enabled", _Enabled);

	TiXmlElement* componentsElement = DBG_NEW TiXmlElement("Components");
	componentsElement->SetDoubleAttribute("Count", _components.size());

	// add the transform to the top of the list
	auto it = _components.find(Transform::kComponentID);
	auto trans = it->second;
	componentsElement->LinkEndChild(trans->ToXML());
	
	for (auto it = _components.begin(); it != _components.end(); it++)
	{
		// we already added the transform component
		if (it->second->GetTypeID() == Transform::kComponentID)
			continue;

		StrongComponentPtr component = it->second;
		auto componentElement = component->ToXML();
		componentsElement->LinkEndChild(componentElement);
	}

	actorElement->LinkEndChild(enabledE);
	actorElement->LinkEndChild(componentsElement);
	return actorElement;
}

std::shared_ptr<Transform> Actor::GetTransform()
{
	if (_pWeakTransformPtr.expired())
		_pWeakTransformPtr = GetComponent<Transform>();

	return _pWeakTransformPtr.lock();
}
