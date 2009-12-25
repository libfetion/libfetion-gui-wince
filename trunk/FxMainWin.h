
/***************************************************************************
 *   Copyright (C) 2008 by DDD                                          *
 *   dedodong@163.com                                                     *
 *                                                                         *
 ***************************************************************************/
#pragma once
#include "afxcmn.h"
#include "MessageLog.h"
#include "BuddyInfoDlg.h"
#include "FxMsgDlgView.h"
#ifdef M8
#include "M8Misc.h"
#else
#include <connmgr.h>
#endif

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

//#ifdef WIN32_PLATFORM_WFSP
protected:  // 控件条嵌入成员
	CCommandBar m_dlgCommandBar;
//#endif // WIN32_PLATFORM_WFSP

	void do_login();
// 对话框数据
	enum { IDD = IDD_WMLF_MAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
#if defined(_DEVICE_RESOLUTION_AWARE)// && !defined(WIN32_PLATFORM_WFSP)
	afx_msg void OnSize(UINT nType, int cx, int cy);
#endif
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

public:

	//login window
	CLoginDlg *loginDlg;
	BuddyOpt *m_BuddyOpt;
	CFxMsgDlgView * m_pFxMsgDlgView;
	//CFxDatabase * m_pFxDB;

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
    BOOL FxMainWin::showMsgDlg();
	BOOL showMsgDlg(CPoint point);
	BOOL showMsgDlg(HTREEITEM hItem);
    
	BOOL showBuddyInfo(HTREEITEM hItem);
public:
	CList<HTREEITEM> filker;
	CList<int> timeOutMsgVector;
public:
	//当前好友资料对话框
	CBuddyInfoDlg* m_BuddyInfoDlg;
public:
	void relogin_fetion();
	void update_account_info();
	void relogin_ok();
	bool hand_SystemNetErr(int errcode);
	void filker_newmsg();
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSendMsg();
public:
	void handle_relogin_Event(int message, WPARAM wParam, LPARAM lParam);
	void hand_MsgSend_Event(int message, int fx_msg, long who);
	void handle_sendmsg(int msgflag, int fx_msg, long account_id);
	void addNewMessage(long account_id, CString newmsg = _T(""));
	void addNewQunMessage(long qun_id,CString newmsg=_T(""));
    BOOL SetUserOnlineState(int iState);
public:
	CONNMGR_CONNECTIONINFO m_ConnInfo;
	BOOL m_isLoginOK;
    // 用户昵称
    CString m_strNickName;
    // 用户昵称显示，会加上在线状态，例如"(隐身)"
	CString m_strNickNameShow;
    // 用户签名
    CString m_strSign;
    afx_msg void OnStnClickedBtnAdd();
    // 聊天记录处理
    CMessageLog m_MessageLog;
    // 消息提示
    void NotifyUser(int EventType, long lAccountID, CString szBuddyName);
    // 是否在收到消息时振动提醒
    BOOL m_bVibrate;
	// 好友上线时是否提醒
	BOOL m_bOnline;

    // 是否在收到消息时静音
    BOOL m_bSilence;

	// 是否屏蔽群消息
	BOOL m_bShieldQunMessage;

	// 是否使用长短信
	BOOL m_bLongSMS;

    afx_msg void OnMainSetVibr();
    afx_msg void OnUpdateMainSetVibr(CCmdUI *pCmdUI);

    afx_msg void OnMainSetSilence();
    afx_msg void OnUpdateMainSetSilence(CCmdUI *pCmdUI);

	afx_msg void OnMainSetOnline();
	afx_msg void OnUpdateMainSetOnline(CCmdUI *pCmdUI);

	afx_msg void OnMainShieldQunMessage();
	afx_msg void OnUpdateShieldQunMessage(CCmdUI *pCmdUI);
    // 程序启动路径
    CString m_strStartupPath;
    // 获取启动路径
    CString GetStartupPath(void);
    // 当前登录用户的ID
    long m_lAccountID;
	CString m_mobile_no;
    afx_msg void OnBdViewinfo();
    afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
private:
    // 处理移动组成功的消息
    void handle_MoveGroupOk(long account_id, int group_id);
    // 处理增加好友请求
    void handle_AddAccountApp(char* uri, char* showname);
public:
    afx_msg void OnAbout();
    afx_msg void OnBdDelete();
    afx_msg void OnBdAddblacklist();
    afx_msg void OnBdRmblacklist();
    afx_msg void OnMainSetNosound();
    BOOL m_bSound;
    
public:
    afx_msg void OnMainShownewmsg();
private:
    BOOL showMsgDlg(long lAccountID);
public:
    afx_msg void OnBdSendmsg();
    afx_msg void OnMainAddbuddy();
protected:
    virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
    void OnMainShownewmsg2(void);
    void OnMainDimiss(void);
public:
	afx_msg void OnMainClean();
protected:
	virtual void OnOK();
	virtual void OnCancel();
public:
	CString GetUserStateString(void);
	UINT GetSettingFromIni(LPCTSTR lpKeyName, int nDefault = 0);
	BOOL SetSettingToIni(LPCTSTR lpKeyName, UINT uValue);
	void RecoveryMenuBar();
	afx_msg void OnSendMyself();
	void RemoveFilker(long lAccountID);
	void RemoveFilker(HTREEITEM hItem);
	afx_msg void OnUpdateAllAccountinfo();
	afx_msg void OnMainSetLongsms();
	afx_msg void OnMyselfInfo();
};
