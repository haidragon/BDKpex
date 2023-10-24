#pragma once
#include "includes.h"
#include <wingdi.h>

namespace overlayOnly
{
	typedef struct LDR_DATA_TABLE_ENTRY
	{
		LIST_ENTRY InLoadOrderModuleList;
		LIST_ENTRY InMemoryOrderModuleList;
		LIST_ENTRY	InInitializationOrderModuleList;
		PVOID DllBase;
		PVOID EntryPoint;
		ULONG SizeOfImage;
		UNICODE_STRING FullDllName;
		UNICODE_STRING BaseDllName;
		ULONG Flags;
		USHORT LoadCount;
		USHORT TlsIndex;
		LIST_ENTRY HashLinks;
		PVOID SectionPointer;
		ULONG CheckSum;
		ULONG TimeDateStamp;
	}LDR_DATA_TABLE_ENTRY, * PLDR_DATA_TABLE_ENTRY;

}

extern "C"
NTKERNELAPI
PVOID
NTAPI
RtlFindExportedRoutineByName(
	PVOID ImageBase,
	PCCH RoutineName
);

typedef HBRUSH(*GdiSelectBrush_t)(_In_ HDC hdc,
	_In_ HBRUSH hbr
	);

typedef BOOL(*PatBlt_t)(_In_ HDC,
	_In_ int x,
	_In_ int y,
	_In_ int w,
	_In_ int h,
	_In_ DWORD
	);

typedef HDC(*NtUserGetDC_t)(HWND hWnd);

typedef HBRUSH(*NtGdiCreateSolidBrush_t)(_In_ COLORREF crColor,
	_In_opt_ HBRUSH	 hbr
	);

typedef int (*ReleaseDC_t)(HDC hdc);
typedef BOOL(*DeleteObjectApp_t)(HANDLE hobj);


typedef BOOL(*NtGdiExtTextOutW_t)(_In_ HDC 	hdc,
	_In_ INT 	x,
	_In_ INT 	y,
	_In_ UINT 	flOpts,
	_In_opt_ LPRECT 	prcl,
	_In_reads_opt_(cwc) LPWSTR 	pwsz,
	_In_range_(0, 0xffff) INT 	cwc,
	_In_reads_opt_(_Inexpressible_(cwc)) LPINT 	pdx,
	_In_ DWORD 	dwCodePage
	);


/* ==== DRAW TEXT ==== */
typedef HFONT(*SelectFont_t)(_In_ HDC 	hdc, //verified
	_In_ HFONT 	hfont
	);

typedef BOOL(*ExtTextOutW_t)(IN HDC hDC, //verified
	IN INT 	XStart,
	IN INT 	YStart,
	IN UINT 	fuOptions,
	IN OPTIONAL LPRECT 	UnsafeRect,
	IN LPWSTR 	UnsafeString,
	IN INT 	Count,
	IN OPTIONAL LPINT 	UnsafeDx,
	IN DWORD 	dwCodePage
	);
typedef DWORD LFTYPE;
typedef HFONT(*HfontCreate_t)(IN PENUMLOGFONTEXDVW pelfw, IN ULONG cjElfw, IN LFTYPE lft, IN FLONG fl, IN PVOID pvCliData); //verified

typedef HBRUSH(*SelectBrush_t)(HDC hdc, HBRUSH hbrush); //verified

/* ===== TEXT END ==== */
namespace overlay
{
	PVOID getSystemRoutineAddress(PCWSTR routine_name);
	PVOID get_system_module_export(LPCWSTR module_name, LPCSTR routine_name);
}

namespace TextString
{
	BOOL extTextOutW_(HDC hdc, INT x, INT y, UINT fuOptions, RECT* lprc, LPWSTR lpString, UINT cwc, INT* lpDx);
	HFONT CreateFontW_(int nHeight,
		int nWidth,
		int nEscapement,
		int nOrientation,
		int nWeight,
		DWORD   fnItalic,
		DWORD   fdwUnderline,
		DWORD   fdwStrikeOut,
		DWORD   fdwCharSet,
		DWORD   fdwOutputPrecision,
		DWORD   fdwClipPrecision,
		DWORD   fdwQuality,
		DWORD   fdwPitchAndFamily,
		LPCWSTR lpszFace);
}