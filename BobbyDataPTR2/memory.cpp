#include "memory.h"


typedef struct _LDR_DATA_TABLE_ENTRY
{
    /* 0x0000 */ struct _LIST_ENTRY InLoadOrderLinks;
    /* 0x0010 */ struct _LIST_ENTRY InMemoryOrderLinks;
    /* 0x0020 */ struct _LIST_ENTRY InInitializationOrderLinks;
    /* 0x0030 */ void* DllBase;
    /* 0x0038 */ void* EntryPoint;
    /* 0x0040 */ unsigned long SizeOfImage;
    /* 0x0048 */ struct _UNICODE_STRING FullDllName;
    /* 0x0058 */ struct _UNICODE_STRING BaseDllName;
    union
    {
        /* 0x0068 */ unsigned char FlagGroup[4];
        /* 0x0068 */ unsigned long Flags;
        struct /* bitfield */
        {
            /* 0x0068 */ unsigned long PackagedBinary : 1; /* bit position: 0 */
            /* 0x0068 */ unsigned long MarkedForRemoval : 1; /* bit position: 1 */
            /* 0x0068 */ unsigned long ImageDll : 1; /* bit position: 2 */
            /* 0x0068 */ unsigned long LoadNotificationsSent : 1; /* bit position: 3 */
            /* 0x0068 */ unsigned long TelemetryEntryProcessed : 1; /* bit position: 4 */
            /* 0x0068 */ unsigned long ProcessStaticImport : 1; /* bit position: 5 */
            /* 0x0068 */ unsigned long InLegacyLists : 1; /* bit position: 6 */
            /* 0x0068 */ unsigned long InIndexes : 1; /* bit position: 7 */
            /* 0x0068 */ unsigned long ShimDll : 1; /* bit position: 8 */
            /* 0x0068 */ unsigned long InExceptionTable : 1; /* bit position: 9 */
            /* 0x0068 */ unsigned long ReservedFlags1 : 2; /* bit position: 10 */
            /* 0x0068 */ unsigned long LoadInProgress : 1; /* bit position: 12 */
            /* 0x0068 */ unsigned long LoadConfigProcessed : 1; /* bit position: 13 */
            /* 0x0068 */ unsigned long EntryProcessed : 1; /* bit position: 14 */
            /* 0x0068 */ unsigned long ProtectDelayLoad : 1; /* bit position: 15 */
            /* 0x0068 */ unsigned long ReservedFlags3 : 2; /* bit position: 16 */
            /* 0x0068 */ unsigned long DontCallForThreads : 1; /* bit position: 18 */
            /* 0x0068 */ unsigned long ProcessAttachCalled : 1; /* bit position: 19 */
            /* 0x0068 */ unsigned long ProcessAttachFailed : 1; /* bit position: 20 */
            /* 0x0068 */ unsigned long CorDeferredValidate : 1; /* bit position: 21 */
            /* 0x0068 */ unsigned long CorImage : 1; /* bit position: 22 */
            /* 0x0068 */ unsigned long DontRelocate : 1; /* bit position: 23 */
            /* 0x0068 */ unsigned long CorILOnly : 1; /* bit position: 24 */
            /* 0x0068 */ unsigned long ChpeImage : 1; /* bit position: 25 */
            /* 0x0068 */ unsigned long ReservedFlags5 : 2; /* bit position: 26 */
            /* 0x0068 */ unsigned long Redirected : 1; /* bit position: 28 */
            /* 0x0068 */ unsigned long ReservedFlags6 : 2; /* bit position: 29 */
            /* 0x0068 */ unsigned long CompatDatabaseProcessed : 1; /* bit position: 31 */
        }; /* bitfield */
    }; /* size: 0x0004 */
    /* 0x006c */ unsigned short ObsoleteLoadCount;
    /* 0x006e */ unsigned short TlsIndex;
    /* 0x0070 */ struct _LIST_ENTRY HashLinks;
    /* 0x0080 */ unsigned long TimeDateStamp;
    /* 0x0088 */ struct _ACTIVATION_CONTEXT* EntryPointActivationContext;
    /* 0x0090 */ void* Lock;
    /* 0x0098 */ struct _LDR_DDAG_NODE* DdagNode;
    /* 0x00a0 */ struct _LIST_ENTRY NodeModuleLink;
    /* 0x00b0 */ struct _LDRP_LOAD_CONTEXT* LoadContext;
    /* 0x00b8 */ void* ParentDllBase;
    /* 0x00c0 */ void* SwitchBackContext;
    /* 0x00c8 */ struct _RTL_BALANCED_NODE BaseAddressIndexNode;
    /* 0x00e0 */ struct _RTL_BALANCED_NODE MappingInfoIndexNode;
    /* 0x00f8 */ unsigned __int64 OriginalBase;
    /* 0x0100 */ union _LARGE_INTEGER LoadTime;
    /* 0x0108 */ unsigned long BaseNameHashValue;
    /* 0x010c */ enum _LDR_DLL_LOAD_REASON LoadReason;
    /* 0x0110 */ unsigned long ImplicitPathOptions;
    /* 0x0114 */ unsigned long ReferenceCount;
    /* 0x0118 */ unsigned long DependentLoadFlags;
    /* 0x011c */ unsigned char SigningLevel;
    /* 0x011d */ char __PADDING__[3];
} LDR_DATA_TABLE_ENTRY, * PLDR_DATA_TABLE_ENTRY; /* size: 0x0120 */

