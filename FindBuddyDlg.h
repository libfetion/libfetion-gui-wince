#pragma once
#include "afxwin.h"


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
    void InitGroupItem(void);
    // 好友ID或电话号码
    CString m_strBuddyID;
    afx_msg void OnFbSetMobile();
    afx_msg void OnFbSetFxno();
    afx_msg void OnFbAdd();
    CComboBox m_cboGroup;
    CString m_strUserName;
    // 将组ID保存到变量中与combobox中的每一个Index相对应
    int m_iGroupIDs[25];
    // 提示出错的信息
    CString m_strInfo;
    // 对方的屏显名称
    CString m_strLocalName;
    afx_msg void OnStnClickedFbBtnFind();
};
