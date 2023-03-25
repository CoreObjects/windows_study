// 异常处理.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <windows.h>
/*
结构化异常SEH；


// . . .
	__try {
		// guarded code
	}
	__except ( /* filter expression *//* ) {
	// termination code
	}
	// . . .
    EXCEPTION_CONTINUE_EXECUTION:异常被消除。在发生异常的位置继续执行
    EXCEPTION_CONTINUE_SEARCH:无法识别异常。处理完交给别的异常处理程序去干
    EXCEPTION_EXECUTE_HANDLER：自己处理完，跳过try
	int a = 0;
	int b = 0;
	__try {

		int c = a / b;
		std::cout << c << std::endl;
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		b = 1;
		std::cout << b << std::endl;
	}
*/
/*
* 向量化异常VEH；
VEH：添加异常
PVOID AddVectoredExceptionHandler(
  ULONG                       First,
  PVECTORED_EXCEPTION_HANDLER Handler
);
First : 0最后调用，非0第一个调用
Handler：异常处理程序地址
返回值：异常处理句柄
EXCEPTION_CONTINUE_EXECUTION:异常被消除。在发生异常的位置继续执行
EXCEPTION_CONTINUE_SEARCH:无法识别异常。处理完交给别的异常处理程序去干
EXCEPTION_EXECUTE_HANDLER：无效等同与EXCEPTION_CONTINUE_SEARCH
*/
LONG NTAPI VEH_Fun(struct _EXCEPTION_POINTERS* ExceptionInfo){
	DWORD dwCode = ExceptionInfo->ExceptionRecord->ExceptionCode;
	if (dwCode == EXCEPTION_BREAKPOINT) {
		std::cout << "int 3异常" << std::endl;
		ExceptionInfo->ContextRecord->Eip += 1;
		return EXCEPTION_CONTINUE_EXECUTION;
	}
	return EXCEPTION_CONTINUE_SEARCH;
}
int main()
{
	
	CONTEXT ctx;
	ctx.ContextFlags = CONTEXT_ALL;
// 	GetThreadContext(GetCurrentThread(), &ctx);
// 	ctx.Eip = (DWORD)VEH_Fun;
// 	SetThreadContext(GetCurrentThread(), &ctx);

	LPVOID lpHandle = AddVectoredExceptionHandler(1, VEH_Fun);
	__asm int 3
    std::cout << "Hello World!\n";

    system("pause");
    return 0;
}


