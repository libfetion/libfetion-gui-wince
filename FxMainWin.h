
/***************************************************************************
 *   Copyright (C) 2008 by DDD                                          *
 *   dedodong@163.com                                                     *
 *                                                                         *
 ***************************************************************************/
#pragma once
#include "afxcmn.h"

class CLoginDlg;
class BuddyOpt;
class FxMsgDlg;

// FxMainWin 对话框
class FxMainWin : public CDialog
{
	DECLARE_DYNAMIC(FxMainWin)

public:
	FxMainWin(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~FxMainWin();

#ifdef WIN32_PLATFORM_WFSP
protected:  // 控件条嵌入成员
	CCommandBar m_dlgCommandBar;
#endif // WIN32_PLATFORM_WFSP

	void do_login();
// 对话框数据
	enum { IDD = IDD_WMLF_MAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	afx_msg void OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/);
#endif
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

public:
	//login window
	CLoginDlg *loginDlg;
	BuddyOpt *m_BuddyOpt;

public:
	CTreeCtrl view;

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	afx_msg void OnTvnKeydownTreeBuddy(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnClose();
protected:
	BOOL handleFx_Sys_Event(int message, WPARAM wParam, LPARAM lParam);
public:
	BOOL showBuddyMenu(CPoint point);
	BOOL showBuddyMenu(HTREEITEM hItem);
public:
	BOOL showMsgDlg(CPoint point);
	BOOL showMsgDlg(HTREEITEM hItem);
public:
	CList<HTREEITEM> filker;
	CList<int> timeOutMsgVector;
	CList<TMPMSG_Info*> tmpMsg;
public:
	//当前聊天对话框
	FxMsgDlg* m_currentMsgDlg;
	//当前聊天Item
	HTREEITEM m_currentItem;
public:
	void relogin_fetion();
	void update_account_info();
	void relogin_ok();
	bool hand_SystemNetErr(int errcode);
	void filker_newmsg();
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	void handle_relogin_Event(int message, WPARAM wParam, LPARAM lParam);
	void hand_MsgSend_Event(int message, int fx_msg, long who);
	void handle_sendmsg(int msgflag, int fx_msg, long account_id);
	void addNewMessage(long account_id, CString newmsg = _T(""));
public:
	BOOL m_isLoginOK;
};
