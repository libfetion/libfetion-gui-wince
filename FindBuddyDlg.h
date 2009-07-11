#pragma once


// CFindBuddyDlg 对话框

class CFindBuddyDlg : public CDialog
{
	DECLARE_DYNAMIC(CFindBuddyDlg)

public:
	CFindBuddyDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFindBuddyDlg();

// 对话框数据
	enum { IDD = IDD_WMLF_FINDBUDDY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	CCommandBar m_dlgCommandBar;

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnSize(UINT nType, int cx, int cy);
    virtual BOOL OnInitDialog();
};
