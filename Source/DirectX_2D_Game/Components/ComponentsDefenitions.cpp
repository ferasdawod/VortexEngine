#include "pch.h"

#include "Camera.h"


#include "Graphics/Rendering/GraphicsDevice.h"

using DirectX::SimpleMath::Vector2;
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Vector4;
using DirectX::SimpleMath::Matrix;


/*
 *Base Actor Definition
 *
 */
#pragma region Base Component


BaseComponent::BaseComponent(const std::string& componentName) : _name(componentName), _pOwner(nullptr), _enabled(true)
{
}

void BaseComponent::RegisterProperties()
{
	RegisterProperty("Enabled", Core::PropertyType::Bool, &_enabled);
}


TiXmlElement* BaseComponent::ToXML() const
{
	TiXmlElement* element =  DBG_NEW TiXmlElement("Component");
	element->SetAttribute("Name", GetName().c_str());

	auto idElement = XmlHelper::ToXml("ObjectId", GetId());
	element->LinkEndChild(idElement);

	auto idStr = std::to_string(GetTypeID());
	element->SetAttribute("TypeID", idStr.c_str());

	auto enabledE = XmlHelper::ToXml("Enabled", _enabled);
	element->LinkEndChild(enabledE);

	return element;
}

void BaseComponent::Initialize()
{
	RegisterProperties();
}

void BaseComponent::Initialize(TiXmlElement* xmlData)
{
	std::string componentName("Blah Blah Blah");
	xmlData->QueryStringAttribute("Name", &componentName);
	assert(componentName == GetName());

	XmlHelper::FromXml(xmlData, "Enabled", _enabled);
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

void Transform::Initialize()
{
	BaseComponent::Initialize();
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

void Transform::RegisterProperties()
{
	BaseComponent::RegisterProperties();

	RegisterProperty("Position", Core::PropertyType::Vector3, &_Position, -1000.f, 1000.f);
	RegisterProperty("Rotation", Core::PropertyType::Quaternion, &_Rotation);
	RegisterProperty("Scale", Core::PropertyType::Vector3, &_Scale);
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

Camera::Camera(float fov, float nearClip, float farClip):
	BaseComponent(STRING(Camera)),
	_FOV(fov),
	_NearClip(nearClip),
	_FarClip(farClip),
	_bViewDirty(true),
	_bProjectionDirty(true),
	_nAspectRatio(-1.0f),
	_bFrustumDirty(true)
{
	Camera::HandleRegistering(true);
}

Camera::~Camera()
{
	Camera::HandleRegistering(false); 
}

void Camera::Initialize()
{
	BaseComponent::Initialize();

	// notify others that we are here
	std::shared_ptr<Camera> strongPtr = this->shared_from_this();
	WeakCameraPtr weakPtr(strongPtr);
	StrongEventDataPtr cameraEvent(DBG_NEW Event_NewCamera(weakPtr, _pOwner->GetId()));
	EventManager::GetPtr()->QueueEvent(cameraEvent);

	_bViewDirty = _bProjectionDirty = true;
}

void Camera::OnUpdate(float deltaTime)
{
	FUNC_PROFILE();

	// TODO DELETE THIS
	// DELETE BLOCK

	//LOG_M("GameTimer = " + std::to_string(deltaTime));
	
	auto transform = _pOwner->GetTransform().lock();

	if (Input::IsMouseButtonDown(MouseKeys::RightButton))
	{
		if (Input::IsKeyDown(KeyCode::W))
		{
			transform->Move(transform->GetWorldMat().Forward() * deltaTime * 5.0f);
		}
		if (Input::IsKeyDown(KeyCode::S))
		{
			transform->Move(transform->GetWorldMat().Forward() * deltaTime * -5.0f);
		}
		if (Input::IsKeyDown(KeyCode::D))
		{
			transform->Move(transform->GetWorldMat().Right() * deltaTime * 5.0f);
		}
		if (Input::IsKeyDown(KeyCode::A))
		{
			transform->Move(transform->GetWorldMat().Left() * deltaTime * 5.0f);
		}
		if (Input::IsKeyDown(KeyCode::E))
		{
			transform->Move(transform->GetWorldMat().Up() * deltaTime * 5.0f);
		}
		if (Input::IsKeyDown(KeyCode::Q))
		{
			transform->Move(transform->GetWorldMat().Down() * deltaTime * 5.0f);
		}

		if (Input::IsKeyDown(KeyCode::Left_Arrow))
		{
			transform->SetRotation(
				transform->GetRotation() * Quaternion::CreateFromAxisAngle
				(Vector3::Up, deltaTime * 3.0f));
		}
		if (Input::IsKeyDown(KeyCode::Right_Arrow))
		{
			transform->SetRotation(
				transform->GetRotation() * Quaternion::CreateFromAxisAngle
				(Vector3::Up, deltaTime * -3.0f));
		}
		if (Input::IsKeyDown(KeyCode::Up_Arrow))
		{
			transform->SetRotation(
				transform->GetRotation() * Quaternion::CreateFromAxisAngle
				(transform->GetWorldMat().Left(), deltaTime * -3.0f));
		}
		if (Input::IsKeyDown(KeyCode::Down_Arrow))
		{
			transform->SetRotation(
				transform->GetRotation() * Quaternion::CreateFromAxisAngle
				(transform->GetWorldMat().Right(), deltaTime * -3.0f));
		}

		auto mouseDelta = Input::MouseDelta();
		if (mouseDelta.x != 0)
		{
			transform->SetRotation(transform->GetRotation() * Quaternion::CreateFromAxisAngle(
				Vector3::Down, deltaTime * mouseDelta.x * 0.2f));
		}
		if (mouseDelta.y != 0)
		{
			transform->SetRotation(transform->GetRotation() * Quaternion::CreateFromAxisAngle(
				transform->GetWorldMat().Left(), deltaTime * mouseDelta.y * 0.2f));
		}
	}

	// END DELETE BLOCK

	auto trans = _pOwner->GetTransform().lock();
	Quaternion currRotation = trans->GetRotation();
	Vector3 currPosition = trans->GetPosition();
	if (_prevRotation != currRotation || _prevPosition != currPosition)
	{
		_bViewDirty = true;
	}

	_prevPosition = currPosition;
	_prevRotation = currRotation;
}

void Camera::RegisterProperties()
{
	BaseComponent::RegisterProperties();
	
	RegisterProperty("FOV", Core::PropertyType::Float, &_FOV);
	RegisterProperty("Near Clip", Core::PropertyType::Float, &_NearClip);
	RegisterProperty("Far Clip", Core::PropertyType::Float, &_FarClip);
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

	auto fovE = XmlHelper::ToXml("FOV", _FOV);
	auto farClipE = XmlHelper::ToXml("FarClip", _FarClip);
	auto nearClipE = XmlHelper::ToXml("NearClip", _NearClip);
	auto viewportElement = _ViewPort.ToXml();

	element->LinkEndChild(fovE);
	element->LinkEndChild(farClipE);
	element->LinkEndChild(nearClipE);
	element->LinkEndChild(viewportElement);

	return element;
}

void Camera::Initialize(TiXmlElement* xmlData)
{
	BaseComponent::Initialize(xmlData);

	XmlHelper::FromXml(xmlData, "FOV", _FOV);
	XmlHelper::FromXml(xmlData, "FarClip", _FarClip);
	XmlHelper::FromXml(xmlData, "NearClip", _NearClip);

	auto viewportElement = xmlData->FirstChildElement("ViewPort");
	_ViewPort.Initialize(viewportElement);
}

#pragma endregion