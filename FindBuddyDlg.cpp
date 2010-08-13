﻿// FindBuddyDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WMLF.h"
#include "FindBuddyDlg.h"

#ifdef M8
#include "M8Misc.h"
#endif

#ifdef WIN32_PLATFORM_WFSP
#include <tpcshell.h>
#endif
// CFindBuddyDlg 对话框

IMPLEMENT_DYNAMIC(CFindBuddyDlg, CDialog)

CFindBuddyDlg::CFindBuddyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFindBuddyDlg::IDD, pParent)
    , m_strBuddyID(_T(""))
    , m_strUserName(_T(""))
    , m_strInfo(_T(""))
    , m_strLocalName(_T(""))
	, m_bAddByMobileNo(TRUE)
	, m_iGroupID(-1)
{

}

CFindBuddyDlg::~CFindBuddyDlg()
{
}

void CFindBuddyDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_FB_EDIT_NO, m_strBuddyID);
    DDX_Control(pDX, IDC_FB_CBO_GROUP, m_cboGroup);
    DDX_Text(pDX, IDC_FB_EDIT_NAME, m_strUserName);
    DDX_Text(pDX, IDC_FB_STATIC_INFO, m_strInfo);
    DDX_Text(pDX, IDC_FB_EDIT_NAME2, m_strLocalName);
}


BEGIN_MESSAGE_MAP(CFindBuddyDlg, CDialog)
    ON_WM_SIZE()
    ON_COMMAND(IDM_FB_SET_MOBILE, &CFindBuddyDlg::OnFbSetMobile)
    ON_COMMAND(IDM_FB_SET_FXNO, &CFindBuddyDlg::OnFbSetFxno)
    ON_COMMAND(IDM_FB_ADD, &CFindBuddyDlg::OnFbAdd)
    ON_STN_CLICKED(IDC_FB_BTN_FIND, &CFindBuddyDlg::OnStnClickedFbBtnFind)
	ON_COMMAND(IDFINDCANCEL, &CFindBuddyDlg::OnFindcancel)
END_MESSAGE_MAP()


// CFindBuddyDlg 消息处理程序

// 修改者务必注意:这里因为每个控件的位置都是有相关性的,所以代码的前后位置一定不要随意更改
// 除非你非常清楚自己的修改会造成什么后果.
// hiizsk   2009.07.11
void CFindBuddyDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);

    int iMargin = DRA::SCALEX(5);
    int iX, iY, iW, iH;

    HWND hwndDlg = this->m_hWnd;
    RECT rcDlg;
    ::GetClientRect(hwndDlg, &rcDlg);
#ifdef M8
    rcDlg.top += 40;
#endif
	HWND hwndCtl = ::GetDlgItem(hwndDlg, IDC_FB_RD_MOBILE);

    iX = rcDlg.left + iMargin;
    iY = rcDlg.top + iMargin;
    iW = DRA::SCALEX(70);;
    iH = DRA::SCALEY(20);

    ::MoveWindow(hwndCtl, iX, iY, iW, iH, false);
    
    hwndCtl = ::GetDlgItem(hwndDlg, IDC_FB_RD_FXNO);
    ::MoveWindow(hwndCtl, iX + iW + iMargin, iY, iW, iH, false);

    iW = rcDlg.right - rcDlg.left - 2 * iMargin;

#ifdef WIN32_PLATFORM_WFSP
    hwndCtl = ::GetDlgItem(hwndDlg, IDC_FB_CBO_NUMTYPE);
    ::MoveWindow(hwndCtl, iX, iY, iW, iH, false);
