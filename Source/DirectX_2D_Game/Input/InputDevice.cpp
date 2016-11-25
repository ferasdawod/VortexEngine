#include "InputDevice.h"

#include "Debugging/Debug.h"

InputDevice* InputDevice::_pInstance = nullptr;

InputDevice::InputDevice()
{
	_pKeyboardDevice = nullptr;
	_pMouseDevice = nullptr;
	_pInputObject = nullptr;
	_prevKeyStates = _currKeyStates = nullptr;

	if (_pInstance == nullptr)
	{
		_pInstance = this;
	}
	else
	{
		LOG_E("You are creating 2 input objects", 0);
	}
}

InputDevice::~InputDevice()
{
	Release();
}

bool InputDevice::Init(HINSTANCE hInstance, HWND hWnd, bool isEditor /* = false */)
{
	Release();

	HRESULT hr = S_OK;
	hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
							(void**)&_pInputObject, 0);

	if (FAILED(hr))
	{
		LOG_E("Failed to create the input object", hr);
		return false;
	}

	// keyboard device creation
	hr = _pInputObject->CreateDevice(GUID_SysKeyboard, &_pKeyboardDevice, 0);
	if (FAILED(hr))
	{
		LOG_E("Failed to create the keyboard device", hr);
		return false;
	}

	hr = _pKeyboardDevice->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
	{
		LOG_E("Failed to set the keyboard data format", hr);
		return false;
	}

	if (!isEditor)
	{
		hr = _pKeyboardDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		if (FAILED(hr))
		{
			LOG_E("Failed to set the cooperative level", hr);
			return false;
		}
	}

	hr = _pKeyboardDevice->Acquire();
	if (FAILED(hr))
		LOG_M("Failed to acquire the keyboard device");

	// mouse device creation
	hr = _pInputObject->CreateDevice(GUID_SysMouse, &_pMouseDevice, 0);
	if (FAILED(hr))
	{
		LOG_E("Failed to create the mouse input device", hr);
		return false;
	}

	hr = _pMouseDevice->SetDataFormat(&c_dfDIMouse);
	if (FAILED(hr))
	{
		LOG_E("Failed to set the mouse data format", hr);
		return false;
	}

	if (!isEditor)
	{
		hr = _pMouseDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		if (FAILED(hr))
		{
			LOG_E("Failed to set the mouse device cooperative level", hr);
			return false;
		}
	}

	hr = _pMouseDevice->Acquire();
	if (FAILED(hr))
		LOG_M("Failed to acquire the mouse device");

	_prevKeyStates = DBG_NEW char[kKeyboardBufferSize];
	_currKeyStates = DBG_NEW char[kKeyboardBufferSize];
	ZeroMemory(_currKeyStates, sizeof(kKeyboardBufferSize));
	ZeroMemory(_prevKeyStates, sizeof(kKeyboardBufferSize));

	ZeroMemory(&_mouseState, sizeof(_mouseState));

	return true;
}

void InputDevice::Release()
{
	SAFE_DELETE_ARRAY(_prevKeyStates);
	SAFE_DELETE_ARRAY(_currKeyStates);
	if (_pKeyboardDevice)
		_pKeyboardDevice->Unacquire();
	if (_pMouseDevice)
		_pMouseDevice->Unacquire();
	SAFE_RELEASE(_pKeyboardDevice);
	SAFE_RELEASE(_pMouseDevice);
	SAFE_RELEASE(_pInputObject);
}

void InputDevice::OnUpdate()
{
	FUNC_PROFILE();

	// on each new frame the current key states become the old key states
	std::swap(_prevKeyStates, _currKeyStates);

	auto hr = _pKeyboardDevice->GetDeviceState(kKeyboardBufferSize, (LPVOID)_currKeyStates);
	if (FAILED(hr))
	{
		hr = _pKeyboardDevice->Acquire();
		if (SUCCEEDED(hr))
		{
			hr = _pKeyboardDevice->GetDeviceState(kKeyboardBufferSize, (LPVOID)_currKeyStates);
		}
	}

	hr = _pMouseDevice->GetDeviceState(sizeof(_mouseState), (LPVOID)&_mouseState);
	if (FAILED(hr))
	{
		hr = _pMouseDevice->Acquire();
		if (SUCCEEDED(hr))
		{
			hr = _pMouseDevice->GetDeviceState(sizeof(_mouseState), (LPVOID)&_mouseState);
		}
	}
}

bool InputDevice::IsKeyDown(KeyCode key) const
{
	return (_currKeyStates[static_cast<int>(key)] & 0x80) != 0;
}

bool InputDevice::IsKeyUp(KeyCode key) const
{
	return !(IsKeyDown(key));
}

bool InputDevice::IsKeyPressed(KeyCode key) const
{
	return	(_prevKeyStates[static_cast<unsigned int>(key)] & 0x80) == 0 &&
			(_currKeyStates[static_cast<unsigned int>(key)] & 0x80) != 0;
}

bool InputDevice::IsKeyReleased(KeyCode key) const
{
	return	(_prevKeyStates[static_cast<unsigned int>(key)] & 0x80) != 0 &&
			!(_currKeyStates[static_cast<unsigned int>(key)] & 0x80) == 0;
}

bool InputDevice::IsMouseButtonDown(MouseKeys key)
{
	int k = static_cast<int>(key);
	return (_mouseState.rgbButtons[k] & 0x80) != 0;
}
