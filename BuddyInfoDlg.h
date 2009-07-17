#pragma once
#include "afxwin.h"


// CBuddyInfoDlg 对话框

class CBuddyInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CBuddyInfoDlg)

public:
	CBuddyInfoDlg(long lAccountID = 0, CWnd* pParent = NULL);   // 标准构造函数
	
	virtual ~CBuddyInfoDlg();

// 对话框数据
	enum { IDD = IDD_WMLF_BUDDYINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	CCommandBar m_dlgCommandBar;

	DECLARE_MESSAGE_MAP()
public:


    virtual BOOL OnInitDialog();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    void CBuddyInfoDlg::InitScrollInfo();

	const Fetion_Account *m_account;
    // 屏显名称
    CString m_strShowName;
    // 所属组
    int m_iGroupID;
    // 飞信号码
    long m_lAccountID;
    // 昵称
    CString m_strNickName;
    CString m_strName;
    CString m_strSex;
    CString m_strProv;
    CString m_strCity;
    // 签名
    CString m_strSign;
    void InitGroupItem(void);
    CComboBox m_cboGroup;
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnOk();
protected:
//	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
//	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void OnCancel();
};
