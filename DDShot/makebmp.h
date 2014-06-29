
#pragma once

#include <ddraw.h>

using namespace std;

HRESULT GatherDDPalette(LPDIRECTDRAWPALETTE pPalette, RGBQUAD pResultQuad[256]);
BOOL CopyPaletteTable(RGBQUAD Dest[256], RGBQUAD Source[256]);
BOOL Produce8BitBMPFromSurface(LPCSTR lpFileName, LPDIRECTDRAWSURFACE pSurface);
BOOL Make8BitBMP(LPCSTR lpFileName, DWORD dwWidth, DWORD dwHeight, PVOID pPictureData, RGBQUAD pPaletteTable[256]);
BOOL WriteBytesToFile(LPCSTR lpFileName, PVOID lpFileData, size_t Size);
