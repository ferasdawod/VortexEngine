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
		
		void RegisterProperty(const char* propertyName, PropertyType type, void* value)
		{
			_properties.push_back(PropertyDescription(propertyName, type, value));
		}

	private:
		std::vector<PropertyDescription> _properties;
	};
}