namespace memory
{
	typedef PVOID(*fnPsGetProcessWow64Process)(PEPROCESS Process);
	uintptr_t get_module_base_with_attatch(WCHAR* ModuleName, PEPROCESS m_Process, ULONG* SizeofImage) {
		fnPsGetProcessWow64Process PsGetProcessWow64Process;
		UNICODE_STRING routineName;
		RtlInitUnicodeString(&routineName, skCrypt(L"PsGetProcessWow64Process"));
		PsGetProcessWow64Process =
			(fnPsGetProcessWow64Process)MmGetSystemRoutineAddress(&routineName);

		KAPC_STATE AttachState;
		KeStackAttachProcess(m_Process, &AttachState);

		PPEB32 pPeb32 = (PPEB32)PsGetProcessWow64Process(m_Process);
		if (!pPeb32 || !pPeb32->Ldr) {
			KeUnstackDetachProcess(&AttachState);
			return 0;
		}

		for (PLIST_ENTRY32 pListEntry = (PLIST_ENTRY32)((PPEB_LDR_DATA32)pPeb32->Ldr)
			->InLoadOrderModuleList.Flink;
			pListEntry != &((PPEB_LDR_DATA32)pPeb32->Ldr)->InLoadOrderModuleList;
			pListEntry = (PLIST_ENTRY32)pListEntry->Flink) {
			PLDR_DATA_TABLE_ENTRY32 pEntry =
				CONTAINING_RECORD(pListEntry, LDR_DATA_TABLE_ENTRY32, InLoadOrderLinks);

			if (wcscmp((PWCH)pEntry->BaseDllName.Buffer, ModuleName) == 0) {
				uintptr_t ModuleAddress = pEntry->DllBase;
				*SizeofImage = pEntry->SizeOfImage;
				KeUnstackDetachProcess(&AttachState);
				return ModuleAddress;
			}
		}

		KeUnstackDetachProcess(&AttachState);
		return 0;
	}

