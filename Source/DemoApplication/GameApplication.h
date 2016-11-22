#pragma once

#include "Application/WindowsApplication.h"
#include "resource.h"

class RenderManager;
class InputDevice;
class Actor;
using StrongActorPtr = std::shared_ptr<Actor>;
class Level;

class GameApplication :	public WindowsApplication
{

public:
	GameApplication();
	virtual ~GameApplication();

protected:
	virtual bool Init() override;
	virtual bool LoadResources() override { return WindowsApplication::LoadResources(); }

	virtual void OnUpdate() override;
	virtual void OnRender() override;

	virtual void OnShutDown() override { WindowsApplication::OnShutDown(); }
	virtual void OnResize(int newWidth, int newHeight) override { if (_pRenderManager) WindowsApplication::OnResize(newWidth, newHeight); }

	virtual bool HandleEvent(StrongEventDataPtr eventData) override { return WindowsApplication::HandleEvent(eventData); }
	virtual void HandleRegistering(bool isRegistering) override { WindowsApplication::HandleRegistering(isRegistering); }

	virtual HICON GetLargeIcon() const { return LoadIcon(_hInstace, (LPCSTR)IDI_ICON1); }
	virtual HICON GetSmallIcon() const { return LoadIcon(_hInstace, (LPCSTR)IDI_ICON1); }
	virtual HCURSOR GetCursor() const { return LoadCursor(_hInstace, (LPCSTR)IDC_POINTER); }

	void			UpdateStates();
	void			UpdateDebugInput();

protected:
	std::unique_ptr<RenderManager>				_pRenderManager;

	std::shared_ptr<Level>						_pLevel;
	StrongActorPtr								_pPlayer;

	std::unique_ptr<InputDevice>				_pInputDevice;
};

