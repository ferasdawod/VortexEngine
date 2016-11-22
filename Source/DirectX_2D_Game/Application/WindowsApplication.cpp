#include "WindowsApplication.h"

#include "Debugging/Debug.h"
#include "Utilities/XmlHelper.h"
#include "Events/EventsTypes.h"

#include "SettingsManager.h"
#include "Events/EventManager.h"

#include "3rd Party/TinyXml/tinyxml.h"

WindowsApplication* WindowsApplication::_instance = nullptr;

WindowsApplication::WindowsApplication() : _nWindowWidth(800), _nWindowHeight(600)
{
	WindowsApplication::_instance = this;
}

WindowsApplication::~WindowsApplication()
{
}

bool WindowsApplication::Run(HINSTANCE hInstace)
{
	_hInstace = hInstace;

	_running = Init();

	if (_running)
	_running = LoadResources();

	if (!_running)
		return false;

	MSG msg = { 0 };
	_gameTimer.Reset();
	while (_running)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			PROFILE("Frame");

			_gameTimer.Tick();
			if (!_bIsPaused)
			{
				{
					PROFILE("Update");
					OnUpdate();
				}
				{
					PROFILE("Render");
					OnRender();
				}
			}
		}
	}

	OnShutDown();

	return true;
}

bool WindowsApplication::Init()
{
	BaseGameApplication::Init();

	LoadSettings();

	if (!InitWindow())
		return false;

	return true;
}

void WindowsApplication::LoadSettings()
{
	SettingsManager::GetPtr()->GetSetting("Application", "WindowWidth", _nWindowWidth);
	SettingsManager::GetPtr()->GetSetting("Application", "WindowHeight", _nWindowHeight);
	SettingsManager::GetPtr()->GetSetting("Application", "ApplicationName", _sAppName);
}

LRESULT CALLBACK WindowsApplication::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (msg)
	{
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			return 0;

		case WM_ACTIVATE:
			if (LOWORD(wParam) == WA_INACTIVE)
				_instance->Pause();
			else
				_instance->Resume();

			return 0;

		case WM_CLOSE:
			if (MessageBox(hWnd, "Are you sure you want to exit", "Exit", MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
			{
				DestroyWindow(hWnd);
			}
			return 0;

		case WM_DESTROY:
			_instance->_running = false;
			PostQuitMessage(0);
			return 0;

		case WM_ENTERSIZEMOVE:
			_instance->Pause();
			_instance->_bIsResizing = true;
			return 0;

		case WM_EXITSIZEMOVE:
			_instance->Resume();
			_instance->_bIsResizing = false;
			PostMessage(_instance->_hWnd, WM_SIZE, wParam, lParam);
			return 0;

		case WM_SIZE:
			if (wParam != SIZE_MINIMIZED && !_instance->_bIsResizing)
			{
				RECT rc;
				GetClientRect(_instance->_hWnd, &rc);
				//int width = LOWORD(lParam);
				//int height = HIWORD(lParam);
				int width = rc.right - rc.left;
				int height = rc.bottom - rc.top;
				if (width != 0 && height != 0)
					_instance->OnResize(width, height);
			}
			return 0;

		case WM_MENUCHAR:
			// don't beep when we alt enter
			return MAKELRESULT(0, MNC_CLOSE);

		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}

bool WindowsApplication::InitWindow()
{
	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(WNDCLASSEX));

	wcex.cbSize = sizeof(wcex);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WindowsApplication::MsgProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = _hInstace;
	wcex.hIcon = GetLargeIcon();
	wcex.hCursor = GetCursor();
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = _sAppName.c_str();
	wcex.hIconSm = GetSmallIcon();

	if (!RegisterClassEx(&wcex))
	{
		LOG_E("Failed to register the window class", 0);
		return false;
	}

	RECT rc = { 0, 0, _nWindowWidth, _nWindowHeight };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	_hWnd = CreateWindow(_sAppName.c_str(), _sAppName.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_DEFAULT,
						 rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, _hInstace, nullptr);

	if (!_hWnd)
	{
		LOG_E("Failed to create the window handle", 0);
		return false;
	}

	ShowWindow(_hWnd, SW_SHOWDEFAULT);
	UpdateWindow(_hWnd);

	return true;
}

bool WindowsApplication::HandleEvent(StrongEventDataPtr eventData)
{
	if (BaseGameApplication::HandleEvent(eventData))
		return true;

	if (eventData->GetID() == Event_SavingSettings::kEventID)
	{
		auto casted = std::dynamic_pointer_cast<Event_SavingSettings>(eventData);
		assert(casted != nullptr);

		auto settingsManager = casted->GetSettingsManager().lock();
		assert(settingsManager);

		settingsManager->SaveSetting("Application", "WindowWidth", _nWindowWidth);
		settingsManager->SaveSetting("Application", "WindowHeight", _nWindowHeight);
		settingsManager->SaveSetting("Application", "ApplicationName", _sAppName);
	}
	else
		Assert(false, "received an event that we didn't want");

	return false;
}

void WindowsApplication::OnResize(int newWidth, int newHeight)
{
	BaseGameApplication::OnResize(newWidth, newHeight);
}

void WindowsApplication::OnShutDown()
{
	BaseGameApplication::OnShutDown();
}

void WindowsApplication::HandleRegistering(bool isRegistering)
{
	BaseGameApplication::HandleRegistering(isRegistering);

	ToggleRegisteration(Event_SavingSettings::kEventID, isRegistering);
}