    struct _PEB_LDR_DATA_21H1
    {
        ULONG Length;                                                           //0x0
        UCHAR Initialized;                                                      //0x4
        VOID* SsHandle;                                                         //0x8
        struct _LIST_ENTRY InLoadOrderModuleList;                               //0x10
        struct _LIST_ENTRY InMemoryOrderModuleList;                             //0x20
        struct _LIST_ENTRY InInitializationOrderModuleList;                     //0x30
        VOID* EntryInProgress;                                                  //0x40
        UCHAR ShutdownInProgress;                                               //0x48
        VOID* ShutdownThreadId;                                                 //0x50
    };
    struct _PEB_21H1
    {
        UCHAR InheritedAddressSpace;                                            //0x0
        UCHAR ReadImageFileExecOptions;                                         //0x1
        UCHAR BeingDebugged;                                                    //0x2
        union
        {
            UCHAR BitField;                                                     //0x3
            struct
            {
                UCHAR ImageUsesLargePages : 1;                                    //0x3
                UCHAR IsProtectedProcess : 1;                                     //0x3
                UCHAR IsImageDynamicallyRelocated : 1;                            //0x3
                UCHAR SkipPatchingUser32Forwarders : 1;                           //0x3
                UCHAR IsPackagedProcess : 1;                                      //0x3
                UCHAR IsAppContainer : 1;                                         //0x3
                UCHAR IsProtectedProcessLight : 1;                                //0x3
                UCHAR IsLongPathAwareProcess : 1;                                 //0x3
            };
        };
        UCHAR Padding0[4];                                                      //0x4
        VOID* Mutant;                                                           //0x8
        VOID* ImageBaseAddress;                                                 //0x10
        struct _PEB_LDR_DATA_21H1* Ldr;                                              //0x18
        struct _RTL_USER_PROCESS_PARAMETERS* ProcessParameters;                 //0x20
        VOID* SubSystemData;                                                    //0x28
        VOID* ProcessHeap;                                                      //0x30
        struct _RTL_CRITICAL_SECTION* FastPebLock;                              //0x38
        union _SLIST_HEADER* volatile AtlThunkSListPtr;                         //0x40
        VOID* IFEOKey;                                                          //0x48
        union
        {
            ULONG CrossProcessFlags;                                            //0x50
            struct
            {
                ULONG ProcessInJob : 1;                                           //0x50
                ULONG ProcessInitializing : 1;                                    //0x50
                ULONG ProcessUsingVEH : 1;                                        //0x50
                ULONG ProcessUsingVCH : 1;                                        //0x50
                ULONG ProcessUsingFTH : 1;                                        //0x50
                ULONG ProcessPreviouslyThrottled : 1;                             //0x50
                ULONG ProcessCurrentlyThrottled : 1;                              //0x50
                ULONG ProcessImagesHotPatched : 1;                                //0x50
                ULONG ReservedBits0 : 24;                                         //0x50
            };
        };
        UCHAR Padding1[4];                                                      //0x54
        union
        {
            VOID* KernelCallbackTable;                                          //0x58
            VOID* UserSharedInfoPtr;                                            //0x58
        };
        ULONG SystemReserved;                                                   //0x60
        ULONG AtlThunkSListPtr32;                                               //0x64
        VOID* ApiSetMap;                                                        //0x68
        ULONG TlsExpansionCounter;                                              //0x70
        UCHAR Padding2[4];                                                      //0x74
        VOID* TlsBitmap;                                                        //0x78
        ULONG TlsBitmapBits[2];                                                 //0x80
        VOID* ReadOnlySharedMemoryBase;                                         //0x88
        VOID* SharedData;                                                       //0x90
        VOID** ReadOnlyStaticServerData;                                        //0x98
        VOID* AnsiCodePageData;                                                 //0xa0
        VOID* OemCodePageData;                                                  //0xa8
        VOID* UnicodeCaseTableData;                                             //0xb0
        ULONG NumberOfProcessors;                                               //0xb8
        ULONG NtGlobalFlag;                                                     //0xbc
        union _LARGE_INTEGER CriticalSectionTimeout;                            //0xc0
        ULONGLONG HeapSegmentReserve;                                           //0xc8
        ULONGLONG HeapSegmentCommit;                                            //0xd0
        ULONGLONG HeapDeCommitTotalFreeThreshold;                               //0xd8
        ULONGLONG HeapDeCommitFreeBlockThreshold;                               //0xe0
        ULONG NumberOfHeaps;                                                    //0xe8
        ULONG MaximumNumberOfHeaps;                                             //0xec
        VOID** ProcessHeaps;                                                    //0xf0
        VOID* GdiSharedHandleTable;                                             //0xf8
        VOID* ProcessStarterHelper;                                             //0x100
        ULONG GdiDCAttributeList;                                               //0x108
        UCHAR Padding3[4];                                                      //0x10c
        struct _RTL_CRITICAL_SECTION* LoaderLock;                               //0x110
        ULONG OSMajorVersion;                                                   //0x118
        ULONG OSMinorVersion;                                                   //0x11c
        USHORT OSBuildNumber;                                                   //0x120
        USHORT OSCSDVersion;                                                    //0x122
        ULONG OSPlatformId;                                                     //0x124
        ULONG ImageSubsystem;                                                   //0x128
        ULONG ImageSubsystemMajorVersion;                                       //0x12c
        ULONG ImageSubsystemMinorVersion;                                       //0x130
        UCHAR Padding4[4];                                                      //0x134
        ULONGLONG ActiveProcessAffinityMask;                                    //0x138
        ULONG GdiHandleBuffer[60];                                              //0x140
        VOID(*PostProcessInitRoutine)();                                       //0x230
        VOID* TlsExpansionBitmap;                                               //0x238
        ULONG TlsExpansionBitmapBits[32];                                       //0x240
        ULONG SessionId;                                                        //0x2c0
        UCHAR Padding5[4];                                                      //0x2c4
        union _ULARGE_INTEGER AppCompatFlags;                                   //0x2c8
        union _ULARGE_INTEGER AppCompatFlagsUser;                               //0x2d0
        VOID* pShimData;                                                        //0x2d8
        VOID* AppCompatInfo;                                                    //0x2e0
        struct _UNICODE_STRING CSDVersion;                                      //0x2e8
        struct _ACTIVATION_CONTEXT_DATA* ActivationContextData;                 //0x2f8
        struct _ASSEMBLY_STORAGE_MAP* ProcessAssemblyStorageMap;                //0x300
        struct _ACTIVATION_CONTEXT_DATA* SystemDefaultActivationContextData;    //0x308
        struct _ASSEMBLY_STORAGE_MAP* SystemAssemblyStorageMap;                 //0x310
        ULONGLONG MinimumStackCommit;                                           //0x318
        VOID* SparePointers[4];                                                 //0x320
        ULONG SpareUlongs[5];                                                   //0x340
        VOID* WerRegistrationData;                                              //0x358
        VOID* WerShipAssertPtr;                                                 //0x360
        VOID* pUnused;                                                          //0x368
        VOID* pImageHeaderHash;                                                 //0x370
        union
        {
            ULONG TracingFlags;                                                 //0x378
            struct
            {
                ULONG HeapTracingEnabled : 1;                                     //0x378
                ULONG CritSecTracingEnabled : 1;                                  //0x378
                ULONG LibLoaderTracingEnabled : 1;                                //0x378
                ULONG SpareTracingBits : 29;                                      //0x378
            };
        };
        UCHAR Padding6[4];                                                      //0x37c
        ULONGLONG CsrServerReadOnlySharedMemoryBase;                            //0x380
        ULONGLONG TppWorkerpListLock;                                           //0x388
        struct _LIST_ENTRY TppWorkerpList;                                      //0x390
        VOID* WaitOnAddressHashTable[128];                                      //0x3a0
        VOID* TelemetryCoverageHeader;                                          //0x7a0
        ULONG CloudFileFlags;                                                   //0x7a8
        ULONG CloudFileDiagFlags;                                               //0x7ac
        CHAR PlaceholderCompatibilityMode;                                      //0x7b0
        CHAR PlaceholderCompatibilityModeReserved[7];                           //0x7b1
        struct _LEAP_SECOND_DATA* LeapSecondData;                               //0x7b8
        union
        {
            ULONG LeapSecondFlags;                                              //0x7c0
            struct
            {
                ULONG SixtySecondEnabled : 1;                                     //0x7c0
                ULONG Reserved : 31;                                              //0x7c0
            };
        };
        ULONG NtGlobalFlag2;                                                    //0x7c4
    };

