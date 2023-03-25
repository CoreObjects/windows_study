#include "pch.h"
#include "OneTimeHook.h"
#include "Windows.h"
char g_OldCode[5] = { 0 };
char g_NewCode[5] = { 0 };
PROC g_HookAddr = NULL;
PROC g_HookTarget = NULL;

#ifndef _WIN64


__declspec(naked) void InlineHookDevice() {
	__asm push g_HookAddr;
	__asm call UnHook
	__asm call g_HookTarget;
	__asm jmp g_HookAddr;
	/*__asm {
		push ebp
		mov ebp, esp
		sub esp, 20h
		lea eax, [ebp - 10]
		push eax
		push 40h
		push 5
		sub dword ptr[ebp + 4], 5
		mov eax, dword ptr[ebp + 4]
		push eax
		call VirtualProtect
		mov eax, dword ptr[ebp + 4]
		lea ebx, m_OldCode
		mov cl, byte ptr[ebx]
		mov byte ptr[eax], cl
		inc eax
		inc ebx
		mov cl, byte ptr[ebx]
		mov byte ptr[eax], cl
		inc eax
		inc ebx
		mov cl, byte ptr[ebx]
		mov byte ptr[eax], cl
		inc eax
		inc ebx
		mov cl, byte ptr[ebx]
		mov byte ptr[eax], cl
		inc eax
		inc ebx
		mov cl, byte ptr[ebx]
		mov byte ptr[eax], cl
		leave
		ret
	}*/
}

void __stdcall UnHook(PROC fpAddress) {
	DWORD dwOld;
	BOOL bRet = VirtualProtect(fpAddress, 5, PAGE_EXECUTE_READWRITE, &dwOld);
	if (bRet) {
		memcpy(fpAddress, g_OldCode, 5);
	}
	VirtualProtect(fpAddress, 5, dwOld, &dwOld);
}

void InlineHookOnceTime(LPVOID lpAddress, LPVOID lpTarget) {
	g_NewCode[0] = 0xE9;
	g_HookAddr = (PROC)lpAddress;
	g_HookTarget = (PROC)lpTarget;
	*(DWORD*)((char*)g_NewCode + 1) = (DWORD)(InlineHookDevice)-((DWORD)lpAddress + 5);
	DWORD dwOld;
	BOOL bRet = VirtualProtect(lpAddress, 5, PAGE_EXECUTE_READWRITE, &dwOld);
	if (bRet) {
		memcpy(g_OldCode, lpAddress, 5);
		memcpy(lpAddress, g_NewCode, 5);
	}
	VirtualProtect(lpAddress, 5, dwOld, &dwOld);
}


#endif // WIN64


