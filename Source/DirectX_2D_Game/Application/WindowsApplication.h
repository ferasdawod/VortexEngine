#pragma once

#include "BaseGameApplication.h"

class WindowsApplication : public BaseGameApplication
{
public:
	WindowsApplication();
	virtual ~WindowsApplication();

	virtual bool Run(HINSTANCE hInstace) override sealed;
	
	virtual HICON	GetLargeIcon() const = 0;
	virtual HICON	GetSmallIcon() const = 0;
	virtual HCURSOR GetCursor() const = 0;

protected:
	virtual bool Init() override;
	virtual bool LoadResources() override { return BaseGameApplication::LoadResources(); }
	virtual void LoadSettings();

	virtual void OnUpdate() override {}
	virtual void OnRender() override {}

	virtual void OnShutDown() override;
	virtual void OnResize(int newWidth, int newHeight) override;
	
	virtual bool HandleEvent(StrongEventDataPtr eventData) override;
	virtual void HandleRegistering(bool isRegistering) override;

private:
	bool		InitWindow();

	static LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static WindowsApplication* _instance;

protected:
	HINSTANCE			_hInstace;
	HWND				_hWnd;

	int					_nWindowWidth;
	int					_nWindowHeight;

	bool				_running;
};

