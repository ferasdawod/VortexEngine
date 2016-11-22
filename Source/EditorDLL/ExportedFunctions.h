#pragma once

#include <Windows.h>

#ifdef EDITORDLL_EXPORTS
	#define EDITOR_API __declspec(dllexport)
#else
	#define EDITOR_API __declspec(dllimport)
#endif

#define EDITOR

extern "C"
{
	// main functions
	EDITOR_API bool		Initialize(int* hInstance, int* hWnd);
	EDITOR_API void		Tick();
	EDITOR_API void		Resize();
	EDITOR_API void		ShutDown();

	// utility functions
	EDITOR_API void		LoadLevel(BSTR levelPath);
}