#pragma once

#include <string>

class AudioComponent : public BaseComponent
{
public:
	static const ComponentTypeId	kComponentID;

public:
	AudioComponent() : BaseComponent(STRING(AudioComponent)), _autoPlay(true), _repeat(false), _volume(0.5f) {}
	virtual ~AudioComponent();

	EXPOSE_ID(kComponentID);

	virtual void Initialize() override;
	virtual void Initialize(TiXmlElement* xmlData) override;
	virtual void OnUpdate(float deltaTime) override {}
	virtual void RegisterProperties() override;
	virtual TiXmlElement* ToXML() const override;

	void Play() const;
	void Pause() const;
	void Stop() const;

	bool GetIsAutoPlay() const { return _autoPlay; }
	void SetIsAutoPlay(bool autoPlay) { _autoPlay = autoPlay; }

	bool GetIsRepeating() const { return _repeat; }
	void SetIsRepeating(bool isRepeating) { _repeat = isRepeating; }

	const std::string& GetAudioFilePath() const { return _audioFilePath; }
	void SetAudioFilePath(const std::string& filePath) { _audioFilePath = filePath; }

protected:
	bool _autoPlay;
	bool _repeat;

	float _volume;

	std::string _audioFilePath;
};

__declspec(selectany) const ComponentTypeId AudioComponent::kComponentID = 0x141d6776;