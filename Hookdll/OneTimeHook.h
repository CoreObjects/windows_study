#pragma once

void InlineHookDevice();
void __stdcall UnHook(PROC fpWindowsApi);
void InlineHookOnceTime(LPVOID lpAddress, LPVOID lpTarget);