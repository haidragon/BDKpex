#pragma once
#include "structures.h"

typedef unsigned long long uint64_t;

namespace physical
{
	NTSTATUS ReadPhysicalAddress(LONGLONG TargetAddress, PVOID lpBuffer, SIZE_T Size, SIZE_T* BytesRead);
	NTSTATUS WritePhysicalAddress(LONGLONG TargetAddress, PVOID lpBuffer, SIZE_T Size, SIZE_T* BytesWritten);
}