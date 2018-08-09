#pragma once
#include "stdafx.h"

typedef struct _MEMBLOCK_ENTRY
{
	struct _MEMBLOCK_ENTRY *Next;
	PVOID  Object;
}MEMBLOCK_ENTRY, *PMEMBLOCK_ENTRY;

typedef struct _MEMPOOL
{
	ULONG 最大数量;
	ULONG 内存块尺寸;
	PMEMBLOCK_ENTRY FreeBlocks;
	PMEMBLOCK_ENTRY CommittedBlocks;
	PMEMBLOCK_ENTRY UnCommittedBlocks;
	PMEMBLOCK_ENTRY MaxReservedBlocks;
}MEMPOOL, *PMEMPOOL;

VOID _mp_create(
	IN ULONG MaximumNumberOfBlocks,
	IN ULONG SizeOfBlock,
	OUT PMEMPOOL MemPool
	);

BOOLEAN _mp_destroy(
	IN OUT PMEMPOOL MemPool
);

PMEMBLOCK_ENTRY _mp_alloc(
	IN PMEMPOOL MemPool,
	OUT PULONG MemBlockIndex OPTIONAL
	);

BOOLEAN _mp_free(
	IN PMEMPOOL MemPool,
	IN PMEMBLOCK_ENTRY MemBlock
	);

BOOLEAN _mp_isvalidindex(
	IN PMEMPOOL MemPool,
	IN ULONG MemBlockIndex,
	OUT PMEMBLOCK_ENTRY *MemBlock
	);