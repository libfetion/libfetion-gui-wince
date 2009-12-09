#pragma once


// CMyselfInfoDlg 对话框

class CMyselfInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CMyselfInfoDlg)

public:
	CMyselfInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMyselfInfoDlg();
    void InitScrollInfo();

// 对话框数据
	enum { IDD = IDD_WMLF_MYSELF_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	CCommandBar m_dlgCommandBar;

	DECLARE_MESSAGE_MAP()
public:
	CString m_strAccountID;
	CString m_strScore;
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CString m_strBirthday;
	CString m_strLunarAnimal;
	CString m_strHoroscope;
	CString m_strProv;
	CString m_strCity;
	CString m_strSex;
	CString m_strBloodType;
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CString m_strMobileNo;
	CString m_strNickName;
	CString m_strNickNameBackup;
	CString m_strSign;
	CString m_strSignBackup;
public:
	afx_msg void OnEdit();
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
