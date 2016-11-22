#include "XmlHelper.h"

#include <DirectXTK/SimpleMath.h>
#include "3rd Party/TinyXml/tinyxml.h"
#include "Debugging/Debug.h"

TiXmlElement* XmlHelper::ToXml(const std::string& eleName, bool value)
{
	return ToXml(eleName, std::to_string(value));
}

TiXmlElement* XmlHelper::ToXml(const std::string& eleName, int value)
{
	return ToXml(eleName, std::to_string(value));
}

TiXmlElement* XmlHelper::ToXml(const std::string& eleName, float value)
{
	return ToXml(eleName, std::to_string(value));
}

TiXmlElement* XmlHelper::ToXml(const std::string& eleName, size_t value)
{
	return ToXml(eleName, std::to_string(value));
}

TiXmlElement* XmlHelper::ToXml(const std::string& eleName, ComponentID value)
{
	return ToXml(eleName, std::to_string(value));
}

TiXmlElement* XmlHelper::ToXml(const std::string& eleName, const std::string& value)
{
	TiXmlElement* element = DBG_NEW TiXmlElement(eleName);
	element->SetAttribute("Value", value);
	return element;
}

TiXmlElement* XmlHelper::ToXml(const std::string& eleName, const Color& value)
{
	TiXmlElement* element = DBG_NEW TiXmlElement(eleName);
	element->SetDoubleAttribute("R", value.R());
	element->SetDoubleAttribute("G", value.G());
	element->SetDoubleAttribute("B", value.B());
	element->SetDoubleAttribute("A", value.A());
	return element;
}

TiXmlElement* XmlHelper::ToXml(const std::string& eleName, const Vector2& value)
{
	TiXmlElement* element = DBG_NEW TiXmlElement(eleName);
	element->SetDoubleAttribute("X", value.x);
	element->SetDoubleAttribute("Y", value.y);
	return element;
}

TiXmlElement* XmlHelper::ToXml(const std::string& eleName, const Vector3& value)
{
	TiXmlElement* element = DBG_NEW TiXmlElement(eleName);
	element->SetDoubleAttribute("X", value.x);
	element->SetDoubleAttribute("Y", value.y);
	element->SetDoubleAttribute("Z", value.z);
	return element;
}

TiXmlElement* XmlHelper::ToXml(const std::string& eleName, const Vector4& value)
{
	TiXmlElement* element = DBG_NEW TiXmlElement(eleName);
	element->SetDoubleAttribute("X", value.x);
	element->SetDoubleAttribute("Y", value.y);
	element->SetDoubleAttribute("Z", value.z);
	element->SetDoubleAttribute("W", value.w);
	return element;
}

void XmlHelper::FromXml(const TiXmlElement* const parent, const std::string& name, bool& outVal)
{
	const TiXmlElement* element = nullptr;
	if (FindChild(parent, name, &element))
	{
		element->QueryBoolAttribute("Value", &outVal);
	}
}

void XmlHelper::FromXml(const TiXmlElement* const parent, const std::string& name, int& outVal)
{
	const TiXmlElement* element = nullptr;
	if (FindChild(parent, name, &element))
	{
		element->QueryIntAttribute("Value", &outVal);
	}
}

void XmlHelper::FromXml(const TiXmlElement* const parent, const std::string& name, float& outVal)
{
	const TiXmlElement* element = nullptr;
	if (FindChild(parent, name, &element))
	{
		element->QueryFloatAttribute("Value", &outVal);
	}
}

void XmlHelper::FromXml(const TiXmlElement* const parent, const std::string& name, size_t& outVal)
{
	const TiXmlElement* element = nullptr;
	if (FindChild(parent, name, &element))
	{
		int tmp;
		element->QueryIntAttribute("Value", &tmp);
		outVal = static_cast<size_t>(tmp);
	}
}

void XmlHelper::FromXml(const TiXmlElement* const parent, const std::string& name, ComponentID& outVal)
{
	const TiXmlElement* element = nullptr;
	if (FindChild(parent, name, &element))
	{
		std::string tmp;
		element->QueryStringAttribute("Value", &tmp);
		outVal = std::stoul(tmp);
	}
}

void XmlHelper::FromXml(const TiXmlElement* const parent, const std::string& name, std::string& outVal)
{
	const TiXmlElement* element = nullptr;
	if (FindChild(parent, name, &element))
	{
		element->QueryStringAttribute("Value", &outVal);
	}
}

void XmlHelper::FromXml(const TiXmlElement* const parent, const std::string& name, Color& outVal)
{
	const TiXmlElement* element = nullptr;
	if (FindChild(parent, name, &element))
	{
		element->QueryFloatAttribute("R", &outVal.x);
		element->QueryFloatAttribute("G", &outVal.y);
		element->QueryFloatAttribute("B", &outVal.z);
		element->QueryFloatAttribute("A", &outVal.w);
	}
}

void XmlHelper::FromXml(const TiXmlElement* const parent, const std::string& name, Vector2& outVal)
{
	const TiXmlElement* element = nullptr;
	if (FindChild(parent, name, &element))
	{
		element->QueryFloatAttribute("X", &outVal.x);
		element->QueryFloatAttribute("Y", &outVal.y);
	}
}

void XmlHelper::FromXml(const TiXmlElement* const parent, const std::string& name, Vector3& outVal)
{
	const TiXmlElement* element = nullptr;
	if (FindChild(parent, name, &element))
	{
		element->QueryFloatAttribute("X", &outVal.x);
		element->QueryFloatAttribute("Y", &outVal.y);
		element->QueryFloatAttribute("Z", &outVal.z);
	}
}

void XmlHelper::FromXml(const TiXmlElement* const parent, const std::string& name, Vector4& outVal)
{
	const TiXmlElement* element = nullptr;
	if (FindChild(parent, name, &element))
	{
		element->QueryFloatAttribute("X", &outVal.x);
		element->QueryFloatAttribute("Y", &outVal.y);
		element->QueryFloatAttribute("Z", &outVal.z);
		element->QueryFloatAttribute("W", &outVal.w);
	}
}

bool XmlHelper::FindChild(const TiXmlElement* const parent, const std::string& name, TiXmlElement const ** outElement)
{
	const TiXmlElement* temp = nullptr;
	temp = parent->FirstChildElement(name);
	if (temp != nullptr)
	{
		*outElement = temp;
		return true;
	}
	else
		return false;
}

TiXmlElement* XmlHelper::FindOrCreate(TiXmlElement* const parent, const std::string& elementName)
{
	auto element = parent->FirstChildElement(elementName);
	if (!element)
	{
		element = DBG_NEW TiXmlElement(elementName);
		parent->LinkEndChild(element);
	}

	return element;
}