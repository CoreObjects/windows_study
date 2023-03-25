#include "pch.h"
#include "VehHook.h"
#include <TlHelp32.h>
#include <iostream>
HOOKNODE* nodeExceptionHook;
CPUHOOKINFO cpuHookInfo[4] = { 0 };

LONG NTAPI VehHookHandle(struct _EXCEPTION_POINTERS* ExceptionInfo) {

	if (ExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_BREAKPOINT) {
		//		std::cout << 1;
		HOOKNODE* tempHookNode = nodeExceptionHook;
		while (tempHookNode != nullptr) {
			if (tempHookNode->HookAddress == ExceptionInfo->ExceptionRecord->ExceptionAddress) {
				DWORD dwOld;
				VirtualProtect(tempHookNode->HookAddress, 1, PAGE_EXECUTE_READWRITE, &dwOld);
				*(UCHAR*)(tempHookNode->HookAddress) = tempHookNode->OldCode;
				tempHookNode->TargetAddress(ExceptionInfo);
				ExceptionInfo->ContextRecord->EFlags |= 0x100;
				VirtualProtect(tempHookNode->HookAddress, 1, dwOld, &dwOld);
				return EXCEPTION_CONTINUE_EXECUTION;
			}
			tempHookNode = tempHookNode->next;
		}

		return EXCEPTION_CONTINUE_SEARCH;
	}
	else if (ExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_SINGLE_STEP) {

		for (int i = 0; i < 4; i++) {
			if (cpuHookInfo[i].lpHookAddress == ExceptionInfo->ExceptionRecord->ExceptionAddress) {
				switch (i) {
				case 0: ExceptionInfo->ContextRecord->Dr0 = (LONG_PTR)cpuHookInfo[i].lpHookAddress; ExceptionInfo->ContextRecord->Dr7 &= 0xFFFFFFFE; break;
				case 1:	ExceptionInfo->ContextRecord->Dr1 = (LONG_PTR)cpuHookInfo[i].lpHookAddress; ExceptionInfo->ContextRecord->Dr7 &= 0xFFFFFFFB; break;
				case 2:	ExceptionInfo->ContextRecord->Dr2 = (LONG_PTR)cpuHookInfo[i].lpHookAddress; ExceptionInfo->ContextRecord->Dr7 &= 0xFFFFFFEF; break;
				case 3:	ExceptionInfo->ContextRecord->Dr3 = (LONG_PTR)cpuHookInfo[i].lpHookAddress; ExceptionInfo->ContextRecord->Dr7 &= 0xFFFFFFBF; break;
				}
				//				std::cout << 1;
				//				ExceptionInfo->ContextRecord->Dr7 = 0;
				cpuHookInfo[i].TargetAddress(ExceptionInfo);
				ExceptionInfo->ContextRecord->EFlags |= 0x100;
				return EXCEPTION_CONTINUE_EXECUTION;
			}
		}
		for (int i = 0; i < 4; i++) {
			if (cpuHookInfo[i].lpHookAddress != nullptr) {
				switch (i) {
				case 0: ExceptionInfo->ContextRecord->Dr0 = (LONG_PTR)cpuHookInfo[i].lpHookAddress; ExceptionInfo->ContextRecord->Dr7 |= 0b1; break;
				case 1:	ExceptionInfo->ContextRecord->Dr1 = (LONG_PTR)cpuHookInfo[i].lpHookAddress; ExceptionInfo->ContextRecord->Dr7 |= 0b100; break;
				case 2:	ExceptionInfo->ContextRecord->Dr2 = (LONG_PTR)cpuHookInfo[i].lpHookAddress; ExceptionInfo->ContextRecord->Dr7 |= 0b10000; break;
				case 3:	ExceptionInfo->ContextRecord->Dr3 = (LONG_PTR)cpuHookInfo[i].lpHookAddress; ExceptionInfo->ContextRecord->Dr7 |= 0b1000000; break;
				}
			}
		}

		//		ExceptionInfo->ContextRecord->Dr7 = 0x455;
		HOOKNODE* tempHookNode = nodeExceptionHook;
		while (tempHookNode != nullptr) {
			DWORD dwOld;
			VirtualProtect(tempHookNode->HookAddress, 1, PAGE_EXECUTE_READWRITE, &dwOld);
			*(UCHAR*)(tempHookNode->HookAddress) = 0xCC;
			VirtualProtect(tempHookNode->HookAddress, 1, dwOld, &dwOld);
			tempHookNode = tempHookNode->next;
		}
		ExceptionInfo->ContextRecord->EFlags &= 0xFFFFFEFF;
		return EXCEPTION_CONTINUE_EXECUTION;
	}
	return EXCEPTION_CONTINUE_SEARCH;
}

