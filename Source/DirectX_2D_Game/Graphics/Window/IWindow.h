#pragma once

#include <Debugging/Debug.h>
#include "WindowInfo.h"

using WindowHandle = HWND;

namespace Core
{
	class IWindow
	{
	public:
		virtual ~IWindow() {}

		virtual bool Initialize(const WindowInfo& info) = 0;
		virtual WindowHandle GetHandle() const = 0;

		virtual const WindowInfo& GetInformation() const = 0;
	};
}