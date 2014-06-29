
#include <Windows.h>
#include <stdio.h>
#include "misc_tools.h"

BOOL PatchMemPtrC(ULONG dwAddr, PVOID lpNewValue, PVOID* lpOldValue)
{
	DWORD dwLastProtection;
	if (!VirtualProtect((PVOID)dwAddr, sizeof(PVOID), PAGE_READWRITE, &dwLastProtection))
		return 0;
	if (lpOldValue) *lpOldValue = *(PVOID*)dwAddr;
	*(PVOID*)dwAddr = lpNewValue;
	return VirtualProtect((PVOID)dwAddr, sizeof(PVOID), dwLastProtection, &dwLastProtection);
}

LPSTR GetShortExeNameA(LPSTR OutBuf, DWORD BufSize, bool bCutExtension)
{
	CHAR *dirend, *namend, buf[MAX_PATH];
	if (GetModuleFileNameA(NULL, buf, MAX_PATH))
	if (dirend = strrchr(buf, '\\'))
	{
		dirend += sizeof(CHAR);
		if (bCutExtension && (namend = strrchr(dirend, '.')))
			*namend = (CHAR)0;
		strcpy_s(OutBuf, BufSize, dirend);
		return OutBuf;
	}
	return 0;
}

LPSTR GetPathUnderExeA(CHAR OutBuf[MAX_PATH], LPCSTR FileName)
{
	CHAR* dirend;
	if (GetModuleFileNameA(NULL, OutBuf, MAX_PATH))
	if (dirend = strrchr(OutBuf, '\\'))
	if (strcpy_s(dirend + sizeof(CHAR), MAX_PATH, FileName) == ERROR_SUCCESS)
		return OutBuf;
	return 0;
}

/*LPSTR GetPathUnderExeA(CHAR OutBuf[MAX_PATH], LPCSTR FileName)
{
	GetModuleFileNameA(NULL, OutBuf, MAX_PATH);
	CHAR* dirend = strrchr(OutBuf, '\\') + 1;
	strcpy_s(dirend, MAX_PATH, FileName);
	return OutBuf;
}*/
