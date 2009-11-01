#pragma once


// CFxMsgDlgPage �Ի���

class CFxMsgDlgPage : public CDialog
{
	DECLARE_DYNAMIC(CFxMsgDlgPage)

public:
	CFxMsgDlgPage(long lAccountID, CWnd* pParent = NULL);   // ��׼���캯��
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
	// �Ƿ���Ⱥ��Ϣ
	BOOL m_IsQun;
	// �Ƿ�Ϊİ����
	BOOL m_IsStranger;

	BOOL m_isSendSMS;
	BOOL m_bMyself;
	BOOL m_bNotReadFlag;

// �Ի�������
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
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
