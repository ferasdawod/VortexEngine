#include "pch.h"
#include "PropertyDescription.h"

PropertyDescription::PropertyDescription(const char* name, PropertyType type, void* value)
	: _name(name), _type(type), _pValue(value)
{
}