// MyThreadPage.cpp: 实现文件
//

#include "pch.h"
#include "进程与线程作业一.h"
#include "MyThreadPage.h"
#include "afxdialogex.h"
#include <TlHelp32.h>
#include "WindowsAxy.h"

// MyThreadPage 对话框

IMPLEMENT_DYNAMIC(MyThreadPage, CDialogEx)

MyThreadPage::MyThreadPage(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

MyThreadPage::~MyThreadPage()
{
}

BOOL MyThreadPage::OnInitDialog() {
	CDialogEx::OnInitDialog();
	m_ThreadList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	CString csTitle;
	csTitle.Format(L"进程%s的线程列表", csProcessName.GetBuffer());
	SetWindowTextW(csTitle);
	m_ThreadList.InsertColumn(0, L"所属进程ID", 0, 100);
	m_ThreadList.InsertColumn(1, L"线程ID", 0, 100);
	m_ThreadList.InsertColumn(1, L"线程优先级", 0, 100);
	THREADENTRY32 te32;
//	std::list<THREADENTRY32> te32List;
// 	WindowsAxy::GetThreadInfoList(te32, dwProcessID);
 	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, dwProcessID);
	if (hSnap != INVALID_HANDLE_VALUE) {
		THREADENTRY32 te32{sizeof(te32)};
		BOOL bRet = Thread32First(hSnap, &te32);
		DWORD dwIndex = 0;
		while (bRet) {
			if (te32.th32OwnerProcessID == dwProcessID) {
				CString csPid;
				csPid.Format(L"%d", te32.th32OwnerProcessID);
				m_ThreadList.InsertItem(dwIndex, csPid);
				CString csTid;
				csTid.Format(L"%d", te32.th32ThreadID);
				m_ThreadList.SetItemText(dwIndex, 1, csTid);
				CString csPriority;
				csPriority.Format(L"%d", te32.tpBasePri);
				m_ThreadList.SetItemText(dwIndex, 2, csPriority);
				dwIndex++;
			}
			bRet = Thread32Next(hSnap, &te32);
		}
		CloseHandle(hSnap);
	}
	
	return TRUE;
}

void MyThreadPage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ThreadList);
}


BEGIN_MESSAGE_MAP(MyThreadPage, CDialogEx)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_32774, &MyThreadPage::OnThreadSuspend)
	ON_COMMAND(ID_32775, &MyThreadPage::OnThreadResume)
	ON_COMMAND(ID_32776, &MyThreadPage::OnKillThread)
END_MESSAGE_MAP()


// MyThreadPage 消息处理程序


void MyThreadPage::OnContextMenu(CWnd* /*pWnd*/, CPoint pt/*point*/) {
	// TODO: 在此处添加消息处理程序代码
	CMenu m_Menu;
	m_Menu.LoadMenuW(IDR_MENU2);
	CMenu* m_SubMenu = m_Menu.GetSubMenu(0);
	TrackPopupMenu(m_SubMenu->m_hMenu, TPM_CENTERALIGN, pt.x, pt.y, 0, m_hWnd, NULL);

}


void MyThreadPage::OnThreadSuspend() {
	// TODO: 在此添加命令处理程序代码
//	AfxMessageBox(L"挂起");
	int nPos = (int)m_ThreadList.GetFirstSelectedItemPosition() - 1;
	CString cstid = L"Tid:" + m_ThreadList.GetItemText(nPos, 1);
	DWORD dwTid = _ttoi(m_ThreadList.GetItemText(nPos, 1));
//	AfxMessageBox(cstid);
	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, dwTid);
	if (hThread != NULL) {
		::SuspendThread(hThread);
		CloseHandle(hThread);
	}
}


void MyThreadPage::OnThreadResume() {
	// TODO: 在此添加命令处理程序代码
	int nPos = (int)m_ThreadList.GetFirstSelectedItemPosition() - 1;
//	CString cstid = L"Tid:" + m_ThreadList.GetItemText(nPos, 1);
	DWORD dwTid = _ttoi(m_ThreadList.GetItemText(nPos, 1));
	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, dwTid);
	if (hThread != NULL) {
		BOOL bRet = ::ResumeThread(hThread);
		CloseHandle(hThread);
	}
}


void MyThreadPage::OnKillThread() {
	// TODO: 在此添加命令处理程序代码
	int nPos = (int)m_ThreadList.GetFirstSelectedItemPosition() - 1;
//	CString cstid = L"Tid:" + m_ThreadList.GetItemText(nPos, 1);
	DWORD dwTid = _ttoi(m_ThreadList.GetItemText(nPos, 1));
	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, dwTid);
	if (hThread != NULL) {
		BOOL bRet = ::TerminateThread(hThread, 0);
		CloseHandle(hThread);
	}

}
