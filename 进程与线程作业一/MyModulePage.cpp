// MyModulePage.cpp: 实现文件
//

#include "pch.h"
#include "进程与线程作业一.h"
#include "MyModulePage.h"
#include "afxdialogex.h"
#include <TlHelp32.h>

// MyModulePage 对话框

IMPLEMENT_DYNAMIC(MyModulePage, CDialogEx)

MyModulePage::MyModulePage(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG3, pParent)
{

}

MyModulePage::~MyModulePage()
{
}

BOOL MyModulePage::OnInitDialog() {
	CDialogEx::OnInitDialog();
	SetWindowTextW(L"模块列表");
	m_ModuleList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_ModuleList.InsertColumn(0, L"模块名", 0, 100);
	m_ModuleList.InsertColumn(1, L"模块路径", 0, 300);
	m_ModuleList.InsertColumn(2, L"模块基址", 0, 100);
	m_ModuleList.InsertColumn(3, L"模块大小", 0, 100);
	InitModuleList();
	return TRUE;
}

VOID MyModulePage::InitModuleList() {
	m_ModuleList.DeleteAllItems();
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE32 | TH32CS_SNAPMODULE, dwProcessId);
	if (hSnap != NULL) {
		MODULEENTRY32 me32 = { sizeof(me32) };
		BOOL bRet = Module32First(hSnap, &me32);
		DWORD dwIndex = 0;
		while(bRet) {
			m_ModuleList.InsertItem(dwIndex, me32.szModule);
			m_ModuleList.SetItemText(dwIndex, 1, me32.szExePath);
			CString csBaseAddr;
			csBaseAddr.Format(L"0x%p", me32.modBaseAddr);
			m_ModuleList.SetItemText(dwIndex, 2, csBaseAddr);
			CString csBaseSize;
			csBaseSize.Format(L"0x%08X", me32.modBaseSize);
			m_ModuleList.SetItemText(dwIndex, 3, csBaseSize);
			bRet = Module32Next(hSnap, &me32);
			dwIndex++;
		}
		CloseHandle(hSnap);
	}
}

void MyModulePage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ModuleList);
}


BEGIN_MESSAGE_MAP(MyModulePage, CDialogEx)
	ON_COMMAND(ID_32780, &MyModulePage::OnFreeLibrary)
	ON_COMMAND(ID_32781, &MyModulePage::OnFlush)
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()


// MyModulePage 消息处理程序


void MyModulePage::OnContextMenu(CWnd* pWnd, CPoint pt/*point*/) {
	// TODO: 在此处添加消息处理程序代码
	CMenu mMenu;
	mMenu.LoadMenu(IDR_MENU3);
	CMenu* mSubMenu = mMenu.GetSubMenu(0);
	mSubMenu->TrackPopupMenu(TPM_CENTERALIGN, pt.x, pt.y, this);
//	TrackPopupMenu(mSubMenu->m_hMenu, TPM_CENTERALIGN, pt.x, pt.y, 0, m_hWnd, NULL);
}


void MyModulePage::OnFreeLibrary() {
	// TODO: 在此添加命令处理程序代码
	int nPos = (int)m_ModuleList.GetFirstSelectedItemPosition() - 1;
	CString csBaseAddr = m_ModuleList.GetItemText(nPos, 2);
	csBaseAddr = csBaseAddr.Right(8);
	
	DWORD dwBaseAddr = wcstol(csBaseAddr, NULL, 16);
	HMODULE hModule = (HMODULE)dwBaseAddr;
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
	if (hProcess != INVALID_HANDLE_VALUE) {
		HANDLE hThread = CreateRemoteThread(hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)FreeLibrary, hModule, 0, NULL);
		if (hThread != NULL) {
			WaitForSingleObject(hThread, -1);
			CloseHandle(hThread);
//			AfxMessageBox(L"卸载成功！！");
		}
		CloseHandle(hProcess);
		InitModuleList();
	}
}


void MyModulePage::OnFlush() {
	// TODO: 在此添加命令处理程序代码
	InitModuleList();
}