#endif

    iY = iY + iH + iMargin;
    hwndCtl = ::GetDlgItem(hwndDlg, IDC_FB_EDIT_NO);
    ::MoveWindow(hwndCtl, iX, iY, 
#if !defined(WIN32_PLATFORM_WFSP)
        iW - DRA::SCALEX(0),
#else
        iW,
#endif
        iH, false);

    hwndCtl = ::GetDlgItem(hwndDlg, IDC_FB_BTN_FIND);
    ::MoveWindow(hwndCtl, iX + iW - DRA::SCALEX(0) + iMargin, iY, 
#if !defined(WIN32_PLATFORM_WFSP)
    DRA::SCALEX(16), DRA::SCALEY(16),
#else
    0, 0,
#endif
    false);
    
    hwndCtl = ::GetDlgItem(hwndDlg, IDC_FB_STATIC_INFO);
    ::MoveWindow(hwndCtl, iX, iY, iW, iH, false);

    iY = iY + iH + iMargin;
    hwndCtl = ::GetDlgItem(hwndDlg, IDC_FB_STATIC_NAME);
    ::MoveWindow(hwndCtl, iX, iY, iW, iH, false);
    
    iY = iY + iH + iMargin;
    hwndCtl = ::GetDlgItem(hwndDlg, IDC_FB_EDIT_NAME);
    ::MoveWindow(hwndCtl, iX, iY, iW, iH, false);

    iY = iY + iH + iMargin;
    hwndCtl = ::GetDlgItem(hwndDlg, IDC_FB_STATIC_GROUP);
    ::MoveWindow(hwndCtl, iX, iY, iW, iH, false);
    
    iY = iY + iH + iMargin;
    hwndCtl = ::GetDlgItem(hwndDlg, IDC_FB_CBO_GROUP);
    ::MoveWindow(hwndCtl, iX, iY, iW, iH, false);

    iY = iY + iH + iMargin;
    hwndCtl = ::GetDlgItem(hwndDlg, IDC_FB_STATIC_NAME2);
    ::MoveWindow(hwndCtl, iX, iY, iW, iH, false);
    
    iY = iY + iH + iMargin;
    hwndCtl = ::GetDlgItem(hwndDlg, IDC_FB_EDIT_NAME2);
    ::MoveWindow(hwndCtl, iX, iY, iW, iH, false);

    hwndCtl = ::GetDlgItem(hwndDlg, IDC_FB_LIST_INFO);
    ::MoveWindow(hwndCtl, 0, 0, 0, 0, true);
}

BOOL CFindBuddyDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

	if(m_bAddByMobileNo)
	{
		OnFbSetMobile();
	}
	else
	{
		OnFbSetFxno();
	}

#ifdef M8
    AddMenuBarForM8(this->GetSafeHwnd(), IDR_MENU_BUDDYINFO);
    FullScreen(this->GetSafeHwnd());
#else
	if (!m_dlgCommandBar.Create(this) ||
		!m_dlgCommandBar.InsertMenuBar(IDR_MENU_FINDBUDDY))
	{
		TRACE0("未能创建 CommandBar\n");
		return FALSE;      // 未能创建
	}
#endif
#ifdef WIN32_PLATFORM_WFSP
    //重写后退键，引发WM_HOTKEY消息
    (void)::SendMessage(SHFindMenuBar (m_hWnd), SHCMBM_OVERRIDEKEY, VK_TBACK,
        MAKELPARAM(SHMBOF_NODEFAULT | SHMBOF_NOTIFY,
        SHMBOF_NODEFAULT | SHMBOF_NOTIFY));
#endif

	InitGroupItem();

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}

void CFindBuddyDlg::InitGroupItem(void)
{	
    Fetion_Group *group = NULL;
	int nIndex = 0;

	DList *tmp_group = (DList *)fx_get_group();
	while(tmp_group)
	{
		group = (Fetion_Group *) tmp_group->data;
		if(group) {
			nIndex = m_cboGroup.AddString(ConvertUtf8ToUtf16(group->name));
			if(nIndex >= 0)
			{
				m_cboGroup.SetItemData(nIndex, group->id);
				if((m_iGroupID > 0) && (m_iGroupID == group->id))
				{
					m_cboGroup.SetCurSel(nIndex);
				}
			}
		}
		tmp_group = d_list_next(tmp_group);
	}
}
void CFindBuddyDlg::OnFbSetMobile()
{
#ifdef WIN32_PLATFORM_PSPC
    ((CButton*)GetDlgItem(IDC_FB_RD_MOBILE))->SetCheck(BST_CHECKED);
    ((CButton*)GetDlgItem(IDC_FB_RD_FXNO))->SetCheck(BST_UNCHECKED);
#endif
#ifdef WIN32_PLATFORM_WFSP
	//SP 2003SDK不支持Radio Button，隐藏掉，改用Combo Box
	((CComboBox*)GetDlgItem(IDC_FB_CBO_NUMTYPE))->SetCurSel(0);
#endif
}

