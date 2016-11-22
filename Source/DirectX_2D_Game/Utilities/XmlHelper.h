#pragma once

#include <string>

namespace DirectX
{
	namespace SimpleMath
	{
		struct Vector4;
		struct Vector3;
		struct Vector2;
		struct Color;
	}
}
using DirectX::SimpleMath::Color;
using DirectX::SimpleMath::Vector2;
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Vector4;

using ComponentID = unsigned long;
class TiXmlElement;

namespace XmlHelper
{
	// serialization methods

	TiXmlElement*		ToXml(const std::string& eleName, bool value);
	TiXmlElement*		ToXml(const std::string& eleName, int value);
	TiXmlElement*		ToXml(const std::string& eleName, float value);
	TiXmlElement*		ToXml(const std::string& eleName, size_t value);
	TiXmlElement*		ToXml(const std::string& eleName, ComponentID value);
	TiXmlElement*		ToXml(const std::string& eleName, const std::string& value);

	TiXmlElement*		ToXml(const std::string& eleName, const Color& value);
	TiXmlElement*		ToXml(const std::string& eleName, const Vector2& value);
	TiXmlElement*		ToXml(const std::string& eleName, const Vector3& value);
	TiXmlElement*		ToXml(const std::string& eleName, const Vector4& value);

	// deserialization methods

	void				FromXml(const TiXmlElement* const parent, const std::string& name, bool& outVal);
	void				FromXml(const TiXmlElement* const parent, const std::string& name, int& outVal);
	void				FromXml(const TiXmlElement* const parent, const std::string& name, float& outVal);
	void				FromXml(const TiXmlElement* const parent, const std::string& name, size_t& outval);
	void				FromXml(const TiXmlElement* const parent, const std::string& name, ComponentID& outVal);
	void				FromXml(const TiXmlElement* const parent, const std::string& name, std::string& outVal);

	void				FromXml(const TiXmlElement* const parent, const std::string& name, Color& outVal);
	void				FromXml(const TiXmlElement* const parent, const std::string& name, Vector2& outVal);
	void				FromXml(const TiXmlElement* const parent, const std::string& name, Vector3& outVal);
	void				FromXml(const TiXmlElement* const parent, const std::string& name, Vector4& outVal);

	bool				FindChild(const TiXmlElement* const parent, const std::string& name, TiXmlElement const ** outElement);
	TiXmlElement*		FindOrCreate(TiXmlElement* const parent, const std::string& elementName);
}