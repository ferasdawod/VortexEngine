#pragma once

#include "LightComponent.h"

class SpotLight : public LightComponent
{
public:
	static const ComponentTypeId	kComponentID;

public:
	SpotLight() : LightComponent(LightType::Spot, STRING(SpotLight)), _ConeAngleDegrees(30.0f), _Falloff(10.0f) {}
	virtual ~SpotLight() {}

	EXPOSE_ID(kComponentID);

	virtual void Initialize() override { LightComponent::Initialize(); }
	virtual void Initialize(TiXmlElement* xmlData) override;

	virtual void OnUpdate(float deltaTime) override {}

	virtual void			RegisterProperties() override;

	virtual TiXmlElement* ToXML() const override;

	DECLARE_PROPERTY(float, ConeAngleDegrees);
	DECLARE_PROPERTY(float, Falloff);
};

__declspec(selectany) const ComponentTypeId SpotLight::kComponentID = 0xFF7E686A;