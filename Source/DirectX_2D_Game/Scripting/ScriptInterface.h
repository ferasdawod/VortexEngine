#pragma once

#include <memory>

#include <Scripting/ScriptClasses/ScriptVector3.h>

using namespace Core;

class Actor;

class ScriptInterface
{
public:
	explicit ScriptInterface(std::shared_ptr<Actor> actor);
	virtual ~ScriptInterface() {}

	float GetXPosition() const;
	float GetYPosition() const;
	float GetZPosition() const;

	ScriptVector3 GetPosition() const;
	
	void Move(float x, float y, float z);
	void SetPosition(float x, float y, float z);
	
	void Rotate(float x, float y, float z);
	void SetRotation(float x, float y, float z);

protected:
	std::weak_ptr<Actor> _actor;
};