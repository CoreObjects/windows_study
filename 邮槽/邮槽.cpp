// 邮槽.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <windows.h>
/*
HANDLE CreateMailslotA(
  [in]           LPCSTR                lpName,
  [in]           DWORD                 nMaxMessageSize,
  [in]           DWORD                 lReadTimeout,
  [in, optional] LPSECURITY_ATTRIBUTES lpSecurityAttributes
);
lpName:邮槽名：\\.\mailslot\[path]name
nMaxMessageSize:填0
lReadTimeout：MAILSLOT_WAIT_FOREVER等待消息
lpSecurityAttributes：安全描述符：NULL
*/
int main()
{
    HANDLE hMailslot = CreateMailslot(L"\\\\.\\mailslot\\Communication", 0, MAILSLOT_WAIT_FOREVER, NULL);
    if (hMailslot != INVALID_HANDLE_VALUE) {
		char szBuffer[MAX_PATH] = { 0 };
		DWORD dwRealRead;
		BOOL bRet = ReadFile(hMailslot, szBuffer, MAX_PATH, &dwRealRead, NULL);
		if (bRet) {
			std::cout << szBuffer << std::endl;
			system("pause");
		}
		CloseHandle(hMailslot);
    }
    
    
}
