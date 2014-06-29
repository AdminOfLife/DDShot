
#include <windows.h>
#include "hooks.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
		InstallHooks();
	else if (dwReason == DLL_PROCESS_DETACH)
		UninstallHooks();

	return 1;
}

