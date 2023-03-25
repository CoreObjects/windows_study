
// 进程与线程作业一Dlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "进程与线程作业一.h"
#include "进程与线程作业一Dlg.h"
#include "afxdialogex.h"
#include "MyThreadPage.h"
#include "psapi.h"
#include "MyInjectShellCode.h"
#include "MyModulePage.h"
#include <TlHelp32.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)

END_MESSAGE_MAP()


// C进程与线程作业一Dlg 对话框



C进程与线程作业一Dlg::C进程与线程作业一Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MY_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void C进程与线程作业一Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ProcessList);
}

BEGIN_MESSAGE_MAP(C进程与线程作业一Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &C进程与线程作业一Dlg::OnRclickList1)
	ON_COMMAND(ID_32771, &C进程与线程作业一Dlg::On32771)
	ON_COMMAND(ID_32773, &C进程与线程作业一Dlg::OnFlush)
	ON_COMMAND(ID_32772, &C进程与线程作业一Dlg::OnShowThreadList)
	ON_COMMAND(ID_32777, &C进程与线程作业一Dlg::OnInjectShellCode)
	ON_COMMAND(ID_32778, &C进程与线程作业一Dlg::OnRemoteInjectLibrary)
	ON_COMMAND(ID_32779, &C进程与线程作业一Dlg::OnShowModuleList)
END_MESSAGE_MAP()


// C进程与线程作业一Dlg 消息处理程序

BOOL C进程与线程作业一Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	SetWindowTextW(L"进程管理");
	InitListControl();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void C进程与线程作业一Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void C进程与线程作业一Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR C进程与线程作业一Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void C进程与线程作业一Dlg::InitListControl() {
	CHeaderCtrl* pHeaderCtrl = m_ProcessList.GetHeaderCtrl();
	if (pHeaderCtrl) {
		int n = pHeaderCtrl->GetItemCount();
		if (n == 0) {
			m_ProcessList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
			m_ProcessList.InsertColumn(0, L"进程名", 0, 100);
			m_ProcessList.InsertColumn(1, L"进程ID", 0, 50);
			m_ProcessList.InsertColumn(2, L"父进程ID", 0, 60);
			m_ProcessList.InsertColumn(3, L"线程数", 0, 50);
			m_ProcessList.InsertColumn(4, L"优先级", 0, 50);
			m_ProcessList.InsertColumn(5, L"进程路径", 0, 250);
			m_ProcessList.InsertColumn(6, L"文件创建时间", 0, 180);
		}
	}
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	PROCESSENTRY32 pe32{ sizeof(PROCESSENTRY32) };
	BOOL bRet = Process32First(hSnap, &pe32);
	DWORD dwIndex = 0;
	while (bRet) {
		CString csProcessId, csParentProcessID,csThreadcnt,csPriority,csTime;
		csParentProcessID.Format(L"%d", pe32.th32ParentProcessID);
		csProcessId.Format(L"%d", pe32.th32ProcessID);
		csThreadcnt.Format(L"%d", pe32.cntThreads);
		csPriority.Format(L"%d", pe32.pcPriClassBase);
		
		m_ProcessList.InsertItem(dwIndex, pe32.szExeFile, 0);
		m_ProcessList.SetItemText(dwIndex, 1, csProcessId);
		m_ProcessList.SetItemText(dwIndex, 2, csParentProcessID);
		m_ProcessList.SetItemText(dwIndex, 3, csThreadcnt);
		m_ProcessList.SetItemText(dwIndex, 4, csPriority);
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
		if (hProcess != INVALID_HANDLE_VALUE) {
			WCHAR wcsFilePath[MAX_PATH]{ 0 };
			GetModuleFileNameExW(hProcess, NULL, wcsFilePath, MAX_PATH);
			m_ProcessList.SetItemText(dwIndex, 5, wcsFilePath);
			WIN32_FIND_DATAW FindFileData;
			FindFirstFile(wcsFilePath, &FindFileData);
			SYSTEMTIME systime;
			FileTimeToSystemTime(&FindFileData.ftCreationTime, &systime);
			csTime.Format(L"%d-%d-%d %02d:%02d:%02d", systime.wYear, systime.wMonth, systime.wDay, systime.wHour, systime.wMinute, systime.wSecond);
			m_ProcessList.SetItemText(dwIndex, 6, csTime);
			CloseHandle(hProcess);
		}
		
		dwIndex++;
		bRet = Process32Next(hSnap, &pe32);
	}
}



void C进程与线程作业一Dlg::OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult) {
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	HMENU hMenu = LoadMenuW((HINSTANCE)GetWindowLongPtr(m_hWnd, GWLP_HINSTANCE), (LPCTSTR)MAKEINTRESOURCEW(IDR_MENU1));
	HMENU hSubMenu = GetSubMenu(hMenu, 0);
	POINT pt;
	GetCursorPos(&pt);
	::TrackPopupMenu(hSubMenu, TPM_CENTERALIGN, pt.x, pt.y, 0, m_hWnd, NULL);
	*pResult = 0;
}


