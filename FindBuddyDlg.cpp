// FindBuddyDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WMLF.h"
#include "FindBuddyDlg.h"

// CFindBuddyDlg 对话框

IMPLEMENT_DYNAMIC(CFindBuddyDlg, CDialog)

CFindBuddyDlg::CFindBuddyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFindBuddyDlg::IDD, pParent)
    , m_strBuddyID(_T(""))
    , m_strUserName(_T(""))
    , m_strInfo(_T(""))
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
}


BEGIN_MESSAGE_MAP(CFindBuddyDlg, CDialog)
    ON_WM_SIZE()
    ON_COMMAND(IDM_FB_SET_MOBILE, &CFindBuddyDlg::OnFbSetMobile)
    ON_COMMAND(IDM_FB_SET_FXNO, &CFindBuddyDlg::OnFbSetFxno)
    ON_COMMAND(IDM_FB_ADD, &CFindBuddyDlg::OnFbAdd)
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

#if !defined(WIN32_PLATFORM_WFSP)
    iW = rcDlg.right - rcDlg.left - 20 - iMargin * 2;
#else
    iW = rcDlg.right - rcDlg.left - iMargin * 2;
#endif
    iH = rcCtl.bottom - rcCtl.top;
    ::MoveWindow(hwndCtl, iX, iY, iW, iH, false);

    iInfoY = iY + iH + iMargin; //保存出Info的Y坐标,因为iH会变,这里提前先加好放着

    hwndCtl = ::GetDlgItem(hwndDlg, IDC_FB_BTN_FIND);
    iX = rcDlg.left + iW + iMargin;
    
#if !defined(WIN32_PLATFORM_WFSP)
    iW = 20;
#else
    iW = 0;
#endif

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
    ::MoveWindow(hwndCtl, 0, 0, 0, 0, true);
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
    ((CButton*)GetDlgItem(IDC_FB_RD_MOBILE))->SetCheck(BST_CHECKED);
    ((CButton*)GetDlgItem(IDC_FB_RD_FXNO))->SetCheck(BST_UNCHECKED);
}

void CFindBuddyDlg::OnFbSetFxno()
{
    ((CButton*)GetDlgItem(IDC_FB_RD_MOBILE))->SetCheck(BST_UNCHECKED);
    ((CButton*)GetDlgItem(IDC_FB_RD_FXNO))->SetCheck(BST_CHECKED);
}

void CFindBuddyDlg::OnFbAdd()
{
    UpdateData();
    bool bMobileNo = ((CButton*)GetDlgItem(IDC_FB_RD_MOBILE))->GetCheck() == BST_CHECKED;
    if((bMobileNo && m_strBuddyID.GetLength() != 11) || (!bMobileNo && m_strBuddyID.GetLength() != 9))
    {
        m_strInfo = _T("请输入正确的号码！手机11位，飞信号9位。");
        UpdateData(FALSE);
        return;
    }
    
    int iNewGroupID = m_iGroupIDs[m_cboGroup.GetCurSel()];

	if (bMobileNo)
		fx_add_buddy_by_mobile(ConvertUtf16ToUtf8(m_strBuddyID), 
				ConvertUtf16ToUtf8(_T("")),
				iNewGroupID, 
				ConvertUtf16ToUtf8(m_strUserName),
				NULL, NULL);
	else
		fx_add_buddy_by_uid(ConvertUtf16ToUtf8(m_strBuddyID), 
				ConvertUtf16ToUtf8(_T("")),
				iNewGroupID, 
				ConvertUtf16ToUtf8(m_strUserName),
				NULL, NULL);
    CDialog::OnOK();
}
