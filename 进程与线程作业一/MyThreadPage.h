#pragma once


// MyThreadPage 对话框

class MyThreadPage : public CDialogEx
{
	DECLARE_DYNAMIC(MyThreadPage)

public:
	MyThreadPage(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~MyThreadPage();
	virtual BOOL OnInitDialog();
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ThreadList;
	DWORD dwProcessID;
	CString csProcessName;
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnThreadSuspend();
	afx_msg void OnThreadResume();
	afx_msg void OnKillThread();
};
