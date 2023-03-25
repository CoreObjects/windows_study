#include "pch.h"
#include "WindowsAxy.h"

BOOL WindowsAxy::CreateProcessAxy(TCHAR* buffer, PROCESS_INFORMATION* pi)
{
	TCHAR* szCommandLine = buffer;//"C:\\Users\\Core_Objects\\Desktop\\CodeinEX.exe";
	STARTUPINFO si = { 0 }; /*= { sizeof(si); }*/
	si.cb = sizeof(si);
	BOOL bRet = ::CreateProcess(
		NULL,//不在此指定可执行文件的文件名
		szCommandLine, //命令行参数
		NULL,//默认进程的安全性
		NULL,//默认线程的安全性
		FALSE,//指定当前进程内的句柄不可以被子进程继承
		CREATE_NEW_CONSOLE,//为新进程创建一个新的控制台窗口
		NULL,//使用本进程的环境变量
		NULL,//使用本进程的驱动器和目录
		&si,
		pi);
	return bRet;
}

BOOL WindowsAxy::CreateProcessAxy(TCHAR* buffer)
{
	PROCESS_INFORMATION pi;
	TCHAR* szCommandLine = buffer;//"C:\\Users\\Core_Objects\\Desktop\\CodeinEX.exe";
	STARTUPINFO si = { 0 }; /*= { sizeof(si); }*/
	si.cb = sizeof(si);
	BOOL bRet = ::CreateProcess(
		NULL,//不在此指定可执行文件的文件名
		szCommandLine, //命令行参数
		NULL,//默认进程的安全性
		NULL,//默认线程的安全性
		FALSE,//指定当前进程内的句柄不可以被子进程继承
		CREATE_NEW_CONSOLE,//为新进程创建一个新的控制台窗口
		NULL,//使用本进程的环境变量
		NULL,//使用本进程的驱动器和目录
		&si,
		&pi);
	if (bRet)
	{
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
	return bRet;
}

BOOL WindowsAxy::CreateProcessAdmAxy(TCHAR* buffer)
{
	SHELLEXECUTEINFO sei = { sizeof(sei) };
	sei.lpVerb = _T("runas");
	sei.lpFile = buffer;
	sei.nShow = SW_SHOWNORMAL;
	return ShellExecuteEx(&sei);
}

std::list<std::string> WindowsAxy::GetProcessList(void)
{
	std::list<std::string> strlist;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pi;
	pi.dwSize = sizeof(PROCESSENTRY32);
	BOOL bRet = Process32First(hSnapshot, &pi);
	std::string str;
	while (bRet)
	{
		str = wchar_tToString(pi.szExeFile) + ": PID = " + std::to_string(pi.th32ProcessID);
		strlist.push_back(str);
		bRet = Process32Next(hSnapshot, &pi);
	}
	CloseHandle(hSnapshot);
	return strlist;
}

std::list<PROCESSENTRY32> WindowsAxy::GetProcessInfoList(void)
{
	std::list<PROCESSENTRY32> ProcessInfolist;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pi;
	pi.dwSize = sizeof(PROCESSENTRY32);
	BOOL bRet = Process32First(hSnapshot, &pi);
	while (bRet)
	{
		ProcessInfolist.push_back(pi);
		bRet = Process32Next(hSnapshot, &pi);
	}
	CloseHandle(hSnapshot);
	return ProcessInfolist;
}

void WindowsAxy::GetProcessInfoList(std::unordered_map<DWORD, std::string>& Pid_name)
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pi;
	pi.dwSize = sizeof(PROCESSENTRY32);
	BOOL bRet = Process32First(hSnapshot, &pi);
	std::string name;
	DWORD pid;
	while (bRet)
	{
		name = wchar_tToString(pi.szExeFile);
		pid = pi.th32ProcessID;
		Pid_name.insert(make_pair(pid, name));
		bRet = Process32Next(hSnapshot, &pi);
	}
	CloseHandle(hSnapshot);
}

