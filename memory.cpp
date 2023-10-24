#include "includes.h"
#include "memory.h"

SHORT(*NtUserGetAsyncKeyState)(INT Key);
PVOID(__fastcall* DataPointer)(DWORD a1, DWORD a2, DWORD a3, DWORD a4);
namespace confirmed
{
	DWORD processId;
	DWORD64 ModuleBase;
}
namespace initialization
{
	BOOL init()
	{
		std::cout << skCrypt("          INITIATION START") << std::endl;

		if (!GetModuleHandleA(skCrypt("win32u.dll")))
		{
			std::cout << skCrypt("[CRITICAL ERROR] System Dynamic Link Libraries Failed to Load") << std::endl;
			Sleep(5000);
			return false;
		}
		*(PVOID*)&DataPointer = GetProcAddress(GetModuleHandleA(skCrypt("win32u.dll")), skCrypt("NtUserCreateDesktopEx"));
		if (!DataPointer)
		{
			std::cout << skCrypt("[CRITICAL ERROR] Pointers Not Found") << std::endl;
			Sleep(5000);
			return false;
		}
		if (!process::TestCommunication())
		{
			std::cout << skCrypt("[CRITICAL ERROR] Driver Not Responding")
				<< std::endl;
			Sleep(5000);
			return false;
		}
		return true;
	}
}

namespace process
{
	DWORD GetprocessID_TRAD_VDP(const char* name)
	{
		BobbyCommuteStructure CommStruct = {};
		SecureZeroMemory(&CommStruct, sizeof(BobbyCommuteStructure));
		CommStruct.Operation = GETPROCESSID_TRAD_VDP;
		CommStruct.name = name;
		DWORD64 FullPath = (DWORD64)&CommStruct;
		DWORD H1 = (DWORD)(FullPath >> 32);
		DWORD H2 = (DWORD)(FullPath);
		DataPointer(0x0, 0x1206, H1, H2);
		return CommStruct.PID;
	}
	DWORD GetSecondprocessID_TRAD_VDP(const char* name)
	{
		BobbyCommuteStructure CommStruct = {};
		SecureZeroMemory(&CommStruct, sizeof(BobbyCommuteStructure));
		CommStruct.Operation = GETPROCESSID_TRAD_VDP;
		CommStruct.name = name;
		CommStruct.Address = 0x2;
		DWORD64 FullPath = (DWORD64)&CommStruct;
		DWORD H1 = (DWORD)(FullPath >> 32);
		DWORD H2 = (DWORD)(FullPath);
		DataPointer(0x0, 0x1206, H1, H2);
		return CommStruct.PID;
	}

	BOOL TestCommunication()
	{
		BobbyCommuteStructure CommStruct = {};
		SecureZeroMemory(&CommStruct, sizeof(BobbyCommuteStructure));
		CommStruct.Operation = TESTCOMMUNICATION;
		DWORD64 FullPath = (DWORD64)&CommStruct;
		DWORD H1 = (DWORD)(FullPath >> 32);
		DWORD H2 = (DWORD)(FullPath);
		DataPointer(0x0, 0x1206, H1, H2);
		return CommStruct.TestCommunication;
	}

	DWORD64 GetAbsoluteModuleBase(DWORD PID)
	{
		BobbyCommuteStructure CommStruct = {};
		SecureZeroMemory(&CommStruct, sizeof(BobbyCommuteStructure));
		CommStruct.Operation = GETABSOLUTEMODULEBASE;
		CommStruct.PID = PID;
		DWORD64 FullPath = (DWORD64)&CommStruct;
		DWORD H1 = (DWORD)(FullPath >> 32);
		DWORD H2 = (DWORD)(FullPath);
		DataPointer(0x0, 0x1206, H1, H2);
		return CommStruct.Address;
	}

	BOOL init(const char* process_name, const char* module_name, DWORD64* ModuleBase, DWORD* ProcId)
	{
		DWORD64 Modulebase = NULL;
		DWORD processId = GetprocessID_TRAD_VDP(process_name);
		if (processId == NULL)
		{
			return false;
		}
		if (module_name == NULL || strlen(module_name) == NULL)
		{
			Modulebase = GetAbsoluteModuleBase(processId);
			if (!Modulebase)
			{
				return false;
			}
		}
		else
		{
			/* read named module base */
		}
		confirmed::processId = processId;
		*ProcId = processId;
		confirmed::ModuleBase = Modulebase;
		*ModuleBase = Modulebase;
		return true;
	}
}


