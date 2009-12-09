// MyselfInfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WMLF.h"
#include "MyselfInfoDlg.h"
#include <tpcshell.h>

#define TIMER_GET_USR_SCORE 50

// CMyselfInfoDlg 对话框

IMPLEMENT_DYNAMIC(CMyselfInfoDlg, CDialog)

CMyselfInfoDlg::CMyselfInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMyselfInfoDlg::IDD, pParent)
	, m_strAccountID(_T(""))
	, m_strScore(_T(""))
	, m_strBirthday(_T(""))
	, m_strLunarAnimal(_T(""))
	, m_strHoroscope(_T(""))
	, m_strProv(_T(""))
	, m_strCity(_T(""))
	, m_strSex(_T(""))
	, m_strBloodType(_T(""))
	, m_strMobileNo(_T(""))
	, m_strNickName(_T(""))
	, m_strSign(_T(""))
{

}

CMyselfInfoDlg::~CMyselfInfoDlg()
{
}

void CMyselfInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MI_EDIT_NICKNAME, m_strNickName);
	DDX_Text(pDX, IDC_MI_EDIT_SIGN, m_strSign);
	DDX_Text(pDX, IDC_MI_FETIONNO, m_strAccountID);
	DDX_Text(pDX, IDC_MI_MOBILENO, m_strMobileNo);
	DDX_Text(pDX, IDC_MI_SCORE, m_strScore);
	DDX_Text(pDX, IDC_MI_BIRTHDAY, m_strBirthday);
	DDX_Text(pDX, IDC_MI_LUNAR_ANIMAL, m_strLunarAnimal);
	DDX_Text(pDX, IDC_MI_HOROSCOPE, m_strHoroscope);
	DDX_Text(pDX, IDC_MI_PROV, m_strProv);
	DDX_Text(pDX, IDC_MI_CITY, m_strCity);
	DDX_Text(pDX, IDC_MI_SEX, m_strSex);
	DDX_Text(pDX, IDC_MI_BLOOD_TYPE, m_strBloodType);
}


BEGIN_MESSAGE_MAP(CMyselfInfoDlg, CDialog)
	ON_WM_TIMER()
	ON_WM_VSCROLL()
	ON_WM_SIZE()
	ON_COMMAND(IDM_EDIT, &CMyselfInfoDlg::OnEdit)
END_MESSAGE_MAP()


// CMyselfInfoDlg 消息处理程序

BOOL CMyselfInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	if (!m_dlgCommandBar.Create(this) ||
		!m_dlgCommandBar.InsertMenuBar(IDR_MENU_MYSELF_INFO))
	{
		TRACE0("未能创建 CommandBar\n");
		return FALSE;      // 未能创建
	}
#ifdef WIN32_PLATFORM_WFSP
    //重写后退键，引发WM_HOTKEY消息
    (void)::SendMessage(SHFindMenuBar (m_hWnd), SHCMBM_OVERRIDEKEY, VK_TBACK,
        MAKELPARAM(SHMBOF_NODEFAULT | SHMBOF_NOTIFY,
        SHMBOF_NODEFAULT | SHMBOF_NOTIFY));
	//如果文本框为多行的则必须加入下面这行代码，单行的则不用~
	::SendMessage(::GetDlgItem(this->m_hWnd, IDC_MI_EDIT_SIGN), EM_SETEXTENDEDSTYLE, 0, ES_EX_CLEARONBACKPRESSHOLD);
