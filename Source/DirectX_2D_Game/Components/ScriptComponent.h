#pragma once

#include "BaseComponent.h"

class ScriptComponent : public BaseComponent
{
public:
	static const ComponentTypeId	kComponentID;
	EXPOSE_ID(kComponentID);

public:
	ScriptComponent();
	virtual ~ScriptComponent();
};

__declspec(selectany) const ComponentTypeId ScriptComponent::kComponentID = 0x93364454;