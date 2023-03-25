
// MyWndDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MyWnd.h"
#include "MyWndDlg.h"
#include "afxdialogex.h"
#include <TlHelp32.h>
#include "psapi.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx {
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
public:
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX) {
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMyWndDlg 对话框



CMyWndDlg::CMyWndDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MYWND_DIALOG, pParent) {
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyWndDlg::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_WindowList);
}

BEGIN_MESSAGE_MAP(CMyWndDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_Menu, &CMyWndDlg::OnMenuFlush)
	ON_COMMAND(ID_32772, &CMyWndDlg::OnExitProcess)
	ON_COMMAND(ID_32773, &CMyWndDlg::OnShowWindow)
	ON_COMMAND(ID_32774, &CMyWndDlg::OnHideWindow)
	ON_COMMAND(ID_32775, &CMyWndDlg::OnMaxWindow)
	ON_COMMAND(ID_32776, &CMyWndDlg::OnMinWindow)
END_MESSAGE_MAP()


// CMyWndDlg 消息处理程序

BOOL CMyWndDlg::OnInitDialog() {
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr) {
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty()) {
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_ImgList.Create(18, 18, ILC_MASK | ILC_COLOR32, 0, 1);
	
	m_WindowList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_WindowList.InsertColumn(0, L"窗口名", 0, 300);
	m_WindowList.InsertColumn(1, L"运行状态", 0, 60);
	m_WindowList.InsertColumn(2, L"进程ID", 0, 50);
	m_WindowList.InsertColumn(3, L"窗口句柄", 0, 90);
	m_WindowList.InsertColumn(4, L"路径", 0, 600);
	m_WindowList.SetImageList(&m_ImgList, LVSIL_SMALL);
	InitWindowList();
	SetWindowText(L"窗口列表");
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMyWndDlg::OnSysCommand(UINT nID, LPARAM lParam) {
	if ((nID & 0xFFF0) == IDM_ABOUTBOX) {
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else {
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMyWndDlg::OnPaint() {
	if (IsIconic()) {
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
	else {
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMyWndDlg::OnQueryDragIcon() {
	return static_cast<HCURSOR>(m_hIcon);
}

void CMyWndDlg::InitWindowList() {
	//	IsHungAppWindow()
// 	HWND hWnd = ::GetActiveWindow();
// 
// 	hWnd = ::GetWindow(hWnd, GW_HWNDFIRST);
// 	while (hWnd) {
// 		WCHAR wcsName[MAX_PATH + 1]{ 0 };
// 		::GetWindowTextW(hWnd, wcsName, MAX_PATH);
// 		if (wcslen(wcsName) != 0) {
// 			printf("%S : %08X\n", wcsName, hWnd);
// 		}
// 		hWnd = ::GetWindow(hWnd, GW_HWNDNEXT);
// 	}
	m_WindowList.DeleteAllItems();
	while (m_ImgList.Remove(0));
	CWnd* pWnd = AfxGetMainWnd()->GetWindow(GW_HWNDFIRST);
	DWORD dwIndex = 0;
	while (pWnd) {
		if (pWnd->IsWindowVisible() && !pWnd->GetOwner()) {
			CString csWindowName;
			pWnd->GetWindowTextW(csWindowName);
			HICON hIcon = pWnd->GetIcon(FALSE);
			csWindowName.TrimLeft();
			csWindowName.TrimRight();
			if (!csWindowName.IsEmpty()) {
				DWORD dwProcessId;
				GetWindowThreadProcessId(pWnd->m_hWnd, &dwProcessId);
				CString csProcessId;
				csProcessId.Format(L"%d", dwProcessId);
				CString csHWND;
				csHWND.Format(L"%p", pWnd->m_hWnd);
				WCHAR wcsProcessPath[MAX_PATH];
				HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
				GetModuleFileNameEx(hProcess, NULL, wcsProcessPath, MAX_PATH);
				CloseHandle(hProcess);
				m_WindowList.InsertItem(dwIndex, csWindowName,dwIndex);
				if (IsHungAppWindow(pWnd->m_hWnd)) {
					m_WindowList.SetItemText(dwIndex, 1, L"未响应");
				}
				else {
					m_WindowList.SetItemText(dwIndex, 1, L"正在运行");
				}
				m_WindowList.SetItemText(dwIndex, 2, csProcessId);
				m_WindowList.SetItemText(dwIndex, 3, csHWND);
				m_WindowList.SetItemText(dwIndex, 4, wcsProcessPath);
//				InsertImage(wcsProcessPath);
				if (hIcon) {
					m_ImgList.Add(hIcon);
				}
				else {
					InsertImage(wcsProcessPath);
				}
				dwIndex++;
			}
		}
		pWnd = pWnd->GetWindow(GW_HWNDNEXT);
	}
}

void CMyWndDlg::InsertImage(CString csNamePath) {
	HICON hSmallIcon = ::ExtractIcon(NULL, csNamePath, 0);
	if (hSmallIcon != NULL) {
		m_ImgList.Add(hSmallIcon);
	}
	else {
		m_ImgList.Add(LoadIcon(NULL, IDI_APPLICATION));
	}

}



void CMyWndDlg::OnContextMenu(CWnd* pWnd, CPoint point) {
	// TODO: 在此处添加消息处理程序代码
	CMenu mMenu;
	mMenu.LoadMenuW(IDR_MENU1);
	CMenu* mSubMenu = mMenu.GetSubMenu(0);
	mSubMenu->TrackPopupMenu(TPM_CENTERALIGN, point.x, point.y, this);
}


void CMyWndDlg::OnMenuFlush() {
	// TODO: 在此添加命令处理程序代码
	InitWindowList();
}


void CMyWndDlg::OnExitProcess() {
	// TODO: 在此添加命令处理程序代码
	int nPos = (int)m_WindowList.GetFirstSelectedItemPosition() - 1;
	WCHAR csProcessId[MAX_PATH]{ 0 };
	m_WindowList.GetItemText(nPos, 2, csProcessId, MAX_PATH);
	int nProcessId = _ttoi(csProcessId);
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, nProcessId);
	::TerminateProcess(hProcess, 0);
	CloseHandle(hProcess);
	InitWindowList();
}


void CMyWndDlg::OnShowWindow() {
	// TODO: 在此添加命令处理程序代码
	int nPos = (int)m_WindowList.GetFirstSelectedItemPosition() - 1;
	CString csWnd = m_WindowList.GetItemText(nPos, 3);
	DWORD dwWnd = wcstol(csWnd, NULL, 16);
	ShowWindowAsync((HWND)dwWnd, SW_SHOW);
}


void CMyWndDlg::OnHideWindow() {
	// TODO: 在此添加命令处理程序代码
	int nPos = (int)m_WindowList.GetFirstSelectedItemPosition() - 1;
	CString csWnd = m_WindowList.GetItemText(nPos, 3);
	DWORD dwWnd = wcstol(csWnd, NULL, 16);
	ShowWindowAsync((HWND)dwWnd, SW_HIDE);
}


void CMyWndDlg::OnMaxWindow() {
	// TODO: 在此添加命令处理程序代码
	int nPos = (int)m_WindowList.GetFirstSelectedItemPosition() - 1;
	CString csWnd = m_WindowList.GetItemText(nPos, 3);
	DWORD dwWnd = wcstol(csWnd, NULL, 16);
	ShowWindowAsync((HWND)dwWnd, SW_MAXIMIZE);
}


void CMyWndDlg::OnMinWindow() {
	// TODO: 在此添加命令处理程序代码
	int nPos = (int)m_WindowList.GetFirstSelectedItemPosition() - 1;
	CString csWnd = m_WindowList.GetItemText(nPos, 3);
	DWORD dwWnd = wcstol(csWnd, NULL, 16);
	ShowWindowAsync((HWND)dwWnd, SW_MINIMIZE);
}
