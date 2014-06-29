
#include <Windows.h>
#include <fstream>
#include <WinGDI.h>
#include "makebmp.h"

BOOL Produce8BitBMPFromSurface(LPCSTR lpFileName, LPDIRECTDRAWSURFACE pSurface)
{
	BOOL result = 0;
	LPDIRECTDRAWPALETTE lpDDPalette = 0;
	if (SUCCEEDED(pSurface->GetPalette(&lpDDPalette)))
	{
		DDSURFACEDESC SurfDesc;
		ZeroMemory(&SurfDesc, sizeof(DDSURFACEDESC));
		SurfDesc.dwSize = sizeof(DDSURFACEDESC);

		if (SUCCEEDED(pSurface->Lock(0, &SurfDesc, DDLOCK_WAIT, 0)))
		{
			RGBQUAD PaletteQuad[256];
			if (SUCCEEDED(GatherDDPalette(lpDDPalette, PaletteQuad)))
				result = Make8BitBMP(lpFileName, SurfDesc.dwWidth, SurfDesc.dwHeight, SurfDesc.lpSurface, PaletteQuad);
			pSurface->Unlock(0);
		}
	}
	return result;
}

HRESULT GatherDDPalette(LPDIRECTDRAWPALETTE pPalette, RGBQUAD pResultQuad[256])
{
	HRESULT result = 0;
	DWORD PalEntry = 0;
	LPPALETTEENTRY lpPalEntry = (LPPALETTEENTRY)&PalEntry;

	for (int i = 0; i < 256; i++)
	{
		if (SUCCEEDED(result = pPalette->GetEntries(0, i, 1, lpPalEntry)))
		{
			pResultQuad[i].rgbRed = lpPalEntry->peRed;
			pResultQuad[i].rgbGreen = lpPalEntry->peGreen;
			pResultQuad[i].rgbBlue = lpPalEntry->peBlue;
			pResultQuad[i].rgbReserved = 0;
		}
		else break;
	}

	return result;
}

BOOL CopyPaletteTable(RGBQUAD Dest[256], RGBQUAD Source[256])
{
	if (Dest && Source)
	for (int i = 0; i < 256; i++)
	{
		Dest[i].rgbRed      = Source[i].rgbRed;
		Dest[i].rgbGreen    = Source[i].rgbGreen;
		Dest[i].rgbBlue     = Source[i].rgbBlue;
		Dest[i].rgbReserved = Source[i].rgbReserved;
	}
	else return 0;
	return 1;
}

BOOL Make8BitBMP(LPCSTR lpFileName, DWORD dwWidth, DWORD dwHeight, PVOID pPictureData, RGBQUAD pPaletteTable[256])
{
	BOOL result = 0;
	DWORD FileSize = 1078 + dwWidth * dwHeight;
	BYTE* FileData = new BYTE[FileSize];

	LPBITMAPFILEHEADER lpBmpHeader = (LPBITMAPFILEHEADER)&FileData[0];
	LPBITMAPINFO lpBmpInfo = (LPBITMAPINFO)&FileData[14];
	
	lpBmpHeader->bfType = 'MB';
	lpBmpHeader->bfSize = FileSize;
	lpBmpHeader->bfReserved1 = 0;
	lpBmpHeader->bfReserved2 = 0;
	lpBmpHeader->bfOffBits = 1078;

	lpBmpInfo->bmiHeader.biSize = 40;
	lpBmpInfo->bmiHeader.biWidth = dwWidth;
	lpBmpInfo->bmiHeader.biHeight = dwHeight;
	lpBmpInfo->bmiHeader.biPlanes = 1;
	lpBmpInfo->bmiHeader.biBitCount = 8;
	lpBmpInfo->bmiHeader.biCompression = 0;
	lpBmpInfo->bmiHeader.biSizeImage = dwWidth * dwHeight;
	lpBmpInfo->bmiHeader.biXPelsPerMeter = 3780;
	lpBmpInfo->bmiHeader.biYPelsPerMeter = 3780;
	lpBmpInfo->bmiHeader.biClrUsed = 256;
	lpBmpInfo->bmiHeader.biClrImportant = 256;

	CopyPaletteTable(lpBmpInfo->bmiColors, pPaletteTable);

	for (UINT i = dwHeight; i > 0; i--)
		memcpy(&FileData[1078 + (dwWidth*(i - 1))], PVOID(DWORD(pPictureData) + dwWidth*(dwHeight - i)), dwWidth);

	result = WriteBytesToFile(lpFileName, FileData, FileSize);

	delete[] FileData;
	return result;
}

BOOL WriteBytesToFile(LPCSTR lpFileName, PVOID lpFileData, size_t Size)
{
	BOOL result = 0;
	ofstream bmpFile(lpFileName, ofstream::binary);
	if (bmpFile)
	{
		bmpFile.write((LPCSTR)lpFileData, Size);
		result = bmpFile.ios::good();
		bmpFile.close();
	}
	return result;
}