#endif

	const Fetion_Personal * pInfo = fx_data_get_PersonalInfo();

	m_strAccountID = fx_get_usr_uid();
	int iScore = fx_get_usr_score();
	if(iScore >= 0)
	{
		m_strScore.Format(_T("%d"), iScore);
	}
	else
	{
		SetTimer(TIMER_GET_USR_SCORE, 1000, NULL);
	}

	if(pInfo)
	{
		m_strNickName = ConvertUtf8ToUtf16(pInfo->nickname);
		m_strNickNameBackup = m_strNickName;
		m_strSign = ConvertUtf8ToUtf16(pInfo->impresa);
		m_strSignBackup = m_strSign;
		if(pInfo->mobile_no)
		{
			m_strMobileNo = ConvertUtf8ToUtf16(pInfo->mobile_no);
		}
		if(pInfo->birthday_valid)
		{
			m_strBirthday = pInfo->birth_date;
		}
		m_strLunarAnimal = GetLunarAnimal(pInfo->lunar_animal);
		m_strHoroscope = GetHoroscope(pInfo->horoscope);
		m_strProv = GetProvince(ConvertUtf8ToUtf16(pInfo->province));
		m_strCity = GetCity(pInfo->city);
		m_strSex = GetSex(pInfo->gender);
		m_strBloodType = GetBloodType(pInfo->blood_type);
	}
	UpdateData(FALSE);
	InitScrollInfo();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CMyselfInfoDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch(nIDEvent)
	{
	case TIMER_GET_USR_SCORE:
		{
			int iScore = fx_get_usr_score();
			if(iScore >= 0)
			{
				m_strScore.Format(_T("%d"), iScore);
				UpdateData(FALSE);
				KillTimer(TIMER_GET_USR_SCORE);
			}
		}
		break;
	default:
		break;
	}

	CDialog::OnTimer(nIDEvent);
}

void CMyselfInfoDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
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

void CMyselfInfoDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
    int iHeight, iWidth;
    int iMargin = DRA::SCALEY(5);

    RECT rcCtl;
    ::GetClientRect(this->m_hWnd, &rcCtl);

    iHeight = rcCtl.bottom - rcCtl.top; //窗体高度
    iWidth = rcCtl.right - rcCtl.left;  //窗体宽度

    int iX, iY;
    int iWLabel, iHLabel;
    int iXValue, iWValue, iHValue;
    
    int iXGroup1, iYGroup1, iWGroup1, iHGroup1; //第一个GROUPBOX根据内容调整大小,变量单独存放
    int iXGroup2, iYGroup2, iWGroup2, iHGroup2; //第二个GROUPBOX根据内容调整大小,变量单独存放

    iWLabel = DRA::SCALEX(60);
    iHLabel = DRA::SCALEX(18);

    iWValue = iWidth - 5 * iMargin - iWLabel;
    iHValue = DRA::SCALEX(20);

    iX = rcCtl.left + iMargin;
    iY = rcCtl.top + iMargin;

    iXGroup1 = iX;
	iYGroup1 = iY;
    iWGroup1 = iWidth - 2*iMargin;

    iX += iMargin;
	iY += iHValue;
    iXValue = iX + iWLabel + iMargin;
    //第一个GROUP第一行
    HWND hwndctl = ::GetDlgItem(this->m_hWnd, IDC_MI_LB_NICKNAME);
    ::MoveWindow(hwndctl,  iX, iY, iWLabel, iHLabel, FALSE);

    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_MI_EDIT_NICKNAME);
    ::MoveWindow(hwndctl,  iXValue, iY, iWValue, iHValue, FALSE);

	iY += iHValue + iMargin;
    //结束调整一行
    //下一行,类推
    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_MI_LB_SIGN);
    ::MoveWindow(hwndctl,  iX, iY, iWLabel, iHLabel, FALSE);

    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_MI_EDIT_SIGN);
    ::MoveWindow(hwndctl,  iXValue, iY, iWValue, iHValue * 3, FALSE);

    iHGroup1 = iY + iHValue * 3 + iMargin - iYGroup1;

	//第一个GROUP
    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_MI_GRP_SET);
    ::MoveWindow(hwndctl,  iXGroup1, iYGroup1, iWGroup1, iHGroup1, SWP_NOZORDER | SWP_NOREDRAW);

    iXGroup2 = iXGroup1;
    iYGroup2 = iYGroup1 + iHGroup1 + iMargin;
    iWGroup2 = iWGroup1;

	iY = iYGroup2 + iHValue;
	//第二个GROUP第一行
    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_MI_LB_FETIONNO);
    ::MoveWindow(hwndctl,  iX, iY, iWLabel, iHLabel, FALSE);

    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_MI_FETIONNO);
    ::MoveWindow(hwndctl,  iXValue, iY, iWValue, iHValue, FALSE);

	iY += iHValue + iMargin;
	//
    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_MI_LB_MOBILENO);
    ::MoveWindow(hwndctl,  iX, iY, iWLabel, iHLabel, FALSE);

    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_MI_MOBILENO);
    ::MoveWindow(hwndctl,  iXValue, iY, iWValue, iHValue, FALSE);

	iY += iHValue + iMargin;
	//
    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_MI_LB_SCORE);
    ::MoveWindow(hwndctl,  iX, iY, iWLabel, iHLabel, FALSE);

    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_MI_SCORE);
    ::MoveWindow(hwndctl,  iXValue, iY, iWValue, iHValue, FALSE);

	iY += iHValue + iMargin;
	//
    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_MI_LB_BIRTHDAY);
    ::MoveWindow(hwndctl,  iX, iY, iWLabel, iHLabel, FALSE);

    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_MI_BIRTHDAY);
    ::MoveWindow(hwndctl,  iXValue, iY, iWValue, iHValue, FALSE);

	iY += iHValue + iMargin;
	//
    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_MI_LB_LUNAR_ANIMAL);
    ::MoveWindow(hwndctl,  iX, iY, iWLabel, iHLabel, FALSE);

    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_MI_LUNAR_ANIMAL);
    ::MoveWindow(hwndctl,  iXValue, iY, iWValue, iHValue, FALSE);

	iY += iHValue + iMargin;
	//
    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_MI_LB_HOROSCOPE);
    ::MoveWindow(hwndctl,  iX, iY, iWLabel, iHLabel, FALSE);

    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_MI_HOROSCOPE);
    ::MoveWindow(hwndctl,  iXValue, iY, iWValue, iHValue, FALSE);

	iY += iHValue + iMargin;
	//
    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_MI_LB_PROV);
    ::MoveWindow(hwndctl,  iX, iY, iWLabel, iHLabel, FALSE);

    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_MI_PROV);
    ::MoveWindow(hwndctl,  iXValue, iY, iWValue, iHValue, FALSE);

	iY += iHValue + iMargin;
	//
    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_MI_LB_CITY);
    ::MoveWindow(hwndctl,  iX, iY, iWLabel, iHLabel, FALSE);

    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_MI_CITY);
    ::MoveWindow(hwndctl,  iXValue, iY, iWValue, iHValue, FALSE);

	iY += iHValue + iMargin;
	//
    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_MI_LB_SEX);
    ::MoveWindow(hwndctl,  iX, iY, iWLabel, iHLabel, FALSE);

    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_MI_SEX);
    ::MoveWindow(hwndctl,  iXValue, iY, iWValue, iHValue, FALSE);

	iY += iHValue + iMargin;
	//
    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_MI_LB_BLOOD_TYPE);
    ::MoveWindow(hwndctl,  iX, iY, iWLabel, iHLabel, FALSE);

    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_MI_BLOOD_TYPE);
    ::MoveWindow(hwndctl,  iXValue, iY, iWValue, iHValue, FALSE);

	iHGroup2 = iY + iHValue + iMargin - iYGroup2;
	//第二个GROUP
    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_MI_GRP_INFO);
    ::MoveWindow(hwndctl,  iXGroup2, iYGroup2, iWGroup2, iHGroup2, SWP_NOZORDER | SWP_NOREDRAW);
	InitScrollInfo();
}

