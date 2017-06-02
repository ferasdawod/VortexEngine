#pragma once

#include <memory>
#include <string>
#include <functional>
#include <map>
#include <vector>
#include <Actors/ActorsDefenitions.h>

class TiXmlElement;
using std::string;

class ComponentFactory
{
public:
	ComponentFactory() {}
	virtual ~ComponentFactory() {}

	bool RegisterComponent(const string& name, std::function<StrongComponentPtr()> func);

	StrongComponentPtr CreateFromXML(TiXmlElement* xmlElement);
	virtual StrongComponentPtr CreateComponent(const string& name);

	const std::vector<string>& GetRegisteredComponents() const { return _componentsNames; }

private:
	size_t CalculateHash(const string& str);

private:
	std::map<int, std::function<StrongComponentPtr()>> _factory;
	std::vector<string> _componentsNames;
};
