// FindBuddyDlg.cpp : 实现文件
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
        iW - DRA::SCALEX(20),
#else
        iW,
#endif
        iH, false);

    hwndCtl = ::GetDlgItem(hwndDlg, IDC_FB_BTN_FIND);
    ::MoveWindow(hwndCtl, iX + iW - DRA::SCALEX(20) + iMargin, iY, 
#if !defined(WIN32_PLATFORM_WFSP)
    20,
#else
    0,
#endif
    iH, false);
    
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

#ifdef WIN32_PLATFORM_WFSP
	//SP 2003SDK不支持Radio Button，隐藏掉，改用Combo Box
	((CComboBox*)GetDlgItem(IDC_FB_CBO_NUMTYPE))->SetCurSel(0);
#endif

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
    InitGroupItem();

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}

void CFindBuddyDlg::InitGroupItem(void)
{	
    Fetion_Group *group = NULL;

	DList *tmp_group = (DList *)fx_get_group();
    int i = 0;
	while(tmp_group)
	{
		group = (Fetion_Group *) tmp_group->data;
		if(group) {
			m_cboGroup.AddString(ConvertUtf8ToUtf16(group->name));
            if(i<25)
                m_iGroupIDs[i] = group->id;
            i++;
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
#ifdef WIN32_PLATFORM_PSPC
    bool bMobileNo = ((CButton*)GetDlgItem(IDC_FB_RD_MOBILE))->GetCheck() == BST_CHECKED;
#endif
#ifdef WIN32_PLATFORM_WFSP
	bool bMobileNo = ((CComboBox*)GetDlgItem(IDC_FB_CBO_NUMTYPE))->GetCurSel() == 0;  //选择手机号
#endif
    if((bMobileNo && m_strBuddyID.GetLength() != 11) || (!bMobileNo && m_strBuddyID.GetLength() != 9))
    {
        m_strInfo = _T("请输入正确的号码！手机11位，飞信号9位。");
        UpdateData(FALSE);
        return;
    }
    
    int iNewGroupID = m_iGroupIDs[m_cboGroup.GetCurSel()];

	if (bMobileNo)
		fx_add_buddy_by_mobile(ConvertUtf16ToUtf8(m_strBuddyID), 
				ConvertUtf16ToUtf8(m_strLocalName),
				iNewGroupID, 
				ConvertUtf16ToUtf8(m_strUserName),
				NULL, NULL);
	else
		fx_add_buddy_by_uid(ConvertUtf16ToUtf8(m_strBuddyID), 
				ConvertUtf16ToUtf8(m_strLocalName),
				iNewGroupID, 
				ConvertUtf16ToUtf8(m_strUserName),
				NULL, NULL);
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
