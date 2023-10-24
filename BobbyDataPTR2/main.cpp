#include "identifiers.h" /*including includes.h*/

INT64(__fastcall* OriginalPointer)(DWORD, DWORD, DWORD, DWORD);
INT64(__fastcall* ApiSetGetPenArbitrationType)(DWORD, DWORD, DWORD, DWORD);
MOUSE_OBJECT mouse_obj = { 0 };

INT64 __fastcall HackedPointer(DWORD a1, DWORD a2, DWORD a3, DWORD a4) 
{
	//print(skCrypt("Function Called! \n"));
	if (a2 == 0x1206)
	{
		//print("Function Decrypted!");
		DWORD64 Combined = 0x0;
		Combined = ((DWORD64)a3) << 32 | a4;
		BobbyCommuteStructure* argument = (BobbyCommuteStructure*)Combined;
		auto args = (BobbyCommuteStructure*)argument;
		if (!mouse_obj.service_callback || !mouse_obj.mouse_device) { Traditional::init_mouse(&mouse_obj); }
		/* re-construct address */
		if (args->Operation == GETPROCESSID) /* Universal Method */
		{
			//print(skCrypt("[Bobby] Process ID"));
			ANSI_STRING AS;
			UNICODE_STRING ProcessName;
			RtlInitAnsiString(&AS, args->name);
			RtlAnsiStringToUnicodeString(&ProcessName, &AS, TRUE);
			args->PID = VirtualMethods::GetProcessIDViaZwQuerySystemInformation(ProcessName);
			RtlFreeUnicodeString(&ProcessName);
		}
		else if (args->Operation == TESTCOMMUNICATION)
		{
			args->TestCommunication = true;
		}
		else if (args->Operation == GETPROCESSID_TRAD_VDP)
		{
			if (args->Address == 0x0)
			{
				PEPROCESS process = 0x0;
				Traditional::GetPEProcessViaName_VersionDependent((char*)(args->name), &process);
				if (!process)
				{
					print(skCrypt("Failed to get PEPROCESS"));
				}
				else
				{
					args->PID = (DWORD)PsGetProcessId(process);
				}
			}
			else if (args->Address == 0x2)
			{
				PEPROCESS process = 0x0;
				Traditional::GetSecondPEProcessViaName_VersionDependent((char*)(args->name), &process);
				if (!process)
				{
					print(skCrypt("Failed to get Second PEPROCESS"));
				}
				else
				{
					args->PID = (DWORD)PsGetProcessId(process);
				}
			}
		}
		else if (args->Operation == GETMODULEBASEBYNAME) /* IN PEPROCESS & processID | OUT Modulebase & ImageSize*/
		{
			PEPROCESS process = NULL;
			if (NT_SUCCESS(PsLookupProcessByProcessId((HANDLE)args->PID, &process)))
			{
				ULONG ModuleSize = 0x0;
				ULONG64 ModuleBase = 0x0;
				ModuleBase = memory::GetModuleBasex64PEB(process, args->wcharName, &ModuleSize);
				args->Address = ModuleBase;
				args->size = ModuleSize;
			}
			else
			{
				print(skCrypt("[ERROR] GET MODULE ABSOLUTE MODULE BASE"));
			}
		}
		else if (args->Operation == GETABSOLUTEMODULEBASE) /* IN peprocess OUT Address*/
		{
			if (args->PID)
			{
				PEPROCESS process = NULL;
				if (NT_SUCCESS(PsLookupProcessByProcessId((HANDLE)args->PID, &process)))
				{
					args->Address = reinterpret_cast <DWORD64>(PsGetProcessSectionBaseAddress(process));
				}
				else
				{
					print(skCrypt("[ERROR] GET MODULE ABSOLUTE MODULE BASE"));
				}
			}
			else
			{
				print(skCrypt("[ERROR] GET MODULEBASE PEPROCESS INPUT EMPTY"));
			}
		}
		else if (args->Operation == CS_GETMODULEBASE) /* CSGO only modulebase*/
		{
			PEPROCESS process = NULL;
			//print("cs module called");
			PsLookupProcessByProcessId((HANDLE)args->PID, &process);
			if (process)
			{
				if (args->Buffer == 0x1) /* Client */
				{
					
					ULONG Size = 0x0;
					args->Address = memory::get_module_base_with_attatch(skCrypt(L"client.dll"), process, &Size);
					
					args->size = Size;
				}
				else if (args->Buffer == 0x2) /* Engine */
				{
					ULONG Size = 0x0;
					args->Address = memory::get_module_base_with_attatch(skCrypt(L"engine.dll"), process, &Size);
					
					args->size = Size;
				}
			}
		}
		else if (args->Operation == CLEARAPCFLAG)
		{
			UINT32 Original_Misc_Flags = Traditional::APCUnQueuable(PsGetCurrentThread());
			args->Buffer = Original_Misc_Flags;
		}
		else if (args->Operation == RESTOREAPCFLAG)
		{
			if (args->Buffer)
			{
				Traditional::RestoreAPCQueuable(PsGetCurrentThread(), args->Buffer);
			}
		}
		else if (args->Operation == READMEMORY)
		{
			if (args->PID && (DWORD64)args->Address)
			{
				SIZE_T Bytes_read = 0x0;
				EnchancedReadMemory(Traditional::GetPeProcessViaPID(args->PID), (ULONG64)args->Address, (PVOID)args->Buffer, args->size, &Bytes_read);
				if (Bytes_read != args->size)
				{
					//print(skCrypt("[WARNING] Read Size Mismatch"));
				}
			}
		}
		else if (args->Operation == AD_READMATRIX)
		{
			if (args->PID && (DWORD64)args->Address)
			{
				SIZE_T Bytes_read = 0x0;
				EnchancedReadMemory(Traditional::GetPeProcessViaPID(args->PID), (ULONG64)args->Address, &args->matrix.m, args->size, &Bytes_read);
				if (Bytes_read != args->size)
				{
					//print(skCrypt("[WARNING] Read Size Mismatch"));
				}
			}
		}
		else if (args->Operation == AD_READVECTOR)
		{
			if (args->PID && (DWORD64)args->Address)
			{
				SIZE_T Bytes_read = 0x0;
				EnchancedReadMemory(Traditional::GetPeProcessViaPID(args->PID), (ULONG64)args->Address, &args->vector, args->size, &Bytes_read);
				if (Bytes_read != args->size)
				{
					//print(skCrypt("[WARNING] Read Size Mismatch"));
				}
			}
		}
		else if (args->Operation == WRITEMEMORY)
		{
			if (args->PID)
			{
				SIZE_T Bytes_wrote = 0x0;
				EnchancedWriteMemory(Traditional::GetPeProcessViaPID(args->PID), (ULONG64)args->Address, (PVOID)args->Buffer, args->size, &Bytes_wrote);
				if (Bytes_wrote != args->size)
				{
					//print(skCrypt("[ERROR] Write Size Mismatch"));
				}else
				{
					//print(skCrypt("Wrote Address: 0x%llx; Buffer: 0x%llx; size: %x"), (ULONG64)args->Address, args->Buffer, Bytes_wrote);
				}
			}
		}
		else if (args->Operation == DRAWOVERLAY)
		{
			if (!args->drawBar && !args->drawBox && !args->drawText && !args->drawFillBox)
				return NULL;
			HDC hdc = ModularOverlay::GetHDC();

			if (args->drawBox)
			{
				/*BOOL drawBox(HDC hdc, int r, int g, int b, int x, int y, int w, int h, int t)*/
				for (int i = 0; i < 64; i++)
				{
					if (!args->MBox[i].Box.T)
						continue;
					ModularOverlay::drawBox(hdc, args->MBox[i].Box.R, args->MBox[i].Box.G, args->MBox[i].Box.B
						, args->MBox[i].Box.X, args->MBox[i].Box.Y, args->MBox[i].Box.W, args->MBox[i].Box.H
						, args->MBox[i].Box.T);
				}
			}
			if (args->drawBar)
			{
				/*BOOL drawBar(HDC hdc, BoxInfo BoxInfo, DWORD MaxHealth, DWORD CurrentHealth, int R, int G, int B)*/
				for (int i = 0; i < 64; i++)
				{
					if (!args->MBox[i].HealthBar.T)
						continue;
					BoxInfo boundingBoxInfo = { args->MBox[i].HealthBar.R, args->MBox[i].HealthBar.G, args->MBox[i].HealthBar.B,
					args->MBox[i].HealthBar.X, args->MBox[i].HealthBar.Y, args->MBox[i].HealthBar.W,
					args->MBox[i].HealthBar.H, args->MBox[i].HealthBar.T };
					ModularOverlay::drawBar(hdc, boundingBoxInfo, args->MBox[i].HealthBar.MaxiumHealth,
						args->MBox[i].HealthBar.CurrentHealth, args->MBox[i].HealthBar.BarR, args->MBox[i].HealthBar.BarG, args->MBox[i].HealthBar.BarB);
				}
			}
			if (args->drawText)
			{
				/*BOOL drawText(HDC hdc, LPWSTR string, int x, int y, int Size)*/
				for (int i = 0; i < 64; i++)
				{
					if (!args->MBox[i].stringInfo.size)
						continue;
					ModularOverlay::drawText(hdc, args->MBox[i].stringInfo.string, args->MBox[i].stringInfo.X, args->MBox[i].stringInfo.Y
					, args->MBox[i].stringInfo.size);	
				}
			}
			if (args->drawFillBox)
			{
				for (int i = 0; i < 64; i++)
				{
					if (!args->MBox[i].FillBoxInfo.T)
						continue;
					ModularOverlay::drawFillBox(hdc, args->MBox[i].FillBoxInfo.R, args->MBox[i].FillBoxInfo.G, args->MBox[i].FillBoxInfo.B,
						args->MBox[i].FillBoxInfo.X, args->MBox[i].FillBoxInfo.Y, args->MBox[i].FillBoxInfo.W, args->MBox[i].FillBoxInfo.H,
						args->MBox[i].FillBoxInfo.T);
				}
			}
			ModularOverlay::ReleaseHDC(hdc);
		}
		else if (args->Operation == PATTERNSCAN)
		{
			PEPROCESS proc = Traditional::GetPeProcessViaPID(args->PID);
			if (proc)
			{

			}

		}
		else if (args->Operation == MOVECURSOR)
		{
			PNF_MOUSE_REQUEST mouse_request = (PNF_MOUSE_REQUEST)args->Buffer;
			Traditional::mouse_event(mouse_obj, mouse_request->x, mouse_request->y, mouse_request->button_flags);
		}
		return NULL;
	}
	return ApiSetGetPenArbitrationType(a1, a2, a3, a4);
}