BOOL WindowsAxy::GetProcessInfoList(std::unordered_map<std::string, DWORD>& Name_Pid)
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pi;
	pi.dwSize = sizeof(PROCESSENTRY32);
	BOOL bRet = Process32First(hSnapshot, &pi);
	std::string name;
	DWORD pid;
	while (bRet)
	{
		name = wchar_tToString(pi.szExeFile);
		pid = pi.th32ProcessID;
		Name_Pid.insert(make_pair(name, pid));
		bRet = Process32Next(hSnapshot, &pi);
	}
	CloseHandle(hSnapshot);
	return TRUE;
}

BOOL WindowsAxy::KillProcess(DWORD pid)
{
	HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
	if (hProcess != NULL)
		return TerminateProcess(hProcess, 0);
	else return FALSE;
}

DWORD WindowsAxy::GetProcessId(const wchar_t* szProcessName)
{
	std::unordered_map<std::string, DWORD> Name_Pid;
	GetProcessInfoList(Name_Pid);
	auto ite = Name_Pid.find(wchar_tToString(szProcessName));
	if (ite != Name_Pid.end())
		return ite->second;
	else
		return 0xCCCCCCCC;
}

std::string WindowsAxy::GetProcessName(DWORD pid)
{
	std::unordered_map<DWORD, std::string> Pid_Name;
	GetProcessInfoList(Pid_Name);
	auto ite = Pid_Name.find(pid);
	if (ite != Pid_Name.end())
		return ite->second;
	else
		return "NO_Find";
}

BOOL WindowsAxy::GetThreadList(std::list<std::string>& ThreadList, DWORD nProcessId /*= NULL*/)
{
	ThreadList.clear();
	std::string str;
	DWORD dwPID = nProcessId;//遍历系统内部所有的线程
	HANDLE hThreadSnap = 0;
	//在创建快照的API里,第一个参数，代表着要获取什么TH32CS_SNAPTHRBAD代表获取系统内所有的线程
	hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (hThreadSnap == INVALID_HANDLE_VALUE)
	{
		//异常返回,代表线程快照，创建失败
		return FALSE;
	}
	THREADENTRY32 stcTe32 = { sizeof(THREADENTRY32) };
	Thread32First(hThreadSnap, &stcTe32);
	do {
		if (dwPID == NULL)
		{
			str = "线程ID:" + std::to_string(stcTe32.th32ThreadID);
			ThreadList.push_back(str);
		}
		else if (dwPID == stcTe32.th32OwnerProcessID)
		{
			str = "线程ID:" + std::to_string(stcTe32.th32ThreadID);
			ThreadList.push_back(str);
		}
	} while (Thread32Next(hThreadSnap, &stcTe32));
	return TRUE;
}

BOOL WindowsAxy::GetThreadInfoList(std::list<THREADENTRY32>& ThreadList, DWORD nProcessId /*= NULL*/)
{
	ThreadList.clear();
	DWORD dwPID = nProcessId;//遍历系统内部所有的线程
	HANDLE hThreadSnap = 0;
	//在创建快照的API里,第一个参数，代表着要获取什么TH32CS_SNAPTHRBAD代表获取系统内所有的线程
	hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (hThreadSnap == INVALID_HANDLE_VALUE)
	{
		//异常返回,代表线程快照，创建失败
		return FALSE;
	}
	THREADENTRY32 stcTe32 = { sizeof(THREADENTRY32) };
	Thread32First(hThreadSnap, &stcTe32);
	do {
		if (dwPID == NULL)
		{
			ThreadList.push_back(stcTe32);
		}
		else if (dwPID == stcTe32.th32OwnerProcessID)
		{
			ThreadList.push_back(stcTe32);
		}
	} while (Thread32Next(hThreadSnap, &stcTe32));
	return TRUE;
}

BOOL WindowsAxy::GetThreadIdArray(std::vector<DWORD>& ThreadIdVector, DWORD nProcessId /*= NULL*/)
{
	ThreadIdVector.clear();
	DWORD dwPID = nProcessId;//遍历系统内部所有的线程
	HANDLE hThreadSnap = 0;
	//在创建快照的API里,第一个参数，代表着要获取什么TH32CS_SNAPTHRBAD代表获取系统内所有的线程
	hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (hThreadSnap == INVALID_HANDLE_VALUE)
	{
		//异常返回,代表线程快照，创建失败
		return FALSE;
	}
	THREADENTRY32 stcTe32 = { sizeof(THREADENTRY32) };
	Thread32First(hThreadSnap, &stcTe32);
	do {
		if (dwPID == NULL)
		{
			ThreadIdVector.push_back(stcTe32.th32ThreadID);
		}
		else if (dwPID == stcTe32.th32OwnerProcessID)
		{
			ThreadIdVector.push_back(stcTe32.th32ThreadID);
		}
	} while (Thread32Next(hThreadSnap, &stcTe32));
	return TRUE;
}

