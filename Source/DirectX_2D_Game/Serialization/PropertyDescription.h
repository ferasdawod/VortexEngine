#pragma once
#include "PropertyType.h"

namespace Core
{
	class PropertyDescription
	{
	public:
		PropertyDescription(const char* theName, PropertyType theType, void* val) : name(theName), type(theType), value(val) {}
		
		const char* name;
		PropertyType type;
		void* value;
	};
}