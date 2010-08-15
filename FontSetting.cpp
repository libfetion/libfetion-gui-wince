// FontSetting.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WMLF.h"
#include "FontSetting.h"

// CFontSetting �Ի���

IMPLEMENT_DYNAMIC(CFontSetting, CDialog)

CFontSetting::CFontSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CFontSetting::IDD, pParent)
	, m_nTreeFontSize(0)
{

}

CFontSetting::~CFontSetting()
{
}

void CFontSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TREEFONTSIZE, m_nTreeFontSize);
}


BEGIN_MESSAGE_MAP(CFontSetting, CDialog)
	ON_COMMAND(IDM_EDIT, &CFontSetting::OnEdit)
END_MESSAGE_MAP()


// CFontSetting ��Ϣ�������

BOOL CFontSetting::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	if (!m_dlgCommandBar.Create(this) ||
		!m_dlgCommandBar.InsertMenuBar(IDR_MENU_BUDDYINFO))
	{
		TRACE0("δ�ܴ��� CommandBar\n");
		return FALSE;      // δ�ܴ���
	}

	((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_TREEFONTSIZE))->SetRange(1,100);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CFontSetting::OnEdit()
{
    UpdateData();

	CDialog::OnOK();
}

