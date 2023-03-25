// TLS.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <windows.h>
VOID NTAPI TlsCallBack(PVOID DllHandle,DWORD Reason,PVOID Reserved) {

}
#pragma comment(linker,"/INCLUDE:__tls_used")

PIMAGE_TLS_CALLBACK tlsCall = TlsCallBack;

#pragma data_seg(".CRT$XLX")

/*
TLS 线程局部存储
TLS执行时机是在Main函数之前
*/

int main()
{
    std::cout << "Hello World!\n";
}