BOOL WindowsAxy::SuspendThread(DWORD Tid)
{
	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, Tid);
	if (hThread)
	{
		::SuspendThread(hThread);
		CloseHandle(hThread);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL WindowsAxy::ResumeThread(DWORD Tid)
{
	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, Tid);
	if (hThread)
	{
		::ResumeThread(hThread);
		CloseHandle(hThread);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL WindowsAxy::KillThread(DWORD Tid)
{
	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, Tid);
	if (hThread)
	{
		::TerminateThread(hThread, 0);
		CloseHandle(hThread);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

HANDLE WindowsAxy::CreateThreadAxy(_Out_ DWORD& Tid, _In_ ThreadCallbackAxy ThreadProc, _In_ LPVOID lp /*= NULL*/, _In_ DWORD dwCreationFlags /*= 0*/, _In_ SIZE_T dwStackSize /*= 0*/, _In_opt_ LPSECURITY_ATTRIBUTES lpThreadAttributes /*= NULL*/)
{
	return CreateThread(lpThreadAttributes, dwStackSize, ThreadProc, lp, dwCreationFlags, &Tid);
}

HANDLE WindowsAxy::CreateThreadAxy(_In_ ThreadCallbackAxy ThreadProc, _In_ LPVOID lp /*= NULL*/, _In_ DWORD dwCreationFlags /*= 0*/, _In_ SIZE_T dwStackSize /*= 0*/, _In_opt_ LPSECURITY_ATTRIBUTES lpThreadAttributes /*= NULL*/)
{
	DWORD Tid;
	return CreateThread(lpThreadAttributes, dwStackSize, ThreadProc, lp, dwCreationFlags, &Tid);
}


HANDLE WindowsAxy::CreateRemoteThreadAXY(HANDLE hProcess, ThreadCallbackAxy ThreadProc, LPVOID lp /*= NULL*/)
{
	return CreateRemoteThread(hProcess, NULL, NULL, ThreadProc, lp, NULL, NULL);
}

BOOL WindowsAxy::WriteProcessMemory(DWORD ProcessId, LPVOID source, LPVOID target, SIZE_T size)
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessId);
	if (hProcess != NULL)
	{
		BOOL bret = ::WriteProcessMemory(hProcess, source, target, size, NULL);
		CloseHandle(hProcess);
		return bret;
	}
	else return FALSE;
}

BOOL WindowsAxy::WriteProcessMemory(HANDLE hProcess, LPVOID source, LPVOID target, SIZE_T size)
{
	return ::WriteProcessMemory(hProcess, source, target, size, NULL);
}

BOOL WindowsAxy::ReadProcessMemory(DWORD ProcessId, LPVOID source, LPVOID target, SIZE_T size)
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessId);
	if (hProcess != NULL)
	{
		return ::ReadProcessMemory(hProcess, source, target, size, NULL);
		CloseHandle(hProcess);
	}
	else return FALSE;
}

BOOL WindowsAxy::ReadProcessMemory(HANDLE hProcess, LPVOID source, LPVOID target, SIZE_T s)
{
	return ::ReadProcessMemory(hProcess, source, target, s, nullptr);
}

