#pragma once

#include "Components/BaseComponent.h"
#include <DirectXTK/SimpleMath.h>

using DirectX::SimpleMath::Matrix;
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Quaternion;

class Transform : public BaseComponent
{
public:
	static const ComponentTypeId kComponentID;

public:
	Transform() : 
		BaseComponent(STRING(Transform)),
		_Position(0.0f, 0.0f, 0.0f),
		_Scale(1.0f, 1.0f, 1.0f),
		_Rotation(0.0f, 0.0f, 0.0f, 1.0f)
	{
	}
	virtual ~Transform() { }

	EXPOSE_ID(kComponentID);

	virtual void		Initialize() override;
	virtual void		Initialize(TiXmlElement* xmlData) override;
	virtual void		OnUpdate(const GameTimer& gameTimer) { }

	virtual void		RegisterProperties() override;

public:

	void				Move(Vector3 offset) { _Position += offset; }
	void				Rotate(float yawDegrees, float pitchDegrees, float rollDegrees);

	DECLARE_PROPERTY(Vector3, Position);
	DECLARE_PROPERTY(Quaternion, Rotation);
	DECLARE_PROPERTY(Vector3, Scale);


	Vector3	GetForward() const { return Matrix::CreateFromQuaternion(_Rotation).Forward(); }
	Vector3	GetUp() const { return Matrix::CreateFromQuaternion(_Rotation).Up(); }
	Vector3	GetRight() const { return Matrix::CreateFromQuaternion(_Rotation).Right(); }

	Matrix GetWorldMat() const
	{ 
		return
			Matrix::CreateScale(_Scale) *
			Matrix::CreateFromQuaternion(_Rotation) * 
			Matrix::CreateTranslation(_Position);
	}

	virtual				TiXmlElement* ToXML() const override;
};

__declspec(selectany) const ComponentTypeId Transform::kComponentID = 0x9DB13CA5;