#pragma once

#include <memory>
#include <string>
#include <functional>
#include <unordered_map>
#include "ActorsDefenitions.h"

using std::string;
class TiXmlElement;
class ComponentFactory;

class ActorFactory
{
public:
	ActorFactory();

	virtual ~ActorFactory() {}

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

	std::shared_ptr<ComponentFactory> GetComponentFactory() const { return _pComponentFactory; }
private:
	std::shared_ptr<ComponentFactory> _pComponentFactory;
};
