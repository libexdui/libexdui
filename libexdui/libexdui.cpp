// libexdui.cpp: 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "libexdui.h"


int WINAPI Ex_Int(DWORD test)
{
	MessageBoxW(NULL, L"Hello World!", NULL, MB_ICONINFORMATION);

	//STRING s = AP2W("hello");
	//ExDbg_Print(DBG_CODE_HANDLE_CHECK, L"Test %d,%s", _FNL, Normal, 123, L"MINE");

	return 0;
}