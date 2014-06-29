
#include <windows.h>
#include <stdio.h>
#include "hooks.h"
#include "makebmp.h"
#include "misc_tools.h"
#include "shot.h"

HHOOK kHook;
HMODULE ddraw;
LPDIRECTDRAW DDObj, DDSherlock;
TSCL NextSCL;
ULONG DDVT;

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (HC_ACTION == nCode)
	if (!!!(lParam & INT_MIN)) //key is in a held state
	if (wParam == VK_PAUSE) // Pause
		PrepareScreenshot(DDObj, KeyPressed(VK_MENU)); //Alt+Pause will give layers

	return CallNextHookEx(kHook, nCode, wParam, lParam);
}

HRESULT WINAPI HookSCL(LPDIRECTDRAW lpTarget, HWND hWnd, DWORD dwFlags)
{
	DDObj = lpTarget;
	return NextSCL(lpTarget, hWnd, dwFlags);
}

void InstallHooks()
{
	GetAppStrings();
	if (!kHook) kHook = SetWindowsHookEx(WH_KEYBOARD, (HOOKPROC)KeyboardProc, 0, GetCurrentThreadId());
	if (!ddraw && (ddraw = LoadLibraryA("ddraw.dll")))
	{
		if (DDCREATE DirectDrawCreate = (DDCREATE)GetProcAddress(ddraw, "DirectDrawCreate"))
		{
			if (SUCCEEDED(DirectDrawCreate(0, &DDSherlock, 0)))
			{
				DDVT = *(PULONG)DDSherlock;
				PatchMemPtrC(DDVT + __SetCooperativeLevel, HookSCL, (PVOID*)&NextSCL);
				DDSherlock->Release();
			}
		}
	}
}

void UninstallHooks()
{
	if (kHook) UnhookWindowsHookEx(kHook);
	if (ddraw)
	{
		PatchMemPtrC(DDVT + __SetCooperativeLevel, NextSCL, 0);
		FreeLibrary(ddraw);
	}
}
