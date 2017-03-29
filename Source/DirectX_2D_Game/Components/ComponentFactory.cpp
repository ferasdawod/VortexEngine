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

	auto findResult = _factory.find(hash);
	if (findResult == _factory.end())
	{
		_factory.emplace(std::make_pair(hash, func));
		return true;
	}

	LOG_W("Attempting to register same component twice");
	return false;
}

StrongComponentPtr ComponentFactory::CreateFromXML(TiXmlElement* xmlElement)
{
	if (xmlElement->ValueStr() != std::string("Component"))
		return nullptr;

	string componentName = "-1";
	xmlElement->QueryStringAttribute("Name", &componentName);

	Assert(componentName != "-1", "The component name is invalid");

	auto component = CreateComponent(componentName);
	if (component == nullptr)
		return nullptr;

	component->Initialize(xmlElement);
	return component;
}

StrongComponentPtr ComponentFactory::CreateComponent(const string& name)
{
	auto hash = CalculateHash(name);
	auto findResult = _factory.find(hash);
	if (findResult == _factory.end())
	{
		LOG_W("component not found");
		return nullptr;
	}

	auto function = findResult->second;
	auto component = function();
	return component;
}

size_t ComponentFactory::CalculateHash(const string& str)
{
	return std::hash<string>{}(str);
}
