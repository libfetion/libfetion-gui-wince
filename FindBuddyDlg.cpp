// FindBuddyDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WMLF.h"
#include "FindBuddyDlg.h"

// CFindBuddyDlg 对话框

IMPLEMENT_DYNAMIC(CFindBuddyDlg, CDialog)

CFindBuddyDlg::CFindBuddyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFindBuddyDlg::IDD, pParent)
{

}

CFindBuddyDlg::~CFindBuddyDlg()
{
}

void CFindBuddyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFindBuddyDlg, CDialog)
    ON_WM_SIZE()
END_MESSAGE_MAP()


// CFindBuddyDlg 消息处理程序

void CFindBuddyDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);
    int iMargin = DRA::SCALEX(5);
    int iX, iY, iW, iH;
    int iInfoX, iInfoY, iInfoW, iInfoH; //显示用户信息的这个LIST因为要独自撑满,所以单独用变量保存,最后再移

    HWND hwndDlg = this->m_hWnd;
    RECT rcDlg;
    ::GetClientRect(hwndDlg, &rcDlg);

	HWND hwndCtl = ::GetDlgItem(hwndDlg, IDC_FB_RD_MOBILE);
	ASSERT(hwndCtl != NULL);
	RECT rcCtl;
	::GetWindowRect(hwndCtl, &rcCtl);
    iX = rcDlg.left + iMargin;
    iY = rcDlg.top + iMargin;
    iW = rcCtl.right - rcCtl.left;
    iH = rcCtl.bottom - rcCtl.top;
    ::MoveWindow(hwndCtl, iX, iY, iW, iH, false);
    
    iInfoX = iX;
    iInfoW = rcDlg.right - rcDlg.left - 2 * iMargin;

    hwndCtl = ::GetDlgItem(hwndDlg, IDC_FB_RD_FXNO);
    ::MoveWindow(hwndCtl, iX + iW + iMargin, iY, iW, iH, false);

    hwndCtl = ::GetDlgItem(hwndDlg, IDC_FB_EDIT_NO);
	::GetWindowRect(hwndCtl, &rcCtl);
    iY = iY + iH + iMargin;
    iW = rcDlg.right - rcDlg.left - 20 - iMargin * 2;
    iH = rcCtl.bottom - rcCtl.top;
    ::MoveWindow(hwndCtl, iX, iY, iW, iH, false);

    iInfoY = iY + iH + iMargin; //保存出Info的Y坐标,因为iH会变,这里提前先加好放着

    hwndCtl = ::GetDlgItem(hwndDlg, IDC_FB_BTN_FIND);
    iX = rcDlg.left + iW + iMargin;
    iW = 20;
    iH = 20;
    ::MoveWindow(hwndCtl, iX, iY, iW, iH, false);

    hwndCtl = ::GetDlgItem(hwndDlg, IDC_FB_EDIT_NAME);
    ::GetWindowRect(hwndCtl, &rcCtl);
    iX = rcDlg.left + iMargin;
    iY = rcDlg.bottom - (rcCtl.bottom - rcCtl.top) - iMargin;
    iH = rcCtl.bottom - rcCtl.top;
    iW = rcDlg.right - rcDlg.left - 2 * iMargin;
    ::MoveWindow(hwndCtl, iX, iY, iW, iH , false);

    hwndCtl = ::GetDlgItem(hwndDlg, IDC_FB_STATIC_NAME);
    ::GetWindowRect(hwndCtl, &rcCtl);
    iW = rcCtl.right - rcCtl.left;
    iH = rcCtl.bottom - rcCtl.top;
    iY = iY - iMargin - iH;
    ::MoveWindow(hwndCtl, iX, iY, iW, iH, false);

    iInfoH = iY - iMargin - iInfoY;

    hwndCtl = ::GetDlgItem(hwndDlg, IDC_FB_LIST_INFO);
    ::MoveWindow(hwndCtl, iInfoX, iInfoY, iInfoW, iInfoH, true);
}

BOOL CFindBuddyDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

	if (!m_dlgCommandBar.Create(this) ||
		!m_dlgCommandBar.InsertMenuBar(IDR_MENU_FINDBUDDY))
	{
		TRACE0("未能创建 CommandBar\n");
		return FALSE;      // 未能创建
	}
    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}
