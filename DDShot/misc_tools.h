
#pragma once

#define KeyPressed(k) (!!(GetAsyncKeyState(k) & 0x8000))

BOOL PatchMemPtrC(ULONG dwAddr, PVOID lpNewValue, PVOID* lpOldValue);
LPSTR GetShortExeNameA(LPSTR OutBuf, DWORD BufSize, bool bCutExtension = true);
LPSTR GetPathUnderExeA(CHAR OutBuf[MAX_PATH], LPCSTR FileName);
