#include "pch.h"
#include "HotFixHook.h"

HotFixHook::HotFixHook() {
	hooknode = nullptr;
}

HotFixHook::~HotFixHook() {
	hotfixHookNode* curHookNode = hooknode;
	while (curHookNode) {
		UnLoadHook(curHookNode->m_HookAddr);
		hotfixHookNode* tempHookNode = curHookNode;
		curHookNode = curHookNode->next;
		delete tempHookNode;
	}
	hooknode = nullptr;
}

void HotFixHook::SetHook(PROC fpWin32Api, PROC fpTarget) {

	unsigned char ucShortJmp[2] = { 0xEB,0xF9 };
	unsigned char ucNewCode[5] = { 0xE9 };
	*(DWORD*)(ucNewCode + 1) = (DWORD)fpTarget - (DWORD)fpWin32Api;
	DWORD dwOld;
	BOOL bRet = VirtualProtect((LPVOID)((DWORD)fpWin32Api - 5), 7, PAGE_EXECUTE_READWRITE, &dwOld);
	if (bRet) {
		memcpy((LPVOID)((DWORD)fpWin32Api - 5), ucNewCode, 5);
		memcpy(fpWin32Api, ucShortJmp, 2);
		InsertHookNode(fpWin32Api, fpTarget);
	}
	VirtualProtect((LPVOID)((DWORD)fpWin32Api - 5), 7, dwOld, &dwOld);
}

void HotFixHook::InsertHookNode(PROC fpWin32Api, PROC fpTarget) {
	hotfixHookNode* curHookNode;
	if (hooknode == nullptr) {
		hooknode = new hotfixHookNode;
		hooknode->next = nullptr;
		curHookNode = hooknode;
	}
	else {
		curHookNode = hooknode;
		while (curHookNode->next) {
			curHookNode = curHookNode->next;
		}
		curHookNode->next = new struct hotfixHookNode;
		curHookNode = curHookNode->next;
		curHookNode->next = nullptr;
	}
	curHookNode->m_HookAddr = fpWin32Api;
	curHookNode->m_HookTarget = fpTarget;
}

void HotFixHook::UnLoadHook(PROC fpWin32Api) {
	unsigned char ucCodes[7] = { 0x90,0x90, 0x90, 0x90, 0x90, 0x8B, 0xFF };
	DWORD dwOld;
	BOOL bRet = VirtualProtect((LPVOID)((DWORD)fpWin32Api - 5), 7, PAGE_EXECUTE_READWRITE, &dwOld);
	if (bRet) {
		memcpy((LPVOID)((DWORD)fpWin32Api - 5), ucCodes, 7);
	}
	VirtualProtect((LPVOID)((DWORD)fpWin32Api - 5), 7, dwOld, &dwOld);
}

void HotFixHook::ReLoadAllHook() {
	hotfixHookNode* curHookNode = hooknode;
	unsigned char ucShortJmp[2] = { 0xEB,0xF9 };
	unsigned char ucNewCode[5] = { 0xE9 };
	while (curHookNode) {
		*(DWORD*)(ucNewCode + 1) = (DWORD)curHookNode->m_HookTarget - (DWORD)curHookNode->m_HookAddr;
		DWORD dwOld;
		BOOL bRet = VirtualProtect((LPVOID)((DWORD)curHookNode->m_HookAddr - 5), 7, PAGE_EXECUTE_READWRITE, &dwOld);
		if (bRet) {
			memcpy((LPVOID)((DWORD)curHookNode->m_HookAddr - 5), ucNewCode, 5);
			memcpy(curHookNode->m_HookAddr, ucShortJmp, 2);
		}
		VirtualProtect((LPVOID)((DWORD)curHookNode->m_HookAddr - 5), 7, dwOld, &dwOld);
		curHookNode = curHookNode->next;
	}
}
