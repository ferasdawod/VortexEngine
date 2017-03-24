#include "pch.h"

#include "ComponentFactory.h"

#include "Components/Camera.h"
#include "Components/MeshRenderer.h"

#include "Components/LightComponents/DirectionalLight.h"
#include "Components/LightComponents/PointLight.h"
#include "Components/LightComponents/SpotLight.h"

bool ComponentFactory::RegisterComponent(const string& name, std::function<StrongComponentPtr()> func)
{
	auto hash = CalculateHash(name);
	_factory.emplace(std::make_pair(hash, func));
}

StrongComponentPtr ComponentFactory::CreateFromXML(TiXmlElement* xmlElement)
{
	if (xmlElement->ValueStr() != std::string("Component"))
		return nullptr;

	ComponentTypeId id = 0;
	string idStr = "-1";
	xmlElement->QueryStringAttribute("TypeID", &idStr);

	Assert(idStr != "-1", "The component id is invalid");

	id = std::stoul(idStr);

	auto component = CreateFromID(id);
	if (component == nullptr)
		return nullptr;

	component->Initialize(xmlElement);
	return component;
}

StrongComponentPtr ComponentFactory::CreateFromID(ComponentTypeId id)
{
	switch (id)
	{
	case Transform::kComponentID:
		return StrongComponentPtr(DBG_NEW Transform);
	case Camera::kComponentID:
		return StrongComponentPtr(DBG_NEW Camera);
		// Render Components
	case MeshRenderer::kComponentID:
		return StrongComponentPtr(DBG_NEW MeshRenderer);
		// light components
	case DirectionalLight::kComponentID:
		return StrongComponentPtr(DBG_NEW DirectionalLight);
	case PointLight::kComponentID:
		return StrongComponentPtr(DBG_NEW PointLight);
	case SpotLight::kComponentID:
		return StrongComponentPtr(DBG_NEW SpotLight);
	default:
		LOG_M("Found a strange component ID");
		return nullptr;
	}
}

size_t ComponentFactory::CalculateHash(const string& str)
{
	return std::hash<string>{}(str);
}
