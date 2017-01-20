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

class ObjectId;
class TiXmlElement;

class XmlHelper
{
public:
	static TiXmlElement*	ToXml(const std::string& eleName, bool value);
	static TiXmlElement*	ToXml(const std::string& eleName, int value);
	static TiXmlElement*	ToXml(const std::string& eleName, float value);
	static TiXmlElement*	ToXml(const std::string& eleName, size_t value);
	static TiXmlElement*	ToXml(const std::string& eleName, const ObjectId& value);
	static TiXmlElement*	ToXml(const std::string& eleName, const std::string& value);
	
	static TiXmlElement*	ToXml(const std::string& eleName, const Color& value);
	static TiXmlElement*	ToXml(const std::string& eleName, const Vector2& value);
	static TiXmlElement*	ToXml(const std::string& eleName, const Vector3& value);
	static TiXmlElement*	ToXml(const std::string& eleName, const Vector4& value);


	static void				FromXml(const TiXmlElement* const parent, const std::string& name, bool& outVal);
	static void				FromXml(const TiXmlElement* const parent, const std::string& name, int& outVal);
	static void				FromXml(const TiXmlElement* const parent, const std::string& name, float& outVal);
	static void				FromXml(const TiXmlElement* const parent, const std::string& name, size_t& outval);
	static void				FromXml(const TiXmlElement* const parent, const std::string& name, ObjectId& outVal);
	static void				FromXml(const TiXmlElement* const parent, const std::string& name, std::string& outVal);

	static void				FromXml(const TiXmlElement* const parent, const std::string& name, Color& outVal);
	static void				FromXml(const TiXmlElement* const parent, const std::string& name, Vector2& outVal);
	static void				FromXml(const TiXmlElement* const parent, const std::string& name, Vector3& outVal);
	static void				FromXml(const TiXmlElement* const parent, const std::string& name, Vector4& outVal);

	static bool				FindChild(const TiXmlElement* const parent, const std::string& name, TiXmlElement const ** outElement);
	static TiXmlElement*	FindOrCreate(TiXmlElement* const parent, const std::string& elementName);
};