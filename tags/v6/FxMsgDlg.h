
/***************************************************************************
 *   Copyright (C) 2008 by DDD                                          *
 *   dedodong@163.com                                                     *
 *                                                                         *
 ***************************************************************************/
#pragma once
#include "afxwin.h"


// FxMsgDlg 对话框

class FxMsgDlg : public CDialog
{
	DECLARE_DYNAMIC(FxMsgDlg)

public:
	FxMsgDlg(long ac_id, CWnd* pParent = NULL, BOOL isSendSMS = FALSE);   // 标准构造函数
	virtual ~FxMsgDlg();

	virtual void OnFinalRelease();

// 对话框数据
	enum { IDD = IDD_WMLF_MSG };

//#ifdef WIN32_PLATFORM_WFSP
protected:  // 控件条嵌入成员
	CCommandBar m_dlgCommandBar;
//#endif // WIN32_PLATFORM_WFSP

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

public:

	long account_id;
	CString account_name;
	BOOL m_isSendSMS;
	const Fetion_Account *m_account;
	BOOL SetNewBuddy(long ac_id);
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnEnChangeEdit1();
public:
	CString m_msgBrowser;
public:
	CString m_msgSend;

protected:
	void printfOnlineInfo();
public:
#if defined(_DEVICE_RESOLUTION_AWARE)// && !defined(WIN32_PLATFORM_WFSP)
	afx_msg void OnSize(UINT nType, int cx, int cy);
#endif
public:
	afx_msg void OnStnClickedSend();
public:
	afx_msg void OnStnClickedChange();
public:
	void addNewMsg(CString msg = _T(""));
	void getMsg(CString &msg);
public:
	CEdit m_browser;
	CEdit m_send;
public:
	BOOL m_isLoginOK;
public:
	afx_msg void OnMsgSend();
public:
	afx_msg void OnMsgChangeMethod();
public:
	afx_msg void OnEnChangeSendMsg();


protected:
	virtual void OnCancel();
public:
    CString m_strInfo;
private:
    // 将聊天记录滚动到最下面
    void ScrollToLast(void);
public:
    afx_msg void OnBdViewinfo();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnBrowserCopy();
	afx_msg void OnSendCopy();
	afx_msg void OnSendPaste();
};