void CFindBuddyDlg::OnFbSetFxno()
{
#ifdef WIN32_PLATFORM_PSPC
    ((CButton*)GetDlgItem(IDC_FB_RD_MOBILE))->SetCheck(BST_UNCHECKED);
    ((CButton*)GetDlgItem(IDC_FB_RD_FXNO))->SetCheck(BST_CHECKED);
#endif
#ifdef WIN32_PLATFORM_WFSP
	((CComboBox*)GetDlgItem(IDC_FB_CBO_NUMTYPE))->SetCurSel(1);
#endif
}

void CFindBuddyDlg::OnFbAdd()
{
    UpdateData();
    bool bMobileNo = true;

#ifdef WIN32_PLATFORM_WFSP
	 bMobileNo = ((CComboBox*)GetDlgItem(IDC_FB_CBO_NUMTYPE))->GetCurSel() == 0;  //选择手机号
#else
    bMobileNo = ((CButton*)GetDlgItem(IDC_FB_RD_MOBILE))->GetCheck() == BST_CHECKED;
#endif
    if((bMobileNo && m_strBuddyID.GetLength() != 11) || (!bMobileNo && m_strBuddyID.GetLength() != 9))
    {
        m_strInfo = _T("请输入正确的号码！手机11位，飞信号9位。");
        UpdateData(FALSE);
        return;
    }
    
    int iNewGroupID = 0;
	if(m_cboGroup.GetCurSel() >= 0)
	{
		iNewGroupID = m_cboGroup.GetItemData(m_cboGroup.GetCurSel());
	}

	CStringA strBuddyID = ConvertUtf16ToUtf8(m_strBuddyID);
	CStringA strLocalName = ConvertUtf16ToUtf8(m_strLocalName);
	CStringA strUserName = ConvertUtf16ToUtf8(m_strUserName);
	if (bMobileNo)
		fx_add_buddy_by_mobile(strBuddyID.GetBuffer(), strLocalName.GetBuffer(), iNewGroupID, strUserName.GetBuffer(), NULL, NULL);
	else
		fx_add_buddy_by_uid(strBuddyID.GetBuffer(), strLocalName.GetBuffer(), iNewGroupID, strUserName.GetBuffer(), NULL, NULL);
		
    CDialog::OnOK();
}

void CFindBuddyDlg::OnStnClickedFbBtnFind()
{

}

void CFindBuddyDlg::OnCancel()
{
#ifdef WIN32_PLATFORM_WFSP
	// 在这里修改后退键的行为为删除EditBox中的内容，而不是退出模态对话框
	SHSendBackToFocusWindow(WM_HOTKEY, 2, MAKELPARAM(MOD_KEYUP, VK_TBACK));
#else
	CDialog::OnCancel();
#endif // WIN32_PLATFORM_W
}

void CFindBuddyDlg::OnFindcancel()
{
	CDialog::OnCancel();
}

LRESULT CFindBuddyDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_CLOSE:
		break;
#ifdef WIN32_PLATFORM_WFSP
		//改变后退键行为
	case WM_HOTKEY:
		if ((VK_TBACK == HIWORD(lParam)) && (MOD_KEYUP == LOWORD(lParam)))
		{
			SHSendBackToFocusWindow(message, wParam, lParam);
		}
		break;
#endif
	default:
		break;
	}

	return CDialog::WindowProc(message, wParam, lParam);
}