NTSTATUS DriverEntry(PMDL mdl, PUNICODE_STRING RegistryPath)
{
	UNREFERENCED_PARAMETER(RegistryPath);
	utilities::printEx(
		skCrypt("Bobby DPH [PHYSICAL EDITION] Version 5.2 October 2022"));
	ULONG pages_cleared = 0x0;
	/*
	if (!Traditional::NullFPN(mdl))
	{
		UNREFERENCED_PARAMETER(mdl); return STATUS_UNSUCCESSFUL;
	}
	*/
	UNREFERENCED_PARAMETER(mdl);

	PVOID win32kBase = process::get_sys_module(skCrypt("\\SystemRoot\\System32\\win32kbase.sys"));

	if (!win32kBase)
	{
		print(skCrypt("win32kBaseEmpty"));
		return STATUS_SUCCESS;
	}
	else
	{
		//print(skCrypt("win32kBase: 0x%x \n"), win32kBase);
	}

	PVOID ZePointer = process::FindPattern((PCHAR)win32kBase,
		skCrypt("[SIGNATURE-REMOVED-FOR-GITHUB]"),
		skCrypt("[SIGNATURE-REMOVED-FOR-GITHUB]")
	);

	PVOID SecondPointer = process::FindPattern((PCHAR)win32kBase,
		skCrypt("[SIGNATURE-REMOVED-FOR-GITHUB]"),
		skCrypt("[SIGNATURE-REMOVED-FOR-GITHUB]")
	);

	auto ApiSetGetPenArbitrationTypeAddress = process::FindPattern((PCHAR)win32kBase,
		skCrypt("[SIGNATURE-REMOVED-FOR-GITHUB]"),
		skCrypt("[SIGNATURE-REMOVED-FOR-GITHUB]")
	);

	ApiSetGetPenArbitrationTypeAddress = RVA(ApiSetGetPenArbitrationTypeAddress, 5);

	//print(skCrypt("ApiSetGetPenArbitrationTypeAddress: 0x%p"), ApiSetGetPenArbitrationTypeAddress);
	if (!ZePointer)
	{
		print(skCrypt("DataPointerNotFound"));
		return STATUS_SUCCESS;
	}
	if (!SecondPointer)
	{
		print(skCrypt("SecondDataPointerNotFound"));
		return STATUS_SUCCESS;
	}
	UINT64 DREFZePointer = (UINT64)(ZePointer);
	DREFZePointer = (UINT64)DREFZePointer + *(PINT)((PBYTE)DREFZePointer + 3) + 7;
	//print(skCrypt("pointer after dereference: 0x%llx"), DREFZePointer);

	UINT64 DEREFSecondPointer = (UINT64)(SecondPointer);
	DEREFSecondPointer = (UINT64)DEREFSecondPointer + *(PINT)((PBYTE)DEREFSecondPointer + 3) + 7;
	//print(skCrypt("second pointer after dereference: 0x%llx"), DEREFSecondPointer);

	*(PVOID*)&ApiSetGetPenArbitrationType = InterlockedExchangePointer((PVOID*)DEREFSecondPointer, (PVOID)HackedPointer);
	/* Written Destination pointer [Hacked Pointer] to ApiSetGetPenArbitrationTypeAddress Data pointer from [RIMProcessAnyPointerDeviceInput]*/

	*(PVOID*)&OriginalPointer = InterlockedExchangePointer((PVOID*)DREFZePointer, ApiSetGetPenArbitrationTypeAddress);
	
	/* ApiSetGetPenArbitrationTypeAddress Written to ApiSetEditionCreateDesktopEntryPoint  */

	print(skCrypt("[SUCCESS] pointers overwritten"));


	if (TRUE) /* OVERLAY ENABLED */
	{
		if (initOverlay())
		{
			//print(skCrypt("[SUCCESS] Overlay Init Successful"));
		}
		else
		{
			print(skCrypt("[ERROR] Overlay Init Failed"));
		}
	}
	print(skCrypt("[SUCCESS] Driver Initiated"));
	return STATUS_SUCCESS;
}
