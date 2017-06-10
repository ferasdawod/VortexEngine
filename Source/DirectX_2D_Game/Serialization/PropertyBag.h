#pragma once

#include <vector>
#include "PropertyDescription.h"
#include "PropertyType.h"

namespace Core
{
	class PropertyBag
	{
	public:
		PropertyBag() {}
		virtual ~PropertyBag() {}

		const std::vector<PropertyDescription>& GetProperties() const { return _properties; }

	protected:
		virtual void RegisterProperties() = 0;
		
		void RegisterProperty(const char* propertyName, PropertyType type, void* value, float min = 0.f, float max = 100.f)
		{
			_properties.push_back(PropertyDescription(propertyName, type, value, min, max));
		}

	private:
		std::vector<PropertyDescription> _properties;
	};
}