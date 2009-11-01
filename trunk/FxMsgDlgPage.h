#pragma once


// CFxMsgDlgPage 对话框

class CFxMsgDlgPage : public CDialog
{
	DECLARE_DYNAMIC(CFxMsgDlgPage)

public:
	CFxMsgDlgPage(long lAccountID, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFxMsgDlgPage();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CString GetTitle();
	void ShowOnlineInfo();
    void ScrollToLast(void);
	void addNewMsg(CString msg = _T(""));
	void getMsg(CString &msg);

	long m_lAccountID;
	CString m_account_name;
	const Fetion_Account *m_account;
	const Fetion_Qun *m_Qun;
	// 是否是群消息
	BOOL m_IsQun;
	// 是否为陌生人
	BOOL m_IsStranger;

	BOOL m_isSendSMS;
	BOOL m_bMyself;
	BOOL m_bNotReadFlag;

// 对话框数据
	enum { IDD = IDD_WMLF_MSG_PAGE };

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void OnMsgChangeMethod();
	void OnMsgSend();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnBrowserCopy();
	afx_msg void OnSendCopy();
	afx_msg void OnSendPaste();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnCancel();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CEdit m_browser;
	CEdit m_send;
	CString m_msgBrowser;
	CString m_msgSend;
    CString m_strInfo;
};
