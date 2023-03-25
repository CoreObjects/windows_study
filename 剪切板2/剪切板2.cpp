// 剪切板2.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <windows.h>
int main()
{
    if (OpenClipboard(NULL)) {
        if (IsClipboardFormatAvailable(CF_TEXT)) {
            HGLOBAL hGlobalClip = GetClipboardData(CF_TEXT);
            char* szBuffer = NULL;
            szBuffer = (char*)GlobalLock(hGlobalClip);
            GlobalUnlock(hGlobalClip);
            std::cout << szBuffer << std::endl;
        }
        CloseClipboard();
    }
    system("pause");
    std::cout << "Hello World!\n";
   
}


