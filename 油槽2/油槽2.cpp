// 油槽2.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <windows.h>
int main()
{
    HANDLE hMailSlot = CreateFile(L"\\\\.\\mailslot\\Communication",\
        GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    char szBuffer[] = "Hello World!";
    DWORD dwRealRead;
    WriteFile(hMailSlot, szBuffer, 13, &dwRealRead, NULL);
    CloseHandle(hMailSlot);
    system("pause");
/*    std::cout << "Hello World!\n";*/
}

