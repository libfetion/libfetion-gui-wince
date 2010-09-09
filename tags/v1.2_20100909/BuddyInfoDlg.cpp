// BuddyInfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WMLF.h"
#include "BuddyInfoDlg.h"

#ifdef M8
#include "M8Misc.h"
#endif

#ifdef WIN32_PLATFORM_WFSP
#include <tpcshell.h>
#endif
// CBuddyInfoDlg 对话框

IMPLEMENT_DYNAMIC(CBuddyInfoDlg, CDialog)


CBuddyInfoDlg::CBuddyInfoDlg(long lAccountID, CWnd* pParent /*=NULL*/)
	: CDialog(CBuddyInfoDlg::IDD, pParent)
    , m_strShowName(_T(""))
    , m_iGroupID(0)
    , m_lAccountID(lAccountID)
	, m_strAccountID(_T(""))
	, m_strMobileNo(_T(""))
    , m_strNickName(_T(""))
    , m_strName(_T(""))
    , m_strSex(_T(""))
    , m_strProv(_T(""))
    , m_strCity(_T(""))
    , m_strSign(_T(""))
{
}

CBuddyInfoDlg::~CBuddyInfoDlg()
{
}

void CBuddyInfoDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_BI_EDT_SHOWNAME, m_strShowName);
    DDX_Text(pDX, IDC_BI_FETIONNO, m_strAccountID);
	DDX_Text(pDX, IDC_BI_MOBILENO, m_strMobileNo);
    DDX_Text(pDX, IDC_BI_NICKNAME, m_strNickName);
    DDX_Text(pDX, IDC_BI_SIGN, m_strSign);
    DDX_Text(pDX, IDC_BI_NAME, m_strName);
	DDX_Text(pDX, IDC_BI_BIRTHDAY, m_strBirthday);
	DDX_Text(pDX, IDC_BI_LUNAR_ANIMAL, m_strLunarAnimal);
	DDX_Text(pDX, IDC_BI_HOROSCOPE, m_strHoroscope);
    DDX_Text(pDX, IDC_BI_PROV, m_strProv);
    DDX_Text(pDX, IDC_BI_CITY, m_strCity);
    DDX_Text(pDX, IDC_BI_SEX, m_strSex);
	DDX_Text(pDX, IDC_BI_BLOOD_TYPE, m_strBloodType);
    DDX_Control(pDX, IDC_BI_CBO_GROUP, m_cboGroup);
}


BEGIN_MESSAGE_MAP(CBuddyInfoDlg, CDialog)
    ON_WM_SIZE()
    ON_WM_VSCROLL()
	ON_COMMAND(IDM_EDIT, &CBuddyInfoDlg::OnEdit)
END_MESSAGE_MAP()


// CBuddyInfoDlg 消息处理程序

BOOL CBuddyInfoDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
#ifdef M8
    AddMenuBarForM8(this->GetSafeHwnd(), IDR_MENU_BUDDYINFO);
    FullScreen(this->GetSafeHwnd());
#else
	if (!m_dlgCommandBar.Create(this) ||
		!m_dlgCommandBar.InsertMenuBar(IDR_MENU_BUDDYINFO))
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
	if(fx_is_pc_user_by_id(m_lAccountID))
	{
		fx_update_account_info_by_id (m_lAccountID);
	}

	updateAccountInfo();

    InitScrollInfo();
    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}

void CBuddyInfoDlg::InitScrollInfo()
{   
    RECT rect;
    this->GetWindowRect(&rect);   
    SCROLLINFO si;   
    si.cbSize = sizeof(SCROLLINFO);   
    si.fMask = SIF_ALL; // SIF_ALL = SIF_PAGE | SIF_RANGE | SIF_POS;   
    si.nMin = 0;   
    RECT minutesRect;   
    GetDlgItem(IDC_BI_GRP_INFO)->GetWindowRect(&minutesRect);   
    si.nMax = minutesRect.bottom;   
    si.nPage =  rect.bottom - rect.top;
    if(si.nMax <= rect.bottom)   
    {   
        si.nMax = 0;   
    } 
  
    si.nPos = 0;   
    SetScrollInfo(SB_VERT, &si, TRUE);   
}

void CBuddyInfoDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);
 
    int iHeight, iWidth;
    int iMargin = DRA::SCALEY(5);

    RECT rcCtl;
    ::GetClientRect(this->m_hWnd, &rcCtl);
#ifdef M8
    rcCtl.top += 40;
#endif

    iHeight = rcCtl.bottom - rcCtl.top; //窗体高度
    iWidth = rcCtl.right - rcCtl.left;  //窗体宽度

    int iX, iY, iW, iH;
    int iWLabel, iHLabel;
    int iXValue, iWValue, iHValue;
    
    int iXGroup2, iYGroup2, iWGroup2, iHGroup2; //第二个GROUPBOX根据内容调整大小,变量单独存放

    iWLabel = DRA::SCALEX(60);
    iHLabel = DRA::SCALEX(18);

    
    iWValue = iWidth - 5 * iMargin - iWLabel;
    iHValue = DRA::SCALEX(20);

    iX = rcCtl.left + iMargin;
    iY = rcCtl.top + iMargin;
    iW = iWidth - 2*iMargin;
    iH = 2 * DRA::SCALEY(20) + 3 * iMargin + DRA::SCALEY(14);

    //这两个是GROUP

    HWND hwndctl = ::GetDlgItem(this->m_hWnd, IDC_BI_GRP_SET);
    ::MoveWindow(hwndctl,  iX, iY, iW, iH, SWP_NOZORDER | SWP_NOREDRAW);
    iXGroup2 = iX;
    iYGroup2 = iY + iMargin + iH;
    iWGroup2 = iW;


    //下面是所有的内部控件
    iX += iMargin;
    iXValue = iX + iMargin + iWLabel;
    iY = rcCtl.top + iMargin + iHValue;
    
    //开始一行
    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_BI_LB_SHOWNAME);
    ::MoveWindow(hwndctl,  iX, iY, iWLabel, iHLabel, FALSE);

    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_BI_EDT_SHOWNAME);
    ::MoveWindow(hwndctl,  iXValue, iY, iWValue, iHValue, FALSE);

    iY = iY + iHValue + iMargin ;
    //结束调整一行
    //下一行,类推
    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_BI_LB_GROUP);
    ::MoveWindow(hwndctl,  iX, iY, iWLabel, iHLabel, FALSE);

    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_BI_CBO_GROUP);
    ::MoveWindow(hwndctl,  iXValue, iY, iWValue, iHValue, FALSE);
    //换一行

    iY = iY + iHValue + 2 * iMargin + iHValue; //到下面的Group里面,多加一个iMargin

    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_BI_LB_FETIONNO);
    ::MoveWindow(hwndctl,  iX, iY, iWLabel, iHLabel, FALSE);

    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_BI_FETIONNO);
    ::MoveWindow(hwndctl,  iXValue, iY, iWValue, iHValue, FALSE);
    
    iY = iY + iHValue + iMargin ;
    //

    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_BI_LB_MOBILENO);
    ::MoveWindow(hwndctl,  iX, iY, iWLabel, iHLabel, FALSE);

    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_BI_MOBILENO);
    ::MoveWindow(hwndctl,  iXValue, iY, iWValue, iHValue, FALSE);
    
    iY = iY + iHValue + iMargin ;
    //

    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_BI_LB_NICKNAME);
    ::MoveWindow(hwndctl,  iX, iY, iWLabel, iHLabel, FALSE);

    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_BI_NICKNAME);
    ::MoveWindow(hwndctl,  iXValue, iY, iWValue, iHValue, FALSE);
    
    iY = iY + iHValue + iMargin ;
    //
    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_BI_LB_SIGN);
    ::MoveWindow(hwndctl,  iX, iY, iWLabel, iHLabel, FALSE);

    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_BI_SIGN);
    ::MoveWindow(hwndctl,  iXValue, iY, iWValue, iHValue * 3, FALSE);

	iY = iY + iHValue * 3 + iMargin ;
    //

    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_BI_LB_NAME);
    ::MoveWindow(hwndctl,  iX, iY, iWLabel, iHLabel, FALSE);

    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_BI_NAME);
    ::MoveWindow(hwndctl,  iXValue, iY, iWValue, iHValue, FALSE);
    
	iY += iHValue + iMargin;
	//
    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_BI_LB_BIRTHDAY);
    ::MoveWindow(hwndctl,  iX, iY, iWLabel, iHLabel, FALSE);

    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_BI_BIRTHDAY);
    ::MoveWindow(hwndctl,  iXValue, iY, iWValue, iHValue, FALSE);

	iY += iHValue + iMargin;
	//
    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_BI_LB_LUNAR_ANIMAL);
    ::MoveWindow(hwndctl,  iX, iY, iWLabel, iHLabel, FALSE);

    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_BI_LUNAR_ANIMAL);
    ::MoveWindow(hwndctl,  iXValue, iY, iWValue, iHValue, FALSE);

	iY += iHValue + iMargin;
	//
    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_BI_LB_HOROSCOPE);
    ::MoveWindow(hwndctl,  iX, iY, iWLabel, iHLabel, FALSE);

    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_BI_HOROSCOPE);
    ::MoveWindow(hwndctl,  iXValue, iY, iWValue, iHValue, FALSE);

    iY = iY + iHValue + iMargin ;
    //

    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_BI_LB_PROV);
    ::MoveWindow(hwndctl,  iX, iY, iWLabel, iHLabel, FALSE);

    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_BI_PROV);
    ::MoveWindow(hwndctl,  iXValue, iY, iWValue, iHValue, FALSE);
    
    iY = iY + iHValue + iMargin ;
    //
    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_BI_LB_CITY);
    ::MoveWindow(hwndctl,  iX, iY, iWLabel, iHLabel, FALSE);

    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_BI_CITY);
    ::MoveWindow(hwndctl,  iXValue, iY, iWValue, iHValue, FALSE);
    
    iY = iY + iHValue + iMargin ;
    //

    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_BI_LB_SEX);
    ::MoveWindow(hwndctl,  iX, iY, iWLabel, iHLabel, FALSE);

    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_BI_SEX);
    ::MoveWindow(hwndctl,  iXValue, iY, iWValue, iHValue, FALSE);
    
	iY += iHValue + iMargin;
	//
    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_BI_LB_BLOOD_TYPE);
    ::MoveWindow(hwndctl,  iX, iY, iWLabel, iHLabel, FALSE);

    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_BI_BLOOD_TYPE);
    ::MoveWindow(hwndctl,  iXValue, iY, iWValue, iHValue, FALSE);
    
    iHGroup2 = iY + iHValue + iMargin - iYGroup2;

    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_BI_GRP_INFO);
    ::MoveWindow(hwndctl, iXGroup2, iYGroup2, iWGroup2, iHGroup2, TRUE);

    InitScrollInfo();
}


