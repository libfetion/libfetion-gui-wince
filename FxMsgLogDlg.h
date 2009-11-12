#pragma once
#include "afxwin.h"


// CFxMsgLogDlg 对话框

class CFxMsgLogDlg : public CDialog
{
	DECLARE_DYNAMIC(CFxMsgLogDlg)

public:
	CFxMsgLogDlg(long lAccountID, CString strName, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFxMsgLogDlg();

	long m_lAccountID;
	CString m_strAccountName;
	DWORD m_dwFirst;
	DWORD m_dwReadCount;

	DWORD m_dwCount;


// 对话框数据
	enum { IDD = IDD_WMLF_MSG_LOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
protected:  // 控件条嵌入成员
	CCommandBar m_dlgCommandBar;
public:
	virtual BOOL OnInitDialog();
	CEdit m_EditMsgLog;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CButton m_btHeadPage;
	CButton m_btPrevPage;
	CButton m_btNextPage;
	CButton m_btTailPage;
	CStatic m_stTitle;
	CStatic m_stPageNum;
	void LoadMsgLog(void);
	afx_msg void OnBnClickedHeadPage();
	afx_msg void OnBnClickedPrevPage();
	afx_msg void OnBnClickedNextPage();
	afx_msg void OnBnClickedTailPage();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
