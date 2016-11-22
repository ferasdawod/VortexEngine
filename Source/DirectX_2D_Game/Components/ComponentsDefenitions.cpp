#include "Events/EventManager.h"
#include "Events/EventsTypes.h"
#include "Actors/Actor.h"

#include "3rd Party/TinyXml/tinyxml.h"

#include "Camera.h"
#include "Transform.h"

#include "Debugging/Debug.h"
#include "Utilities/XmlHelper.h"

#include "Graphics/Rendering/GraphicsDevice.h"
#include "Input/Input.h"
#include "Application/GameTimer.h"

using DirectX::SimpleMath::Vector2;
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Vector4;
using DirectX::SimpleMath::Matrix;


/*
 *Base Actor Definition
 *
 */
#pragma region Base Actor

ComponentID BaseComponent::_nIdCounter = IComponent::INVALID_ID;

BaseComponent::BaseComponent(const std::string& componentName)
: _UniqueID(GeneraateUniqueID()), _Name(componentName), _Owner(nullptr), _Enabled(true)
{
}

TiXmlElement* BaseComponent::ToXML() const
{
	TiXmlElement* element =  DBG_NEW TiXmlElement("Component");
	element->SetAttribute("Name", GetName().c_str());

	auto idStr = std::to_string(GetUniqueID());
	element->SetAttribute("UniqueID", idStr.c_str());

	idStr = std::to_string(GetTypeID());
	element->SetAttribute("TypeID", idStr.c_str());

	auto enabledE = XmlHelper::ToXml("Enabled", _Enabled);
	element->LinkEndChild(enabledE);

	return element;
}

void BaseComponent::Initialize(TiXmlElement* xmlData)
{
	std::string componentName("Blah Blah Blah");
	xmlData->QueryStringAttribute("Name", &componentName);
	assert(componentName == GetName());

	XmlHelper::FromXml(xmlData, "Enabled", _Enabled);
}

#pragma endregion

/*
 *Transform Component
 *
 **/
#pragma region Transform Component

TiXmlElement* Transform::ToXML() const
{
	TiXmlElement* element = BaseComponent::ToXML();

	TiXmlElement* posE = XmlHelper::ToXml("Position", _Position);
	TiXmlElement* rotationE = XmlHelper::ToXml("Rotation", Vector4(_Rotation));
	TiXmlElement* scaleE = XmlHelper::ToXml("Scale", _Scale);

	element->LinkEndChild(posE);
	element->LinkEndChild(rotationE);
	element->LinkEndChild(scaleE);

	return element;
}

void Transform::Initialize(TiXmlElement* xmlData)
{
	BaseComponent::Initialize(xmlData);

	XmlHelper::FromXml(xmlData, "Position", _Position);
	XmlHelper::FromXml(xmlData, "Scale", _Scale);

	Vector3 temp;
	XmlHelper::FromXml(xmlData, "Rotation", temp);
	temp.x = DirectX::XMConvertToRadians(temp.x);
	temp.y = DirectX::XMConvertToRadians(temp.y);
	temp.z = DirectX::XMConvertToRadians(temp.z);
	_Rotation = Quaternion::CreateFromYawPitchRoll(temp.x, temp.y, temp.z);
}

void Transform::Rotate(float yawDegrees, float pitchDegrees, float rollDegrees)
{
	Quaternion rotation = Quaternion::CreateFromYawPitchRoll(
		DirectX::XMConvertToRadians(yawDegrees),
		DirectX::XMConvertToRadians(pitchDegrees),
		DirectX::XMConvertToRadians(rollDegrees));

	_Rotation *= rotation;
}

#pragma endregion

/*
/*	Camera Component Definition
*/
#pragma region Camera Component

void Camera::Initialize()
{
	// notify others that we are here
	std::shared_ptr<Camera> strongPtr = this->shared_from_this();
	WeakCameraPtr weakPtr(strongPtr);
	StrongEventDataPtr cameraEvent(DBG_NEW Event_SetActiveCamera(weakPtr, _Owner->GetUniqueID()));
	EventManager::GetPtr()->QueueEvent(cameraEvent);

	_bViewDirty = _bProjectionDirty = true;
}

