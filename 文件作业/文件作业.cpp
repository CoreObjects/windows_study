// 文件作业.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <windows.h>
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
bool CopyFileMy(const char* source, const char* target) {

    HANDLE hFile = CreateFileA(source, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        return false;
    }
    DWORD dwLength = GetFileSize(hFile, NULL);
    char* szBuffer = new char[dwLength + 1]{ 0 };
    DWORD dwRealLength;
    BOOL bret = ReadFile(hFile, szBuffer, dwLength, &dwRealLength, NULL);
    if (!bret) {
        return false;
    }
    CloseHandle(hFile);
    hFile = CreateFileA(target, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		return false;
	}
    bret = WriteFile(hFile, szBuffer, dwLength + 1, &dwRealLength, NULL);
	if (!bret) {
		return false;
	}
    return true;
}
bool MoveFlieMy(const char* source, const char* target) {
    if (!CopyFileMy(source, target)) {
        return false;
    }
    DeleteFileA(source);
    return true;
}
bool traverseFile(const char* dir) {
	WIN32_FIND_DATAA FindData{ 0 };
    char* szFinder = new char[strlen(dir) + 0xFF]{ 0 };
    strcpy_s(szFinder, strlen(dir)+1, dir);
    strcat_s(szFinder, strlen(szFinder) + 4, "*.*");
	HANDLE hFile = FindFirstFileA(szFinder, &FindData);
	do {
        if (strcmp(FindData.cFileName, ".") == 0 || strcmp(FindData.cFileName, "..") == 0) {
            continue;
        }
        std::cout << FindData.cFileName << std::endl;
        if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            memset(szFinder, 0, strlen(dir) + 0xFF);
            strcpy_s(szFinder, strlen(dir) + strlen(szFinder) + 1, dir);
            strcat_s(szFinder, strlen(FindData.cFileName) + strlen(szFinder) + 1, FindData.cFileName);
            strcat_s(szFinder, strlen(szFinder) + 2, "\\");
            traverseFile(szFinder);
        }
	} while (FindNextFileA(hFile, &FindData));
	FindClose(hFile);
    delete[]szFinder;
    return true;
}
bool DeleteDir(const char* dir) {
	WIN32_FIND_DATAA FindData{ 0 };
	char* szFinder = new char[strlen(dir) + 0xFF]{ 0 };
	strcpy_s(szFinder, strlen(dir) + 1, dir);
	strcat_s(szFinder, strlen(szFinder) + 4, "*.*");
	HANDLE hFile = FindFirstFileA(szFinder, &FindData);
	do {
		if (strcmp(FindData.cFileName, ".") == 0 || strcmp(FindData.cFileName, "..") == 0) {
			continue;
		}
		if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			memset(szFinder, 0, strlen(dir) + 0xFF);
			strcpy_s(szFinder, strlen(dir) + strlen(szFinder) + 1, dir);
			strcat_s(szFinder, strlen(FindData.cFileName) + strlen(szFinder) + 1, FindData.cFileName);
			strcat_s(szFinder, strlen(szFinder) + 2, "\\");
            DeleteDir(szFinder);
			char* szFile = new char[strlen(dir) + 0xFF]{ 0 };
			strcpy_s(szFile, strlen(dir) + 1, dir);
			strcat_s(szFile, strlen(szFinder) + strlen(FindData.cFileName), FindData.cFileName);
            RemoveDirectoryA(szFile);
            delete[]szFile;
        }
        else {
            char* szFile = new char[strlen(dir) + 0xFF]{ 0 };
			strcpy_s(szFile, strlen(dir) + 1, dir);
            strcat_s(szFile, strlen(szFinder) + strlen(FindData.cFileName), FindData.cFileName);
            DeleteFileA(szFile);
            delete[]szFile;
        }
	} while (FindNextFileA(hFile, &FindData));
	FindClose(hFile);
    RemoveDirectoryA(dir);
	return false;
}
bool EnCodeDir(const char* dir,char key) {
	WIN32_FIND_DATAA FindData{ 0 };
	char* szFinder = new char[strlen(dir) + 0xFF]{ 0 };
	strcpy_s(szFinder, strlen(dir) + 1, dir);
	strcat_s(szFinder, strlen(szFinder) + 4, "*.*");
	HANDLE hFile = FindFirstFileA(szFinder, &FindData);
	do {
		if (strcmp(FindData.cFileName, ".") == 0 || strcmp(FindData.cFileName, "..") == 0) {
			continue;
		}
		if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			memset(szFinder, 0, strlen(dir) + 0xFF);
			strcpy_s(szFinder, strlen(dir) + strlen(szFinder) + 1, dir);
			strcat_s(szFinder, strlen(FindData.cFileName) + strlen(szFinder) + 1, FindData.cFileName);
			strcat_s(szFinder, strlen(szFinder) + 2, "\\");
            EnCodeDir(szFinder,key);
		}
		else {
			char* szFile = new char[strlen(dir) + 0xFF]{ 0 };
			strcpy_s(szFile, strlen(dir) + 1, dir);
			strcat_s(szFile, strlen(szFinder) + strlen(FindData.cFileName), FindData.cFileName);
			std::cout << FindData.cFileName << std::endl;
			HANDLE hFile2 = CreateFileA(szFile, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile2 == INVALID_HANDLE_VALUE)
			{
				return false;
			}
			DWORD dwLength = GetFileSize(hFile2, NULL);
			DWORD dwRealReadLength, dwRealWriteLength;
			char* szBuffer = new char[dwLength];
			BOOL bRet = ReadFile(hFile2, szBuffer, dwLength, &dwRealReadLength, NULL);
			if (bRet == 0) {
				return false;
			}
			for (DWORD i = 0; i < dwRealReadLength; i++){
				szBuffer[i] ^= key;
			}
			SetFilePointer(hFile2, 0, NULL, FILE_BEGIN);
			bRet = WriteFile(hFile2, szBuffer, dwRealReadLength, &dwRealWriteLength, NULL);
			if (bRet == 0) {
				return false;
			}
			CloseHandle(hFile2);
			delete[]szBuffer;
			delete[]szFile;
		}
	} while (FindNextFileA(hFile, &FindData));
	FindClose(hFile);
	delete[]szFinder;
	return true;
}
int main()
{
//     MoveFlieMy("C:\\Users\\89598\\source\\repos\\windows核心编程2\\文件作业\\cobj1\\1111.txt",\
//         "C:\\Users\\89598\\source\\repos\\windows核心编程2\\文件作业\\cobj2\\1111.txt");
    bool bret = EnCodeDir("C:\\Users\\89598\\source\\repos\\windows核心编程2\\文件作业\\cobj2\\",0xCC);
    system("pause");
    std::cout << "Hello World!\n";
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
