
#pragma once

#include <ddraw.h>

typedef HRESULT(WINAPI *TSCL)(LPDIRECTDRAW, HWND, DWORD);
typedef HRESULT(WINAPI *DDCREATE)(GUID*, LPDIRECTDRAW*, IUnknown*);

#define __SetCooperativeLevel 0x50

void InstallHooks();
void UninstallHooks();

