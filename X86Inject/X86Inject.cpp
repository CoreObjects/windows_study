

#include <iostream>
#include <windows.h>
#include <TlHelp32.h>

void RemoteInject(DWORD dwProcessId, CHAR* csDllPath) {
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
	if (hProcess != INVALID_HANDLE_VALUE) {
		LPVOID lpBuffer = VirtualAllocEx(hProcess, NULL, MAX_PATH, MEM_COMMIT, PAGE_READWRITE);
		if (lpBuffer) {
			SIZE_T dwRealWrite;
			BOOL bRet = WriteProcessMemory(hProcess, lpBuffer, csDllPath, strlen(csDllPath), &dwRealWrite);
			if (bRet) {
				HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, lpBuffer, 0, NULL);
				if (hThread) {
					WaitForSingleObject(hThread, -1);
					CloseHandle(hThread);
				}
			}
			VirtualFreeEx(hProcess, lpBuffer, 0, MEM_RELEASE);
		}
		CloseHandle(hProcess);
	}
}

void RemoteFreelibrary(DWORD dwProcessId, DWORD dwModule) {
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
	if (hProcess != INVALID_HANDLE_VALUE) {
		HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)FreeLibrary, (LPVOID)dwModule, 0, NULL);
		if (hThread) {
			WaitForSingleObject(hThread, -1);
			CloseHandle(hThread);
		}
		CloseHandle(hProcess);
	}
}

void Inject(DWORD dwProcessId, CHAR* csDllPath) {
	RemoteInject(dwProcessId, csDllPath);
	MODULEENTRY32 me32 = { sizeof(me32) };
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE32 | TH32CS_SNAPMODULE, dwProcessId);
	if (hSnap != INVALID_HANDLE_VALUE) {
		BOOL bRet = Module32First(hSnap, &me32);
		while (bRet) {
			if (!strcmp(csDllPath, (const char*)me32.szExePath)) {
				DWORD dwModeBase = (DWORD)me32.modBaseAddr;
				DWORD dwModSize = me32.modBaseSize;
				char* szBuffer = new char[me32.modBaseSize];
				HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
				if (hProcess != INVALID_HANDLE_VALUE) {
					ReadProcessMemory(hProcess, (LPVOID)dwModeBase, szBuffer, dwModSize, NULL);
					RemoteFreelibrary(dwProcessId, dwModeBase);
					LPVOID lpModAddr = VirtualAllocEx(hProcess, (LPVOID)dwModeBase, dwModSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
					WriteProcessMemory(hProcess, lpModAddr, szBuffer, dwModSize, NULL);

					CloseHandle(hProcess);
				}
				break;
			}
			bRet = Module32Next(hSnap, &me32);
		}
		CloseHandle(hSnap);
	}
}

int main(int argc, char* argv[]) {
	for (int i = 0; i < argc; i++) {
		//         std::cout << argv[i] << std::endl;
		     }
		if (argc != 4) {
			return 0;
		}
		if (argv[1][0] == '0') {
			DWORD dwProcessId = atol(argv[2]);
			CHAR csDllPath[MAX_PATH]{ 0 };
			memcpy(csDllPath, argv[3], strlen(argv[3]));
			RemoteInject(dwProcessId, csDllPath);
		}
		else if (argv[1][0] == '1') {
			DWORD dwProcessId = atol(argv[2]);
			DWORD dwModule = atol(argv[3]);
			RemoteFreelibrary(dwProcessId, dwModule);

		}
		else if (argv[1][0] == '2') {
			DWORD dwProcessId = atol(argv[2]);
			CHAR csDllPath[MAX_PATH]{ 0 };
			memcpy(csDllPath, argv[3], strlen(argv[3]));

			Inject(640, (char*)"C:\\Users\\89598\\source\\repos\\windows核心编程2\\Debug\\Hookdll.dll");

		}
	}
}

