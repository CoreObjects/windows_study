﻿// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"

#include "Mydll.h"

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
) {
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
		MessageBox(NULL, L"Fuck", L"Fuck", MB_OK);
		break;
	case DLL_THREAD_ATTACH:break;
	case DLL_THREAD_DETACH:break;
	case DLL_PROCESS_DETACH:break;
	}
	return TRUE;
}
void show(const char* szBuffer) {
	std::cout << szBuffer << std::endl;
}
