// FontSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "WMLF.h"
#include "FontSetting.h"

// CFontSetting 对话框

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


// CFontSetting 消息处理程序

BOOL CFontSetting::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	if (!m_dlgCommandBar.Create(this) ||
		!m_dlgCommandBar.InsertMenuBar(IDR_MENU_BUDDYINFO))
	{
		TRACE0("未能创建 CommandBar\n");
		return FALSE;      // 未能创建
	}

	((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_TREEFONTSIZE))->SetRange(1,100);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CFontSetting::OnEdit()
{
    UpdateData();

	CDialog::OnOK();
}