void CMyselfInfoDlg::InitScrollInfo()
{   
    RECT rect;
    this->GetWindowRect(&rect);   
    SCROLLINFO si;   
    si.cbSize = sizeof(SCROLLINFO);   
    si.fMask = SIF_ALL; // SIF_ALL = SIF_PAGE | SIF_RANGE | SIF_POS;   
    si.nMin = 0;   
    RECT minutesRect;   
    GetDlgItem(IDC_MI_GRP_INFO)->GetWindowRect(&minutesRect);   
    si.nMax = minutesRect.bottom;   
    si.nPage =  rect.bottom - rect.top;
    if(si.nMax <= rect.bottom)   
    {   
        si.nMax = 0;   
    } 
  
    si.nPos = 0;   
    SetScrollInfo(SB_VERT, &si, TRUE);   
}

void CMyselfInfoDlg::OnEdit()
{
	// TODO: 在此添加命令处理程序代码
    UpdateData();
    if(m_strNickName != m_strNickNameBackup)
    {
		CStringA NickName = ConvertUtf16ToUtf8(m_strNickName);
		fx_set_user_nickname(NickName.GetBuffer(), NULL, NULL);
    }
    if(m_strSign != m_strSignBackup)
    {
		CStringA Sign = ConvertUtf16ToUtf8(m_strSign);
		fx_set_user_impresa(Sign.GetBuffer(), NULL, NULL);
    }
	SendMessage(WM_CLOSE,0, 0);
}

LRESULT CMyselfInfoDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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
