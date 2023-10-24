#include "process.h"

namespace process
{
	void* get_sys_module(const char* module_name)
	{
		void* module_base = 0;
		ULONG bytes = 0;
		NTSTATUS status = ZwQuerySystemInformation(SystemModuleInformation, NULL, bytes, &bytes);

		if (!bytes) { return NULL; }

		PRTL_PROCESS_MODULES modules = (PRTL_PROCESS_MODULES)ExAllocatePoolWithTag(NonPagedPool, bytes, 'bobi');

		status = ZwQuerySystemInformation(SystemModuleInformation, modules, bytes, &bytes);

		if (!NT_SUCCESS(status)) { return NULL; }

		PRTL_PROCESS_MODULE_INFORMATION module = modules->Modules;

		for (ULONG i = 0; i < modules->NumberOfModules; i++)
		{
			if (strcmp((char*)module[i].FullPathName, module_name) == 0)
			{
				module_base = module[i].ImageBase;
				break;
			}
		}

		if (modules) { ExFreePoolWithTag(modules, 'bobi'); }

		if (module_base <= 0) { return NULL; }

		return module_base;
	}

	PIMAGE_NT_HEADERS getHeader(PVOID module) {
		return (PIMAGE_NT_HEADERS)((PBYTE)module + PIMAGE_DOS_HEADER(module)->e_lfanew);
	}
	PBYTE FindPattern(PVOID module, DWORD size, LPCSTR pattern, LPCSTR mask) 
	{

		auto checkMask = [](PBYTE buffer, LPCSTR pattern, LPCSTR mask) -> BOOL
		{
			for (auto x = buffer; *mask; pattern++, mask++, x++) {
				auto addr = *(BYTE*)(pattern);
				if (addr != *x && *mask != '?')
					return FALSE;
			}

			return TRUE;
		};

		for (auto x = 0; x < size - strlen(mask); x++) {

			auto addr = (PBYTE)module + x;
			if (checkMask(addr, pattern, mask))
				return addr;
		}

		return NULL;
	}
	PBYTE FindPattern(PVOID base, LPCSTR pattern, LPCSTR mask) 
	{

		auto header = getHeader(base);
		auto section = IMAGE_FIRST_SECTION(header);

		for (auto x = 0; x < header->FileHeader.NumberOfSections; x++, section++) {

			/*
			* Avoids non paged memory,
			* As well as greatly speeds up the process of scanning 30+ sections.
			*/
			if (!memcmp(section->Name, ".text", 5) || !memcmp(section->Name, "PAGE", 4)) {
				auto addr = FindPattern((PBYTE)base + section->VirtualAddress, section->Misc.VirtualSize, pattern, mask);
				if (addr) 
				{
					return addr;
				}
			}
		}

		return NULL;
	}

	bool NullPageFrameNumbersFromMdl(PMDL mdl)
	{
		PPFN_NUMBER mdl_pages = MmGetMdlPfnArray(mdl);
		if (!mdl_pages) { return false; }

		ULONG mdl_page_count = ADDRESS_AND_SIZE_TO_SPAN_PAGES(MmGetMdlVirtualAddress(mdl), MmGetMdlByteCount(mdl));
		
		ULONG null_pfn = 0x0;
		MM_COPY_ADDRESS source_address = { 0 };
		source_address.VirtualAddress = &null_pfn;
		print(skCrypt("Cleared Pages of FPN: %d"), mdl_page_count);
		for (ULONG i = 0; i < mdl_page_count; i++)
		{
			size_t bytes = 0;
			MmCopyMemory(&mdl_pages[i], source_address, sizeof(ULONG), MM_COPY_MEMORY_VIRTUAL, &bytes);
		}
		return true;
	}


}
