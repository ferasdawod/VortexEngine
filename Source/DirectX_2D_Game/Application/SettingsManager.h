#pragma once

#include <memory>
#include <string>

#include "3rd Party/TinyXml/tinyxml.h"
#include "Utilities/XmlHelper.h"

#include "Utilities/Singleton.h"

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
using std::string;

class SettingsManager : public Singleton<SettingsManager>
{
public:
	SettingsManager() {}
	virtual ~SettingsManager() {}

	bool				Initialize(const std::string& filePath);
	bool				SaveToFile() const;

	template <typename T>
	void				SaveSetting(const string& catagory, const string& name, const T& value)
	{
		auto catagoryE = GetCatagoryElement(catagory);
		RemoveOldSetting(catagoryE, name);

		catagoryE->LinkEndChild(XmlHelper::ToXml(name, value));
	}

	template <typename T>
	void				GetSetting(const string& catagory, const string& name, T& outValue)
	{
		auto catagoryE = GetCatagoryElement(catagory);
		XmlHelper::FromXml(catagoryE, name, outValue);
	}

protected:
	void				RemoveOldSetting(TiXmlElement* catagoryE, const string& name);
	TiXmlElement*		GetCatagoryElement(const string& catagory);

protected:
	std::string						_sFilePath;
	std::shared_ptr<TiXmlDocument>	_pSettingsDocument;
	bool							_bIsInitialized;
};