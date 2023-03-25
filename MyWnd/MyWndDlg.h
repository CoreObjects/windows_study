
// MyWndDlg.h: 头文件
//

#pragma once


// CMyWndDlg 对话框
class CMyWndDlg : public CDialogEx
{
// 构造
public:
	CMyWndDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYWND_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	void InitWindowList();
	void InsertImage(CString csNamePath);
public:
	CListCtrl m_WindowList;
	CImageList m_ImgList;
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnMenuFlush();
	afx_msg void OnExitProcess();
	afx_msg void OnShowWindow();
	afx_msg void OnHideWindow();
	afx_msg void OnMaxWindow();
	afx_msg void OnMinWindow();
};
