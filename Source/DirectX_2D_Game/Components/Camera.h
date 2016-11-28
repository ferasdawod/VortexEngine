#pragma once

#include "Components/BaseComponent.h"
#include "Events/EventListener.h"

#include <DirectXTK/SimpleMath.h>
#include <DirectXCollision.h>
#include <memory>

#include "Actors/Actor.h"
#include "Transform.h"
#include <Graphics/ViewPort.h>

using DirectX::SimpleMath::Matrix;
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Quaternion;
using DirectX::BoundingFrustum;

class Transform;

class Camera : public BaseComponent, public EventListener,
						public std::enable_shared_from_this<Camera>
{
public:
	static const ComponentID	kComponentID;

public:
	Camera() : Camera(DirectX::XM_PIDIV4, 0.1f, 100.0f) {}
	Camera(float fov, float nearClip, float farClip);
	virtual ~Camera();

	EXPOSE_ID(kComponentID);

	virtual void	Initialize() override;
	virtual void	Initialize(TiXmlElement* xmlData) override;

	virtual void	OnUpdate(const GameTimer& gameTimer) override;

	Matrix GetViewMatrix()
	{ 
		if (_bViewDirty) RebuildView(); 
		return _viewMatrix; 
	}

	Matrix GetProjectionMatrix()
	{
		if (_bProjectionDirty) RebuildProjection();
		return _projectionMatrix;
	}

	Matrix GetViewProjectionMatrix() { return GetViewMatrix() * GetProjectionMatrix(); }

	
	// Returns the camera view frustum in view space
	BoundingFrustum		GetViewFrustum()
	{ 
		if (_bProjectionDirty) RebuildProjection();
		if (_bFrustumDirty)
		{
			BoundingFrustum::CreateFromMatrix(_boundingFrustum, _projectionMatrix);
			// this needs to happen because we are using RH stuff
			std::swap(_boundingFrustum.Near, _boundingFrustum.Far);
			_bFrustumDirty = false;
		}

		return _boundingFrustum;
	}

	DECLARE_PROPERTY_READ_ONLY(float, FOV);
	// Sets the field of view and causes the projection matrix to be rebuilt
	void			SetFOV(float fov) { _FOV = fov; _bProjectionDirty = true; }

	DECLARE_PROPERTY(float, NearClip);
	DECLARE_PROPERTY(float, FarClip);
	DECLARE_PROPERTY(ViewPort, ViewPort);

	float			GetAspectRatio()
	{
		if (_bProjectionDirty) RebuildProjection();
		return _nAspectRatio;
	}

	virtual bool			HandleEvent(StrongEventDataPtr eventData);

	virtual TiXmlElement*	ToXML() const override;

protected:
	void			RebuildView();
	void			RebuildProjection();

	virtual void	HandleRegistering(bool isRegistering) override;

protected:
	Matrix								_viewMatrix;
	Matrix								_projectionMatrix;

	bool								_bViewDirty;
	bool								_bProjectionDirty;

	Vector3								_prevPosition;
	Quaternion							_prevRotation;

	float								_nAspectRatio;

	bool								_bFrustumDirty;
	BoundingFrustum						_boundingFrustum;
};

__declspec(selectany) const ComponentID Camera::kComponentID = 0x59772A72;
