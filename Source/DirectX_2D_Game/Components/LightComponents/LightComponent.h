#pragma once

#include "Components/BaseComponent.h"
#include "Graphics/Colors.h"
#include "Utilities/Macros.h"

using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Matrix;

class Transform;

enum class LightType
{
	Directional = 0,
	Point,
	Spot,

	LightTypeCount,		// keep this at the bottom as a counter
};

class LightComponent : public BaseComponent, public std::enable_shared_from_this<LightComponent>
{
public:
	LightComponent(LightType type, const std::string& name) : BaseComponent(name), _LightType(type), _Strength(0.6f), _DiffuseColor(MyColors::White)
	{	}
	virtual ~LightComponent() = default;

	virtual void			Initialize() override;
	virtual void			Initialize(TiXmlElement* xmlData) override;

	virtual void			OnUpdate(const GameTimer& gameTimer) = 0;

	virtual void RegisterProperties() override;


	virtual TiXmlElement*	ToXML() const override;

	DECLARE_PROPERTY(LightType, LightType);
	DECLARE_PROPERTY(float, Strength);
	DECLARE_PROPERTY(Color, DiffuseColor);
};