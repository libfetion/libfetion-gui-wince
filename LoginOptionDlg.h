#pragma once
#include "afxwin.h"


// CLoginOptionDlg �Ի���

class CLoginOptionDlg : public CDialog
{
	DECLARE_DYNAMIC(CLoginOptionDlg)

public:
	CLoginOptionDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLoginOptionDlg();
	void InitUsersList(void);
	void GetSelectedUserOption(void);
	void WriteLoginOptionToIni();

	CString m_strStartupPath;
	// �ֻ�����
	CString m_mobile_no;
	BOOL m_bHaveEdit;

protected:  // �ؼ���Ƕ���Ա
	CCommandBar m_dlgCommandBar;

	// �Ի�������
	enum { IDD = IDD_WMLF_LOGIN_OPTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_cboUsersList2;
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboUsers2();
	CString m_strFetionNo;
	afx_msg void OnEdit();
	CString m_strInfo;
	afx_msg void OnSize(UINT nType, int cx, int cy);
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};

