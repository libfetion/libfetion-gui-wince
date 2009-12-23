#pragma once
#include "afxcmn.h"
#include "FxMsgDlgPage.h"

// CFxMsgDlgView 对话框

class CFxMsgDlgView : public CDialog
{
	DECLARE_DYNAMIC(CFxMsgDlgView)

public:
	CFxMsgDlgView(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFxMsgDlgView();
	void ShowMenuBar();
	CFxMsgDlgPage * ChatWith(long lAccountID, BOOL bMyself = FALSE);
	CFxMsgDlgPage * SeekPage(long lAccountID, int & nItem);
	void CreateMsgPage(CFxMsgDlgPage * pMsgPage);
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

// 对话框数据
	enum { IDD = IDD_WMLF_MSG_VIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_TabChat;
	virtual BOOL OnInitDialog();
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void OnOK();
protected:  // 控件条嵌入成员
	CCommandBar m_dlgCommandBar;
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTcnSelchangeTabChat(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMsgChangeMethod();
	afx_msg void OnMsgSend();
    afx_msg void OnBdViewinfo();
	afx_msg void OnEndTalk();
	afx_msg void OnPrevTalk();
	afx_msg void OnNextTalk();
	afx_msg void OnShowMsglog();
};
