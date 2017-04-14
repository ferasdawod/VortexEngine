
#include <Windows.h>

#include "Debugging/Debug.h"
#include <Engine/Engine.h>
#include "MyApplication.h"


int custom_main(HINSTANCE hInstance)
{
	DEBUG_LEAK_CHECK();

	{
		Core::Engine engine;
		std::unique_ptr<MyApplication> custom_game(DBG_NEW MyApplication);
		engine.Run(std::move(custom_game));
	}

	return (EXIT_SUCCESS);
}


int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	return custom_main(hInstance);
}

int main()
{
	auto hInstance = GetModuleHandle(nullptr);
	return custom_main(hInstance);
}