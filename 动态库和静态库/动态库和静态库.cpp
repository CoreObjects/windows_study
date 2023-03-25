// 动态库和静态库.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <iostream>
#include <windows.h>
#include <TlHelp32.h>
BOOL RemoteInject(DWORD dwProcessId, LPCTSTR szPath) {
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
    //注入远程线程回调函数参数
    if (hProcess == INVALID_HANDLE_VALUE) {
        return FALSE;
    }
    LPVOID lpBuffer = VirtualAllocEx(hProcess, NULL, MAX_PATH, MEM_COMMIT, PAGE_READWRITE);
    if (lpBuffer == NULL) {
        return FALSE;
    }
    DWORD dwWriteSize;
    BOOL bRet = WriteProcessMemory(hProcess, lpBuffer, szPath, (wcslen(szPath) + 1) * 2, &dwWriteSize);
    if (bRet == FALSE) {
        return FALSE;
    }
    HANDLE hThread = CreateRemoteThread(hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibraryW, lpBuffer, 0, NULL);
    if (hThread != NULL) {
        WaitForSingleObject(hThread, -1);
        CloseHandle(hThread);
    }
    VirtualFreeEx(hProcess, lpBuffer, 0, MEM_RELEASE);
    CloseHandle(hProcess);
    return TRUE;
}
void ShowModule(DWORD dwProcessId) {
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, dwProcessId);
    MODULEENTRY32 me32 = { sizeof(MODULEENTRY32) };
    BOOL bRet = Module32First(hSnap, &me32);
    while (bRet)
    {
        std::wcout << me32.szModule << std::endl;
        std::wcout << me32.szExePath << std::endl;
        std::wcout <<"0x" << std::hex << (DWORD)me32.modBaseAddr << std::endl;
        std::wcout << me32.modBaseSize << std::endl;
        bRet = Module32Next(hSnap, &me32);
    }
}
BOOL RemoteFreeLibrary(DWORD dwProcessId,LPCWSTR wcsModuleName) {
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, dwProcessId);
	MODULEENTRY32 me32 = { sizeof(MODULEENTRY32) };
	BOOL bRet = Module32First(hSnap, &me32);
	while (bRet) {
        if (!wcscmp(wcsModuleName,me32.szModule)) {

            break;
        }
		bRet = Module32Next(hSnap, &me32);
	}
    HMODULE hModule = (HMODULE)me32.modBaseAddr;
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
	if (hProcess != INVALID_HANDLE_VALUE) {
		HANDLE hThread = CreateRemoteThread(hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)FreeLibrary, hModule, 0, NULL);
		if (hThread != NULL) {
			WaitForSingleObject(hThread, -1);
			CloseHandle(hThread);
			//			AfxMessageBox(L"卸载成功！！");
		}
		CloseHandle(hProcess);
	}
    return TRUE;
}
int main()
{
    using MyShow = void(*)(const char*);
    std::wcout.imbue(std::locale("chs"));
//     HMODULE hModule = LoadLibraryW(L"Mydll2.dll");
//     if (hModule != NULL) {
//         MyShow show = (MyShow)GetProcAddress(hModule, "show");
//         show("Fuck");
//     }

//    ShowModule(33060);
    
    RemoteInject(29012, L"Hookdll.dll");
//    RemoteFreeLibrary(24808, L"InlineHookdll.dll");
//    ShowModule(33060);
//    LoadLibraryW(L"C:\\Users\\89598\\source\\repos\\windows核心编程2\\Debug\\MFCLibrary1.dll");
//    system("pause");
}

