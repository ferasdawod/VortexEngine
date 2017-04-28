#pragma once

#include <Utilities/UniqueObject.h>
#include <Components/IComponent.h>
#include <vector>

class Transform;
class GameTimer;

class Actor : public UniqueObject, public std::enable_shared_from_this<Actor>
{
	friend class ActorFactory;

	// only the actor factory can create new actor instances so the only ctor is private
private:
	// creates an empty actor with the specified name
	explicit Actor(const std::string& actorName = "Actor");

public:
	// the destructor for the actor class
	~Actor();

	// called each frame to update the actor and its components
	void OnUpdate(float deltaTime);
	
	// called to destroy the actor along with all of its components
	void Destroy();

	// serializes the actor to an XML node element
	TiXmlElement* ToXML() const;

public:
	// return the name of this actor
	const std::string& GetName() const { return _name; }
	// set the name for this actor
	void SetName(const std::string& newName) { _name = newName; }

	// returns true if the actor is enabled, false otherwise
	bool IsEnabled() const { return _enabled; }
	// sets the enabled state for this actor
	void SetEnabled(bool value) { _enabled = value; }

	// add a new component of the given type to this actor
	// returns a weak pointer to the component object
	template <typename ComponentType>
	std::weak_ptr<ComponentType> AddComponent()
	{
		std::shared_ptr<ComponentType> component(DBG_NEW ComponentType);
		if (AddComponent(component))
			return std::weak_ptr<ComponentType>(component);
		
		// return an invalid pointer if the component could not be added
		return std::weak_ptr<ComponentType>();
	}

	// returns a pointer to an owned component of the given type
	// returns a null pointer if a matching component couldn't be found
	template <typename ComponentType>
	std::weak_ptr<ComponentType> GetComponent() const
	{
		auto typeID = ComponentType::kComponentID;
		auto findIt = std::find_if(_components.begin(), _components.end(),
			[&](StrongComponentPtr component)
		{
			return component->GetTypeID() == typeID;
		});

		if (findIt == _components.end())
		{
			return std::weak_ptr<ComponentType>();
		}

		std::shared_ptr<ComponentType> strongPtr(std::tr1::static_pointer_cast<ComponentType>(*findIt));
		std::weak_ptr<ComponentType> weakPtr(strongPtr);
		return weakPtr;
	}

	// add the provided component instance to this actor
	// returns true if the component was added
	bool AddComponent(StrongComponentPtr component);
	
	// this is a convenience method  to return a pointer to this actor's transform component 
	std::weak_ptr<Transform> GetTransform();

	// removes the first component with the provided type from this actor's components
	template <typename ComponentType>
	bool RemoveComponent()
	{
		auto typeID = ComponentType::kComponentID;
		// search in the component array by the type id for the provided component type
		auto findIt = std::find_if(_components.begin(), _components.end(),
			[&](StrongComponentPtr component)
		{
			return component->GetTypeID() == typeID;
		});

		if (findIt != _components.end())
		{
			_components.erase(findIt);
			return true;
		}
	
		return false;
	}

	// remove a component from this actor
	bool RemoveComponent(StrongComponentPtr component);
	
	// remove a component with the provided id from this actor
	bool RemoveComponent(const ObjectId& uniqueComponentID);
	
	const std::vector<StrongComponentPtr>& GetComponents() const { return _components; }

	// return the component count for this actor
	int GetComponentsCount() const { return _components.size(); }

private:
	std::string _name;
	bool _enabled;

	std::vector<StrongComponentPtr> _components;
	std::weak_ptr<Transform> _pWeakTransformPtr;
};

