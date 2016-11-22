#include "EditorApplication.h"

#include "Input/InputDevice.h"
#include "Graphics/Rendering/RenderManager.h"
#include "Events/EventManager.h"
#include "Events/EventsTypes.h"
#include "Actors/Actor.h"
#include "Actors/ActorFactory.h"
#include "Components/Transform.h"
#include "Level/Level.h"
#include "Input\Input.h"

#include "Debugging/Debug.h"
#include "3rd Party/TinyXml/tinyxml.h"

EditorApplication* _gspEditorApplication = nullptr;

EditorApplication::~EditorApplication()
{
	_pPlayer->Destroy();
}

bool EditorApplication::Initialize(HINSTANCE hInstance, HWND hOutput)
{
	_hInstance = hInstance;
	_hWnd = hOutput;

	if (!Init() || !LoadResources())
		return false;

	return true;
}

bool EditorApplication::Init()
{
	bool good =  BaseGameApplication::Init();
	if (!good)
		return false;

	_pInputDevice.reset(DBG_NEW InputDevice);
	good = _pInputDevice->Init(_hInstance, _hWnd, true);
	if (!good)
		return false;

	_pRenderManager.reset(DBG_NEW RenderManager);
	good = _pRenderManager->Initialize(_hWnd);
	if (!good)
		return false;

	ActorFactory actorFactory;
	_pPlayer = actorFactory.CreateCameraActor();
	auto playerTransform = _pPlayer->GetTransform();
	playerTransform->SetPosition(Vector3(3.0f, 1.0f, -3.0f));
	playerTransform->SetRotation(Quaternion::CreateFromYawPitchRoll(
		DirectX::XMConvertToRadians(142.5), DirectX::XMConvertToRadians(-15), 0.0f));

	_pLevel.reset(DBG_NEW Level("Untitled1"));
	auto planeActor = actorFactory.CreatePlaneActor();
	planeActor->GetTransform()->SetScale(Vector3::One * 50.0f);
	auto lightActor = actorFactory.CreateDirectionalLight();

	_pLevel->AddActor(planeActor);
	_pLevel->AddActor(lightActor);

	return true;
}

bool EditorApplication::LoadResources()
{
	return BaseGameApplication::LoadResources();
}

void EditorApplication::Tick()
{
	_gameTimer.Tick();
	OnUpdate();
	OnRender();
}

void EditorApplication::OnUpdate()
{
	_pInputDevice->OnUpdate();
	_pPlayer->OnUpdate(_gameTimer);
	_pLevel->OnUpdate(_gameTimer);

	_pEventManager->OnUpdate(_gameTimer);
	UpdateDebugInput();
}

void EditorApplication::OnRender()
{
	_pRenderManager->OnRender();
}

void EditorApplication::OnResize(int newWidth, int newHeight)
{
	BaseGameApplication::OnResize(newWidth, newHeight);
}

void EditorApplication::OnShutDown()
{
	BaseGameApplication::OnShutDown();
}

bool EditorApplication::HandleEvent(StrongEventDataPtr eventData)
{
	return BaseGameApplication::HandleEvent(eventData);
}

void EditorApplication::HandleRegistering(bool isRegistering)
{
	BaseGameApplication::HandleRegistering(isRegistering);
}

void EditorApplication::Resize()
{
	RECT rc;
	GetClientRect(_hWnd, &rc);

	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;

	if (width != 0 && height != 0)
		OnResize(width, height);
}

void EditorApplication::LoadLevel(const std::string& levelPath)
{
	_pLevel->LoadLevel(levelPath);
}

void EditorApplication::UpdateDebugInput()
{
	if (Input::IsKeyPressed(KeyCode::F1))
		_pRenderManager->ToggleWireframe();

	if (Input::IsKeyPressed(KeyCode::F2))
		_pRenderManager->ToggleLighting();

	if (Input::IsKeyPressed(KeyCode::F3))
		_pRenderManager->ToggleTexturing();

	if (Input::IsKeyPressed(KeyCode::F4))
		_pRenderManager->ToggleShadows();

	if (Input::IsKeyPressed(KeyCode::F5))
		_pRenderManager->ToggleSoftShadows();

	if (Input::IsKeyPressed(KeyCode::F7))
		EventManager::GetPtr()->QueueEvent(StrongEventDataPtr(DBG_NEW Event_ReloadMaterial()));

	if (Input::IsKeyPressed(KeyCode::F8))
		_pLevel->ReloadLevel();
}

