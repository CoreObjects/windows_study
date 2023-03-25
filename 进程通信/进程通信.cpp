// 进程通信.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <windows.h>
#define BUFF_SIZE 0x1000
TCHAR szName[] = L"obj";
/*
HANDLE CreateFileMappingA(
  [in]           HANDLE                hFile,
  [in, optional] LPSECURITY_ATTRIBUTES lpFileMappingAttributes,
  [in]           DWORD                 flProtect,
  [in]           DWORD                 dwMaximumSizeHigh,
  [in]           DWORD                 dwMaximumSizeLow,
  [in, optional] LPCSTR                lpName
);
hFile : 文件句柄，没有文件用INVALID_HANDLE_VALUE
lpFileMappingAttributes：安全描述符：NULL
flProtect:保护属性：
dwMaximumSizeHigh：高32位
dwMaximumSizeLow： 地32位
lpName：名字
LPVOID MapViewOfFile(
  [in] HANDLE hFileMappingObject,
  [in] DWORD  dwDesiredAccess,
  [in] DWORD  dwFileOffsetHigh,
  [in] DWORD  dwFileOffsetLow,
  [in] SIZE_T dwNumberOfBytesToMap
);
hFileMappingObject：文件映射句柄
dwDesiredAccess：权限,FILE_MAP_READ,FILE_MAP_WRITE,FILE_MAP_ALL_ACCESS,FILE_MAP_EXECUTE
dwFileOffsetHigh:文件偏移高32位填0；
dwFileOffsetLow：文件偏移低32位填0；
dwNumberOfBytesToMap：文件映射的大小
*/

int main()
{
    HANDLE hFileMap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, BUFF_SIZE, szName);
    if (hFileMap) {
       LPVOID lpBuffer = MapViewOfFile(hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, BUFF_SIZE);
       if (lpBuffer) {
           memcpy(lpBuffer, L"hello FileMap", 2 * wcslen(L"hello FileMap") + 2);
           system("pause");
           UnmapViewOfFile(lpBuffer);
       }
       CloseHandle(hFileMap);
    }
    

}