    typedef struct Warning
    {
        UINT Failed_To_Read_Dll_Name, Dll_Base_Empty_or_Size_of_Image_Null, Dll_Name_Empty,
            Failed_To_Read_LdrCurrentNode_ListEntry, String_Compare_Fail;
    };

    ULONG64 GetModuleBasex64PEB(PEPROCESS proc, const WCHAR* ModuleName, ULONG* ModuleSize)
    {
        Warning warning; RtlSecureZeroMemory(&warning, sizeof(Warning));
        _PEB_21H1* pPeb = (_PEB_21H1*)PsGetProcessPeb(proc);
        if (!pPeb)
        {
            print(skCrypt("Failed To Get Process Peb"));
            return false;
        }
        print(skCrypt("Peb from Ps Address: %llx"), (ULONG64)pPeb);
        SIZE_T size_read;
        _PEB_21H1 procPeb = { 0 };
        NTSTATUS status = EnchancedReadMemory(proc, (ULONG64)pPeb, &procPeb, sizeof(_PEB_21H1), &size_read);
        if (!NT_SUCCESS(status))
        {
            print(skCrypt("Failed To Read PEB Physical Memory"));
            return false;
        }
        _PEB_LDR_DATA_21H1 pebLdrData = { 0 };
        status = EnchancedReadMemory(proc, (ULONG64)procPeb.Ldr, &pebLdrData, sizeof(_PEB_LDR_DATA_21H1), &size_read);
        if (!NT_SUCCESS(status))
        {
            print(skCrypt("Failed To Read PEB Ldr Physical Memory"));
            return false;
        }

        LIST_ENTRY* pLdrListHead = (LIST_ENTRY*)pebLdrData.InLoadOrderModuleList.Flink;
        LIST_ENTRY* pLdrCurrentNode = pebLdrData.InLoadOrderModuleList.Flink;

        do
        {
            LDR_DATA_TABLE_ENTRY lstEntry = { 0 };
            status = EnchancedReadMemory(proc, (ULONG64)pLdrCurrentNode, &lstEntry, sizeof(LDR_DATA_TABLE_ENTRY), &size_read);
            if (!NT_SUCCESS(status))
            {
                warning.Failed_To_Read_LdrCurrentNode_ListEntry++;
                continue;
            }
            pLdrCurrentNode = lstEntry.InLoadOrderLinks.Flink;

            if (lstEntry.BaseDllName.Length > 0)
            {
                WCHAR strBaseDllName[MAX_PATH] = { 0 };
                status = EnchancedReadMemory(proc, (ULONG64)lstEntry.BaseDllName.Buffer, &strBaseDllName, lstEntry.BaseDllName.Length, &size_read);
                if (!NT_SUCCESS(status))
                {
                    warning.Failed_To_Read_Dll_Name++;
                    continue;
                }
                if (lstEntry.DllBase != nullptr && lstEntry.SizeOfImage != 0)
                {
                    if (_wcsicmp(strBaseDllName, ModuleName) == 0)
                    {

                        *ModuleSize = (ULONG)lstEntry.SizeOfImage;
                        return (ULONG64)lstEntry.DllBase;
                    }
                    else
                    {
                        warning.String_Compare_Fail++;
                    }
                    //utilities::print("Base Address: 0x%llx, size: 0x%lx", (ULONG64)lstEntry.DllBase, (ULONG)lstEntry.SizeOfImage);
                    //return (ULONG64)lstEntry.DllBase;
                }
                else
                {
                    warning.Dll_Base_Empty_or_Size_of_Image_Null++;
                }
            }
            else
                warning.Dll_Name_Empty++;
        } while (pLdrListHead != pLdrCurrentNode);
        return false;
    }
}

