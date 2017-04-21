#pragma once

#include <vector>
#include "PropertyDescription.h"
#include "PropertyType.h"

class PropertyBag
{
public:
	PropertyBag() {}
	virtual ~PropertyBag() {}

	virtual void RegisterProperties() = 0;
	void RegisterProperty(const char* propertyName, PropertyType type, void* value);

	const std::vector<PropertyDescription>& GetProperties() const { return _properties; }

protected:
	std::vector<PropertyDescription> _properties;
};