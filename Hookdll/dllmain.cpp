// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include "InlineHook.h"
#include "HotFixHook.h"
#include "OneTimeHook.h"
#include "VehHook.h"
#include <iostream>
InlineHook hook;
HotFixHook hhook;
int WINAPI MyMessageBoxW(_In_opt_ HWND hWnd, _In_opt_ LPCWSTR lpText, _In_opt_ LPCWSTR lpCaption, _In_ UINT uType) {
    hook.UnLoadHook((PROC)MessageBoxW);
	MessageBoxW(NULL, L"Fuck", L"Fuck you", MB_OK);
    hook.ReLoadHook((PROC)MessageBoxW);
	return 0;
}
int WINAPI hotfixMessageBoxW(_In_opt_ HWND hWnd, _In_opt_ LPCWSTR lpText, _In_opt_ LPCWSTR lpCaption, _In_ UINT uType) {
	using Myfun = int (WINAPI*)(_In_opt_ HWND hWnd, _In_opt_ LPCWSTR lpText, _In_opt_ LPCWSTR lpCaption, _In_ UINT uType);
	Myfun mymessagebox = (Myfun)((DWORD)MessageBoxW + 2);
    return mymessagebox(NULL, L"Fuck", L"Fuck you", MB_OK);
}
void msg() {
    HMODULE hModule = GetModuleHandle(NULL);
    LPVOID lpParam = (LPVOID)((DWORD)hModule + 0x17B30);
    DWORD dwOld;
    BOOL bRet = VirtualProtect(lpParam, 10, PAGE_EXECUTE_READWRITE, &dwOld);
    if (bRet) {
        memcpy(lpParam, L"Fuck", 10);
    }
//    MessageBoxW(NULL, L"Fuck", L"Fuck you", MB_OK);
}
extern"C" __declspec(dllexport) void radd() {

}
#ifndef _WIN64
LONG NTAPI VehMessageBoxW(struct _EXCEPTION_POINTERS* ExceptionInfo) {
	//    std::cout << 4 << std::endl;
	LPCWSTR szbuffer = L"dfuck";
	*(DWORD*)(ExceptionInfo->ContextRecord->Esp + 8) = (DWORD)szbuffer;
	*(DWORD*)(ExceptionInfo->ContextRecord->Esp + 12) = (DWORD)szbuffer;
	//    ExceptionInfo->ContextRecord->Eip += 2;
	//    std::cout << 5 << std::endl;
	return EXCEPTION_CONTINUE_EXECUTION;
}
LONG NTAPI VehMessageBoxA(struct _EXCEPTION_POINTERS* ExceptionInfo) {
	//    std::cout << 4 << std::endl;
// 	LPCSTR szbuffer = "fuck";
// 	*(DWORD*)(ExceptionInfo->ContextRecord->Esp + 8) = (DWORD)szbuffer;
// 	*(DWORD*)(ExceptionInfo->ContextRecord->Esp + 12) = (DWORD)szbuffer;
// 	ExceptionInfo->ContextRecord->Eip += 2;
	LPCSTR szbuffer = "fuck";
	*(DWORD*)(ExceptionInfo->ContextRecord->Esp + 8) = (DWORD)szbuffer;
	*(DWORD*)(ExceptionInfo->ContextRecord->Esp + 12) = (DWORD)szbuffer;
	//    MessageBoxA(NULL, "Fuck", "Fuck", MB_OK);

		//    std::cout << 5 << std::endl;
	return EXCEPTION_CONTINUE_EXECUTION;
}
#else
LONG NTAPI VehMessageBoxW(struct _EXCEPTION_POINTERS* ExceptionInfo) {
	LPCWSTR szbuffer = L"dfuck";
	ExceptionInfo->ContextRecord->R8 = (LONG_PTR)szbuffer;
	ExceptionInfo->ContextRecord->Rdx = (LONG_PTR)szbuffer;
	return EXCEPTION_CONTINUE_EXECUTION;
}
LONG NTAPI VehMessageBoxA(struct _EXCEPTION_POINTERS* ExceptionInfo) {
	
	LPCSTR szbuffer = "fuck";
	ExceptionInfo->ContextRecord->R8 = (LONG_PTR)szbuffer;
	ExceptionInfo->ContextRecord->Rdx = (LONG_PTR)szbuffer;
	return EXCEPTION_CONTINUE_EXECUTION;
}

#endif


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        InitVehHook();
        SetVehHook(MessageBoxA, VehMessageBoxA);
        SetVehHook(MessageBoxW, VehMessageBoxW);
//       hhook.SetHook((PROC)MessageBoxW, (PROC)hotfixMessageBoxW);
//        hook.SetHook((PROC)MessageBoxA, (PROC)MyMessageBoxW);
//        hook.SetHook((PROC)MessageBoxW, (PROC)MyMessageBoxW);
//        InlineHookOnceTime(MessageBoxA, msg);
        break;
    case DLL_THREAD_ATTACH:
//        SetThreadHook(GetCurrentThread());
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
 //       DeleteAllHook();
        break;
    }
    return TRUE;
}

//ebp
//eip