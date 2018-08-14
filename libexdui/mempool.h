#pragma once
#include "stdafx.h"
#define MEMBLOCK_ALLOCATED 1;

typedef struct _MEMBLOCK
{
	ULONG Flags;
}MEMBLOCK, *PMEMBLOCK;

typedef struct _MEMPOOL
{
	ULONG MaximumNumberOfBlocks;
	ULONG SizeOfBlockTableEntry;
	ULONG Reserved[2];
	MEMBLOCK FreeBlocks;
	MEMBLOCK CommittedBlocks;
	MEMBLOCK UnCommittedBlocks;
	MEMBLOCK MaxReservedBlocks;
}MEMPOOL, *PMEMPOOL;

typedef VOID(WINAPI *lpRtlInitializeHandleTable)(ULONG MaximumNumberOfHandles, ULONG SizeOfHandleTableEntry, PVOID *HandleTable);
typedef LONG(WINAPI *lpRtlDestroyHandleTable)(PVOID *HandleTable);
typedef PVOID(WINAPI *lpRtlAllocateHandle)(PVOID *HandleTable, ULONG *HandleIndex);
typedef BOOLEAN(WINAPI *lpRtlFreeHandle)(PVOID *HandleTable, LPVOID Handle);
typedef BOOLEAN(WINAPI *lpRtlIsValidHandle)(PVOID *HandleTable, LPVOID Handle);
typedef BOOLEAN(WINAPI *lpRtlIsValidIndexHandle)(PVOID *HandleTable, ULONG Index, LPVOID *Handle);

lpRtlInitializeHandleTable RtlInitializeHandleTable = (lpRtlInitializeHandleTable)GetProcAddress(g_hMod_NtDll, "RtlInitializeHandleTable");
lpRtlDestroyHandleTable RtlDestroyHandleTable = (lpRtlDestroyHandleTable)GetProcAddress(g_hMod_NtDll, "RtlDestroyHandleTable");
lpRtlAllocateHandle RtlAllocateHandle = (lpRtlAllocateHandle)GetProcAddress(g_hMod_NtDll, "RtlAllocateHandle");
lpRtlFreeHandle RtlFreeHandle = (lpRtlFreeHandle)GetProcAddress(g_hMod_NtDll, "RtlFreeHandle");
lpRtlIsValidHandle RtlIsValidHandle = (lpRtlIsValidHandle)GetProcAddress(g_hMod_NtDll, "RtlIsValidHandle");;
lpRtlIsValidIndexHandle RtlIsValidIndexHandle = (lpRtlIsValidIndexHandle)GetProcAddress(g_hMod_NtDll, "RtlIsValidIndexHandle");