namespace VirtualMethods
{	
	ULONG GetProcessIDViaZwQuerySystemInformation(UNICODE_STRING targetName)
	{
		UNICODE_STRING processName;
		ULONG bufferSize = 0;
		NTSTATUS ntstatus;
		if (ZwQuerySystemInformation(SystemProcessInformation, NULL, 0, &bufferSize) == STATUS_INFO_LENGTH_MISMATCH)
		{
			if (bufferSize)
			{
				PVOID memory = ExAllocatePoolWithTag(PagedPool, bufferSize, (ULONG)(skCrypt("dadh").decrypt()));
				if (memory)
				{
					ntstatus = ZwQuerySystemInformation(SystemProcessInformation, memory, bufferSize, &bufferSize);
					if (NT_SUCCESS(ntstatus))
					{
						PSYSTEM_PROCESSES processEntry = (PSYSTEM_PROCESSES)memory;
						do {
							processName = processEntry->ProcessName;
							if (RtlEqualUnicodeString(&targetName, &processName, false))
							{
								unsigned __int64 processID = processEntry->ProcessId;
								ULONG pid = processID;
								if (pid != NULL)
								{
									ExFreePoolWithTag(memory, (ULONG)(skCrypt("dadh").decrypt()));
									return pid;
								}
							}
							processEntry = (PSYSTEM_PROCESSES)((BYTE*)processEntry + processEntry->NextEntryDelta);
						} while (processEntry->NextEntryDelta);
					}
					ExFreePoolWithTag(memory, (ULONG)(skCrypt("dadh").decrypt()));
					return 0;
				}
			}
		}
	}
}




