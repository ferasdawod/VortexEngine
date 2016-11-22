#pragma once

#include "Keys.h"
#include <DirectXTK/SimpleMath.h>

using DirectX::SimpleMath::Vector3;

namespace Input
{
	bool IsKeyDown(KeyCode key);
	bool IsKeyUp(KeyCode key);
	
	bool IsKeyPressed(KeyCode key);
	bool IsKeyReleased(KeyCode key);

	Vector3	MouseDelta();
	bool	IsMouseButtonDown(MouseKeys key);
}