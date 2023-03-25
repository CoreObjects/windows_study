// 剪切板.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <windows.h>
int main()
{
    const char* pStr = "This is a string!";
    BOOL bRet = OpenClipboard(NULL);
    if (bRet) {
        char* szBuffer = NULL;
        EmptyClipboard();
        HGLOBAL hGlobalClip = GlobalAlloc(GHND, strlen(pStr) + 1);
        if (hGlobalClip) {
			LPVOID lpBuffer = GlobalLock(hGlobalClip);
			if (lpBuffer != NULL) {
				memcpy(lpBuffer, pStr, strlen(pStr) + 1);
			}
			GlobalUnlock(hGlobalClip);
			SetClipboardData(CF_TIFF, hGlobalClip);
			CloseClipboard();
        }
        
    }
    system("pause");
    std::cout << "Hello World!\n";
}

