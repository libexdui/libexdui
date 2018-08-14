#pragma once
#include "mempool.h"

#define EHT_ELEMENT 1 //DUIÔªËØ
#define EHT_CANVAS 2 //»­²¼
#define EHT_IMAGE 3 //Í¼Æ¬
#define EHT_FONT 4 //×ÖÌå

typedef struct _HANDLEBLOCK
{
	MEMBLOCK mb;
	PVOID Object;
}HANDLEBLOCK, *PHANDLEBLOCK;

MEMPOOL g_hTable_handle;

#define EHL_INDEXBIT 0x00FFFF00
#define EHL_TYPEBIT 0x000000F0
#define EHL_INDEXOFFSET 8
#define EHL_TYPEOFFSET 4

#define _handle_create_imp(index,type) (((ULONG_PTR)index << EHL_INDEXOFFSET)| (ULONG_PTR)type << EHL_TYPEOFFSET)
#define _handle_getindex(handle) ((((ULONG_PTR)handle) & EHL_INDEXBIT) >> EHL_INDEXOFFSET)
#define _handle_gettype(handle) ((((ULONG_PTR)handle) & EHL_TYPEBIT) >> EHL_TYPEOFFSET)
// 0(8)|index(16)|type(4)0(4)

void _handle_init(ULONG nMaxHandles)
{
	RtlInitializeHandleTable(nMaxHandles, sizeof(HANDLEBLOCK), (PVOID*)&g_hTable_handle);
}

long _handle_unint()
{
	return RtlDestroyHandleTable((PVOID*)&g_hTable_handle);
}


HANDLE _handle_create(DWORD nObjectType,LPVOID lpObject)
{
	PVOID ret = NULL;
	ULONG index;
	HANDLEBLOCK *entry = NULL;
	entry = (PHANDLEBLOCK)RtlAllocateHandle((PVOID*)&g_hTable_handle, &index);
	if (entry)
	{
		entry->mb.Flags |= MEMBLOCK_ALLOCATED;
		entry->Object = lpObject;
		ret = (PVOID)_handle_create_imp(index, (BYTE)nObjectType);
	}
	return ret;
}

bool _handle_destroy(HANDLE handle)
{
	BOOL ret = FALSE;
	ULONG index;
	LPVOID entry = NULL;
	index = (ULONG)_handle_getindex(handle);
	if (RtlIsValidIndexHandle((PVOID*)&g_hTable_handle, index, &entry))
	{
		if (RtlIsValidHandle((PVOID*)&g_hTable_handle, entry))
		{
			ret = RtlFreeHandle((PVOID*)&g_hTable_handle, entry);
		}
	}
	return ret;
}

bool _handle_validate(HANDLE handle, DWORD nObjectType, PVOID *Object)
{
	BOOL ret = FALSE;
	ULONG index;
	PVOID entry = NULL;

	if (_handle_gettype(handle) == nObjectType)
	{
		index = (ULONG)_handle_getindex(handle);
		if (RtlIsValidIndexHandle((PVOID*)&g_hTable_handle, index, &entry))
		{
			if (RtlIsValidHandle((PVOID*)&g_hTable_handle, entry))
			{
				*Object = ((PHANDLEBLOCK)entry)->Object;
				ret = TRUE;
			}
		}
	}
	return ret;
}