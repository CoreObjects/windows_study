#include "pch.h"
#include "InlineHook.h"

InlineHook::InlineHook() {
	hooknode = nullptr;
}

InlineHook::~InlineHook() {
	DeleteAllHook();
}

void InlineHook::SetHook(PROC fpAddress, PROC fpTarget) {
	HookNode* curHookNode;
	if (hooknode == nullptr) {
		hooknode = new HookNode;
		hooknode->next = nullptr;
		curHookNode = hooknode;
	}
	else {
		curHookNode = hooknode;
		while (curHookNode->next) {
			curHookNode = curHookNode->next;
		}
		curHookNode->next = new struct HookNode;
		curHookNode = curHookNode->next; 
		curHookNode->next = nullptr;
	}
	curHookNode->m_HookAddr = fpAddress;
	curHookNode->m_HookTarget = fpTarget;
	curHookNode->m_NewCode[0] = 0xE9;
	*(DWORD*)((char*)curHookNode->m_NewCode + 1) = (DWORD)fpTarget - ((DWORD)fpAddress + 5);
	DWORD dwOld;
	BOOL bRet = VirtualProtect(fpAddress, 5, PAGE_EXECUTE_READWRITE, &dwOld);
	if (bRet) {
		memcpy(curHookNode->m_OldCode, fpAddress, 5);
		memcpy(fpAddress, curHookNode->m_NewCode, 5);
	}
	VirtualProtect(fpAddress, 5, dwOld, &dwOld);
}

void InlineHook::DeleteHook(PROC fpAddress) {
	HookNode* curHookNode = hooknode;
	HookNode* preHookNode = hooknode;
	while (curHookNode) {
		if (curHookNode->m_HookAddr == fpAddress)
			break;
		curHookNode = curHookNode->next;
		preHookNode = curHookNode;
	}
	if (curHookNode != nullptr) {
		DWORD dwOld;
		BOOL bRet = VirtualProtect(fpAddress, 5, PAGE_EXECUTE_READWRITE, &dwOld);
		if (bRet) {
			memcpy(fpAddress, curHookNode->m_OldCode, 5);
		}
		VirtualProtect(fpAddress, 5, dwOld, &dwOld);
		preHookNode->next = curHookNode->next;
		delete curHookNode;
	}
}

void InlineHook::UnLoadHook(PROC fpAddress) {
	HookNode* curHookNode = hooknode;
	while (curHookNode) {
		if(curHookNode->m_HookAddr == fpAddress)
			break;
		curHookNode = curHookNode->next;
	}
	if (curHookNode != nullptr) {
		DWORD dwOld;
		BOOL bRet = VirtualProtect(fpAddress, 5, PAGE_EXECUTE_READWRITE, &dwOld);
		if (bRet) {
			memcpy(fpAddress, curHookNode->m_OldCode, 5);
		}
		VirtualProtect(fpAddress, 5, dwOld, &dwOld);
	}
	
}

void InlineHook::ReLoadHook(PROC fpAddress) {
	HookNode* curHookNode = hooknode;
	while (curHookNode) {
		if (curHookNode->m_HookAddr == fpAddress)
			break;
		curHookNode = curHookNode->next;
	}
	if (curHookNode != nullptr) {
		DWORD dwOld;
		BOOL bRet = VirtualProtect(fpAddress, 5, PAGE_EXECUTE_READWRITE, &dwOld);
		if (bRet) {
			memcpy(fpAddress, curHookNode->m_NewCode, 5);
		}
		VirtualProtect(fpAddress, 5, dwOld, &dwOld);
	}
	
}

void InlineHook::DeleteAllHook() {
	HookNode* curHookNode = hooknode;
	while (curHookNode) {
		DWORD dwOld;
		BOOL bRet = VirtualProtect(curHookNode->m_HookAddr, 5, PAGE_EXECUTE_READWRITE, &dwOld);
		if (bRet) {
			memcpy(curHookNode->m_HookAddr, curHookNode->m_OldCode, 5);
		}
		VirtualProtect(curHookNode->m_HookAddr, 5, dwOld, &dwOld);
		HookNode* tempHookNode = curHookNode;
		curHookNode = curHookNode->next;
		delete tempHookNode;
	}
	hooknode = nullptr;
}

void InlineHook::UnLoadAllHook() {
	HookNode* curHookNode = hooknode;
	while (curHookNode) {
		DWORD dwOld;
		BOOL bRet = VirtualProtect(curHookNode->m_HookAddr, 5, PAGE_EXECUTE_READWRITE, &dwOld);
		if (bRet) {
			memcpy(curHookNode->m_HookAddr, curHookNode->m_OldCode, 5);
		}
		VirtualProtect(curHookNode->m_HookAddr, 5, dwOld, &dwOld);
		curHookNode = curHookNode->next;
	}
}

void InlineHook::ReLoadAllHook() {
	HookNode* curHookNode = hooknode;
	while (curHookNode) {
		DWORD dwOld;
		BOOL bRet = VirtualProtect(curHookNode->m_HookAddr, 5, PAGE_EXECUTE_READWRITE, &dwOld);
		if (bRet) {
			memcpy(curHookNode->m_HookAddr, curHookNode->m_NewCode, 5);
		}
		VirtualProtect(curHookNode->m_HookAddr, 5, dwOld, &dwOld);
		curHookNode = curHookNode->next;
	}
}


