
#pragma once

#include <ddraw.h>

#define EnumSurfacesCustom(AllLayers, SurfDesc, CallbackFunc) \
	EnumSurfaces(DDENUMSURFACES_DOESEXIST + 1 + !AllLayers, SurfDesc, (PVOID)AllLayers, (LPDDENUMSURFACESCALLBACK)CallbackFunc)

BOOL GetAppStrings();
BOOL PrepareScreenshot(LPDIRECTDRAW lpDD, BOOL bLayered = false);
LPSTR GetScreenshotPath(LPSTR OutBuf, DWORD BufSize, BOOL bLayered = false);
HRESULT WINAPI EnumScreenshot(LPDIRECTDRAWSURFACE, LPDDSURFACEDESC, BOOL);
//HRESULT WINAPI EnumScreenshot(LPDIRECTDRAWSURFACE4, LPDDSURFACEDESC2, LPVOID);
