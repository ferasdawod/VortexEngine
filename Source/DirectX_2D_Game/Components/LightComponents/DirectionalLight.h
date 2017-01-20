#pragma once

#include "LightComponent.h"

class DirectionalLight : public LightComponent
{
public:
	static const ComponentTypeId	kComponentID;

public:
	DirectionalLight()
		: LightComponent(LightType::Directional, STRING(DirectionalLight)), _ShadowStrength(1.0f) {}
	virtual ~DirectionalLight() {}

	EXPOSE_ID(kComponentID);

	DECLARE_PROPERTY(float, ShadowStrength);

	virtual void			Initialize() override { LightComponent::Initialize(); }
	virtual void			Initialize(TiXmlElement* xmlData) override;

	virtual void			OnUpdate(const GameTimer& gameTimer) override {}
	
	virtual TiXmlElement*	ToXML() const override;

};

__declspec(selectany) const ComponentTypeId DirectionalLight::kComponentID = 0x4C0A6864;
