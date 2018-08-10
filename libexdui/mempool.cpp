#include "stdafx.h"
#include "mempool.h"

DWORD m_page_size;

BOOL _mp_create(
	IN ULONG MaximumNumberOfBlocks,
	IN ULONG SizeOfBlock,
	IN DWORD dwFlags, //MPF_
	OUT PMEMPOOL MemPool)
{
	ZeroMemory(MemPool, sizeof(*MemPool));
	MemPool->hHeap = HeapCreate(HEAP_GENERATE_EXCEPTIONS, 0, 0);
	if (MemPool->hHeap)
	{
		InitializeCriticalSection(&MemPool->cs);
		MemPool->nMaxBlocks = MaximumNumberOfBlocks;
		MemPool->nSizeOfBlock = SizeOfBlock + sizeof(MEMBLOCK_ENTRY);
		MemPool->BaseAddr = HeapAlloc(MemPool->hHeap, HEAP_ZERO_MEMORY, MemPool->nSizeOfBlock * MaximumNumberOfBlocks);
		return TRUE;
	}
	return FALSE;
};

BOOL _mp_destroy(
	IN OUT PMEMPOOL MemPool)
{
	if (MemPool->BaseAddr)
	{
		HeapDestroy(MemPool->hHeap);
		VirtualFree(MemPool->CommittedBlocks, NULL, MEM_RELEASE);
		DeleteCriticalSection(&MemPool->cs);
		ZeroMemory(MemPool, sizeof(*MemPool));
		return TRUE;
	}
	
	return FALSE;
};

PMEMBLOCK_ENTRY _mp_alloc(
	IN PMEMPOOL MemPool,
	OUT PULONG MemBlockIndex OPTIONAL)
{
	DWORD Status;
	PVOID BaseAddress;
	SIZE_T ReserveSize;
	SIZE_T CommitSize;
	PMEMBLOCK_ENTRY p, *pp;

	if (MemPool->FreeBlocks == NULL) 
	{
		if (MemPool->UnCommittedBlocks == NULL) 
		{
			ReserveSize = MemPool->nMaxBlocks * MemPool->nSizeOfBlock;
			BaseAddress = VirtualAlloc(NULL, ReserveSize, MEM_RESERVE, PAGE_READWRITE);
			if (BaseAddress) 
			{
					MemPool->CommittedBlocks = (PMEMBLOCK_ENTRY)BaseAddress;
					MemPool->UnCommittedBlocks = (PMEMBLOCK_ENTRY)BaseAddress;
					MemPool->MaxReservedBlocks = (PMEMBLOCK_ENTRY)((PCHAR)BaseAddress + ReserveSize);
			}
			else
			{
					Status = STATUS_NO_MEMORY;
			}
		}

		p = MemPool->UnCommittedBlocks;
		if (p >= MemPool->MaxReservedBlocks)
		{
			Status = STATUS_NO_MEMORY;
		}else 
		{
			if (!m_page_size)
			{
				SYSTEM_INFO *si;
				GetSystemInfo(si);
				m_page_size = si->dwPageSize;
			}
			CommitSize = m_page_size; 
			BaseAddress = VirtualAlloc(&p, CommitSize, MEM_COMMIT, PAGE_READWRITE);
			if (BaseAddress)
			{
				MemPool->UnCommittedBlocks = (PMEMBLOCK_ENTRY)((PCH)p + CommitSize);
			}
		}
	}

	if (!SUCCEEDED(Status))
	{
		return NULL;
	}

	pp = &MemPool->FreeBlocks;
	while (p < MemPool->UnCommittedBlocks) 
	{
		*pp = p;
		pp = &p->NextFree;
		p = (PMEMBLOCK_ENTRY)((PUCHAR)p + MemPool->nSizeOfBlock);
	}
	// Remove handle table entry from head of free list.
	p = MemPool->FreeBlocks;
	MemPool->FreeBlocks = p->NextFree;
	// Clear free list link field, which also leaves the handle allocated bit
	// clear.  This allows the caller to mark it is allocated after they are
	// done filling in their portion.
	p->NextFree = NULL;
	// If requested, return the index of this handle table entry
	*MemBlockIndex = (ULONG)(((PCHAR)p - (PCHAR)MemPool->CommittedBlocks) / MemPool->nSizeOfBlock);
	return p;

	/*
	EnterCriticalSection(&cs);//¼ÓËø
	for (int i = 0; i<10; i++) {
		n_AddValue++;
		cout << "n_AddValue in SecondThread is " << n_AddValue << endl;

	}
	LeaveCriticalSection(&cs);//½âËø
	*/
}



BOOL _mp_free(
	IN PMEMPOOL MemPool,
	IN PMEMBLOCK_ENTRY MemBlock)
{
	RtlZeroMemory(MemBlock, MemPool->nSizeOfBlock);
	MemBlock->NextFree = MemPool->FreeBlocks;
	MemPool->FreeBlocks = MemBlock;
	return TRUE;
};

BOOL _mp_isvalidindex(
	IN PMEMPOOL MemPool,
	IN ULONG MemBlockIndex,
	OUT PMEMBLOCK_ENTRY *MemBlock)
{
	return TRUE;
};