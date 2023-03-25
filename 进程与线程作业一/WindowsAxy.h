#pragma once
#include <windows.h>
#include <stdio.h>
#include <TlHelp32.h>
#include <list>
#include <string>
#include <unordered_map>
#include <vector>
#include <xlocbuf>
#include "tchar.h"

class WindowsAxy
{
	using ThreadCallbackAxy = DWORD(WINAPI*)(LPVOID);
public:
	//进程相关
	static BOOL CreateProcessAxy(TCHAR*, PROCESS_INFORMATION*);
	static BOOL CreateProcessAxy(TCHAR*);
	static BOOL CreateProcessAdmAxy(TCHAR*);
	static std::list<std::string> GetProcessList(void);
	static std::list<PROCESSENTRY32> GetProcessInfoList(void);
	static void GetProcessInfoList(std::unordered_map<DWORD, std::string>&);
	static BOOL GetProcessInfoList(std::unordered_map<std::string, DWORD>&);
	static BOOL KillProcess(DWORD pid);
	static DWORD GetProcessId(const wchar_t* szProcessName);
	static std::string GetProcessName(DWORD pid);
	//线程相关
	static BOOL GetThreadList(std::list<std::string>& ThreadList, DWORD nProcessId = NULL);
	static BOOL GetThreadInfoList(std::list<THREADENTRY32>& ThreadList, DWORD nProcessId = NULL);
	static BOOL GetThreadIdArray(std::vector<DWORD>& ThreadIdVector, DWORD nProcessId = NULL);
	static BOOL SuspendThread(DWORD Tid);
	static BOOL ResumeThread(DWORD Tid);
	static BOOL KillThread(DWORD Tid);
	static HANDLE CreateThreadAxy(
		_Out_ DWORD& Tid,
		_In_ ThreadCallbackAxy ThreadProc,
		_In_ LPVOID lp = NULL,
		_In_ DWORD dwCreationFlags = 0,
		_In_ SIZE_T dwStackSize = 0,
		_In_opt_ LPSECURITY_ATTRIBUTES lpThreadAttributes = NULL);
	static HANDLE CreateThreadAxy(
		_In_ ThreadCallbackAxy ThreadProc,
		_In_ LPVOID lp = NULL,
		_In_ DWORD dwCreationFlags = 0,
		_In_ SIZE_T dwStackSize = 0,
		_In_opt_ LPSECURITY_ATTRIBUTES lpThreadAttributes = NULL);
	static HANDLE CreateRemoteThreadAXY(HANDLE hProcess, ThreadCallbackAxy ThreadProc, LPVOID lp = NULL);
	//内存相关
	static BOOL WriteProcessMemory(DWORD ProcessId, LPVOID source, LPVOID target, SIZE_T size);
	static BOOL WriteProcessMemory(HANDLE hProcess, LPVOID source, LPVOID target, SIZE_T size);
	static BOOL ReadProcessMemory(DWORD ProcessId, LPVOID source, LPVOID target, SIZE_T size);
	static BOOL ReadProcessMemory(HANDLE hProcess, LPVOID source, LPVOID target, SIZE_T s);
	//定时器相关
	static HANDLE WaitUntilTime(WORD Year, WORD Month, WORD Day,WORD Hour, WORD Minute, WORD Second, DWORD ms = 0);
	//模块相关
	static BOOL GetProcessModuleList(_Out_ std::list<MODULEENTRY32>& ModuleList, _In_ const wchar_t* ProcessName);
	static BOOL GetProcessModuleNameList(_Out_ std::list<std::string>& ModuleList, _In_ const wchar_t* ProcessName);
	static BOOL GetProcessModuleMap(_Out_ std::unordered_map<std::string, MODULEENTRY32>& ModuleMap, _In_ const wchar_t* ProcessName);
	static DWORD GetModuleBaseAddr(const wchar_t* ModuleName, DWORD ProcessID = 0);
	//注入相关
	static VOID RomoteInject(DWORD dwPid, TCHAR* szDllPath);
	static VOID RomoteUnInject(DWORD dwPid, TCHAR* szDllName);

	//工具函数
	static wchar_t* stringToWchar_t(std::string str);
	static std::string wchar_tToString(const wchar_t* wchar);
	static bool IsDebuggerPresentAXY();
	//编码转换相关
	static std::wstring Utf8ToUnicode(const std::string& strUTF8);
	static std::string UnicodeToUtf8(const std::wstring& strUnicode);
	static std::wstring StringToWString(const std::string& str);
	static std::string WStringToString(const std::wstring& wstr);
	static std::string Utf8ToANSI(const std::string& strUtf8);
	static std::string ANSIToUtf8(const std::string& strANSI);

};

