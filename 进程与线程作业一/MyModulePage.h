#pragma once


// MyModulePage 对话框

class MyModulePage : public CDialogEx
{
	DECLARE_DYNAMIC(MyModulePage)

public:
	MyModulePage(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~MyModulePage();
	virtual BOOL OnInitDialog();
	VOID InitModuleList();
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ModuleList;
	DWORD dwProcessId;
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnFreeLibrary();
	afx_msg void OnFlush();
};
