// 进程与线程.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
/*
UINT WinExec(
  [in] LPCSTR lpCmdLine,
  [in] UINT   uCmdShow
);
*/
/*
BOOL CreateProcessW(
  [in, optional]      LPCWSTR               lpApplicationName,
  [in, out, optional] LPWSTR                lpCommandLine,
  [in, optional]      LPSECURITY_ATTRIBUTES lpProcessAttributes,
  [in, optional]      LPSECURITY_ATTRIBUTES lpThreadAttributes,
  [in]                BOOL                  bInheritHandles,
  [in]                DWORD                 dwCreationFlags,
  [in, optional]      LPVOID                lpEnvironment,
  [in, optional]      LPCWSTR               lpCurrentDirectory,
  [in]                LPSTARTUPINFOW        lpStartupInfo,
  [out]               LPPROCESS_INFORMATION lpProcessInformation
);
lpApplicationName : 文件名
lpCommandLine : 命令行
lpProcessAttributes： 主进程安全描述符；是否可以被继承；不需要的话填NULL
lpThreadAttributes： 主线程安全描述符；是否可以被继承；不需要的话填NULL
bInheritHandles：    资源句柄，其他句柄是否可被继承，不可以的话填NULL
dwCreationFlags：    进程优先级以及创建标志位：https://learn.microsoft.com/en-us/windows/win32/procthread/process-creation-flags 默认填0；
常用如下：CREATE_NO_WINDOW：隐藏控制台;CREATE_SUSPENDED：挂起状态下运行DEBUG_PROCESS:调试模式启动CREATE_NEW_CONSOLE:创建新控制台
lpEnvironment：填NULL
lpCurrentDirectory：进程当前目录的完整路径；可以填NULL
lpStartupInfo:启动信息
typedef struct _STARTUPINFOW {
	DWORD   cb;				//整个结构的大小，需要提前设置
	LPWSTR  lpReserved;		//
	LPWSTR  lpDesktop;		//指定桌面的
	LPWSTR  lpTitle;		//控制台标题
	DWORD   dwX;			//位置信息
	DWORD   dwY;
	DWORD   dwXSize;		//尺寸信息
	DWORD   dwYSize;
	DWORD   dwXCountChars;	//控制台显示长宽
	DWORD   dwYCountChars;	//
	DWORD   dwFillAttribute;//控制台背景色RGB()
	DWORD   dwFlags;		//
	WORD    wShowWindow;	//显示模式
	WORD    cbReserved2;
	LPBYTE  lpReserved2;
	HANDLE  hStdInput;		//控制台输入句柄
	HANDLE  hStdOutput;		//控制台输出句柄
	HANDLE  hStdError;		//控制台错误句柄
} STARTUPINFOW, *LPSTARTUPINFOW;
lpProcessInformation:接受创建进程线程的信息
*/
/*
HANDLE OpenProcess(
  [in] DWORD dwDesiredAccess,
  [in] BOOL  bInheritHandle,
  [in] DWORD dwProcessId
);
dwDesiredAccess :PROCESS_ALL_ACCESS
bInheritHandle： 是否可被继承
dwProcessId：进程Id
*/
/*
HANDLE CreateToolhelp32Snapshot(
  [in] DWORD dwFlags,
  [in] DWORD th32ProcessID
);
dwFlags:
TH32CS_SNAPALL:快照所有的
TH32CS_SNAPHEAPLIST：快照堆
TH32CS_SNAPMODULE：快照模块
TH32CS_SNAPMODULE32：快照32位模块
TH32CS_SNAPPROCESS：快照进程
TH32CS_SNAPTHREAD： 快照线程
th32ProcessID： 进程Id
*/
/*
BOOL Process32First(
  [in]      HANDLE           hSnapshot,
  [in, out] LPPROCESSENTRY32 lppe
);
hSnapshot:快照句柄
LPPROCESSENTRY32 lppe：进程信息；
typedef struct tagPROCESSENTRY32
{
	DWORD   dwSize;
	DWORD   cntUsage;
	DWORD   th32ProcessID;          // this process
	ULONG_PTR th32DefaultHeapID;
	DWORD   th32ModuleID;           // associated exe
	DWORD   cntThreads;
	DWORD   th32ParentProcessID;    // this process's parent process
	LONG    pcPriClassBase;         // Base priority of process's threads
	DWORD   dwFlags;
	CHAR    szExeFile[MAX_PATH];    // Path
} PROCESSENTRY32;
*/
/*
HANDLE CreateThread(
  [in, optional]  LPSECURITY_ATTRIBUTES   lpThreadAttributes,
  [in]            SIZE_T                  dwStackSize,
  [in]            LPTHREAD_START_ROUTINE  lpStartAddress,
  [in, optional]  __drv_aliasesMem LPVOID lpParameter,
  [in]            DWORD                   dwCreationFlags,
  [out, optional] LPDWORD                 lpThreadId
);
lpThreadAttributes:安全描述符：填NULL；
dwStackSize：堆栈大小：填NULL:默认1M
lpStartAddress：线程回调函数
lpParameter：回调函数参数
dwCreationFlags：创建标志：0：线立即运行。CREATE_SUSPENDED：线程在挂起状态下创建，并且在调用 ResumeThread 函数之前不会运行。
lpThreadId：输出线程Id
返回值：线程句柄；
*/
VOID ThreadCallBack() {
	while (true) {
		std::cout << "线程回调函数" << std::endl;
		Sleep(1000);
	}
	
}
int main()
{
    //进程： 数据结构 PEB EPROCESS
    //线程列表：
    //主线程：
    //
    //
/*
	STARTUPINFOW starupinfo = { sizeof(STARTUPINFOW) };
	PROCESS_INFORMATION processInfo;
	BOOL bRet = CreateProcessW(L"C:\\Users\\89598\\source\\repos\\贪吃蛇\\Debug\\贪吃蛇.exe"\
		, NULL, NULL, NULL, 0, CREATE_NEW_CONSOLE, NULL, NULL,\
		&starupinfo, &processInfo);
	if (!bRet) {
		std::cout << "创建进程失败" << std::endl;
		return 0;
	}
	else {
		TerminateProcess(processInfo.hProcess, 0);
	//	TerminateThread(processInfo.hThread, 0);
	}
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, 14600);
	if (hProcess == NULL) {
		std::cout << "OpenProcess Error Code:" << GetLastError() << std::endl;
	}
	else {
		TerminateProcess(hProcess, 0);
		CloseHandle(hProcess);
	}
	CloseHandle(processInfo.hProcess);
	CloseHandle(processInfo.hThread);
*/
///*
	std::wcout.imbue(std::locale("chs"));
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	PROCESSENTRY32 processInfo{ sizeof(PROCESSENTRY32) };
	BOOL bRet = Process32First(hSnap, &processInfo);
	while (bRet) {
		std::wcout << processInfo.szExeFile << ":" << processInfo.th32ProcessID << std::endl;
		if (!wcscmp(processInfo.szExeFile, L"calc.exe")) {
			HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processInfo.th32ProcessID);
			TerminateProcess(hProcess, 0);
			CloseHandle(hProcess);
		}
		bRet = Process32Next(hSnap, &processInfo);
	}
	CloseHandle(hSnap);
//*/
	/*
	DWORD dwThreadId;
	HANDLE hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ThreadCallBack, NULL, 0, &dwThreadId);
	HANDLE hThread2 = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ThreadCallBack, NULL, 0, &dwThreadId);
	if(hThread != NULL){
		WaitForSingleObject(hThread, -1);
		WaitForSingleObject(hThread2, -1);
		CloseHandle(hThread);
	}
	
	system("pause");
    return 0;
	*/
}

