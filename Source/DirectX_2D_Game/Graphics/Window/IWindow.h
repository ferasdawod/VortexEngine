#pragma once

#include <Debugging/Debug.h>

using WindowHandle = HWND;

class IWindow
{
public:
	virtual bool Initialize() = 0;
	virtual WindowHandle GetHandle() = 0;
};