void InitVehHook() {
	nodeExceptionHook = nullptr;
	memset(cpuHookInfo, 0, sizeof(cpuHookInfo));
	AddVectoredExceptionHandler(1, VehHookHandle);
}

void SetVehHook(LPVOID lpHookAddress, HookTargetProc lpTarget) {
	HOOKNODE* tempHookNode = new HOOKNODE;
	tempHookNode->HookAddress = lpHookAddress;
	tempHookNode->TargetAddress = lpTarget;
	tempHookNode->next = nullptr;
	DWORD dwOld;
	if (VirtualProtect(lpHookAddress, 1, PAGE_EXECUTE_READWRITE, &dwOld)) {
		tempHookNode->OldCode = *(unsigned char*)lpHookAddress;
		*(unsigned char*)lpHookAddress = 0xCC;
		VirtualProtect(lpHookAddress, 1, dwOld, &dwOld);
		tempHookNode->next = nodeExceptionHook;
		nodeExceptionHook = tempHookNode;
	}
}

void SetVehCpuHook(LPVOID lpHookAddress, HookTargetProc lpTarget) {
	int i;
	for (i = 0; i < 4; i++) {
		//		std::cout << i << std::endl;
		if (cpuHookInfo[i].lpHookAddress == nullptr) {
			//			std::cout << i << std::endl;
			cpuHookInfo[i].lpHookAddress = lpHookAddress;
			cpuHookInfo[i].TargetAddress = lpTarget;
			DWORD dwProcessId = GetCurrentProcessId();
			HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, dwProcessId);
			if (INVALID_HANDLE_VALUE != hSnap) {
				THREADENTRY32 te32 = { sizeof(te32) };
				BOOL bRet = Thread32First(hSnap, &te32);
				while (bRet) {
					if (te32.th32OwnerProcessID == dwProcessId) {
						HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, te32.th32ThreadID);
						if (hThread != NULL) {
							CONTEXT ctx;
							ctx.ContextFlags = CONTEXT_ALL;
							GetThreadContext(hThread, &ctx);
							switch (i) {
							case 0: ctx.Dr0 = (DWORD)lpHookAddress; ctx.Dr7 |= 0b1; break;
							case 1:	ctx.Dr1 = (DWORD)lpHookAddress; ctx.Dr7 |= 0b100; break;
							case 2:	ctx.Dr2 = (DWORD)lpHookAddress; ctx.Dr7 |= 0b10000; break;
							case 3:	ctx.Dr3 = (DWORD)lpHookAddress; ctx.Dr7 |= 0b1000000; break;
							}
							ctx.Dr7 &= 0xfff0ffff;
							SetThreadContext(hThread, &ctx);
							CloseHandle(hThread);
						}
					}
					bRet = Thread32Next(hSnap, &te32);
				}
				CloseHandle(hSnap);
			}
			break;
		}
	}
	if (i == 4) {
		MessageBoxA(NULL, "请下软件断点", "硬件断点已满", MB_OK);
	}

}

void SetThreadHook(HANDLE hThread) {
	if (hThread != NULL) {

		CONTEXT ctx;
		ctx.ContextFlags = CONTEXT_ALL;
		GetThreadContext(hThread, &ctx);
		ctx.Dr0 = (DWORD)cpuHookInfo[0].lpHookAddress;
		ctx.Dr1 = (DWORD)cpuHookInfo[1].lpHookAddress;
		ctx.Dr2 = (DWORD)cpuHookInfo[2].lpHookAddress;
		ctx.Dr3 = (DWORD)cpuHookInfo[3].lpHookAddress;
		if (cpuHookInfo[0].lpHookAddress)
			ctx.Dr7 |= 0x401;
		else if (cpuHookInfo[1].lpHookAddress)
			ctx.Dr7 |= 0x404;
		else if (cpuHookInfo[2].lpHookAddress)
			ctx.Dr7 |= 0x410;
		else if (cpuHookInfo[3].lpHookAddress)
			ctx.Dr7 |= 0x440;
		SetThreadContext(hThread, &ctx);

	}

}

void DeleteAllHook() {
	std::cout << 1 << std::endl;
	HOOKNODE* tempHookNode = nodeExceptionHook;
	while (tempHookNode != nullptr) {
		DWORD dwOld;
		VirtualProtect(tempHookNode->HookAddress, 1, PAGE_EXECUTE_READWRITE, &dwOld);
		*(UCHAR*)(tempHookNode->HookAddress) = tempHookNode->OldCode;
		VirtualProtect(tempHookNode->HookAddress, 1, dwOld, &dwOld);
		HOOKNODE* delNode = tempHookNode;
		tempHookNode = tempHookNode->next;
		delete delNode;
	}
	nodeExceptionHook = nullptr;
}
