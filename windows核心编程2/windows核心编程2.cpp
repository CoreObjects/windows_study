// windows核心编程2.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <windows.h>
#include <iostream>
#include <locale>
/*
HANDLE CreateFileA(
  [in]           LPCSTR                lpFileName,
  [in]           DWORD                 dwDesiredAccess,
  [in]           DWORD                 dwShareMode,
  [in, optional] LPSECURITY_ATTRIBUTES lpSecurityAttributes,
  [in]           DWORD                 dwCreationDisposition,
  [in]           DWORD                 dwFlagsAndAttributes,
  [in, optional] HANDLE                hTemplateFile
  lpFileName:文件名
  dwDesiredAccess: GENERIC_READ | GENERIC_WRITE（读写模式）
  dwShareMode: 0独占，FILE_SHARE_DELETE（共享删除），FILE_SHARE_READ（共享读）FILE_SHARE_WRITE（共享写）
  lpSecurityAttributes：安全描述符填NULL
  dwCreationDisposition：
  CREATE_ALWAYS 始终创建新文件。（存在就覆盖）
  CREATE_NEW仅 当该文件尚不存在时才创建一个新文件（存在就失败）
  OPEN_ALWAYS 始终打开文件。（不存在就创建）
  OPEN_EXISTING 仅当文件或设备存在时才打开该文件或设备。（不存在失败）
  TRUNCATE_EXISTING 打开一个文件并截断该文件，使其大小为零字节，仅当它存在时。如果指定的文件不存在，函数将失败，最后一个错误代码设置为 ERROR_FILE_NOT_FOUND ( 2) 。
  dwFlagsAndAttributes：属性标志填FILE_ATTRIBUTE_NORMAL 只读，只写等等。
  hTemplateFile：填NULL
);
*/
/*
BOOL WriteFile(
  [in]                HANDLE       hFile,
  [in]                LPCVOID      lpBuffer,
  [in]                DWORD        nNumberOfBytesToWrite,
  [out, optional]     LPDWORD      lpNumberOfBytesWritten,
  [in, out, optional] LPOVERLAPPED lpOverlapped
);
hFile:文件句柄
lpBuffer： 
nNumberOfBytesToWrite:写的长度
lpNumberOfBytesWritten：实际写的长度
lpOverlapped：NULL
*/
/*
DWORD GetFileSize(
  [in]            HANDLE  hFile,
  [out, optional] LPDWORD lpFileSizeHigh
);
hFile:句柄
lpFileSizeHigh：高32位
返回值：低八位
BOOL GetFileSizeEx(
  [in]  HANDLE         hFile,
  [out] PLARGE_INTEGER lpFileSize
);
PLARGE_INTEGER：
  typedef union _LARGE_INTEGER {
	struct {
		DWORD LowPart;
		LONG HighPart;
	} DUMMYSTRUCTNAME;
	struct {
		DWORD LowPart;
		LONG HighPart;
	} u;
	LONGLONG QuadPart;
} LARGE_INTEGER;
*/
/*
BOOL ReadFile(
  [in]                HANDLE       hFile,
  [out]               LPVOID       lpBuffer,
  [in]                DWORD        nNumberOfBytesToRead,
  [out, optional]     LPDWORD      lpNumberOfBytesRead,
  [in, out, optional] LPOVERLAPPED lpOverlapped
);
同WriteFile
*/
/*
DWORD SetFilePointer(
  [in]                HANDLE hFile,
  [in]                LONG   lDistanceToMove,
  [in, out, optional] PLONG  lpDistanceToMoveHigh,
  [in]                DWORD  dwMoveMethod
);
hFile :文件句柄
lDistanceToMove:偏移量
lpDistanceToMoveHigh：高32位
dwMoveMethod：偏移起点：FILE_BEGIN（头），FILE_CURRENT（当前），FILE_END（尾）

*/
/*
BOOL MoveFile(
  [in] LPCTSTR lpExistingFileName,
  [in] LPCTSTR lpNewFileName
);
lpExistingFileName---->lpNewFileName
*/
/*
BOOL CopyFileA(
  [in] LPCSTR lpExistingFileName,
  [in] LPCSTR lpNewFileName,
  [in] BOOL   bFailIfExists
);
[in] BOOL   bFailIfExists
如果此参数为 TRUE ，并且 lpNewFileName 指定的新文件已存在，则函数将失败。
如果此参数为 FALSE 且新文件已存在，则函数将覆盖现有文件并成功。
*/
/*
BOOL CreateDirectory(
  [in]           LPCTSTR               lpPathName,
  [in, optional] LPSECURITY_ATTRIBUTES lpSecurityAttributes
);
*/
/*
HANDLE FindFirstFileA(
  [in]  LPCSTR             lpFileName,
  [out] LPWIN32_FIND_DATAA lpFindFileData
);
typedef struct _WIN32_FIND_DATAA {
	DWORD dwFileAttributes; 
	FILETIME ftCreationTime; 
	FILETIME ftLastAccessTime; 
	FILETIME ftLastWriteTime; 
	DWORD nFileSizeHigh;   
	DWORD nFileSizeLow;
	DWORD dwReserved0;
	DWORD dwReserved1;
	_Field_z_ CHAR   cFileName[ MAX_PATH ];  //文件名
	_Field_z_ CHAR   cAlternateFileName[ 14 ];
} WIN32_FIND_DATAA, *PWIN32_FIND_DATAA, *LPWIN32_FIND_DATAA;
*/
/*
BOOL FindNextFileW(
  [in]  HANDLE             hFindFile,
  [out] LPWIN32_FIND_DATAW lpFindFileData
);
*/
/*
BOOL GetFileInformationByHandle(
  [in]  HANDLE                       hFile,
  [out] LPBY_HANDLE_FILE_INFORMATION lpFileInformation
);
typedef struct _BY_HANDLE_FILE_INFORMATION {
	DWORD dwFileAttributes;
	FILETIME ftCreationTime;
	FILETIME ftLastAccessTime;
	FILETIME ftLastWriteTime;
	DWORD dwVolumeSerialNumber;
	DWORD nFileSizeHigh;
	DWORD nFileSizeLow;
	DWORD nNumberOfLinks;
	DWORD nFileIndexHigh;
	DWORD nFileIndexLow;
} BY_HANDLE_FILE_INFORMATION, *PBY_HANDLE_FILE_INFORMATION, *LPBY_HANDLE_FILE_INFORMATION;
*/
/*
BOOL GetVolumeInformationW(
  [in, optional]  LPCWSTR lpRootPathName,
  [out, optional] LPWSTR  lpVolumeNameBuffer,
  [in]            DWORD   nVolumeNameSize,
  [out, optional] LPDWORD lpVolumeSerialNumber,
  [out, optional] LPDWORD lpMaximumComponentLength,
  [out, optional] LPDWORD lpFileSystemFlags,
  [out, optional] LPWSTR  lpFileSystemNameBuffer,
  [in]            DWORD   nFileSystemNameSize
);
*/
int main()
{
	
	//1.ASCII UNICODE
	//2.A,W,Ex
	HANDLE hFile = CreateFileA("C:\\Users\\89598\\source\\repos\\windows核心编程2\\fuck.ini",\
		GENERIC_READ | GENERIC_WRITE,\
		0, NULL,\
		OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		std::cout << "创建文件失败"<<std::endl;
	}

//	DWORD dwFliePointer = SetFilePointer(hFile, 0, NULL, FILE_END);

	CHAR szBuffer[MAX_PATH] = { 0 };
	DWORD dwFileLength = GetFileSize(hFile, NULL);
	DWORD dwNumberOfBytesWritten;


	BOOL bRetReadFile = ReadFile(hFile, szBuffer, dwFileLength, &dwNumberOfBytesWritten, NULL);
	if (!bRetReadFile) {
		std::cout << "读文件失败" << std::endl;
	}
	std::cout << szBuffer << std::endl;

	DWORD dwFliePointer = SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
	CHAR szBuffer1[MAX_PATH] = "2222";
	strcat_s(szBuffer, "1234");
	BOOL bRetWriteFile = WriteFile(hFile, szBuffer1, strlen(szBuffer1), &dwNumberOfBytesWritten, NULL);
	if (!bRetWriteFile) {
		std::cout << "写文件失败" << std::endl;
	}
	CloseHandle(hFile);
// 	MoveFile(L"C:\\Users\\89598\\source\\repos\\windows核心编程2\\fuck.ini",\
// 		L"C:\\Users\\89598\\source\\repos\\windows核心编程2\\rk1\\fuck.ini");
// 	DeleteFile(L"C:\\Users\\89598\\source\\repos\\windows核心编程2\\rk1\\fuck.ini");
	goto P1;
	CopyFile(L"C:\\Users\\89598\\source\\repos\\windows核心编程2\\fuck.ini",\
		L"C:\\Users\\89598\\source\\repos\\windows核心编程2\\rk1\\fuck.ini", TRUE);
	CreateDirectory(L"C:\\Users\\89598\\source\\repos\\windows核心编程2\\rk3", NULL);
	RemoveDirectory(L"C:\\Users\\89598\\source\\repos\\windows核心编程2\\rk2");//只能删除空目录
P1:
	WIN32_FIND_DATA FindFileData;
//	hFile = FindFirstFile(L"C:\\Users\\89598\\source\\repos\\windows核心编程2\\rk3\\*.*", &FindFileData);
	hFile = FindFirstFile(L".\\*.*", &FindFileData);
	if (INVALID_HANDLE_VALUE == hFile) {
		std::cout << "查找文件失败" << std::endl;
	}
	std::wcout.imbue(std::locale("chs"));
	do 
	{
		SYSTEMTIME systime;
		FileTimeToSystemTime(&FindFileData.ftCreationTime, &systime);
		std::wcout << systime.wYear << "-"<<systime.wMonth<<"-"<<systime.wDay<<"-"<<systime.wHour\
			<<":"<<systime.wMinute<<":"<<systime.wSecond << std::endl;
		std::wcout << FindFileData.cFileName << std::endl;
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			std::wcout << "目录" << std::endl;
		}
	} while (FindNextFile(hFile, &FindFileData));
	FindClose(hFile);
	//C:\Users\89598\source\repos\windows核心编程2
	std::cout << std::endl;
	hFile = CreateFileA("C:\\Users\\89598\\source\\repos\\windows核心编程2\\fuck.ini", \
		GENERIC_READ | GENERIC_WRITE, \
		0, NULL, \
		OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	BY_HANDLE_FILE_INFORMATION bhFileInfo;
	GetFileInformationByHandle(hFile, &bhFileInfo);
	CloseHandle(hFile);
	UINT ite = GetDriveType(L"C:");
	system("pause");
}


