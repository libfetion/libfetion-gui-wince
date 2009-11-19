#pragma once
#include "afxcmn.h"
#include "FxMsgDlgPage.h"

// CFxMsgDlgView �Ի���

class CFxMsgDlgView : public CDialog
{
	DECLARE_DYNAMIC(CFxMsgDlgView)

public:
	CFxMsgDlgView(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFxMsgDlgView();
	void ShowMenuBar();
	CFxMsgDlgPage * ChatWith(long lAccountID, BOOL bMyself = FALSE);
	CFxMsgDlgPage * SeekPage(long lAccountID, int & nItem);
	CFxMsgDlgPage * AddNewChat(long lAccountID, BOOL bMyself = FALSE);
	void ShowChat(CFxMsgDlgPage * pSeekMsgPage);
	CFxMsgDlgPage * GetCurrentDlgPage();
	BOOL addNewMsg(long lAccountID, CString msg);
	void updateAccountInfo(long lAccountID);
	void SetNotReadFlag(int nItem, BOOL bNotRead);
	void LoginOK(BOOL bLoginOK);

	BOOL m_isLoginOK;
	BOOL m_isShow;
	CWnd* m_pParentWnd;
	CImageList m_ImageList;

// �Ի�������
	enum { IDD = IDD_WMLF_MSG_VIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_TabChat;
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
protected:  // �ؼ���Ƕ���Ա
	CCommandBar m_dlgCommandBar;
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTcnSelchangeTabChat(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMsgChangeMethod();
	afx_msg void OnMsgSend();
    afx_msg void OnBdViewinfo();
	afx_msg void OnEndTalk();
	afx_msg void OnNextTalk();
protected:
	virtual void OnCancel();
public:
	afx_msg void OnShowMsglog();
};