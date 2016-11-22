#pragma once

#include "Application/BaseGameApplication.h"
#include <memory>

class RenderManager;
class InputDevice;
class Actor;
using StrongActorPtr = std::shared_ptr<Actor>;
class Level;

class EditorApplication : public BaseGameApplication
{
public:
	EditorApplication() = default;
	virtual ~EditorApplication();

	virtual bool Initialize(HINSTANCE hInstance, HWND hOutput);

	virtual void Tick();
	virtual void Resize();
	virtual void OnShutDown() override;

	virtual void LoadLevel(const std::string& levelPath);

protected:
	virtual bool Run(HINSTANCE hInstace) override { return true; }
	virtual bool Init() override;
	virtual bool LoadResources() override;

	virtual void OnUpdate() override;
	virtual void OnRender() override;

	virtual void OnResize(int newWidth, int newHeight) override;

	virtual bool HandleEvent(StrongEventDataPtr eventData);
	virtual void HandleRegistering(bool isRegistering);

	void UpdateDebugInput();

protected:
	HINSTANCE	_hInstance;
	HWND		_hWnd;

	std::unique_ptr<RenderManager>				_pRenderManager;

	std::shared_ptr<Level>						_pLevel;
	StrongActorPtr								_pPlayer;

	std::unique_ptr<InputDevice>				_pInputDevice;
};

extern EditorApplication* _gspEditorApplication;