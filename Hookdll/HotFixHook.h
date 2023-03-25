#pragma once
struct hotfixHookNode {
	struct hotfixHookNode* next;
	PROC m_HookAddr = NULL;
	PROC m_HookTarget = NULL;
};
class HotFixHook {
public:
	HotFixHook();
	~HotFixHook();
	void SetHook(PROC fpWin32Api, PROC fpTarget);

	void InsertHookNode(PROC fpWin32Api, PROC fpTarget);

	void UnLoadHook(PROC fpWin32Api);

	void ReLoadAllHook();
private:
	hotfixHookNode* hooknode;
};

