#include "pch.h"

#include "ScriptInterface.h"

ScriptInterface::ScriptInterface(std::shared_ptr<Actor> actor)
	: _actor(actor)
{
}

float ScriptInterface::GetXPosition() const
{
	auto actor = _actor.lock();
	auto transform = actor->GetTransform().lock();
	return transform->GetPosition().x;
}

float ScriptInterface::GetYPosition() const
{
	auto actor = _actor.lock();
	auto transform = actor->GetTransform().lock();
	return transform->GetPosition().y;
}

float ScriptInterface::GetZPosition() const
{
	auto actor = _actor.lock();
	auto transform = actor->GetTransform().lock();
	return transform->GetPosition().z;
}

Core::ScriptVector3 ScriptInterface::GetPosition() const
{
	auto actor = _actor.lock();
	auto transform = actor->GetTransform().lock();
	auto position = transform->GetPosition();

	ScriptVector3 vector3(position.x, position.y, position.z);
	return vector3;
}

void ScriptInterface::Move(float x, float y, float z)
{
	auto actor = _actor.lock();
	auto transform = actor->GetTransform().lock();
	transform->Move(Vector3(x, y, z));
}

void ScriptInterface::SetPosition(float x, float y, float z)
{
}

void ScriptInterface::Rotate(float x, float y, float z)
{
	auto actor = _actor.lock();
	auto transform = actor->GetTransform().lock();
	transform->Rotate(x, y, z);
}

void ScriptInterface::SetRotation(float x, float y, float z)
{
}
