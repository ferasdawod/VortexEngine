#include "SettingsManager.h"

#include "Events/EventsTypes.h"
#include "Events/EventManager.h"
#include "Debugging/Debug.h"
#include "Utilities/XmlHelper.h"

bool SettingsManager::Initialize(const std::string& filePath)
{
	_pSettingsDocument.reset(DBG_NEW TiXmlDocument(filePath));
	if (!_pSettingsDocument->LoadFile())
	{
		// if loading failed we need to initialize an empty document
		auto msg = "Could not load the settings file, " + std::string(_pSettingsDocument->ErrorDesc()) +
			"\nCreating an empty settings file";
		LOG_W(msg);
		_pSettingsDocument->Clear();

		TiXmlElement* rootE = DBG_NEW TiXmlElement("Settings");
		_pSettingsDocument->LinkEndChild(rootE);
	}

	_bIsInitialized = true;
	_sFilePath = filePath;
	return true;
}

bool SettingsManager::SaveToFile() const
{
	return _pSettingsDocument->SaveFile();
}

void SettingsManager::RemoveOldSetting(TiXmlElement* catagoryE, const string& name)
{
	const TiXmlElement* prevValue = nullptr;
	if (XmlHelper::FindChild(catagoryE, name, &prevValue))
	{
		catagoryE->RemoveChild(const_cast<TiXmlElement*>(prevValue));
	}
}

TiXmlElement* SettingsManager::GetCatagoryElement(const string& catagory)
{
	return XmlHelper::FindOrCreate(_pSettingsDocument->RootElement(), catagory);
}

