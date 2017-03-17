#include "pch.h"

#include "WindowsWindow.h"

bool Core::WindowsWindow::Initialize(const WindowInfo& info)
{
	_info = info;
	auto hInstance = GetModuleHandle(nullptr);

	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(WNDCLASSEX));

	wcex.cbSize = sizeof(wcex);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = MsgProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	// TODO get icons
	//wcex.hIcon = GetLargeIcon();
	wcex.hCursor = GetCursor();
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = info.Title.c_str();
	//wcex.hIconSm = GetSmallIcon();

	if (!RegisterClassEx(&wcex))
	{
		LOG_E("Failed to register the window class", 0);
		return false;
	}

	RECT rc = { 0, 0, info.Width, info.Heigt};
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	_handle = CreateWindow(info.Title.c_str(), info.Title.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_DEFAULT,
		rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance, nullptr);

	if (!_handle)
	{
		LOG_E("Failed to create the window handle", 0);
		return false;
	}

	ShowWindow(_handle, SW_SHOWDEFAULT);
	UpdateWindow(_handle);

	return true;
}

void Core::WindowsWindow::SetTitle(const string& title)
{
	_info.Title = title;
	SetWindowText(_handle, title.c_str());
}

LRESULT Core::WindowsWindow::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	auto eventManager = EventManager::GetPtr();
	static auto isResizing = false;

	switch (msg)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		return 0;

	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE)
			eventManager->QueueEvent(StrongEventDataPtr(DBG_NEW PauseEvent));
		else
			eventManager->QueueEvent(StrongEventDataPtr(DBG_NEW ResumeEvent));

		return 0;

	case WM_CLOSE:
		if (MessageBox(hWnd, "Are you sure you want to exit", "Exit", MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
		{
			DestroyWindow(hWnd);
		}
		return 0;

	case WM_DESTROY:
		eventManager->QueueEvent(StrongEventDataPtr(DBG_NEW ShutdownEvent));
		PostQuitMessage(0);
		return 0;

	case WM_ENTERSIZEMOVE:
		eventManager->QueueEvent(StrongEventDataPtr(DBG_NEW PauseEvent));
		isResizing = true;
		return 0;

	case WM_EXITSIZEMOVE:
		eventManager->QueueEvent(StrongEventDataPtr(DBG_NEW ResumeEvent));
		isResizing = false;
		PostMessage(hWnd, WM_SIZE, wParam, lParam);
		return 0;

	case WM_SIZE:
		if (wParam != SIZE_MINIMIZED && !isResizing)
		{
			RECT rc;
			GetClientRect(hWnd, &rc);
			//int width = LOWORD(lParam);
			//int height = HIWORD(lParam);
			int width = rc.right - rc.left;
			int height = rc.bottom - rc.top;
			if (width != 0 && height != 0)
				eventManager->QueueEvent(StrongEventDataPtr(DBG_NEW Event_WindowResized(width, height)));
		}
		return 0;

	case WM_MENUCHAR:
		// don't beep when we alt enter
		return MAKELRESULT(0, MNC_CLOSE);

	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}
