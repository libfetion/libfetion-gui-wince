#pragma once


// CFontSetting �Ի���

class CFontSetting : public CDialog
{
	DECLARE_DYNAMIC(CFontSetting)

public:
	CFontSetting(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFontSetting();

// �Ի�������
	enum { IDD = IDD_WMLF_FONT_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CCommandBar m_dlgCommandBar;
	afx_msg void OnEdit();
	UINT m_nTreeFontSize;
};
