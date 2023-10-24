#include "physicalMemory.h"

namespace cr3
{
	ULONG_PTR GetProcessCr3(PEPROCESS pProcess)
	{
		PUCHAR process = (PUCHAR)pProcess;
		ULONG_PTR process_dirbase = *(PULONG_PTR)(process + 0x28); //dirbase x64 0x28, 32bit is 0x18
		if (process_dirbase == 0)
		{
			DWORD UserDirOffset = 0x0388; // Windows 21H1
			ULONG_PTR process_userdirbase = *(PULONG_PTR)(process + UserDirOffset);
			return process_userdirbase;
		}
		return process_dirbase;
	}
	ULONG_PTR GetKernelDirBase()
	{
		PUCHAR process = (PUCHAR)PsGetCurrentProcess();
		ULONG_PTR cr3 = *(PULONG_PTR)(process + 0x28); //dirbase x64 is 0x28, 32bit is 0x18
		return cr3;
	}
}

namespace Utilities
{
#define PAGE_OFFSET_SIZE 12
	static const uint64_t PMASK = (~0xfull << 8) & 0xfffffffffull;
	uint64_t TranslateLinearAddress(uint64_t directoryTableBase, uint64_t virtualAddress) {
		directoryTableBase &= ~0xf;

		uint64_t pageOffset = virtualAddress & ~(~0ul << PAGE_OFFSET_SIZE);
		uint64_t pte = ((virtualAddress >> 12) & (0x1ffll));
		uint64_t pt = ((virtualAddress >> 21) & (0x1ffll));
		uint64_t pd = ((virtualAddress >> 30) & (0x1ffll));
		uint64_t pdp = ((virtualAddress >> 39) & (0x1ffll));

		SIZE_T readsize = 0;
		uint64_t pdpe = 0;
		physical::ReadPhysicalAddress((directoryTableBase + 8 * pdp), &pdpe, sizeof(pdpe), &readsize);
		if (~pdpe & 1)
			return 0;

		uint64_t pde = 0;
		physical::ReadPhysicalAddress(((pdpe & PMASK) + 8 * pd), &pde, sizeof(pde), &readsize);
		if (~pde & 1)
			return 0;

		/* 1GB large page, use pde's 12-34 bits */
		if (pde & 0x80)
			return (pde & (~0ull << 42 >> 12)) + (virtualAddress & ~(~0ull << 30));

		uint64_t pteAddr = 0;
		physical::ReadPhysicalAddress(((pde & PMASK) + 8 * pt), &pteAddr, sizeof(pteAddr), &readsize);
		if (~pteAddr & 1)
			return 0;

		/* 2MB large page */
		if (pteAddr & 0x80)
			return (pteAddr & PMASK) + (virtualAddress & ~(~0ull << 21));

		virtualAddress = 0;
		physical::ReadPhysicalAddress(((pteAddr & PMASK) + 8 * pte), &virtualAddress, sizeof(virtualAddress), &readsize);
		virtualAddress &= PMASK;

		if (!virtualAddress)
			return 0;

		return virtualAddress + pageOffset;
	}

	BOOL checkWithinModuleRegion(DWORD64 Address, DWORD64 ModuleBase, DWORD64 ModuleSize)
	{
		DWORD64 ModuleMaxium = ModuleBase + ModuleSize;
		if (Address <= ModuleMaxium && Address >= ModuleBase)
		{
			return true;
		}
		else return false;
	}
}

namespace physical
{
	NTSTATUS ReadPhysicalAddress(LONGLONG TargetAddress, PVOID lpBuffer, SIZE_T Size, SIZE_T* BytesRead)
	{
		MM_COPY_ADDRESS AddrToRead = { 0 };
		AddrToRead.PhysicalAddress.QuadPart = TargetAddress;
		return MmCopyMemory(lpBuffer, AddrToRead, Size, MM_COPY_MEMORY_PHYSICAL, BytesRead);
	}
	NTSTATUS WritePhysicalAddress(LONGLONG TargetAddress, PVOID lpBuffer, SIZE_T Size, SIZE_T* BytesWritten)
	{
		if (!TargetAddress)
			return STATUS_UNSUCCESSFUL;

		PHYSICAL_ADDRESS AddrToWrite = { 0 };
		AddrToWrite.QuadPart = TargetAddress;

		PVOID pmapped_mem = MmMapIoSpaceEx(AddrToWrite, Size, PAGE_READWRITE);

		if (!pmapped_mem)
			return STATUS_UNSUCCESSFUL;

		memcpy(pmapped_mem, lpBuffer, Size);

		*BytesWritten = Size;
		MmUnmapIoSpace(pmapped_mem, Size);
		return STATUS_SUCCESS;
	}

}

NTSTATUS EnchancedReadMemory(PEPROCESS pp, ULONG64 Address, PVOID AllocatedBuffer, SIZE_T size, SIZE_T* read)
{
	PEPROCESS pProcess = pp; NTSTATUS NtRet;
	if (pProcess)
	{
		NtRet = STATUS_SUCCESS;
	}
	else return STATUS_UNSUCCESSFUL;

	ULONG_PTR process_dirbase = cr3::GetProcessCr3(pProcess);
	ObDereferenceObject(pProcess);

	SIZE_T CurOffset = 0;
	SIZE_T TotalSize = size;
	while (TotalSize)
	{

		uint64_t CurPhysAddr = Utilities::TranslateLinearAddress(process_dirbase, Address + CurOffset);
		if (!CurPhysAddr) return STATUS_UNSUCCESSFUL;

		ULONG64 ReadSize = min(PAGE_SIZE - (CurPhysAddr & 0xFFF), TotalSize);
		SIZE_T BytesRead = 0;
		NtRet = physical::ReadPhysicalAddress(CurPhysAddr, (PVOID)((ULONG64)AllocatedBuffer + CurOffset), ReadSize, &BytesRead);
		TotalSize -= BytesRead;
		CurOffset += BytesRead;
		if (NtRet != STATUS_SUCCESS) break;
		if (BytesRead == 0) break;
	}

	*read = CurOffset;
	return NtRet;
}

NTSTATUS EnchancedWriteMemory(PEPROCESS pp, ULONG64 Address, PVOID AllocatedBuffer, SIZE_T size, SIZE_T* written)
{
	PEPROCESS pProcess = pp; NTSTATUS NtRet;
	if (pProcess)
	{
		NtRet = STATUS_SUCCESS;
	}
	else return STATUS_UNSUCCESSFUL;

	ULONG_PTR process_dirbase = cr3::GetProcessCr3(pProcess);
	ObDereferenceObject(pProcess);

	SIZE_T CurOffset = 0;
	SIZE_T TotalSize = size;
	while (TotalSize)
	{
		uint64_t CurPhysAddr = Utilities::TranslateLinearAddress(process_dirbase, Address + CurOffset);
		if (!CurPhysAddr) return STATUS_UNSUCCESSFUL;

		ULONG64 WriteSize = min(PAGE_SIZE - (CurPhysAddr & 0xFFF), TotalSize);
		SIZE_T BytesWritten = 0;
		NtRet = physical::WritePhysicalAddress(CurPhysAddr, (PVOID)((ULONG64)AllocatedBuffer + CurOffset), WriteSize, &BytesWritten);
		TotalSize -= BytesWritten;
		CurOffset += BytesWritten;
		if (NtRet != STATUS_SUCCESS) break;
		if (BytesWritten == 0) break;
	}

	*written = CurOffset;
	return NtRet;
}

namespace pattern
{


}