void Camera::OnUpdate(const GameTimer& gameTimer)
{
	PROFILE("Camera::OnUpdate");

	// TODO DELETE THIS
	// DELETE BLOCK
	
	auto transform = _Owner->GetTransform();
	if (Input::IsKeyDown(KeyCode::W))
	{
		transform->Move(transform->GetWorldMat().Forward() * gameTimer.DeltaTime() * 5.0f);
	}
	if (Input::IsKeyDown(KeyCode::S))
	{
		transform->Move(transform->GetWorldMat().Forward() * gameTimer.DeltaTime() * -5.0f);
	}
	if (Input::IsKeyDown(KeyCode::D))
	{
		transform->Move(transform->GetWorldMat().Right() * gameTimer.DeltaTime() * 5.0f);
	}
	if (Input::IsKeyDown(KeyCode::A))
	{
		transform->Move(transform->GetWorldMat().Left() * gameTimer.DeltaTime() * 5.0f);
	}
	if (Input::IsKeyDown(KeyCode::E))
	{
		transform->Move(transform->GetWorldMat().Up() * gameTimer.DeltaTime() * 5.0f);
	}
	if (Input::IsKeyDown(KeyCode::Q))
	{
		transform->Move(transform->GetWorldMat().Down() * gameTimer.DeltaTime() * 5.0f);
	}

	if (Input::IsKeyDown(KeyCode::Left_Arrow))
	{
		transform->SetRotation(
			transform->GetRotation() * Quaternion::CreateFromAxisAngle
			(Vector3::Up, gameTimer.DeltaTime() * 3.0f));
	}
	if (Input::IsKeyDown(KeyCode::Right_Arrow))
	{
		transform->SetRotation(
			transform->GetRotation() * Quaternion::CreateFromAxisAngle
			(Vector3::Up, gameTimer.DeltaTime() * -3.0f));
	}
	if (Input::IsKeyDown(KeyCode::Up_Arrow))
	{
		transform->SetRotation(
			transform->GetRotation() * Quaternion::CreateFromAxisAngle
			(transform->GetWorldMat().Right(), gameTimer.DeltaTime() * -3.0f));
	}
	if (Input::IsKeyDown(KeyCode::Down_Arrow))
	{
		transform->SetRotation(
			transform->GetRotation() * Quaternion::CreateFromAxisAngle
			(transform->GetWorldMat().Left(), gameTimer.DeltaTime() * -3.0f));
	}

	auto mouseDelta = !Input::IsMouseButtonDown(MouseKeys::RightButton) ? Vector3::Zero : Input::MouseDelta();
	if (mouseDelta.x != 0)
	{
		transform->SetRotation(transform->GetRotation() * Quaternion::CreateFromAxisAngle(
			Vector3::Down, gameTimer.DeltaTime() * mouseDelta.x * 0.2f));
	}
	if (mouseDelta.y != 0)
	{
		transform->SetRotation(transform->GetRotation() * Quaternion::CreateFromAxisAngle(
			transform->GetWorldMat().Left(), gameTimer.DeltaTime() * mouseDelta.y * 0.2f));
	}
	
	// END DELETE BLOCK

	auto trans = _Owner->GetTransform();
	Quaternion currRotation = trans->GetRotation();
	Vector3 currPosition = trans->GetPosition();
	if (_prevRotation != currRotation || _prevPosition != currPosition)
	{
		_bViewDirty = true;
	}

	_prevPosition = currPosition;
	_prevRotation = currRotation;
}

void Camera::RebuildView()
{
	Matrix rotationMat = Matrix::CreateFromQuaternion(_prevRotation);
	auto camForward = rotationMat.Forward();

	auto lookAt = _prevPosition + camForward;

	_viewMatrix = Matrix::CreateLookAt(_prevPosition, lookAt, rotationMat.Up());

	_bViewDirty = false;
}

void Camera::RebuildProjection()
{
	_nAspectRatio = (float)GraphicsDevice::GetPtr()->BackBufferWidth() / GraphicsDevice::GetPtr()->BackBufferHeight();
	_projectionMatrix = Matrix::CreatePerspectiveFieldOfView(_FOV, _nAspectRatio, _NearClip, _FarClip);
	_bProjectionDirty = false;
	_bFrustumDirty = true;
}

void Camera::HandleRegistering(bool isRegistering)
{
	ToggleRegisteration(Event_WindowResized::kEventID, isRegistering);
}


bool Camera::HandleEvent(StrongEventDataPtr eventData)
{
	if (eventData->GetID() == Event_WindowResized::kEventID)
	{
		_bProjectionDirty = true;
	}
	else
		assert(false);	// we should not get any other events

	return false;
}

TiXmlElement* Camera::ToXML() const
{
	TiXmlElement* element = BaseComponent::ToXML();

	TiXmlElement* fovE = XmlHelper::ToXml("FOV", _FOV);
	TiXmlElement* farClipE = XmlHelper::ToXml("FarClip", _FarClip);
	TiXmlElement* nearClipE = XmlHelper::ToXml("NearClip", _NearClip);

	element->LinkEndChild(fovE);
	element->LinkEndChild(farClipE);
	element->LinkEndChild(nearClipE);

	return element;
}

void Camera::Initialize(TiXmlElement* xmlData)
{
	BaseComponent::Initialize(xmlData);

	XmlHelper::FromXml(xmlData, "FOV", _FOV);
	XmlHelper::FromXml(xmlData, "FarClip", _FarClip);
	XmlHelper::FromXml(xmlData, "NearClip", _NearClip);
}

#pragma endregion