#include "pch.h"
#include "AudioComponent.h"
#include "Audio/AudioManager.h"

AudioComponent::~AudioComponent()
{
}

void AudioComponent::Initialize()
{
	BaseComponent::Initialize();

	// TODO play audio file if it is auto play == true
	LOG_W("Audio component initialize");
}

void AudioComponent::Initialize(TiXmlElement* xmlData)
{
	BaseComponent::Initialize(xmlData);

	XmlHelper::FromXml(xmlData, "Volume", _volume);
	XmlHelper::FromXml(xmlData, "IsRepeating", _repeat);
	XmlHelper::FromXml(xmlData, "AutoPlay", _autoPlay);
	XmlHelper::FromXml(xmlData, "AudioFilePath", _audioFilePath);
}

void AudioComponent::RegisterProperties()
{
	BaseComponent::RegisterProperties();

	RegisterProperty("Volume", Core::PropertyType::Float, &_volume, 0, 1.f);
	RegisterProperty("Is Repeating", Core::PropertyType::Bool, &_repeat);
	RegisterProperty("Auto Play", Core::PropertyType::Bool, &_autoPlay);
}

TiXmlElement* AudioComponent::ToXML() const
{
	// TODO write this
	return DBG_NEW TiXmlElement("Blah Blah");
}

void AudioComponent::Play() const
{
	// TODO write this
	auto audioManager = Core::AudioManager::GetPtr();
	audioManager->Play2D(_audioFilePath);
}

void AudioComponent::Pause() const
{
	// TODO write this
	auto audioManager = Core::AudioManager::GetPtr();
}

void AudioComponent::Stop() const
{
	// TODO write this
	auto audioManager = Core::AudioManager::GetPtr();
}
