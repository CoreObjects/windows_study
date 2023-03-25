// MyInjectShellCode.cpp: 实现文件
//

#include "pch.h"
#include "进程与线程作业一.h"
#include "MyInjectShellCode.h"
#include "afxdialogex.h"


// MyInjectShellCode 对话框

IMPLEMENT_DYNAMIC(MyInjectShellCode, CDialogEx)

MyInjectShellCode::MyInjectShellCode(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent) {

}

MyInjectShellCode::~MyInjectShellCode() {
}

BOOL MyInjectShellCode::OnInitDialog() {
	CDialogEx::OnInitDialog();
	SetWindowTextW(L"注入ShellCode");
	return TRUE;
}

void MyInjectShellCode::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_ShellCode);
	DDX_Control(pDX, IDC_EDIT2, m_ShellCodePath);
	DDX_Control(pDX, IDC_RADIO1, m_EditInject);
	DDX_Control(pDX, IDC_RADIO2, m_FileInject);
}


BEGIN_MESSAGE_MAP(MyInjectShellCode, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &MyInjectShellCode::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &MyInjectShellCode::OnBnClickedButton2)
END_MESSAGE_MAP()


// MyInjectShellCode 消息处理程序


void MyInjectShellCode::OnBnClickedButton1() {
	// TODO: 在此添加控件通知处理程序代码

	if (m_EditInject.GetCheck()) {
		CString csShellCode;
		m_ShellCode.GetWindowTextW(csShellCode);
		int nLen = csShellCode.GetLength() / 3;
		WCHAR* lpShellCode = csShellCode.GetBuffer();
		unsigned char* shellcode = new unsigned char[nLen] {0};
		DWORD dwShellIndex = 0;
		for (int i = 0; i < csShellCode.GetLength(); i++) {
			unsigned char ucHigh, ucLow;
			if (lpShellCode[i] >= L'0' && lpShellCode[i] <= L'9') {
				ucHigh = lpShellCode[i] - L'0';
			}
			else if (lpShellCode[i] >= L'A' && lpShellCode[i] <= L'F') {
				ucHigh = lpShellCode[i] - L'A' + 0xA;
			}
			else {
				continue;
			}
			if (lpShellCode[i + 1] >= L'0' && lpShellCode[i + 1] <= L'9') {
				ucLow = lpShellCode[i + 1] - L'0';
			}
			else if (lpShellCode[i + 1] >= L'A' && lpShellCode[i + 1] <= L'F') {
				ucLow = lpShellCode[i + 1] - L'A' + 0xA;
			}
			else {
				continue;
			}
			shellcode[dwShellIndex] = ucHigh * 0x10 + ucLow;
			dwShellIndex++;
		}
		// 	for (int i = 0; i < nLen; i++) {
		// 		unsigned char ucHigh, ucLow;
		// 		if (lpShellCode[3 * i] >= L'0' && lpShellCode[3 * i] <= L'9') {
		// 			ucHigh = lpShellCode[3 * i] - L'0';
		// 		}
		// 		else if(lpShellCode[3 * i] >= L'A' && lpShellCode[3 * i] <= L'F'){
		// 			ucHigh = lpShellCode[3 * i] - L'A' + 0xA;
		// 		}
		// 		else {
		// 			continue;
		// 		}
		// 		if (lpShellCode[3 * i + 1] >= L'0' && lpShellCode[3 * i + 1] <= L'9') {
		// 			ucLow = lpShellCode[3 * i + 1] - L'0';
		// 		}
		// 		else if (lpShellCode[3 * i + 1] >= L'A' && lpShellCode[3 * i + 1] <= L'F') {
		// 			ucLow = lpShellCode[3 * i + 1] - L'A' + 0xA;
		// 		}
		// 		else {
		// 			continue;
		// 		}
		// 		shellcode[dwShellIndex] = ucHigh * 0x10 + ucLow;
		// 		dwShellIndex++;
		// 	}
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
		if (hProcess != INVALID_HANDLE_VALUE) {
			LPVOID lpBuffer = VirtualAllocEx(hProcess, NULL, nLen, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
			if (lpBuffer) {
				DWORD dwRealWrite;
				BOOL bRet = WriteProcessMemory(hProcess, lpBuffer, shellcode, nLen, &dwRealWrite);
				if (bRet) {
					HANDLE hThread = CreateRemoteThread(hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)lpBuffer, NULL, 0, NULL);
					if (hThread)CloseHandle(hThread);
				}
			}
			CloseHandle(hProcess);
		}
	}
	else if(m_FileInject.GetCheck()){
		CString szPath;
		m_ShellCodePath.GetWindowTextW(szPath);
		HANDLE hFile = CreateFile(szPath, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile != INVALID_HANDLE_VALUE) {
			DWORD dwLength = GetFileSize(hFile, NULL);
			CHAR* szBuffer = new CHAR[dwLength]{ 0 };
			DWORD dwReadRead;
			BOOL bRet = ReadFile(hFile, szBuffer, dwLength, &dwReadRead, NULL);
			if (bRet) {
				HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
				if (hProcess != INVALID_HANDLE_VALUE) {
					LPVOID lpBuffer = VirtualAllocEx(hProcess, NULL, dwLength, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
					if (lpBuffer) {
						SIZE_T dwRealWrite;
						BOOL bRetWrite = WriteProcessMemory(hProcess, lpBuffer, szBuffer, dwLength, &dwRealWrite);
						if (bRetWrite) {
							HANDLE hThread = CreateRemoteThread(hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)lpBuffer, NULL, 0, NULL);
							if (hThread)CloseHandle(hThread);
						}
					}
					CloseHandle(hProcess);
				}
			}
			CloseHandle(hFile);
		}

	}


}


void MyInjectShellCode::OnBnClickedButton2() {
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog cFileDlg(TRUE, NULL, NULL, NULL, L"二进制文件|*.bin|AllFiles|*.*||", this);
	cFileDlg.DoModal();
	CString szPath = cFileDlg.GetPathName();
	m_ShellCodePath.SetWindowText(szPath);
}
