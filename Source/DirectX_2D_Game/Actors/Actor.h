#pragma once

#include <map>
#include <string>
#include <algorithm>
#include "ActorsDefenitions.h"
#include "Debugging/Debug.h"
#include "Utilities/Macros.h"

class TiXmlElement;
using ComponentsMap = std::map<ComponentID, StrongComponentPtr>;
class Transform;
class GameTimer;

class Actor : public std::enable_shared_from_this<Actor>
{
	friend class ActorFactory;

public:
	static const ActorID INVALID_ID;

private:
	Actor(const std::string& actorName = "Actor");

public:
	~Actor();
	void								OnUpdate(const GameTimer& gameTimer);
	void								Destroy();

	TiXmlElement*						ToXML() const;

public:
	DECLARE_PROPERTY_READ_ONLY(ActorID, UniqueID);
	DECLARE_STRING_PROPERTY(Name);
	DECLARE_PROPERTY(bool, Enabled);

	template <typename ComponentType>
	std::weak_ptr<ComponentType>		AddComponent()
	{
		std::shared_ptr<ComponentType> component(DBG_NEW ComponentType);
		if (AddComponent(component))
			return std::weak_ptr<ComponentType>(component);
		else
			return std::weak_ptr<ComponentType>();
	}

	bool								AddComponent(StrongComponentPtr component);
	
	std::shared_ptr<Transform>			GetTransform();

	template <typename ComponentType>
	bool								RemoveComponent()
	{
		auto typeID = ComponentType::kComponentID;
		auto findIt = std::find_if(_components.begin(), _components.end(),
								   [typeID](const std::pair<ComponentID, StrongComponentPtr>& pair)
		{
			return pair.second->GetTypeID() == typeID;
		});

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
	bool								RemoveComponent(StrongComponentPtr component);
	bool								RemoveComponent(ComponentID uniqueComponentID);
	int									GetComponentsCount() const { return _components.size(); }

	template <typename ComponentType>
	std::weak_ptr<ComponentType>		GetComponent() const
	{
		auto typeID = ComponentType::kComponentID;
		auto findIt = std::find_if(_components.begin(), _components.end(),
								   [&](const std::pair<ComponentID, StrongComponentPtr>& pair)
		{
			return pair.second->GetTypeID() == typeID;
		});

		if (findIt == _components.end())
		{
			return std::weak_ptr<ComponentType>();
		}
		else
		{
			std::shared_ptr<ComponentType> strongPtr(std::tr1::static_pointer_cast<ComponentType>(findIt->second));
			std::weak_ptr<ComponentType> weakPtr(strongPtr);
			return weakPtr;
		}
	}

private:
	ComponentsMap						_components;
	std::weak_ptr<Transform>			_pWeakTransformPtr;

#pragma region Static ID Generation
private:
	static ActorID _nIdCounter;
	static ActorID	GenerateUniqueID() { _nIdCounter++; return _nIdCounter; }
private:
	static ActorID	GetUniqueIDCounter() { return _nIdCounter; }
	static void		SetUniqueIDCounter(ActorID id) { _nIdCounter = id; }
#pragma endregion
};