namespace Traditional
{
	PEPROCESS GetPeProcessViaPID(DWORD PID)
	{
		if (PID != NULL)
		{
			PEPROCESS process = NULL;
			if (NT_SUCCESS(PsLookupProcessByProcessId((HANDLE)PID, &process)))
			{
				return process;
			}
			else { process = NULL; print(skCrypt("[ERROR] PEPROCESS UNABLE TO BE OBTAINED")); return process; }
		}
		else { print(skCrypt("[ERROR] PID NULL")); return NULL; }
	}

	NTSTATUS GetPEProcessViaName_VersionDependent(CHAR* process_name, PEPROCESS* process)
	{
		PEPROCESS sys_process = PsInitialSystemProcess;
		PEPROCESS cur_entry = sys_process;
		CHAR image_name[15];

		do
		{
			RtlCopyMemory((PVOID)(&image_name), (PVOID)((uintptr_t)cur_entry + 0x05a8) /*EPROCESS->ImageFileName*/, sizeof(image_name));

			if (strstr(image_name, process_name))
			{
				ULONG active_threads;
				RtlCopyMemory((PVOID)&active_threads, (PVOID)((uintptr_t)cur_entry + 0x05f0) /*EPROCESS->ActiveThreads*/, sizeof(active_threads));
				if (active_threads)
				{
					*process = cur_entry;
					return STATUS_SUCCESS;
				}
			}

			PLIST_ENTRY list = (PLIST_ENTRY)((uintptr_t)(cur_entry)+0x0448) /*EPROCESS->ActiveProcessLinks*/;
			cur_entry = (PEPROCESS)((uintptr_t)list->Flink - 0x0448);

		} while (cur_entry != sys_process);

		return STATUS_NOT_FOUND;
	}

	NTSTATUS GetSecondPEProcessViaName_VersionDependent(CHAR* process_name, PEPROCESS* process)
	{
		BOOL FirstMatch = false;
		PEPROCESS sys_process = PsInitialSystemProcess;
		PEPROCESS cur_entry = sys_process;
		CHAR image_name[15];

		do
		{
			RtlCopyMemory((PVOID)(&image_name), (PVOID)((uintptr_t)cur_entry + 0x05a8) /*EPROCESS->ImageFileName*/, sizeof(image_name));

			if (strstr(image_name, process_name))
			{
				if (FirstMatch == false)
				{
					FirstMatch = true;
				}
				else
				{
					ULONG active_threads;
					RtlCopyMemory((PVOID)&active_threads, (PVOID)((uintptr_t)cur_entry + 0x05f0) /*EPROCESS->ActiveThreads*/, sizeof(active_threads));
					if (active_threads)
					{
						*process = cur_entry;
						return STATUS_SUCCESS;
					}
				}
			}

			PLIST_ENTRY list = (PLIST_ENTRY)((uintptr_t)(cur_entry)+0x0448) /*EPROCESS->ActiveProcessLinks*/;
			cur_entry = (PEPROCESS)((uintptr_t)list->Flink - 0x0448);

		} while (cur_entry != sys_process);

		return STATUS_NOT_FOUND;
	}

