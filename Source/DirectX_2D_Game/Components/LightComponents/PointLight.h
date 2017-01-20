#pragma once

#include "LightComponent.h"

class PointLight : public LightComponent
{
public:
	static const ComponentTypeId	kComponentID;

public:
	PointLight() : LightComponent(LightType::Point, STRING(PointLight)), _Falloff(1.0f), _Range(2.0f) {}
	virtual ~PointLight() {}

	EXPOSE_ID(kComponentID);

	virtual void Initialize() override { LightComponent::Initialize(); }
	virtual void Initialize(TiXmlElement* xmlData) override;

	virtual void OnUpdate(const GameTimer& gameTimer) {}

	virtual TiXmlElement* ToXML() const override;

	// The higher is value the harder the edges of the light will be
	DECLARE_PROPERTY(float, Falloff);
	DECLARE_PROPERTY(float, Range);
};

__declspec(selectany) const ComponentTypeId PointLight::kComponentID = 0x4BB94318;