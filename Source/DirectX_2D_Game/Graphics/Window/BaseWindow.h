#pragma once

#include "IWindow.h"

namespace Core
{
	class BaseWindow : public IWindow
	{
	public:
		BaseWindow() : _handle(nullptr) {}
		virtual ~BaseWindow() {}

		WindowHandle GetHandle() override { return _handle; }
		const Core::WindowInfo& GetInformation() const override { return _info; }

	protected:
		WindowHandle	_handle;
		WindowInfo		_info;
	};
}