HANDLE WindowsAxy::WaitUntilTime(WORD Year, WORD Month, WORD Day, WORD Hour, WORD Minute, WORD Second, DWORD ms /*= 0*/)
{
	HANDLE g_WaitableTimer = CreateWaitableTimer(NULL, FALSE, L"TIME1");
	SYSTEMTIME st;
	FILETIME ftLocal, ftUTC;
	LARGE_INTEGER liUTC;
	st.wYear = Year;
	st.wMonth = Month;
	st.wDayOfWeek = 0;
	st.wDay = Day;
	st.wHour = Hour;
	st.wMinute = Minute;
	st.wSecond = Second;
	st.wMilliseconds = ms;
	SystemTimeToFileTime(&st, &ftLocal);
	LocalFileTimeToFileTime(&ftLocal, &ftUTC);
	liUTC.LowPart = ftUTC.dwLowDateTime;
	liUTC.HighPart = ftUTC.dwHighDateTime;
	//liUTC.QuadPart = -100000000;
	SetWaitableTimer(g_WaitableTimer, &liUTC, 0, NULL, NULL, FALSE);
	WaitForSingleObject(g_WaitableTimer, INFINITE);
	return g_WaitableTimer;
}

BOOL WindowsAxy::GetProcessModuleList(_Out_ std::list<MODULEENTRY32>& ModuleList, _In_ const wchar_t* ProcessName)
{
	DWORD ProcessID = GetProcessId(ProcessName);
	ModuleList.clear();
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, ProcessID);
	if (hSnapshot == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	MODULEENTRY32 me32;
	me32.dwSize = sizeof(MODULEENTRY32);
	BOOL bRet = Module32First(hSnapshot, &me32);
	while (bRet) {
		ModuleList.push_back(me32);

		bRet = Module32Next(hSnapshot, &me32);
	}
	return TRUE;
}

BOOL WindowsAxy::GetProcessModuleNameList(_Out_ std::list<std::string>& ModuleList, _In_ const wchar_t* ProcessName)
{
	DWORD ProcessID = GetProcessId(ProcessName);
	ModuleList.clear();
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, ProcessID);
	if (hSnapshot == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	MODULEENTRY32 me32;
	me32.dwSize = sizeof(MODULEENTRY32);
	BOOL bRet = Module32First(hSnapshot, &me32);
	while (bRet) {
		ModuleList.push_back(wchar_tToString(me32.szModule));
		bRet = Module32Next(hSnapshot, &me32);
	}
	return TRUE;
}

BOOL WindowsAxy::GetProcessModuleMap(_Out_ std::unordered_map<std::string, MODULEENTRY32>& ModuleMap, _In_ const wchar_t* ProcessName)
{
	DWORD ProcessID = GetProcessId(ProcessName);
	ModuleMap.clear();
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, ProcessID);
	if (hSnapshot == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	MODULEENTRY32 me32;
	me32.dwSize = sizeof(MODULEENTRY32);
	BOOL bRet = Module32First(hSnapshot, &me32);
	while (bRet) {
		ModuleMap.insert(make_pair(wchar_tToString(me32.szModule), me32));
		bRet = Module32Next(hSnapshot, &me32);
	}
	return TRUE;
}

DWORD WindowsAxy::GetModuleBaseAddr(const wchar_t* ModuleName, DWORD ProcessID)
{
	if (ProcessID == 0)
	{
		ProcessID = GetProcessId(ModuleName);
	}
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, ProcessID);
	if (hSnapshot == INVALID_HANDLE_VALUE)
	{
		return 0;
	}
	MODULEENTRY32 me32;
	me32.dwSize = sizeof(MODULEENTRY32);
	BOOL bRet = Module32First(hSnapshot, &me32);
	while (bRet) {
		//std::cout << WindowsAxy::wchar_tToString(me32.szModule) << std::endl;

		int nFlag = wcscmp(me32.szModule, ModuleName);
		if (nFlag == 0)
		{
			return (DWORD)me32.modBaseAddr;
		}
		bRet = Module32Next(hSnapshot, &me32);

	}
	return -1;
}

VOID WindowsAxy::RomoteInject(DWORD dwPid, TCHAR* szDllPath)
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	LPVOID pAddress = VirtualAllocEx(hProcess, NULL, 0x100, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	::WriteProcessMemory(hProcess, pAddress, szDllPath, (wcslen(szDllPath) + 1) * 2, NULL);
	HANDLE hThread = CreateRemoteThread(hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibraryW, pAddress, NULL, NULL);
	WaitForSingleObject(hThread, INFINITE);
	VirtualFreeEx(hProcess, pAddress, 0x100, MEM_RESERVE);
	CloseHandle(hProcess);
	CloseHandle(hThread);
	MessageBox(NULL, L"注入成功", L"提示", MB_OK);
}