void CBuddyInfoDlg::InitGroupItem(void)
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
			}
		}
		tmp_group = d_list_next(tmp_group);
	}
}

void CBuddyInfoDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    SCROLLINFO si = {   sizeof(SCROLLINFO),    
        SIF_PAGE|SIF_POS|SIF_RANGE|SIF_TRACKPOS,    
        0, 0, 0, 0,    
        0   
    };   
    int imi = si.nPos;   
    GetScrollInfo(SB_VERT, &si);   
    switch (nSBCode)   
    {           
    case SB_TOP:   
        si.nPos = si.nMin;   
        break;   
    case SB_BOTTOM:   
        si.nPos = si.nMax - si.nPage;   
        break;   
    case SB_LINEDOWN:   
        if(si.nPos + (int)si.nPage < si.nMax)   
        {   
            SetScrollPos(SB_VERT, GetScrollPos(SB_VERT)+5);   
            ::ScrollWindowEx(m_hWnd,0,-5,NULL, NULL,NULL, NULL,SW_ERASE|SW_SCROLLCHILDREN);       
        }   
        break;           
    case SB_LINEUP:    
        if(si.nPos > si.nMin)   
        {   
            SetScrollPos(SB_VERT, GetScrollPos(SB_VERT)-5);   
            ::ScrollWindowEx(m_hWnd,0,5,NULL, NULL,NULL, NULL,SW_ERASE|SW_SCROLLCHILDREN);       
        }   
        if(si.nPos < si.nMin)   
            si.nPos = si.nMin;         
        break;        
    case SB_THUMBPOSITION:   
        SetScrollPos(SB_VERT, GetScrollPos(SB_VERT) + nPos - si.nPos);   
        ::ScrollWindowEx(m_hWnd,0,si.nPos - nPos,NULL, NULL,NULL, NULL,SW_ERASE|SW_SCROLLCHILDREN);       
        break;   
    case SB_PAGEUP:   
        if(si.nPos > si.nMin)   
        {   
            SetScrollPos(SB_VERT, GetScrollPos(SB_VERT)-5);   
            ::ScrollWindowEx(m_hWnd,0,5,NULL, NULL,NULL, NULL,SW_ERASE|SW_SCROLLCHILDREN);       
        }   
        if(si.nPos < si.nMin)   
            si.nPos = si.nMin;         
        break;        
  
    case SB_PAGEDOWN:   
        if(si.nPos + (int)si.nPage < si.nMax)   
        {   
            SetScrollPos(SB_VERT, GetScrollPos(SB_VERT)+5);   
            ::ScrollWindowEx(m_hWnd,0,-5,NULL, NULL,NULL, NULL,SW_ERASE|SW_SCROLLCHILDREN);       
        }   
        break;     
    }

    CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CBuddyInfoDlg::OnEdit()
{
    UpdateData();
    int iNewGroupID = 0;
	if(m_cboGroup.GetCurSel() >= 0)
	{
		iNewGroupID = m_cboGroup.GetItemData(m_cboGroup.GetCurSel());
	}
    if(m_iGroupID != iNewGroupID)
    {
        fx_move_group_buddy_by_id(m_lAccountID, iNewGroupID, NULL, NULL);
    }

    if(m_strShowName != ConvertUtf8ToUtf16(m_account->local_name))
    {
		CStringA showName = ConvertUtf16ToUtf8(m_strShowName);
		fx_set_buddyinfo(m_lAccountID, showName.GetBuffer(), NULL, NULL); 
    }

    SendMessage(WM_CLOSE,0, 0);
}

