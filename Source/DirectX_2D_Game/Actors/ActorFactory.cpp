#include "pch.h"
#include "ActorFactory.h"

#include "Components/Camera.h"
#include "Components/MeshRenderer.h"

#include "Components/LightComponents/DirectionalLight.h"
#include "Components/LightComponents/PointLight.h"
#include "Components/LightComponents/SpotLight.h"
#include <Components/ComponentFactory.h>

ActorFactory::ActorFactory()
{
	_pComponentFactory.reset(DBG_NEW ComponentFactory);
}

StrongActorPtr ActorFactory::CreateEmptyActor(const std::string& actorName)
{
	StrongActorPtr actor(DBG_NEW Actor(actorName));
	StrongComponentPtr transform(DBG_NEW Transform());
	actor->AddComponent(transform);

	return actor;
}

StrongActorPtr ActorFactory::CreatePlaneActor()
{
	auto actor = CreateEmptyActor("Plane");

	StrongComponentPtr planeRenderer(DBG_NEW MeshRenderer("Plane.mesh"));
	actor->AddComponent(planeRenderer);

	return actor;
}

StrongActorPtr ActorFactory::CreateBoxActor()
{
	auto actor = CreateEmptyActor("Box");

	StrongComponentPtr boxRender(DBG_NEW MeshRenderer("Box.mesh"));
	actor->AddComponent(boxRender);

	return actor;
}

StrongActorPtr ActorFactory::CreateCylinderActor()
{
	auto actor = CreateEmptyActor("Cylinder");

	StrongComponentPtr cylinderRender(DBG_NEW MeshRenderer("Cylinder.mesh"));
	actor->AddComponent(cylinderRender);

	return actor;
}

StrongActorPtr ActorFactory::CreateSphereActor()
{
	auto actor = CreateEmptyActor("Sphere");

	StrongComponentPtr sphereRender(DBG_NEW MeshRenderer("Sphere.mesh"));
	actor->AddComponent(sphereRender);

	return actor;
}

StrongActorPtr ActorFactory::CreateMeshActor(const string& meshfilePath)
{
	auto actor = CreateEmptyActor("Mesh");

	StrongComponentPtr meshComponent(DBG_NEW MeshRenderer(meshfilePath));
	actor->AddComponent(meshComponent);

	return actor;
}

StrongActorPtr ActorFactory::CreateCameraActor()
{
	auto actor = CreateEmptyActor("Camera");

	StrongComponentPtr camera(DBG_NEW Camera());
	actor->AddComponent(camera);

	return actor;
}

StrongActorPtr ActorFactory::CreateDirectionalLight()
{
	auto actor = CreateEmptyActor("Directional Light");

	StrongComponentPtr light(DBG_NEW DirectionalLight());
	actor->AddComponent(light);

	auto transform = actor->GetComponent<Transform>().lock();
	transform->Rotate(0.0f, -90.0f, 0.0f);

	return actor;
}

StrongActorPtr ActorFactory::CreatePointLight()
{
	auto actor = CreateEmptyActor("Point Light");

	StrongComponentPtr light(DBG_NEW PointLight());
	actor->AddComponent(light);

	return actor;
}

StrongActorPtr ActorFactory::CreateSpotLight()
{
	auto actor = CreateEmptyActor("Spot Light");

	StrongComponentPtr light(DBG_NEW SpotLight());
	actor->AddComponent(light);

	return actor;
}

//template <typename T>
//void ActorFactory::RegisterComponent()
//{
//	T::_typeId = 0;
//	auto fun = []() { return StrongComponentPtr(DBG_NEW T()); };
//	_componentsFactory.emplace(std::make_pair(T::_typeId, fun));
//}


StrongActorPtr ActorFactory::CreateFromXML(const std::string& fileName)
{
	std::shared_ptr<TiXmlDocument> d(DBG_NEW TiXmlDocument(fileName));
	if (!d->LoadFile())
	{
		std::string msg = "Failed to open actor xml file : " + fileName;
		LOG_M(msg);

		return StrongActorPtr();
	}

	TiXmlElement* actorElement = d->RootElement();
	if (actorElement->ValueStr() != std::string("Actor"))
	{
		std::string msg = "The actor xml : " + fileName + " is not properly formatted \n the root element should be of type actor";
		LOG_M(msg);

		return StrongActorPtr();
	}

	return CreateFromXML(actorElement);
}

StrongActorPtr ActorFactory::CreateFromXML(TiXmlElement* actorElement)
{
	FUNC_PROFILE();

	Assert(actorElement->ValueStr() == std::string("Actor"), "the element is not an actor");

	string prefabPath;
	auto isPrefab = actorElement->QueryStringAttribute("PrefabPath", &prefabPath);
	// if this actor reference a prefab then load it from the prefab 
	if (isPrefab == TIXML_SUCCESS)
		return CreateFromXML(prefabPath);

	string actorName = "Actor";
	actorElement->QueryStringAttribute("Name", &actorName);
	StrongActorPtr actor(DBG_NEW Actor(actorName));

	XmlHelper::FromXml(actorElement, "Enabled", actor->_enabled);

	TiXmlElement* componentsElement = actorElement->FirstChildElement(std::string("Components"));
	int numComponents = -1;
	componentsElement->QueryIntAttribute("Count", &numComponents);

	for (TiXmlElement* component = componentsElement->FirstChildElement(); component; component = component->NextSiblingElement())
	{
		auto com = _pComponentFactory->CreateFromXML(component);
		if (com != nullptr)
			actor->AddComponent(com);
	}

	Assert(actor->GetComponentsCount() == numComponents, "Created components count does not match the count in the file");

	return actor;
}

void ActorFactory::SaveActor(const StrongActorPtr actor, const string& fileName)
{
	if (!actor)
	{
		LOG_M("Attempting to save an invalid actor to file : " + fileName);
		return;
	}
	std::shared_ptr<TiXmlDocument> document(DBG_NEW TiXmlDocument());
	document->LinkEndChild(actor->ToXML());
	document->SaveFile(fileName);
}
