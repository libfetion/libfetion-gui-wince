#pragma once
#include "afxwin.h"


// CAuthCodeDlg 对话框

#define WM_NEED_AUTH_CODE WM_USER+0x201

class CAuthCodeDlg : public CDialog
{
	DECLARE_DYNAMIC(CAuthCodeDlg)

public:
	CAuthCodeDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAuthCodeDlg();

	Fetion_Verfy * m_verfy;
// 对话框数据
	enum { IDD = IDD_WMLF_AUTH_CODE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	CCommandBar m_dlgCommandBar;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CStatic m_stAuthText;
	afx_msg void OnPaint();
protected:
	virtual void OnOK();
public:
	CString m_strInputAuthCode;
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
