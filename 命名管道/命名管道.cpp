// 命名管道.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <Windows.h>
/*
HANDLE CreateNamedPipeA(
  [in]           LPCSTR                lpName,
  [in]           DWORD                 dwOpenMode,
  [in]           DWORD                 dwPipeMode,
  [in]           DWORD                 nMaxInstances,
  [in]           DWORD                 nOutBufferSize,
  [in]           DWORD                 nInBufferSize,
  [in]           DWORD                 nDefaultTimeOut,
  [in, optional] LPSECURITY_ATTRIBUTES lpSecurityAttributes
);
https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-createnamedpipea
lpName:命名管道名字，格式：\\.\pipe\pipename
dwOpenMode：打开模式：PIPE_ACCESS_DUPLEX双向，PIPE_ACCESS_INBOUND客户端-》服务器，PIPE_ACCESS_OUTBOUND，服务器-》客户端，填FILE_FLAG_OVERLAPPED
dwPipeMode：PIPE_TYPE_BYTE字节流，IPE_TYPE_MESSAGEPIPE_TYPE_MESSAGE
nMaxInstances：可以为此管道创建的最大实例数。几条管道
nOutBufferSize缓冲区
nInBufferSize输入缓冲区大小
nDefaultTimeOut：默认超时值
lpSecurityAttributes：安全描述符：NULL
*/
/*
HANDLE CreateEventA(
  [in, optional] LPSECURITY_ATTRIBUTES lpEventAttributes,
  [in]           BOOL                  bManualReset,
  [in]           BOOL                  bInitialState,
  [in, optional] LPCSTR                lpName
);
lpEventAttributes：
*/
int main() {
	HANDLE hPipe = CreateNamedPipe(L"\\\\.\\pipe\\Communication", PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED, PIPE_TYPE_BYTE, 1, 1024, 1024, 0, NULL);
	if (hPipe != INVALID_HANDLE_VALUE) {
		HANDLE hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		if (hEvent) {
			OVERLAPPED ovlap;
			ZeroMemory(&ovlap, sizeof(ovlap));
			ovlap.hEvent = hEvent;
			ConnectNamedPipe(hPipe, &ovlap);
			WaitForSingleObject(hEvent, INFINITE);
			CloseHandle(hEvent);
			char szBuffer[0x100] = { 0 };
			DWORD dwReadSize = 0;
			BOOL bRet = ReadFile(hPipe, szBuffer, 0x100, &dwReadSize, NULL);
			if (bRet) {
				std::cout << szBuffer << std::endl;
				char WriteBuffer[] = "Hello";
				DWORD dwWriteSize = 0;
				WriteFile(hPipe, WriteBuffer, strlen(WriteBuffer) + 1, &dwWriteSize, NULL);
				system("pause");
				return 0;
			}
		}
	}



}

