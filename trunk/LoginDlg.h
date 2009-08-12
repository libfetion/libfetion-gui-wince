
/***************************************************************************
 *   Copyright (C) 2008 by DDD                                          *
 *   dedodong@163.com                                                     *
 *                                                                         *
 ***************************************************************************/
// WMLFDlg.h : 头文件
//

#pragma once
#include "afxwin.h"

#define FX_LOGINSTATE_MSG WM_USER+1 

// CLoginDlg 对话框
class CLoginDlg : public CDialog
{
// 构造
public:
	CLoginDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_WMLF_DIALOG };

//#ifdef WIN32_PLATFORM_WFSP
protected:  // 控件条嵌入成员
	CCommandBar m_dlgCommandBar;
//#endif // WIN32_PLATFORM_WFSP

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
#if defined(_DEVICE_RESOLUTION_AWARE)// && !defined(WIN32_PLATFORM_WFSP)
	afx_msg void OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/);
#endif
	DECLARE_MESSAGE_MAP()
public:
	// 手机号码
	CString m_mobile_no;
	// 飞信ID
	CString m_fetion_id;
public:
	// 飞信登录密码
	CString m_passwd;

	//save fetion server address
	CString m_server_addr;

public:
	afx_msg void OnBnClickedLogin();
public:
	// 飞信登录状态
	CString m_login_state;
public:
	// 飞信登录标志
	BOOL m_LoginFlag;
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	afx_msg void OnBnClickedRemAccount();
	afx_msg void OnRemPassUpdateUI(CCmdUI* cmdui);
public:
	CButton RemAccount;
public:
	afx_msg void OnLogin();
	afx_msg void OnLoginCancel();
public:
	afx_msg void OnIDM_Cancel();
protected:
	virtual void OnCancel();
public:
    // 自动连接网络
    BOOL EstablishConnection(void);
	CString GetFetionNoFromHttpsWeb(CString strMobileNo, CString strPwd, int& netflag);
protected:
    void OnRemPassChanged();
    // 是否记住密码
    BOOL m_bRemPass;
    BOOL m_bLoginOffLine;
public:
    // 是否已经开始登录
    bool m_bIsLoging;
};
