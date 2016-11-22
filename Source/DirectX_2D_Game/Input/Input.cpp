#include "Input.h"

#include "InputDevice.h"

bool Input::IsKeyDown(KeyCode key)
{
	return InputDevice::Get()->IsKeyDown(key);
}

bool Input::IsKeyUp(KeyCode key)
{
	return InputDevice::Get()->IsKeyUp(key);
}

bool Input::IsKeyPressed(KeyCode key)
{
	return InputDevice::Get()->IsKeyPressed(key);
}

bool Input::IsKeyReleased(KeyCode key)
{
	return InputDevice::Get()->IsKeyDown(key);
}

Vector3 Input::MouseDelta()
{
	return InputDevice::Get()->MouseDelta();
}

bool Input::IsMouseButtonDown(MouseKeys key)
{
	return InputDevice::Get()->IsMouseButtonDown(key);
}
