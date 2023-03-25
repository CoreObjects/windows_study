//计算机\HKEY_LOCAL_MACHINE\SOFTWARE\WOW6432Node\Microsoft\Windows\CurrentVersion\Uninstall\
//计算机\HKEY_LOCAL_MACHINE\SOFTWARE\WOW6432Node\Microsoft\Windows\CurrentVersion\Run

//计算机\HKEY_CURRENT_USER\SOFTWARE\Microsoft\Windows\CurrentVersion\Run\QQDisabled
#include <iostream>
#include <windows.h>
#include <Shlwapi.h>
#pragma comment(lib,"Shlwapi.lib")
/*
LSTATUS RegOpenKeyExA(
  [in]           HKEY   hKey,           //根
  [in, optional] LPCSTR lpSubKey,       //子项路径
  [in]           DWORD  ulOptions,      //填0
  [in]           REGSAM samDesired,     //权限：https://learn.microsoft.com/en-us/windows/win32/sysinfo/registry-key-security-and-access-rights
  [out]          PHKEY  phkResult       //返回值
);
*/
typedef struct _SOFTINFO {
    std::string szSoftName;       //名称
    std::string szSoftVer;        //软件版本号
    std::string szSoftVenRel;     //软件发布厂商
    std::string szSoftData;       //软件安装日期
    std::string szSoftInstallPath;  //软件安装路径
    std::string szSoftUnistallPath; //软件卸载路径
}SOFTINFO,*PSOFTINFO;
VOID ShowInstall(const char* szKey) {
    SOFTINFO stcSoftInfo;
    //打开注册表的Key
    HKEY key;
    HKEY hSubKey;
    char szSubKey[MAX_PATH]{ 0 };
    DWORD dwIndex = 0;
    DWORD dwBufferSize = 255;
    DWORD dwType = REG_SZ;
    DWORD dwSize = 255;
    RegOpenKeyExA(HKEY_LOCAL_MACHINE, szKey, 0, KEY_ALL_ACCESS, &key);
    while (SHEnumKeyExA(key, dwIndex++, szSubKey, &dwBufferSize) != ERROR_NO_MORE_ITEMS) {
        
        char szPath[MAX_PATH]{ 0 };
        sprintf_s(szPath, "%s\\%s", szKey, szSubKey);
        char szBuffer[MAX_PATH]{ 0 };
        if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, szPath, 0, KEY_ALL_ACCESS, &hSubKey) == ERROR_SUCCESS) {

			RegQueryValueExA(hSubKey, "DisplayName", NULL, &dwType, (LPBYTE)szBuffer, &dwSize);
            dwSize = 255;
            stcSoftInfo.szSoftName = szBuffer;
            if (stcSoftInfo.szSoftName.empty()) {
				dwBufferSize = 255;
				RegCloseKey(hSubKey);
                continue;
            }
			RegQueryValueExA(hSubKey, "DisplayVersion", NULL, &dwType, (LPBYTE)szBuffer, &dwSize);
			dwSize = 255;
            stcSoftInfo.szSoftVer = szBuffer;
			RegQueryValueExA(hSubKey, "InstallLocation", NULL, &dwType, (LPBYTE)szBuffer, &dwSize);
			dwSize = 255;
            stcSoftInfo.szSoftInstallPath = szBuffer;
			RegQueryValueExA(hSubKey, "Publisher", NULL, &dwType, (LPBYTE)szBuffer, &dwSize);
			dwSize = 255;
            stcSoftInfo.szSoftVenRel = szBuffer;
			RegQueryValueExA(hSubKey, "UninstallString", NULL, &dwType, (LPBYTE)szBuffer, &dwSize);
			dwSize = 255;
            stcSoftInfo.szSoftUnistallPath = szBuffer;
			RegQueryValueExA(hSubKey, "InstallDate", NULL, &dwType, (LPBYTE)szBuffer, &dwSize);
			dwSize = 255;
            stcSoftInfo.szSoftData = szBuffer;
            dwBufferSize = 255;
			RegCloseKey(hSubKey);
			std::cout << "名称" << stcSoftInfo.szSoftName << std::endl;
			std::cout << "软件安装日期" << stcSoftInfo.szSoftData << std::endl;
			std::cout << "软件版本号" << stcSoftInfo.szSoftVer << std::endl;
			std::cout << "软件发布厂商" << stcSoftInfo.szSoftVenRel << std::endl;
			std::cout << "软件安装路径" << stcSoftInfo.szSoftInstallPath << std::endl;
			std::cout << "软件卸载路径" << stcSoftInfo.szSoftUnistallPath << std::endl;
            std::cout <<  std::endl;
        }
       
    }
}
int main()
{
    ShowInstall("SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall");
    system("pause");
}

