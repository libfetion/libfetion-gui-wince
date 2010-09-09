#pragma once


// CFontSetting 对话框

class CFontSetting : public CDialog
{
	DECLARE_DYNAMIC(CFontSetting)

public:
	CFontSetting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFontSetting();

// 对话框数据
	enum { IDD = IDD_WMLF_FONT_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CCommandBar m_dlgCommandBar;
	afx_msg void OnEdit();
	UINT m_nTreeFontSize;
};
