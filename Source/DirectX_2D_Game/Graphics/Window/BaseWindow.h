#pragma once

#include "IWindow.h"

namespace Engine
{
	class BaseWindow : public IWindow
	{
	public:
		BaseWindow() : _handle(nullptr) {}
		virtual ~BaseWindow() {}

		WindowHandle GetHandle() override { return _handle; }
		const Engine::WindowInfo& GetInformation() const override { return _windowInfo; }

	protected:
		WindowHandle	_handle;
		WindowInfo		_info;
	};
}