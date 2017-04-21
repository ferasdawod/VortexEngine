#pragma once
#include "PropertyType.h"


class PropertyDescription
{
public:
	PropertyDescription(const char* name, PropertyType type, void* value);

	const char* GetName() const { return _name; }
	PropertyType GetType() const { return _type; }
	void* GetValue() const { return _pValue; }

protected:
	const char* _name;
	PropertyType _type;
	void* _pValue;
};