void C进程与线程作业一Dlg::On32771() {
	// TODO: 在此添加命令处理程序代码
	int nPos = (int)m_ProcessList.GetFirstSelectedItemPosition();
	nPos -= 1;
	CString csPID = m_ProcessList.GetItemText(nPos, 1);
	DWORD dwPid = _ttoi(csPID);
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	TerminateProcess(hProcess, 0);
	m_ProcessList.DeleteAllItems();
	InitListControl();
}


void C进程与线程作业一Dlg::OnFlush() {
	// TODO: 在此添加命令处理程序代码
	m_ProcessList.DeleteAllItems();
	InitListControl();
}


void C进程与线程作业一Dlg::OnShowThreadList() {
	// TODO: 在此添加命令处理程序代码
	MyThreadPage page;
	int nPos = (int)m_ProcessList.GetFirstSelectedItemPosition();
	nPos -= 1;
	CString csPID = m_ProcessList.GetItemText(nPos, 1);
	page.dwProcessID = _ttoi(csPID);
	page.csProcessName = m_ProcessList.GetItemText(nPos, 0);
	page.DoModal();
}





void C进程与线程作业一Dlg::OnInjectShellCode() {
	// TODO: 在此添加命令处理程序代码
	MyInjectShellCode page;
	int nPos = (int)m_ProcessList.GetFirstSelectedItemPosition() - 1;
	CString csPID = m_ProcessList.GetItemText(nPos, 1);
	page.dwProcessId = _ttoi(csPID);
	page.DoModal();

}
/*
explicit CFileDialog(
	BOOL bOpenFileDialog,
	LPCTSTR lpszDefExt = NULL,
	LPCTSTR lpszFileName = NULL,
	DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
	LPCTSTR lpszFilter = NULL,
	CWnd* pParentWnd = NULL,
	DWORD dwSize = 0,
	BOOL bVistaStyle = TRUE);

	bOpenFileDialog 是否打开文件对话框
	lpszDefExt：默认文件扩展名，填NULL
	lpszFileName：“文件”框中的文件名
	dwFlags：
	lpszFilter：文件过滤器：L"文本文件|*.txt|AllFiles|*.*||"
*/
BOOL C进程与线程作业一Dlg::RemoteInject(DWORD dwProcessId, LPCTSTR szPath) {
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
	//注入远程线程回调函数参数
	if (hProcess == INVALID_HANDLE_VALUE) {
		return FALSE;
	}
	LPVOID lpBuffer = VirtualAllocEx(hProcess, NULL, MAX_PATH, MEM_COMMIT, PAGE_READWRITE);
	if (lpBuffer == NULL) {
		return FALSE;
	}
	SIZE_T dwWriteSize;
	BOOL bRet = WriteProcessMemory(hProcess, lpBuffer, szPath, (wcslen(szPath) + 1) * 2, &dwWriteSize);
	if (bRet == FALSE) {
		return FALSE;
	}
	HANDLE hThread = CreateRemoteThread(hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibraryW, lpBuffer, 0, NULL);
	if (hThread != NULL) {
		WaitForSingleObject(hThread, -1);
		CloseHandle(hThread);
	}
	VirtualFreeEx(hProcess, lpBuffer, 0, MEM_RELEASE);
	CloseHandle(hProcess);
	return TRUE;
}
void C进程与线程作业一Dlg::OnRemoteInjectLibrary() {
	// TODO: 在此添加命令处理程序代码
	CFileDialog fileDlg(TRUE, NULL, L"选择dll", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"动态链接库|*.dll||");
	fileDlg.DoModal();
	CString csFilePath = fileDlg.GetPathName();
	int nPos = (int)m_ProcessList.GetFirstSelectedItemPosition() - 1;
	DWORD dwProcessId = _ttoi(m_ProcessList.GetItemText(nPos, 1));
	BOOL bRet = RemoteInject(dwProcessId, csFilePath.GetBuffer());
	if (bRet) {
		AfxMessageBox(L"注入成功！！");
	}
	else {
		AfxMessageBox(L"注入失败！！");
	}
}


void C进程与线程作业一Dlg::OnShowModuleList() {
	// TODO: 在此添加命令处理程序代码
	MyModulePage page;
	int nPos = (int)m_ProcessList.GetFirstSelectedItemPosition() - 1;
	CString csPID = m_ProcessList.GetItemText(nPos, 1);
	page.dwProcessId = _ttoi(csPID);
	page.DoModal();
}