DWORD PassProcessId()
{
	if (!confirmed::processId)
	{
		std::cout << skCrypt("[ERROR] ProcessId Empty") << std::endl;
	}
	return confirmed::processId;
}
DWORD64 PassModuleBase()
{
	if (!confirmed::ModuleBase)
	{
		std::cout << skCrypt("[ERROR] ModuleBase Empty") << std::endl;
	}
	return confirmed::ModuleBase;
}
namespace memory
{
	bool ReadMemory(DWORD64 address, PVOID pBuffer, DWORD size)
	{
		if (!confirmed::processId)
		{
			std::cout << skCrypt("[CRITICAL ERROR] PID EMPTY") << std::endl;
			return NULL;
		}
			
		BobbyCommuteStructure CommStruct = {};
		SecureZeroMemory(&CommStruct, sizeof(BobbyCommuteStructure));
		CommStruct.Operation = READMEMORY;
		CommStruct.Address = address;
		CommStruct.size = size;
		CommStruct.PID = confirmed::processId;
		CommStruct.Buffer = (DWORD64)pBuffer;
		DWORD64 FullPath = (DWORD64)&CommStruct;
		DWORD H1 = (DWORD)(FullPath >> 32);
		DWORD H2 = (DWORD)(FullPath);
		DataPointer(0x0, 0x1206, H1, H2);
		return true;
	}

	BOOL WriteMemory(DWORD64 address, PVOID pbuffer, DWORD Size)
	{
		if (!confirmed::processId)
			return NULL;
		BobbyCommuteStructure CommStruct = {};
		SecureZeroMemory(&CommStruct, sizeof(BobbyCommuteStructure));
		CommStruct.Address = address;
		CommStruct.size = Size;
		CommStruct.PID = confirmed::processId;
		CommStruct.Buffer = (DWORD64)pbuffer;
		CommStruct.Operation = WRITEMEMORY;
		DWORD64 FullPath = (DWORD64)&CommStruct;
		DWORD H1 = (DWORD)(FullPath >> 32);
		DWORD H2 = (DWORD)(FullPath);
		DataPointer(0x0, 0x1206, H1, H2);
		return true;
	}

	BOOL render(BOOL drawBar, BOOL drawText, BOOL drawBox, BOOL drawFill, DrawInfo DrawList[64])
	{
		BobbyCommuteStructure CommStruct = {};
		SecureZeroMemory(&CommStruct, sizeof(BobbyCommuteStructure));
		CommStruct.Operation = DRAWOVERLAY;
		CommStruct.drawBar = drawBar;
		CommStruct.drawText = drawText;
		CommStruct.drawBox = drawBox;
		CommStruct.drawFillBox = drawFill;
		for (int i = 0; i < 64; i++)
		{
			CommStruct.MBox[i] = DrawList[i];
		}

		DWORD64 FullPath = (DWORD64)&CommStruct;
		DWORD H1 = (DWORD)(FullPath >> 32);
		DWORD H2 = (DWORD)(FullPath);
		DataPointer(0x0, 0x1206, H1, H2);
		return (DWORD32)CommStruct.Buffer;
	}

	uint32_t ClearAPCFlag()
	{
		BobbyCommuteStructure CommStruct = {};
		SecureZeroMemory(&CommStruct, sizeof(BobbyCommuteStructure));
		CommStruct.Operation = CLEARAPCFLAG;
		DWORD64 FullPath = (DWORD64)&CommStruct;
		DWORD H1 = (DWORD)(FullPath >> 32);
		DWORD H2 = (DWORD)(FullPath);
		DataPointer(0x0, 0x1206, H1, H2);
		return CommStruct.Buffer;
	}

	bool RestoreAPCFlag(uint32_t buffer)
	{
		BobbyCommuteStructure CommStruct = {};
		SecureZeroMemory(&CommStruct, sizeof(BobbyCommuteStructure));
		CommStruct.Operation = RESTOREAPCFLAG;
		CommStruct.Buffer = buffer;
		DWORD64 FullPath = (DWORD64)&CommStruct;
		DWORD H1 = (DWORD)(FullPath >> 32);
		DWORD H2 = (DWORD)(FullPath);
		DataPointer(0x0, 0x1206, H1, H2);
		return true;
	}

	bool SetMouseEvent(long x, long y, unsigned short bu)
	{
		BobbyCommuteStructure CommStruct = {};
		SecureZeroMemory(&CommStruct, sizeof(BobbyCommuteStructure));
		CommStruct.Operation = MOVECURSOR;

		NF_MOUSE_REQUEST buffer;
		buffer = { x, y, bu };
		CommStruct.Buffer = (DWORD64)&buffer;
		DWORD64 FullPath = (DWORD64)&CommStruct;
		DWORD H1 = (DWORD)(FullPath >> 32);
		DWORD H2 = (DWORD)(FullPath);
		DataPointer(0x0, 0x1206, H1, H2);
		return true;
	}

}
