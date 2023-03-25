#pragma once


// MyInjectShellCode 对话框

class MyInjectShellCode : public CDialogEx
{
	DECLARE_DYNAMIC(MyInjectShellCode)

public:
	MyInjectShellCode(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~MyInjectShellCode();
	virtual BOOL OnInitDialog();
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_ShellCode;
	afx_msg void OnBnClickedButton1();
	DWORD dwProcessId;
	CEdit m_ShellCodePath;
	CButton m_EditInject;
	CButton m_FileInject;
	afx_msg void OnBnClickedButton2();
};
