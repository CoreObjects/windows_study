// process2.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <Windows.h>



int main() {
	if (!WaitNamedPipe(L"\\\\.\\pipe\\Communication", NMPWAIT_WAIT_FOREVER)) {
		std::cout << "WaitNamedPipe Error Code:" << GetLastError() << std::endl;
		return -1;
	}
	HANDLE hPipe = CreateFile(L"\\\\.\\pipe\\Communication", GENERIC_READ | GENERIC_WRITE, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	char WriteBuffer[] = "Hello";
	DWORD dwWriteSize = 0;
	WriteFile(hPipe, WriteBuffer, strlen(WriteBuffer) + 1, &dwWriteSize, NULL);
	char szBuffer[0x100] = { 0 };
	DWORD dwReadSize = 0;
	ReadFile(hPipe, szBuffer, 0x100, &dwReadSize, NULL);
	std::cout << szBuffer << std::endl;
	system("pause");
	return 0;
}
