#pragma once
#include "stdafx.h"

typedef struct _MEMBLOCK_ENTRY
{
	struct _MEMBLOCK_ENTRY *NextFree;
	DWORD dwFlags;
	PVOID  Object;
}MEMBLOCK_ENTRY, *PMEMBLOCK_ENTRY;

typedef struct _MEMPOOL
{
	HANDLE hHeap;
	CRITICAL_SECTION cs;
	ULONG nMaxBlocks;
	ULONG nSizeOfBlock;
	DWORD dwFlags;
	LPVOID BaseAddr;
	PMEMBLOCK_ENTRY FreeBlocks;
	PMEMBLOCK_ENTRY CommittedBlocks;
	PMEMBLOCK_ENTRY UnCommittedBlocks;
	PMEMBLOCK_ENTRY MaxReservedBlocks;
}MEMPOOL, *PMEMPOOL;

BOOL _mp_create(
	IN ULONG MaximumNumberOfBlocks,
	IN ULONG SizeOfBlock,
	IN DWORD dwFlags,
	OUT PMEMPOOL MemPool
	);

BOOL _mp_destroy(
	IN OUT PMEMPOOL MemPool
);

PMEMBLOCK_ENTRY _mp_alloc(
	IN PMEMPOOL MemPool,
	OUT PULONG MemBlockIndex OPTIONAL
	);

BOOL _mp_free(
	IN PMEMPOOL MemPool,
	IN PMEMBLOCK_ENTRY MemBlock
	);

BOOL _mp_isvalidindex(
	IN PMEMPOOL MemPool,
	IN ULONG MemBlockIndex,
	OUT PMEMBLOCK_ENTRY *MemBlock
	);