#pragma once

#include "BaseWindow.h"

namespace Core
{
	class WindowsWindow : public BaseWindow
	{
	public:
		WindowsWindow() {}
		virtual ~WindowsWindow() {}

		bool Initialize(const WindowInfo& info) override;

		void SetTitle(const string& title);

	protected:
		static LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	};
}