#include "ExportedFunctions.h"

#include "Source/EditorApplication.h"
#include "Graphics/Rendering/RenderManager.h"
#include "Actors/Actor.h"
#include "Level/Level.h"
#include "Input/InputDevice.h"
#include "Utilities/Utils.h"
#include "Debugging/Debug.h"

#include <iostream>
using std::cout;
using std::endl;

EDITOR_API bool Initialize(int* hInstance, int* hWnd)
{
	cout << "Begin Initializing" << endl;

	Assert(_gspEditorApplication == nullptr, "The editor application is already initialized");
	_gspEditorApplication = DBG_NEW EditorApplication;

	HINSTANCE instance = (HINSTANCE)hInstance;
	HWND wnd = (HWND)hWnd;
	bool ret = _gspEditorApplication->Initialize(instance, wnd);

	cout << "Finished Initializing" << endl;
	return ret;
}

EDITOR_API void Tick()
{
	_gspEditorApplication->Tick();
}

EDITOR_API void Resize()
{
	cout << "Resize()" << endl;
	_gspEditorApplication->Resize();
}

EDITOR_API void ShutDown()
{
	cout << "ShutDown()" << endl;

	Assert(_gspEditorApplication != nullptr, "Shutting down an application that wasn't initialized");
	if (_gspEditorApplication)
	{
		_gspEditorApplication->OnShutDown();
		SAFE_DELETE(_gspEditorApplication);
	}
}

EDITOR_API void LoadLevel(BSTR levelPath)
{
	auto levelStr = Utils::WideStr2Str(std::wstring(levelPath, SysStringLen(levelPath)));
	cout << "Loading Level : " << levelStr << endl;
	_gspEditorApplication->LoadLevel(levelStr);
}
