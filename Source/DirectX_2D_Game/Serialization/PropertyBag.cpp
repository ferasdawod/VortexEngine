#include "pch.h"

#include "PropertyBag.h"

void PropertyBag::RegisterProperty(const char* propertyName, PropertyType type, void* value)
{
	_properties.push_back(PropertyDescription(propertyName, type, value));
}