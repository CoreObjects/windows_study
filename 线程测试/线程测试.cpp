﻿// 线程测试.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <Windows.h>
int m = 100;
const char* szBuffer = "Dll1.dll";
#pragma data_seg("123")

unsigned char hexData1[154] = {
	0x55, 0x8B, 0xEC, 0x83, 0xEC, 0x10, 0x64, 0xA1, 0x30, 0x00, 0x00, 0x00, 0x8B, 0x40, 0x0C, 0x8B,
	0x40, 0x1C, 0x8B, 0x00, 0x8B, 0x00, 0x8B, 0x40, 0x08, 0x89, 0x45, 0xFC, 0x8B, 0xC8, 0xBA, 0xE8,
	0x22, 0x19, 0x64, 0xE8, 0x0B, 0x00, 0x00, 0x00, 0x8D, 0x1D,
	0x00, 0x30, 0x40, 0x00, 
	0x53, 0xFF,
	0xD0, 0xC9, 0xC3, 0x55, 0x8B, 0xEC, 0x83, 0xEC, 0x0C, 0x8B, 0xD9, 0x89, 0x55, 0xF8, 0x56, 0x57,
	0x8B, 0x43, 0x3C, 0x8B, 0x44, 0x18, 0x78, 0x8B, 0x54, 0x18, 0x1C, 0x8B, 0x4C, 0x18, 0x20, 0x03,
	0xD3, 0x8B, 0x7C, 0x18, 0x24, 0x03, 0xCB, 0x89, 0x55, 0xF4, 0x03, 0xFB, 0x8B, 0x31, 0x03, 0xF3,
	0x89, 0x4D, 0xFC, 0x8A, 0x06, 0x0F, 0xBE, 0xD0, 0x84, 0xC0, 0x74, 0x12, 0x46, 0xC1, 0xCA, 0x07,
	0x8A, 0x0E, 0x0F, 0xBE, 0xC1, 0x03, 0xD0, 0x84, 0xC9, 0x75, 0xF1, 0x8B, 0x4D, 0xFC, 0x3B, 0x55,
	0xF8, 0x74, 0x08, 0x83, 0xC1, 0x04, 0x83, 0xC7, 0x02, 0xEB, 0xD1, 0x0F, 0xB7, 0x07, 0x8B, 0x4D,
	0xF4, 0x5F, 0x5E, 0x8B, 0x04, 0x81, 0x03, 0xC3, 0xC9, 0xC3
};

unsigned char hexData[] = {
	0x55, 0x8B, 0xEC, 0x83, 0xEC, 0x10, 0x64, 0xA1, 0x30, 0x00, 0x00, 0x00, 0x8B, 0x40, 0x0C, 0x8B,
	0x40, 0x1C, 0x8B, 0x00, 0x8B, 0x00, 0x8B, 0x40, 0x08, 0x89, 0x45, 0xFC, 0x8B, 0xC8, 0xBA, 0xE8,
	0x22, 0x19, 0x64, 0xE8, 0x0B, 0x00, 0x00, 0x00, 0x8B, 0x1D,
	0x00, 0x30, 0x40, 0x00,
	0x53, 0xFF,
	0xD0, 0xC9, 0xC3, 0x55, 0x8B, 0xEC, 0x83, 0xEC, 0x0C, 0x8B, 0xD9, 0x89, 0x55, 0xF8, 0x56, 0x57,
	0x8B, 0x43, 0x3C, 0x8B, 0x44, 0x18, 0x78, 0x8B, 0x54, 0x18, 0x1C, 0x8B, 0x4C, 0x18, 0x20, 0x03,
	0xD3, 0x8B, 0x7C, 0x18, 0x24, 0x03, 0xCB, 0x89, 0x55, 0xF4, 0x03, 0xFB, 0x8B, 0x31, 0x03, 0xF3,
	0x89, 0x4D, 0xFC, 0x8A, 0x06, 0x0F, 0xBE, 0xD0, 0x84, 0xC0, 0x74, 0x12, 0x46, 0xC1, 0xCA, 0x07,
	0x8A, 0x0E, 0x0F, 0xBE, 0xC1, 0x03, 0xD0, 0x84, 0xC9, 0x75, 0xF1, 0x8B, 0x4D, 0xFC, 0x3B, 0x55,
	0xF8, 0x74, 0x08, 0x83, 0xC1, 0x04, 0x83, 0xC7, 0x02, 0xEB, 0xD1, 0x0F, 0xB7, 0x07, 0x8B, 0x4D,
	0xF4, 0x5F, 0x5E, 0x8B, 0x04, 0x81, 0x03, 0xC3, 0xC9, 0xC3
};
#pragma data_seg()
#pragma comment(linker,"/SECTION:123,RWE")
int main()
 {
// 	*(DWORD*)(hexData1 + 42) = (DWORD)szBuffer;
// 	__asm {
// 		lea eax, hexData1
// 		push eax
// 		ret
// 	}
// HANDLE hFile = CreateFile(L"C:\\Users\\89598\\source\\repos\\windows核心编程2\\Debug\\进程与线程作业一.exe", GENERIC_READ | GENERIC_WRITE, \
// 	0, NULL, \
// 	OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
// 
// CloseHandle(hFile);
// 	STARTUPINFO startupInfo{ sizeof(startupInfo) };
// 	PROCESS_INFORMATION procInfo;
// 	WCHAR wcsCmd[] = L"C:\\Users\\89598\\source\\repos\\windows核心编程2\\Debug\\X86Inject.exe 0 12138 C:\\Users\\89598\\source\\repos\\windows核心编程2\\Debug\\Hookdll.dll";
// 	CreateProcess(L"C:\\Users\\89598\\source\\repos\\windows核心编程2\\Debug\\X86Inject.exe", wcsCmd, NULL, NULL, FALSE, 0, NULL, NULL, &startupInfo, &procInfo);
// 	CloseHandle(procInfo.hProcess);
// 	CloseHandle(procInfo.hThread);
//	HMODULE hModule = LoadLibrary(L"Dll1.dll");
	
	int a=0;
	while (1)
	{
		std::cout << a;
		a++;
	}
	while (true) {
		MessageBoxW(NULL, L"Mymsgw", L"Mymsgw", MB_OK);
		MessageBoxA(NULL, "MymsgA", "MymsgA", MB_OK);
// 		if (hModule) {
// 			FreeLibrary(hModule);
// 		}
	}
// 	DWORD dwThreadId;
// 	HANDLE hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ThreadCallBack, NULL, 0, &dwThreadId);
// 	if (hThread != NULL) {
// 		WaitForSingleObject(hThread, -1);
// 		CloseHandle(hThread);
// 	}
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
