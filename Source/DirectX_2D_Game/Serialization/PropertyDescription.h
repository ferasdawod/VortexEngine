#pragma once
#include "PropertyType.h"

namespace Core
{
	class PropertyDescription
	{
	public:
		PropertyDescription(const char* theName, PropertyType theType, void* val, float min = 0.f, float max = 100.f)
			: name(theName), type(theType), value(val), minValue(min), maxValue(max)
		{
		}
		
		const char* name;
		PropertyType type;
		void* value;

		float minValue;
		float maxValue;
	};
}