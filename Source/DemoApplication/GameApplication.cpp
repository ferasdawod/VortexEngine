#include "GameApplication.h"

#include "Input/InputDevice.h"
#include "Input/Input.h"

#include "Graphics/Rendering/RenderManager.h"
#include "Actors/Actor.h"
#include "Actors/ActorFactory.h"
#include "Components/Transform.h"
#include <Components/Camera.h>
#include "Level/Level.h"

#include "Events/EventManager.h"
#include "Application/SettingsManager.h"
#include "Events/EventsTypes.h"

#include "Debugging/Debug.h"
#include "3rd Party/TinyXml/tinyxml.h"

#include <sstream>
#include <Graphics/ViewPort.h>

GameApplication::GameApplication()
{
}

GameApplication::~GameApplication() 
{
	if (_pPlayer)
		_pPlayer->Destroy();
}

bool GameApplication::Init()
{
	if (!WindowsApplication::Init())
		return false;

	_pInputDevice.reset(DBG_NEW InputDevice());
	auto result = _pInputDevice->Init(_hInstace, _hWnd);
	if (!result)
		return false;

	_pRenderManager.reset(DBG_NEW RenderManager());
	result = _pRenderManager->Initialize(_hWnd);
	if (!result)
		return false;

	// creating a test level
	//_pLevel.reset(DBG_NEW Level("Demo"));
	//
	//ActorFactory actorFactory;
	//for (auto index_i = 0; index_i < 20; index_i++)
	//{
	//	for (auto index_j = 0; index_j < 20; index_j++)
	//	{
	//		auto actor = actorFactory.CreateFromXML("Assets\\Actors\\obj.actor");
	//		actor->GetTransform()->SetPosition(Vector3(index_i, 0.f, index_j));
	//		_pLevel->AddActor(actor);
	//	}
	//}
	//
	//auto light = actorFactory.CreateDirectionalLight();
	//light->GetTransform()->Rotate(30.f, 30.f, 0.f);
	//_pLevel->AddActor(light);
	//
	//auto ground = actorFactory.CreatePlaneActor();
	////ground->GetTransform()->SetScale(Vector3(5.f, 5.f, 5.f));
	//ground->GetTransform()->SetPosition(Vector3(0.f, -3.f, 0.f));
	//_pLevel->AddActor(ground);
	//
	//_pPlayer = actorFactory.CreateCameraActor();

	ActorFactory actorFactory;
	_pLevel = Level::CreateFromFile("Assets\\Levels\\SuperMegaLevel.level");
	_pPlayer = actorFactory.CreateCameraActor();
	
	auto playerTransform = _pPlayer->GetComponent<Transform>().lock();
	playerTransform->SetPosition(Vector3(-5.0f, 5.0f, -10.0f));
	playerTransform->Rotate(-130.0f, -30.0f, 0.0f);

	auto camera = _pPlayer->GetComponent<Camera>().lock();
	camera->SetViewPort(ViewPort(0.25f, 0.25f, 0.5f, 0.5f));

	return true;
}

void GameApplication::OnUpdate()
{
	_pInputDevice->OnUpdate();

	auto lightActor = _pLevel->FindActor("Directional Light").lock()->GetTransform();
	lightActor->Rotate(_gameTimer.DeltaTime() * 10.f, 0.0f, 0.0f);
	
	auto earthActor = _pLevel->FindActor("Earth Sphere").lock()->GetTransform();
	earthActor->Rotate(_gameTimer.DeltaTime() * 30.f, 0.f, 0.f);

	_pLevel->OnUpdate(_gameTimer);
	_pPlayer->OnUpdate(_gameTimer);

	_pEventManager->OnUpdate(_gameTimer);

	//UpdateStates();
	UpdateDebugInput();
}

void GameApplication::OnRender()
{
	_pRenderManager->OnRender();
}

void GameApplication::UpdateStates()
{
	using namespace std;

	static int frameCount = 0;
	static float timeElapsed = 0.0f;

	++frameCount;

	if ((_gameTimer.GameTime() - timeElapsed) >= 0.1f)
	{
		float fps = static_cast<float>(frameCount);
		float milliSecPerFrame = 1000.0f / fps;

		stringstream stream;
		stream.precision(6);
		stream << _sAppName << " - " << "FPS: " << fps << " - " << "Frame Time: " << milliSecPerFrame << "(ms)";
		SetWindowText(_hWnd, stream.str().c_str());

		frameCount = 0;
		timeElapsed += 1.0f;
	}
}

void GameApplication::UpdateDebugInput()
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