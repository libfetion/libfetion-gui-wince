#pragma once
#include "afxwin.h"


// CFxMsgLogDlg �Ի���

class CFxMsgLogDlg : public CDialog
{
	DECLARE_DYNAMIC(CFxMsgLogDlg)

public:
	CFxMsgLogDlg(long lAccountID, CString strName, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFxMsgLogDlg();

	long m_lAccountID;
	CString m_strAccountName;
	DWORD m_dwFirst;
	DWORD m_dwReadCount;

	DWORD m_dwCount;


// �Ի�������
	enum { IDD = IDD_WMLF_MSG_LOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
protected:  // �ؼ���Ƕ���Ա
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
