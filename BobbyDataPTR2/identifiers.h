#pragma once
#include "structures.h"
#define TESTCOMMUNICATION 0x48182938
#define WRITEMEMORY 0x19781202
#define READMEMORY 0x19750621
#define GETABSOLUTEMODULEBASE 0x20021206
#define GETMODULEBASEBYNAME 0x28374956
#define OVERLAY 0x18279283
#define GETPROCESSID 0x29182736
#define GETPROCESSID_TRAD_VDP 0x2837489A
#define DRAWOVERLAY 0x69877332
#define CLEARAPCFLAG 0x85712832
#define RESTOREAPCFLAG 0x81379837
#define AD_READMATRIX 0x478126
#define AD_READVECTOR 0x478126
#define CS_GETMODULEBASE 0x92839
#define PATTERNSCAN 0x4927192
#define MOVECURSOR 0x8927212

namespace process
{
	void* get_sys_module(const char* module_name);
	PBYTE FindPattern(PVOID base, LPCSTR pattern, LPCSTR mask);
	bool NullPageFrameNumbersFromMdl(PMDL mdl);
}
namespace utilities
{
	void printEx(PCCH format, ...);
}
namespace memory
{
	uintptr_t get_module_base_with_attatch(WCHAR* ModuleName, PEPROCESS m_Process, ULONG* SizeofImage);
	ULONG64 GetModuleBasex64PEB(PEPROCESS proc, const WCHAR* ModuleName, ULONG* ModuleSize);
	
}
namespace pattern
{
	uintptr_t PatternFind(PEPROCESS pp, uintptr_t start, uintptr_t length, const char* pattern, const char* mask);
}
namespace VirtualMethods
{
	ULONG GetProcessIDViaZwQuerySystemInformation(UNICODE_STRING targetName);
}

namespace Traditional
{
	PEPROCESS GetPeProcessViaPID(DWORD PID);
	NTSTATUS GetPEProcessViaName_VersionDependent(CHAR* process_name, PEPROCESS* process);
	NTSTATUS GetSecondPEProcessViaName_VersionDependent(CHAR* process_name, PEPROCESS* process);
	BOOL NullFPN(PMDL mdl);
	UINT32 APCUnQueuable(PETHREAD commsthread);
	bool RestoreAPCQueuable(PETHREAD commsthread, UINT32 OriginalValue);
	NTSTATUS init_mouse(PMOUSE_OBJECT mouse_obj);
	void mouse_event(MOUSE_OBJECT mouse_obj, long x, long y, unsigned short button_flags);
}

NTSTATUS EnchancedReadMemory(PEPROCESS pp, ULONG64 Address, PVOID AllocatedBuffer, SIZE_T size, SIZE_T* read);
NTSTATUS EnchancedWriteMemory(PEPROCESS pp, ULONG64 Address, PVOID AllocatedBuffer, SIZE_T size, SIZE_T* written);

namespace overlay
{
	NTSTATUS DrawBox(int r, int g, int b, int x, int y, int w, int h, int t);
	NTSTATUS DrawText(DrawStringInfo stringInfo);
}

namespace CompositeOverlay
{
	BOOL Draw(DrawHealthBarInfo DHBInfo[20], DWORD BarAmount);
	BOOL Draw2(LPWSTR ConsoleMessage[5]);
}

BOOL initOverlay();

namespace ModularOverlay
{
	BOOL drawBox(HDC hdc, int r, int g, int b, int x, int y, int w, int h, int t);
	BOOL drawBar(HDC hdc, BoxInfo BoxInfo, DWORD MaxHealth, DWORD CurrentHealth, int R, int G, int B);
	BOOL drawText(HDC hdc, LPWSTR string, int x, int y, int Size);
	BOOL drawFillBox(HDC hdc, int r, int g, int b, int x, int y, int w, int h, int t);
	HDC GetHDC();
	BOOL ReleaseHDC(HDC hdc);
}