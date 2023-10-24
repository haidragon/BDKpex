#pragma once
#include <ntifs.h>
#include <windef.h>
#include <ntimage.h>
#include <ntddk.h>
//#include <sysinfoapi.h>
#include "ObfuscateCHAR.h"
#include "ObfuscateWCHAR.h"

/*
extern "C" {
	unsigned int _fltused = 1;
}
*/

#define print(text, ...)	(DbgPrintEx(0, 0, text, ##__VA_ARGS__))
#define RVA(addr, size)			((PBYTE)(addr + *(DWORD*)(addr + ((size) - 4)) + size))

typedef struct Vec3
{
	float x, y, z;
};

typedef struct VMatrix
{
	float m[16];
};

typedef struct DrawStringInfo
{
	DWORD size;
	DWORD X;
	DWORD Y;
	LPWSTR string;
};

typedef struct DrawHealthBarInfo
{
	int R, G, B, X, Y, W, H, T;
	int MaxiumHealth, CurrentHealth;
	int BarR, BarG, BarB;
	BOOL DownState;
};
typedef struct BoxInfo
{
	int R, G, B, X, Y, W, H, T;
};

typedef struct DrawInfo
{
	BoxInfo Box;
	DrawHealthBarInfo HealthBar;
	DrawStringInfo stringInfo;
	BoxInfo FillBoxInfo;
};
typedef struct BobbyCommuteStructure
{
	BOOL TestCommunication;

	DWORD Operation;
	DWORD64 Address;
	DWORD64 Buffer;
	DWORD PID;
	DWORD size;
	const char* name;
	const WCHAR* wcharName;
	DrawInfo MBox[64];

	BOOL drawBar;
	BOOL drawBox;
	BOOL drawText;
	BOOL drawFillBox;
	Vec3 vector;
	VMatrix matrix;

	LPWSTR ConsoleOut;
};

