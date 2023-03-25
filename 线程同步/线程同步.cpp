// 线程同步.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <windows.h>
#include <iostream>
/*
1.原子操作
InterlockedAdd
InterlockedAnd
InterlockedOr
2.临界区，关键段：
CRITICAL_SECTION g_Cs;
InitializeCriticalSection(&g_Cs);
DeleteCriticalSection(&g_Cs);
EnterCriticalSection(&g_Cs);
LeaveCriticalSection(&g_Cs);
3.事件
WaitForSingleObject(g_hEvent, -1); 非触发态阻塞
ResetEvent(g_hEvent);   设为非触发态
SetEvent(g_hEvent);     设置为触发态
HANDLE CreateEventA(
  [in, optional] LPSECURITY_ATTRIBUTES lpEventAttributes,
  [in]           BOOL                  bManualReset,        //手动触发还是被动触发
  [in]           BOOL                  bInitialState,       //触发态还是非触发态
  [in, optional] LPCSTR                lpName
);
4.信号量：
HANDLE CreateSemaphoreW(
  [in, optional] LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
  [in]           LONG                  lInitialCount,       //初始计数
  [in]           LONG                  lMaximumCount,       //最大计数
  [in, optional] LPCWSTR               lpName
);
WaitForSingleObjectEx：
TURE：等待
ReleaseSemaphore：+1
WaitForSingleObjectEx(g_Semaphore, -1, TRUE);-1 小于0等，大于0进
5.互斥体
CreateMutex(NULL, FALSE, L"`");:阻止同名互斥体起来
FALSE:互斥体没起来
*/
HANDLE g_hEvent;
CRITICAL_SECTION g_Cs;
int g_nNumber = 0;
HANDLE g_Semaphore;
HANDLE g_hMutex;
void ThreadCallBack1(LPVOID) {
    WaitForSingleObject(g_hMutex, -1);
//    ResetEvent(g_hEvent);
    for (int i = 0; i < 2000000; i++)
    {
//      EnterCriticalSection(&g_Cs);
        g_nNumber++;
//       LeaveCriticalSection(&g_Cs);
//        InterlockedAdd((volatile LONG*)&g_nNumber, 1);
    }
//    SetEvent(g_hEvent);
    WaitForSingleObject(g_hMutex, -1);
}
void ThreadCallBack2(LPVOID) {
    WaitForSingleObject(g_hMutex,-1);
//	WaitForSingleObject(g_hEvent, -1);
//	ResetEvent(g_hEvent);
	for (int i = 0; i < 2000000; i++) {
//        EnterCriticalSection(&g_Cs);
        g_nNumber++;
//        LeaveCriticalSection(&g_Cs);
//        InterlockedAdd((volatile LONG*)&g_nNumber, -1);
	}
//    SetEvent(g_hEvent);
    ReleaseMutex(g_hMutex);
}

int main()
{
 //   InitializeCriticalSection(&g_Cs);
//    g_hEvent = CreateEvent(NULL, TRUE, TRUE, L"Event1");
    g_hMutex = CreateMutex(NULL, TRUE, L"`");
    ReleaseMutex(g_hMutex);
    HANDLE hThread1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadCallBack1, NULL, 0, NULL);
    HANDLE hThread2 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadCallBack2, NULL, 0, NULL);
    if (hThread1 && hThread2)
    {
		WaitForSingleObject(hThread2, -1);
		WaitForSingleObject(hThread1, -1);
    }
    std::cout << g_nNumber;
 //   DeleteCriticalSection(&g_Cs);
}


