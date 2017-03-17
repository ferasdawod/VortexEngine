#pragma once

#include <memory>
#include <string>
#include <functional>
#include <unordered_map>
#include "ActorsDefenitions.h"

using std::string;
class TiXmlElement;

class ActorFactory
{
public:
	StrongActorPtr	CreateEmptyActor(const string& actorName = "EmptyActor");

	StrongActorPtr	CreatePlaneActor();
	StrongActorPtr	CreateBoxActor();
	StrongActorPtr	CreateCylinderActor();
	StrongActorPtr	CreateSphereActor();

	StrongActorPtr	CreateMeshActor(const string& meshfilePath);

	StrongActorPtr	CreateCameraActor();

	StrongActorPtr	CreateDirectionalLight();

	StrongActorPtr	CreateFromXML(const string& fileName);
	StrongActorPtr	CreateFromXML(TiXmlElement* actorElement);

	void			SaveActor(const StrongActorPtr actor, const string& fileName);

	//template <typename T>
	//void			RegisterComponent();

private:
	StrongComponentPtr CreateComponentFromXML(TiXmlElement* xmlElement);
	virtual StrongComponentPtr CreateComponentFromID(ComponentTypeId id);

	//std::unordered_map<ComponentTypeId, std::function<IComponent>> _componentsFactory;
};
