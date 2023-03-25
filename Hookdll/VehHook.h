#pragma once



using HookTargetProc = LONG(NTAPI*)(struct _EXCEPTION_POINTERS* ExceptionInfo);


struct VEH_HOOK {
	LPVOID HookAddress;
	HookTargetProc TargetAddress;
	UCHAR OldCode;
	struct VEH_HOOK* next;
};

struct VEHCPUHOOK {
	LPVOID lpHookAddress;
	HookTargetProc TargetAddress;
};
using HOOKNODE = struct VEH_HOOK;
using CPUHOOKINFO = struct VEHCPUHOOK;
void InitVehHook();

void SetVehHook(LPVOID lpHookAddress, HookTargetProc lpTarget);

void SetVehCpuHook(LPVOID lpHookAddress, HookTargetProc lpTarget);

void SetThreadHook(HANDLE hThread);

void DeleteAllHook();



