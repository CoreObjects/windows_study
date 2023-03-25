#pragma once
#include <iostream>
extern "C" { 
	__declspec(dllexport) void show(const char* szBuffer);
}