void CBuddyInfoDlg::updateAccountInfo()
{
	m_account = fx_get_account_by_id(m_lAccountID);
    if(NULL == m_account)
        return;

	m_strShowName =  ConvertUtf8ToUtf16(m_account->local_name);

    m_iGroupID = fx_get_account_group_id(m_account);

	if(fx_is_pc_user_by_id(m_lAccountID))
	{
		//飞信用户
		m_strAccountID.Format(_T("%d"), m_lAccountID);
		if (m_account->personal)
		{
			m_strMobileNo = ConvertUtf8ToUtf16(m_account->personal->mobile_no);
		}
	}
	else
	{
		//手机用户
		m_strAccountID = _T("");
		//获取显示正确的号码，如果是手机用户，则返回手机号码，其它返回飞信号
		char* original = fx_get_original_ID(m_lAccountID);
		m_strMobileNo =  ConvertUtf8ToUtf16(original);
		free(original);
	}

// in some case, the m_account->personal maybe is NULL.
	if (m_account->personal)
	{
		m_strNickName = ConvertUtf8ToUtf16(m_account->personal->nickname);
		m_strSign = ConvertUtf8ToUtf16(m_account->personal->impresa);
		m_strName = ConvertUtf8ToUtf16(m_account->personal->name);
		if(m_account->personal->birthday_valid)
		{
			m_strBirthday = m_account->personal->birth_date;
		}
		m_strLunarAnimal = GetLunarAnimal(m_account->personal->lunar_animal);
		m_strHoroscope = GetHoroscope(m_account->personal->horoscope);
		m_strProv = GetProvince( ConvertUtf8ToUtf16(m_account->personal->province));
		m_strCity = GetCity(m_account->personal->city);
		m_strSex = GetSex(m_account->personal->gender);
		m_strBloodType = GetBloodType(m_account->personal->blood_type);
	}

	for(int i = 0; i < m_cboGroup.GetCount(); i++)
    {
        if(m_iGroupID == m_cboGroup.GetItemData(i))
        {
            m_cboGroup.SetCurSel(i);
            break;
        }
    }
	UpdateData(FALSE);
}

LRESULT CBuddyInfoDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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