	BOOL NullFPN(PMDL mdl)
	{
		PPFN_NUMBER mdl_pages = MmGetMdlPfnArray(mdl);
		if (!mdl_pages) { return false; }

		ULONG mdl_page_count = ADDRESS_AND_SIZE_TO_SPAN_PAGES(MmGetMdlVirtualAddress(mdl), MmGetMdlByteCount(mdl));
		ULONG null_pfn = 0x0;
		MM_COPY_ADDRESS source_address = { 0 };
		source_address.VirtualAddress = &null_pfn;

		for (ULONG i = 0; i < mdl_page_count; i++)
		{
			size_t bytes = 0;
			MmCopyMemory(&mdl_pages[i], source_address, sizeof(ULONG), MM_COPY_MEMORY_VIRTUAL, &bytes);
		}
		print(skCrypt("Cleared %d pages of mdl"), mdl_page_count);
		return true;
	}

	UINT32 APCUnQueuable(PETHREAD commsthread)
	{
        UINT32 OriginalFlag;
		UINT32* miscFlags = (UINT32*)((char*)commsthread + 0x74);
		print(skCrypt("Previous misc flag: 0x%x \n"), *miscFlags);
        OriginalFlag = *miscFlags;


		*miscFlags &= 0xffffbfff;
		miscFlags = (UINT32*)((char*)commsthread + 0x74);
		print(skCrypt("Current misc flag: 0x%x \n"), *miscFlags);
		return OriginalFlag;
	}

	bool RestoreAPCQueuable(PETHREAD commsthread, UINT32 OriginalValue)
	{
		UINT32* miscFlags = (UINT32*)((char*)commsthread + 0x74);
		*miscFlags = OriginalValue;
		miscFlags = (UINT32*)((char*)commsthread + 0x74);
		if (*miscFlags == OriginalValue)
		{
			return true;
		}
		else return false;
	}