VOID WindowsAxy::RomoteUnInject(DWORD dwPid, TCHAR* szDllName)
{
	std::unordered_map<std::string, MODULEENTRY32> ModuleMap;
	GetProcessModuleMap(ModuleMap, L"ProcessAdm.exe");
	auto ite = ModuleMap.find(WindowsAxy::wchar_tToString(szDllName));
	//ite->second.modBaseAddr
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	LPTHREAD_START_ROUTINE lpFreeLibrary = (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(L"kernel32.dll"), "FreeLibrary");
	HANDLE hThread = CreateRemoteThread(hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)lpFreeLibrary, ite->second.modBaseAddr, NULL, NULL);
	WaitForSingleObject(hThread, -1);
	CloseHandle(hProcess);
	CloseHandle(hThread);
	MessageBox(NULL, L"UnInject Success!", L"Success", MB_OK);
}

wchar_t* WindowsAxy::stringToWchar_t(std::string str)
{
	std::string temp = str;
	int len = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)temp.c_str(), -1, NULL, 0);
	wchar_t* wszUtf8 = new wchar_t[len + 1];
	memset(wszUtf8, 0, len * 2 + 2);
	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)temp.c_str(), -1, (LPWSTR)wszUtf8, len);
	return wszUtf8;
}

std::string WindowsAxy::wchar_tToString(const wchar_t* wchar) {
	std::string szDst;
	wchar_t* wText = (wchar_t*)wchar;
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, NULL, 0, NULL, FALSE);
	char* psText;
	psText = new char[dwNum];
	WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, psText, dwNum, NULL, FALSE);
	szDst = psText;
	delete[]psText;
	return szDst;
}

bool WindowsAxy::IsDebuggerPresentAXY()
{
	bool bRet = false;
	__asm {
		mov eax, fs: [0x30]
		mov al, byte ptr[eax + 2]
		mov bRet, al
	}
	return bRet;
}

std::wstring WindowsAxy::Utf8ToUnicode(const std::string& strUTF8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, NULL, 0);
	if (len == 0)
	{
		return L"";
	}

	wchar_t* pRes = new wchar_t[len];
	if (pRes == NULL)
	{
		return L"";
	}

	MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, pRes, len);
	pRes[len - 1] = L'\0';
	std::wstring result = pRes;
	delete[] pRes;

	return result;
}

std::string WindowsAxy::UnicodeToUtf8(const std::wstring& strUnicode)
{
	int len = WideCharToMultiByte(CP_UTF8, 0, strUnicode.c_str(), -1, NULL, 0, NULL, NULL);
	if (len == 0)
	{
		return "";
	}

	char* pRes = new char[len];
	if (pRes == NULL)
	{
		return "";
	}

	WideCharToMultiByte(CP_UTF8, 0, strUnicode.c_str(), -1, pRes, len, NULL, NULL);
	pRes[len - 1] = '\0';
	std::string result = pRes;
	delete[] pRes;

	return result;
}

std::wstring WindowsAxy::StringToWString(const std::string& str)
{
	int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
	if (len == 0)
	{
		return L"";
	}

	wchar_t* pRes = new wchar_t[len];
	if (pRes == NULL)
	{
		return L"";
	}
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, pRes, len);
	pRes[len - 1] = L'\0';
	std::wstring result = pRes;
	delete[] pRes;
	return result;
}

std::string WindowsAxy::WStringToString(const std::wstring& wstr)
{
	int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
	if (len == 0)
	{
		return "";
	}

	char* pRes = new char[len];
	if (pRes == NULL)
	{
		return "";
	}

	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, pRes, len, NULL, NULL);
	pRes[len - 1] = '\0';
	std::string result = pRes;
	delete[] pRes;

	return result;
}

std::string WindowsAxy::Utf8ToANSI(const std::string& strUtf8)
{
	return WStringToString(Utf8ToUnicode(strUtf8));
}

std::string WindowsAxy::ANSIToUtf8(const std::string& strANSI)
{
	return UnicodeToUtf8(StringToWString(strANSI));
}
