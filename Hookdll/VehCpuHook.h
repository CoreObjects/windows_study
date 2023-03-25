#pragma once

#ifndef HOOkTARGETPROC

#define HOOkTARGETPROC
using HookTargetProc = LONG(NTAPI*)(struct _EXCEPTION_POINTERS* ExceptionInfo);


#endif
struct VEHCPUHOOKINFO {
	LPVOID HookAddress;
	HookTargetProc TargetAddress;
	struct VEHCPUHOOKINFO* next;
};

void InitCpuHook();