    /*
    uintptr_t PatternScan(PEPROCESS Proc, DWORD ModuleBase, DWORD ModuleSize, UINT* arr, const char* pattern, int offset, int extra) {
        uintptr_t scan = 0x0;
        const char* pat = pattern;
        uintptr_t firstMatch = 0;
        for (uintptr_t pCur = (uintptr_t)arr; pCur < (uintptr_t)arr + ModuleSize; ++pCur) {
            if (!*pat) { scan = firstMatch; break; }
            if (*(UINT8*)pat == '\?' || *(UINT8*)pCur == ((((pat[0] & (~0x20)) >= 'A' && (pat[0] & (~0x20)) <= 'F') ? ((pat[0] & (~0x20)) - 'A' + 0xa) : ((pat[0] >= '0' && pat[0] <= '9') ? pat[0] - '0' : 0)) << 4 | (((pat[1] & (~0x20)) >= 'A' && (pat[1] & (~0x20)) <= 'F') ? ((pat[1] & (~0x20)) - 'A' + 0xa) : ((pat[1] >= '0' && pat[1] <= '9') ? pat[1] - '0' : 0)))) {
                if (!firstMatch) firstMatch = pCur;
                if (!pat[2]) { scan = firstMatch; break; }
                if (*(WORD*)pat == 16191 || *(UINT8*)pat != '\?') pat += 3;
                else pat += 2;
            }
            else { pat = pattern; firstMatch = 0; }
        }
        if (!scan) return 0x0;
        UINT32 read;
        (DWORD)EnchancedReadMemory(Proc, (ULONG64)(scan - (uintptr_t)arr + (uintptr_t)ModuleBase + offset), &read, sizeof(read), NULL);
        return read + extra;
    }

    DWORD64 PatternScan(PEPROCESS proc, DWORD64 ModuleBase, DWORD ModuleSize, const char* Pattern)
    {

        auto bytes = new UINT8[ModuleSize];
        SIZE_T Bytes_Read;

        EnchancedReadMemory(proc, ModuleBase, bytes, ModuleSize, &Bytes_Read);
        DWORD Result = PatternScan(proc, ModuleBase, ModuleSize, Pattern);
        delete[] bytes;
        return Result;
    }
    */
    NTSTATUS init_mouse(PMOUSE_OBJECT mouse_obj)
    {
        UNICODE_STRING class_string;
        RtlInitUnicodeString(&class_string, L"\\Driver\\MouClass");

        PDRIVER_OBJECT class_driver_object = NULL;
        NTSTATUS status = ObReferenceObjectByName(&class_string, OBJ_CASE_INSENSITIVE, NULL, 0, *IoDriverObjectType, KernelMode, NULL, (PVOID*)&class_driver_object);
        if (!NT_SUCCESS(status)) { return status; }

        UNICODE_STRING hid_string;
        RtlInitUnicodeString(&hid_string, L"\\Driver\\MouHID");

        PDRIVER_OBJECT hid_driver_object = NULL;
        status = ObReferenceObjectByName(&hid_string, OBJ_CASE_INSENSITIVE, NULL, 0, *IoDriverObjectType, KernelMode, NULL, (PVOID*)&hid_driver_object);
        if (!NT_SUCCESS(status))
        {
            if (class_driver_object) { ObDereferenceObject(class_driver_object); }
            return status;
        }

        PVOID class_driver_base = NULL;

        PDEVICE_OBJECT hid_device_object = hid_driver_object->DeviceObject;
        while (hid_device_object && !mouse_obj->service_callback)
        {
            PDEVICE_OBJECT class_device_object = class_driver_object->DeviceObject;
            while (class_device_object && !mouse_obj->service_callback)
            {
                if (!class_device_object->NextDevice && !mouse_obj->mouse_device)
                {
                    mouse_obj->mouse_device = class_device_object;
                }

                PULONG_PTR device_extension = (PULONG_PTR)hid_device_object->DeviceExtension;
                ULONG_PTR device_ext_size = ((ULONG_PTR)hid_device_object->DeviceObjectExtension - (ULONG_PTR)hid_device_object->DeviceExtension) / 4;
                class_driver_base = class_driver_object->DriverStart;
                for (ULONG_PTR i = 0; i < device_ext_size; i++)
                {
                    if (device_extension[i] == (ULONG_PTR)class_device_object && device_extension[i + 1] > (ULONG_PTR)class_driver_object)
                    {
                        mouse_obj->service_callback = (MouseClassServiceCallback)(device_extension[i + 1]);
                        break;
                    }
                }
                class_device_object = class_device_object->NextDevice;
            }
            hid_device_object = hid_device_object->AttachedDevice;
        }

        if (!mouse_obj->mouse_device)
        {
            PDEVICE_OBJECT target_device_object = class_driver_object->DeviceObject;
            while (target_device_object)
            {
                if (!target_device_object->NextDevice)
                {
                    mouse_obj->mouse_device = target_device_object;
                    break;
                }
                target_device_object = target_device_object->NextDevice;
            }
        }

        ObDereferenceObject(class_driver_object);
        ObDereferenceObject(hid_driver_object);
        
        print(skCrypt("Mouse Init Finished"));

        return STATUS_SUCCESS;
    }
    void mouse_event(MOUSE_OBJECT mouse_obj, long x, long y, unsigned short button_flags)
    {
        ULONG input_data;
        KIRQL irql;
        MOUSE_INPUT_DATA mid = { 0 };

        mid.LastX = x;
        mid.LastY = y;
        mid.ButtonFlags = button_flags;

        KeRaiseIrql(DISPATCH_LEVEL, &irql);
        mouse_obj.service_callback(mouse_obj.mouse_device, &mid, (PMOUSE_INPUT_DATA)&mid + 1, &input_data);
        KeLowerIrql(irql);
    }
    
}
