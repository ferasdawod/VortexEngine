#include "GameApplication.h"
#include "Debugging/Debug.h"


int custom_main(HINSTANCE hInstance)
{
	DEBUG_LEAK_CHECK();

	std::unique_ptr<IApplication> app(DBG_NEW GameApplication);
	bool retValue = app->Run(hInstance);

	return retValue ? 0 : 1;
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
