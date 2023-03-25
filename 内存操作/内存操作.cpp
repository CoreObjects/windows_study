// 内存操作.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <windows.h>
#include <Psapi.h>
/*
typedef struct _MEMORYSTATUSEX {
  DWORD     dwLength;
  DWORD     dwMemoryLoad;
  DWORDLONG ullTotalPhys;
  DWORDLONG ullAvailPhys;
  DWORDLONG ullTotalPageFile;
  DWORDLONG ullAvailPageFile;
  DWORDLONG ullTotalVirtual;
  DWORDLONG ullAvailVirtual;
  DWORDLONG ullAvailExtendedVirtual;
} MEMORYSTATUSEX, *LPMEMORYSTATUSEX;
dwLength:初始化结构体大小
dwMemoryLoad：一个介于 0 和 100 之间的数字，它指定正在使用的物理内存的大致百分比
ullTotalPhys：实际物理内存量
ullAvailPhys：当前可用的物理内存量（以字节为单位）
ullTotalPageFile：系统或当前进程的当前已提交内存限制
ullAvailPageFile：当前进程可以提交的最大内存量（以字节为单位）
ullTotalVirtual：调用进程的虚拟地址空间的用户模式部分的大小（以字节为单位）
ullAvailVirtual：调用进程的虚拟地址空间的用户模式部分中当前未保留和未提交的内存量
*/
/*
BOOL GetProcessMemoryInfo(
  [in]  HANDLE                   Process,
  [out] PPROCESS_MEMORY_COUNTERS ppsmemCounters,
  [in]  DWORD                    cb
);
typedef struct _PROCESS_MEMORY_COUNTERS {
	DWORD cb;
	DWORD PageFaultCount;
	SIZE_T PeakWorkingSetSize;
	SIZE_T WorkingSetSize;
	SIZE_T QuotaPeakPagedPoolUsage;
	SIZE_T QuotaPagedPoolUsage;
	SIZE_T QuotaPeakNonPagedPoolUsage;
	SIZE_T QuotaNonPagedPoolUsage;
	SIZE_T PagefileUsage;
	SIZE_T PeakPagefileUsage;
} PROCESS_MEMORY_COUNTERS;
typedef PROCESS_MEMORY_COUNTERS *PPROCESS_MEMORY_COUNTERS;
*/
/*
GetCurrentProcess();
GetCurrentProcessId();
*/

int main()
{
    MEMORYSTATUSEX lpMemStatus = { sizeof(MEMORYSTATUSEX) };
    GlobalMemoryStatusEx(&lpMemStatus);
    //系统信息
    SYSTEM_INFO lpSysInfo;
    GetSystemInfo(&lpSysInfo);
    //进程内存信息
	PROCESS_MEMORY_COUNTERS lpProcessMemoryInfo;
	GetProcessMemoryInfo(GetCurrentProcess(), &lpProcessMemoryInfo, sizeof(lpProcessMemoryInfo));
	LPVOID lpBuffer = VirtualAlloc(NULL, 0x100, MEM_COMMIT, PAGE_READONLY);
	if (lpBuffer != NULL) {
		DWORD dwOld;
		BOOL bRet = VirtualProtect(lpBuffer, 0x100, PAGE_EXECUTE_READWRITE, &dwOld);
		memcpy(lpBuffer, "123456", 7);
		VirtualProtect(lpBuffer, 0x100, dwOld, &dwOld);
		memcpy(lpBuffer, "123456", 7);
	}
	
	MEMORY_BASIC_INFORMATION lpMemInfo;
	VirtualQueryEx(GetCurrentProcess(), lpBuffer, &lpMemInfo, sizeof(lpMemInfo));
	if (lpBuffer)
	{
		VirtualFree(lpBuffer, 0, MEM_RELEASE);
	}
    std::cout << "Hello World!\n";
}
