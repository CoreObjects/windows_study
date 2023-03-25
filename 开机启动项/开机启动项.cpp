// 开机启动项.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#define  _CRT_SECURE_NO_WARNINGS
#include<windows.h>
#include <iostream>

BOOL ExeShell(const char* cmd, char* result) {
	char szBuffer[MAX_PATH] = { 0 };
	FILE* pipe = _popen(cmd, "r");
	while (!feof(pipe)) {
		if (fgets(szBuffer, MAX_PATH, pipe)) {
			strcat(result, szBuffer);
		}
	}
	_pclose(pipe);
	return TRUE;
}
int main()
{
 	STARTUPINFOA si = { sizeof(si) }; PROCESS_INFORMATION pi;
	CHAR cmd[] = "C:\\Program Files\\Oray\\SunLogin\\SunloginClient\\SunloginClient.exe --mod=uninstall";
		//"\"C:\\Program Files (x86)\\Tencent\\QQPCMgr\\13.10.21914.214\\Uninst.exe\"";
//	system(cmd);
 	BOOL bRet = CreateProcessA(NULL, cmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	while (true) {
		char cmd[MAX_PATH] = { 0 };
		std::cin >> cmd;
		char result[MAX_PATH]{ 0 };
		ExeShell(cmd, result);
		std::cout << result;
	}
    std::cout << "Hello World!\n";
}


