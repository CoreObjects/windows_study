// 消息钩子注入.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

BOOL MsgHookInject(DWORD dwProcessId, LPCSTR lpMoudlePath) {
	//SetWindowsHookEx
	HHOOK hHook;
	HMODULE hModule = LoadLibraryA(lpMoudlePath);
	DWORD dwFuncAddr = (DWORD)GetProcAddress(hModule, "radd");
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, dwProcessId);
	THREADENTRY32 te32 = { sizeof(THREADENTRY32) };
	while (Thread32Next(hSnap, &te32)) {
		if (te32.th32OwnerProcessID == dwProcessId) {
			hHook = SetWindowsHookEx(WH_GETMESSAGE, (HOOKPROC)dwFuncAddr, hModule, te32.th32ThreadID);
			break;
		}
	}
	return TRUE;
}

int main() {
	MsgHookInject(32324, "C:\\Users\\89598\\source\\repos\\windows核心编程2\\Debug\\msgDll.dll");
	system("pause");
	return 0;
}



