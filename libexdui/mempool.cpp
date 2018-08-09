#include "mempool.h"


VOID _mp_create(
	IN ULONG MaximumNumberOfBlocks,
	IN ULONG SizeOfBlock,
	OUT PMEMPOOL MemPool)
{
	ZeroMemory(MemPool, sizeof(*MemPool));
	MemPool->������� = MaximumNumberOfBlocks;
	MemPool->�ڴ��ߴ� = SizeOfBlock;
	return;
};

BOOLEAN _mp_destroy(
	IN OUT PMEMPOOL MemPool)
{
	BOOL Ret;
	Ret = VirtualFree(MemPool->CommittedBlocks, NULL, MEM_RELEASE);
	return Ret;
};

PMEMBLOCK_ENTRY _mp_alloc(
	IN PMEMPOOL MemPool,
	OUT PULONG MemBlockIndex OPTIONAL)
{
	DWORD Status;
	PVOID BaseAddress;
	ULONG n;
	SIZE_T ReserveSize;
	SIZE_T CommitSize;
	PMEMBLOCK_ENTRY p, *pp;

	if (MemPool->FreeBlocks == NULL) {
			if (MemPool->UnCommittedBlocks == NULL) {
				ReserveSize = MemPool->������� * MemPool->�ڴ��ߴ�;
				BaseAddress = VirtualAlloc(NULL, ReserveSize, MEM_RESERVE, PAGE_READWRITE);
				if (BaseAddress) {
					MemPool->CommittedBlocks = (PMEMBLOCK_ENTRY)BaseAddress;
					MemPool->UnCommittedBlocks = (PMEMBLOCK_ENTRY)BaseAddress;
					MemPool->MaxReservedBlocks = (PMEMBLOCK_ENTRY)((PCHAR)BaseAddress + ReserveSize);
				}
				else
				{
					Status = ERROR_INVALID_ADDRESS;
				}
			}
			p = MemPool->UnCommittedBlocks;
			if (p >= MemPool->MaxReservedBlocks) {
					Status = STATUS_NO_MEMORY;
				}
				else {
					CommitSize = MemPool->�ڴ��ߴ�;
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
		pp = &p->Next;
		p = (PMEMBLOCK_ENTRY)((PUCHAR)p + MemPool->�ڴ��ߴ�);
	}
	// Remove handle table entry from head of free list.
	p = MemPool->FreeBlocks;
	MemPool->FreeBlocks = p->Next;
	// Clear free list link field, which also leaves the handle allocated bit
	// clear.  This allows the caller to mark it is allocated after they are
	// done filling in their portion.
	p->Next = NULL;
	// If requested, return the index of this handle table entry
	*MemBlockIndex = (ULONG)(((PCHAR)p - (PCHAR)MemPool->CommittedBlocks) / MemPool->�ڴ��ߴ�);
	return p;
}



BOOLEAN _mp_free(
	IN PMEMPOOL MemPool,
	IN PMEMBLOCK_ENTRY MemBlock)
{
};

BOOLEAN _mp_isvalidindex(
	IN PMEMPOOL MemPool,
	IN ULONG MemBlockIndex,
	OUT PMEMBLOCK_ENTRY *MemBlock)
{

};