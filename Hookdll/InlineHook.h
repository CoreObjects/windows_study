#pragma once
#include "framework.h"
struct HookNode {
	struct HookNode* next;
	char m_OldCode[5] = { 0 };
	char m_NewCode[5] = { 0 };
	PROC m_HookAddr = NULL;
	PROC m_HookTarget = NULL;
};
class InlineHook {
public:
	InlineHook();
	~InlineHook();
	void SetHook(PROC fpAddress, PROC fpTarget);
	void DeleteHook(PROC fpAddress);
	void UnLoadHook(PROC fpAddress);
	void ReLoadHook(PROC fpAddress);
	void DeleteAllHook();
	void UnLoadAllHook();
	void ReLoadAllHook();
private:
	HookNode* hooknode;
};
