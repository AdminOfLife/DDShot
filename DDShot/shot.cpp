
#include <Windows.h>
#include <stdio.h>
#include "shot.h"
#include "misc_tools.h"
#include "makebmp.h"

UINT nLayer = 0;
char SubDirPath[MAX_PATH], ExeName[MAX_PATH], DirPath[MAX_PATH];

BOOL PrepareScreenshot(LPDIRECTDRAW lpDD, BOOL bLayered)
{
	if (lpDD)
	{
		if (!bLayered)
		{
			DDSURFACEDESC ReqDesc;
			ZeroMemory(&ReqDesc, sizeof(DDSURFACEDESC));
			ReqDesc.dwSize = sizeof(DDSURFACEDESC);
			ReqDesc.dwFlags = DDSD_BACKBUFFERCOUNT;
			ReqDesc.dwBackBufferCount = 1;
			return SUCCEEDED(lpDD->EnumSurfacesCustom(bLayered, &ReqDesc, EnumScreenshot));
		}
		else
		{
			nLayer = 0;
			return SUCCEEDED(lpDD->EnumSurfacesCustom(bLayered, NULL, EnumScreenshot));
		}
	}
	return 0;
}

HRESULT WINAPI EnumScreenshot(LPDIRECTDRAWSURFACE pSurface, LPDDSURFACEDESC lpSurfaceDesc, BOOL bLayered)
{
	char SSPath[MAX_PATH];
	if (GetScreenshotPath(SSPath, MAX_PATH, bLayered))
		Produce8BitBMPFromSurface(SSPath, pSurface);
	return bLayered;
}

BOOL GetAppStrings()
{
	return GetPathUnderExeA(DirPath, "Screenshots") && GetShortExeNameA(ExeName, MAX_PATH);
}

LPSTR GetScreenshotPath(LPSTR OutBuf, DWORD BufSize, BOOL bLayered)
{
	if (OutBuf)
	{
		SYSTEMTIME sysTime;
		BOOL bCreateDir = CreateDirectoryA(DirPath, 0);
		if (bCreateDir || GetLastError() == ERROR_ALREADY_EXISTS)
		if (!bLayered)
		{
			GetSystemTime(&sysTime);
			if (sprintf_s(OutBuf, BufSize, "%s\\%s_%04u%02u%02u_%02u-%02u-%02u-%03u.bmp", DirPath, ExeName,
					sysTime.wYear, sysTime.wMonth, sysTime.wDay,
					sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds))
				return OutBuf;
		}
		else
		{
			if (!nLayer)
			{
				GetSystemTime(&sysTime);
				if (!sprintf_s(SubDirPath, MAX_PATH, "%s\\%s_%04u%02u%02u_%02u-%02u-%02u-%03u", DirPath, ExeName,
						sysTime.wYear, sysTime.wMonth, sysTime.wDay,
						sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds))
					return 0;
				bCreateDir = CreateDirectoryA(SubDirPath, 0);
				if (!(bCreateDir || GetLastError() == ERROR_ALREADY_EXISTS))
					return 0;
			}

			nLayer++;
			if (sprintf_s(OutBuf, BufSize, "%s\\Layer_%u.bmp", SubDirPath, nLayer))
				return OutBuf;
		}
	}
	return 0;
}
