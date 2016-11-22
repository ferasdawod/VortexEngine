#pragma once

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include "Keys.h"
#include <DirectXTK/SimpleMath.h>

using DirectX::SimpleMath::Vector3;

class InputDevice
{
	static const size_t			kKeyboardBufferSize = 256;

public:
								InputDevice();
								~InputDevice();
	bool						Init(HINSTANCE hInstance, HWND hWnd, bool isEditor = false);
	void						Release();

public:
	void						OnUpdate();

	static InputDevice*			Get() { return _pInstance; }
private:
	static InputDevice*			_pInstance;

public:
	bool						IsKeyDown(KeyCode key) const;
	bool						IsKeyUp(KeyCode key) const;

	bool						IsKeyPressed(KeyCode) const;
	bool						IsKeyReleased(KeyCode key) const;

	Vector3						MouseDelta() const 
	{ 
		return Vector3(static_cast<float>(_mouseState.lX), 
					   static_cast<float>(_mouseState.lY),
					   static_cast<float>(_mouseState.lZ));
	}
	bool						IsMouseButtonDown(MouseKeys key);

private:
	LPDIRECTINPUT8				_pInputObject;

	LPDIRECTINPUTDEVICE8		_pKeyboardDevice;
	LPDIRECTINPUTDEVICE8		_pMouseDevice;

	char*						_currKeyStates;
	char*						_prevKeyStates;

	DIMOUSESTATE				_mouseState;
};