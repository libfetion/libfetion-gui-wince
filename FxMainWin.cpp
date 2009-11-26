/***************************************************************************
 *   Copyright (C) 2008 by DDD                                          *
 *   dedodong@163.com                                                     *
 *                                                                         *
 ***************************************************************************/

// FxMainWin.cpp : 实现文件
#include "stdafx.h"
#include "WMLF.h"
#include "FxMainWin.h"
#include "LoginDlg.h"
#include "FxBuddy.h"
#include "FindBuddyDlg.h"
#include "Notify.h"
#include "BuddyInfoDlg.h"
#include "About.h"
#include "IniWR.h"
#include "FxDatabase.h"

#ifdef M8
#include "M8Misc.h"
#endif

extern CFxDatabase * g_pFxDB;

#define TIMER_NEWMSG  15
#define TIMER_RELOGIN 20
#define TIMER_UPDATE_ACCOUNTINFO 25
#define TIMER_ADD_ITEM_TO_TREE 30

void  Sys_EventListener (int message, WPARAM wParam, LPARAM lParam, void* args)
{
	if(!args)
		return;
	FxMainWin * mainWind = (FxMainWin *) args;
	if (message == FX_NEW_MESSAGE)
		::SendMessage(mainWind->m_hWnd, message + WM_USER, wParam, lParam);
	else
		::PostMessage(mainWind->m_hWnd, message + WM_USER, wParam, lParam);
}

BOOL FxMainWin::handleFx_Sys_Event(int message, WPARAM wParam, LPARAM lParam)
{
	if (message < WM_USER)
		return FALSE;

	switch(message - WM_USER)
	{
	case FX_CURRENT_VERSION:
		//emit signal_Current_Version((int)wParam);
		return TRUE;
	case FX_ADDACCOUNT_APP:
		handle_AddAccountApp((char*)(lParam), (char*)wParam);
		return TRUE;
	case FX_MOVE_GROUP_OK:
		handle_MoveGroupOk((long)lParam, (int)wParam);
		return TRUE;
	case FX_NEW_MESSAGE:
		addNewMessage(long(lParam));
		return TRUE;
	case FX_NEW_QUN_MESSAGE:
		addNewQunMessage(long(lParam));
		return TRUE;
	case FX_SET_STATE_OK:
		//emit signal_set_state((int)wParam);
		m_strNickNameShow = m_strNickName + GetUserStateString();
		this->UpdateData(FALSE);
		return TRUE;

	case FX_STATUS_ONLINE:
        //如果触发事件的不是当前用户,则提示用户上线
        if(m_lAccountID != lParam)
            NotifyUser(2, (long)lParam, L"");

    case FX_SET_BUDDY_INFO_OK:
	case FX_STATUS_OFFLINE: 
	case FX_STATUS_BUSY: 
	case FX_STATUS_AWAY: 
	case FX_STATUS_MEETING:
	case FX_STATUS_PHONE:  
	case FX_STATUS_DINNER: 
	case FX_STATUS_EXTENDED_AWAY:
	case FX_STATUS_NUM_PRIMITIVES:
	case FX_ACCOUNT_UPDATA_OK:
	case FX_REMOVE_BLACKLIST_OK:
	case FX_ADD_BLACKLIST_OK:
	case FX_STATUS_SMSEXTENED:
		this->m_BuddyOpt->updateAccountInfo(lParam);
		if(m_BuddyInfoDlg && (m_BuddyInfoDlg->m_lAccountID == lParam))
		{
			m_BuddyInfoDlg->updateAccountInfo();
		}
		m_pFxMsgDlgView->updateAccountInfo(lParam);
		return TRUE;

	case FX_SET_REFUSE_SMS_DAY_OK:
		//emit signal_UpdateSmsDay((wParam));
		return TRUE;

	case FX_SYS_ERR_NETWORK:
		hand_SystemNetErr( wParam);
		return TRUE;

	case FX_SYS_DEREGISTERED:
		MessageBox(_T("您已经在其它地方登录，程序将退出！"), _T("LibFetion"), MB_ICONSTOP);
		::exit(0); //fixed: no release the res of this app...
		return TRUE;

	case FX_DIA_SEND_OK: 
	case FX_DIA_SEND_FAIL: 
	case FX_DIA_SEND_TIMEOUT: 
	case FX_SMS_OK: 
	case FX_SMS_FAIL: 
	case FX_SMS_FAIL_LIMIT: 
	case FX_SMS_TIMEOUT: 
	case FX_QUN_SEND_OK: 
	case FX_QUN_SEND_FAIL: 
	case FX_QUN_SEND_TIMEOUT: 
	case FX_QUN_SMS_OK: 
	case FX_QUN_SMS_FAIL: 
	case FX_QUN_SMS_FAIL_LIMIT: 
	case FX_QUN_SMS_TIMEOUT: 
		hand_MsgSend_Event( message - WM_USER, wParam, (long)lParam);
		return TRUE;

	case FX_ADD_BUDDY_OK:
		//this->m_BuddyOpt->delAccount_direct(account_id);  
		this->m_BuddyOpt->addAccountToGroup( fx_get_account_by_id((long)lParam) );
		update_account_info();
		return TRUE;

	case FX_RENAME_GROUP_OK:
		//emit signal_reName_group( message, wParam, (qlonglong)lParam);
		return TRUE;
	//case FX_SET_BUDDY_INFO_OK:
        //这个消息被放到上面与更新状态等消息一并处理
		//emit signal_reName_buddy( message, wParam, (qlonglong)lParam);
		return TRUE;
	case FX_ADD_GROUP_OK:
		//emit signal_add_group( message, wParam, (qlonglong)lParam);
		return TRUE;
	case FX_DEL_GROUP_OK:
		//emit signal_del_group( message, wParam, (qlonglong)lParam);
		return TRUE;
	case FX_DEL_BUDDY_OK:
		this->m_BuddyOpt->delAccount_direct((long)lParam);  
		return TRUE;

	case FX_ADD_GROUP_FAIL:
	case FX_DEL_GROUP_FAIL:
	case FX_SET_BUDDY_INFO_FAIL:
	case FX_RENAME_GROUP_FAIL:  //ignored this message
		if(wParam)
			free((char*)(int)wParam);
		return TRUE;

	case FX_SET_NICKNAME_OK:
		//emit signal_set_nickname_ok();
		return TRUE;

#if 0 //follow message is ignored
	case FX_MOVE_GROUP_FAIL:
	case FX_REMOVE_BLACKLIST_FAIL:
	case FX_ADD_BLACKLIST_FAIL:
	case FX_REMOVE_BLACKLIST_TIMEOUT:
	case FX_ADD_BLACKLIST_TIMEOUT:
	case FX_RENAME_GROUP_TIMEOUT:
	case FX_SET_BUDDY_INFO_TIMEOUT:
	case FX_SET_REFUSE_SMS_DAY_FAIL:
#endif

		//for relogin message. note: not for login
	case FX_LOGIN_URI_ERROR:
	case FX_LOGIN_FAIL:
	case FX_LOGIN_NETWORK_ERROR:
	case FX_LOGIN_UNKOWN_ERROR :   
	case FX_LOGIN_UNKOWN_USR:
	case FX_LOGIN_GP_FAIL:
		relogin_fetion();
		break;
	case FX_LOGIN_OK :
		KillTimer(TIMER_RELOGIN);
		relogin_ok();
		break;
//for relogin message. note: not for login

	default:
		break;
	}
	return FALSE;
}

// FxMainWin 对话框

IMPLEMENT_DYNAMIC(FxMainWin, CDialog)

FxMainWin::FxMainWin(CWnd* pParent /*=NULL*/)
	: CDialog(FxMainWin::IDD, pParent)
	, loginDlg(NULL)
	, m_BuddyOpt(NULL)
	, m_BuddyInfoDlg(NULL)
	, m_isLoginOK(FALSE)
    , m_strNickName(_T(""))
    , m_strNickNameShow(_T(""))
    , m_strSign(_T(""))
    , m_strStartupPath(_T(""))
    , m_lAccountID(0)
	, m_mobile_no(_T(""))
	, m_pFxMsgDlgView(NULL)
	//, m_pFxDB(NULL)
{
}

FxMainWin::~FxMainWin()
{
	if(NULL != m_pFxMsgDlgView)
	{
		delete m_pFxMsgDlgView;
		m_pFxMsgDlgView = NULL;
	}
	if(NULL != g_pFxDB)
	{
		delete g_pFxDB;
		g_pFxDB = NULL;
	}

}

void FxMainWin::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_TREE_BUDDY, view);
    DDX_Text(pDX, IDC_INFO_NAME, m_strNickNameShow);
    DDX_Text(pDX, IDC_INFO_SIGN, m_strSign);
}


BEGIN_MESSAGE_MAP(FxMainWin, CDialog)
    ON_WM_SIZE()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_CLOSE()
	ON_WM_TIMER()
    ON_COMMAND(IDM_MAIN_SENDMSG, &FxMainWin::OnSendMsg)
    ON_STN_CLICKED(IDC_BTN_ADD, &FxMainWin::OnStnClickedBtnAdd)
    ON_COMMAND(IDM_MAIN_SET_VIBR, &FxMainWin::OnMainSetVibr)
    ON_COMMAND(IDM_MAIN_SET_SILENCE, &FxMainWin::OnMainSetSilence)
    ON_UPDATE_COMMAND_UI(IDM_MAIN_SET_SILENCE, &FxMainWin::OnUpdateMainSetSilence)
    ON_UPDATE_COMMAND_UI(IDM_MAIN_SET_VIBR, &FxMainWin::OnUpdateMainSetVibr)
	ON_COMMAND(IDM_MAIN_SET_ONLINE,&FxMainWin::OnMainSetOnline)
	ON_UPDATE_COMMAND_UI(IDM_MAIN_SET_ONLINE,&FxMainWin::OnUpdateMainSetOnline)
	ON_COMMAND(IDM_MAIN_SET_SHIELDQUNMESSAGE,&FxMainWin::OnMainShieldQunMessage)
	ON_UPDATE_COMMAND_UI(IDM_MAIN_SET_SHIELDQUNMESSAGE,&FxMainWin::OnUpdateShieldQunMessage)
    ON_COMMAND(IDM_BD_VIEWINFO, &FxMainWin::OnBdViewinfo)
    ON_WM_INITMENUPOPUP()
//    ON_COMMAND(IDM_MAIN_ADDBUDDY, &FxMainWin::OnMainAddbuddy)
ON_COMMAND(IDM_ABOUT, &FxMainWin::OnAbout)
ON_COMMAND(IDM_BD_DELETE, &FxMainWin::OnBdDelete)
ON_COMMAND(IDM_BD_ADDBLACKLIST, &FxMainWin::OnBdAddblacklist)
ON_COMMAND(IDM_BD_RMBLACKLIST, &FxMainWin::OnBdRmblacklist)
ON_COMMAND(IDM_MAIN_SET_NOSOUND, &FxMainWin::OnMainSetNosound)
ON_COMMAND(IDM_MAIN_SHOWNEWMSG, &FxMainWin::OnMainShownewmsg)
ON_COMMAND(IDM_BD_SENDMSG, &FxMainWin::OnBdSendmsg)
ON_COMMAND(IDM_MAIN_ADDBUDDY, &FxMainWin::OnMainAddbuddy)
ON_COMMAND(IDM_BD_MOVEGROUP, &FxMainWin::OnBdMovegroup)
ON_COMMAND(IDM_MAIN_CLEAN, &FxMainWin::OnMainClean)
ON_COMMAND(IDM_SEND_MYSELF, &FxMainWin::OnSendMyself)
ON_COMMAND(IDM_UPDATE_ALL_ACCOUNTINFO, &FxMainWin::OnUpdateAllAccountinfo)
END_MESSAGE_MAP()

#ifdef WIN32_PLATFORM_WFSP
WNDPROC prevProc;  //取得原窗口过程函数指针
CTreeCtrl *g_cTree;  //CTreeCtrl指针，用于获取Dlg中的控件

LRESULT CALLBACK MyTreeProc(
							HWND hwnd,      // handle to window
							UINT uMsg,      // message identifier
							WPARAM wParam,  // first message parameter
							LPARAM lParam   // second message parameter
							)
{
    if(WM_GETDLGCODE == uMsg)
    {
        return   DLGC_WANTMESSAGE; 
    }
    else if(WM_KEYDOWN == uMsg)
    {
        switch(wParam)  //确认键按下
        {
        case '5':
        //case VK_RETURN:
            {
                //展开选中节点
                HTREEITEM hItem = g_cTree->GetSelectedItem();
                if ((hItem != NULL) && g_cTree->ItemHasChildren(hItem))
                {
                    g_cTree->Expand(hItem, TVE_TOGGLE);
                    /*
                    if ((g_cTree->GetItemState(hItem, TVIS_EXPANDED) & TVIS_EXPANDED) == FALSE) //未展开
                    {
                    g_cTree->Expand(hItem, TVE_EXPAND);
                    g_cTree->EnsureVisible(hItem);
                    }
                    else  //展开
                    {
                    g_cTree->Expand(hItem, TVE_COLLAPSE);
                    }
                    */
                }
                break;
            }
        case '0':
            ShowWindow(::GetParent(hwnd), SW_MINIMIZE );
            break;
        case '1':
            g_cTree->SelectItem(g_cTree->GetFirstVisibleItem());
            break;
        case '2': 
            {
                HTREEITEM hItem = g_cTree->GetNextItem(g_cTree->GetSelectedItem(),TVGN_PREVIOUSVISIBLE);
                if(hItem)
                {
                    g_cTree->SelectItem(hItem);
                }
                break;
            }
        case '4':
            keybd_event(VK_LEFT, 0, KEYEVENTF_EXTENDEDKEY, 0);
            keybd_event(VK_LEFT, 0, KEYEVENTF_KEYUP, 0);
            break;
        case '6':
            keybd_event(VK_RIGHT, 0, KEYEVENTF_EXTENDEDKEY, 0);
            keybd_event(VK_RIGHT, 0, KEYEVENTF_KEYUP, 0);
            break;
        case '7':
            {
                HTREEITEM hItem = g_cTree->GetNextVisibleItem(g_cTree->GetSelectedItem());
                if(hItem)
                {
                    while(1)
                    {
                        if(NULL == g_cTree->GetNextVisibleItem(hItem))
                            break;
                        hItem = g_cTree->GetNextVisibleItem(hItem);
                    }
                    g_cTree->SelectItem(hItem);
                }
                break;
            }
        case '8':
            {
                HTREEITEM hItem = g_cTree->GetNextItem(g_cTree->GetSelectedItem(),TVGN_NEXTVISIBLE);
                if(hItem)
                {
                    g_cTree->SelectItem(hItem);
                }
                break;
            }
        case '3':
            break;
        case '9':
            keybd_event(0x5b, 0, KEYEVENTF_EXTENDEDKEY, 0);
            keybd_event(0x5b, 0, KEYEVENTF_KEYUP, 0);
            break;
        case VK_TBACK:
            break;
        }
    }
    return prevProc(hwnd,uMsg,wParam,lParam);
}
#endif // WIN32_PLATFORM_WFSP

// FxMainWin 消息处理程序

BOOL FxMainWin::OnInitDialog()
{
    fx_init();
	do_login();
	CDialog::OnInitDialog();
	//set the fetion system msg call back function
	fx_set_system_msg_cb (Sys_EventListener, this);

	::SHDoneButton(this->m_hWnd,SHDB_HIDE);

	m_pFxMsgDlgView = new CFxMsgDlgView(this);
	m_pFxMsgDlgView->Create(IDD_WMLF_MSG_VIEW, this);
	m_pFxMsgDlgView->ModifyStyleEx(0, WS_EX_CONTROLPARENT);

#ifdef WIN32_PLATFORM_WFSP
	g_cTree = &view;  
	prevProc=(WNDPROC)SetWindowLong(view.m_hWnd,GWL_WNDPROC,(LONG)MyTreeProc);
#endif // WIN32_PLATFORM_WFSP

	//#ifdef WIN32_PLATFORM_WFSP
#ifndef M8
	if (!m_dlgCommandBar.Create(this) ||
		!m_dlgCommandBar.InsertMenuBar(IDR_MAIN_MENU))
	{
		TRACE0("未能创建 CommandBar\n");
		return FALSE;      // 未能创建
	}
#else
    AddMenuBarForM8(this->GetSafeHwnd(), IDR_MAIN_MENU);
    FullScreen(this->GetSafeHwnd());

#endif
//#endif // WIN32_PLATFORM_WFSP
	UpdateData(FALSE);

	g_pFxDB = new CFxDatabase;
	if(!g_pFxDB->Init(m_strStartupPath + _T("\\Users\\") + m_mobile_no + _T("\\") + m_mobile_no + _T(".db")))
	//if(!g_pFxDB->Init(_T("\\") + m_mobile_no + _T(".db")))
	{
		MessageBox(_T("打开数据库文件失败，程序将退出！"), _T("LibFetion"), MB_ICONSTOP);
		//::exit(0);
	}

	m_BuddyOpt = new BuddyOpt(&view);
  
	// TODO: 在此添加额外的初始化代码
	SetTimer(TIMER_ADD_ITEM_TO_TREE, 1, NULL);

    return TRUE;  // return TRUE unless you set the focus to a control
}

void FxMainWin::do_login()
{
    GetStartupPath();
	loginDlg = new CLoginDlg;
	loginDlg->m_strStartupPath = m_strStartupPath;

	ZeroMemory(&m_ConnInfo, sizeof(m_ConnInfo));
	m_ConnInfo.cbSize      = sizeof(m_ConnInfo);
	m_ConnInfo.dwParams    = CONNMGR_PARAM_GUIDDESTNET; 
	m_ConnInfo.dwPriority  = CONNMGR_PRIORITY_USERINTERACTIVE; 
	loginDlg->m_pConnInfo = &m_ConnInfo;

	loginDlg->DoModal();

	//if login false, will exit the application
	if (loginDlg->m_LoginFlag != TRUE)
	{
		m_isLoginOK = FALSE;
		//AfxMessageBox(_T("will quit"));
		fx_terminate();
		::exit(0);
	} else {
		m_isLoginOK = TRUE;
	}
#if DEBUG_GUI
    m_lAccountID = 0;
    m_strNickName = _T("测试");
    m_strSign = _T("落花不是无情物");
#else
    const Fetion_Personal * pInfo = fx_data_get_PersonalInfo();
    m_strNickName = ConvertUtf8ToUtf16(pInfo->nickname);
	m_strNickNameShow = m_strNickName + GetUserStateString();
    m_strSign = ConvertUtf8ToUtf16(pInfo->impresa);
    CStringA tmp_id = ConvertUtf16ToUtf8(loginDlg->m_fetion_id);
	m_mobile_no = loginDlg->m_mobile_no;
	m_lAccountID = atol(tmp_id.GetBuffer());

	{
		//读取设置信息
		//新消息提醒
		m_bSound = GetSettingFromIni(_T("Sound"), 1);
		//震动提醒
		m_bVibrate = GetSettingFromIni(_T("Vibrate"));
		//上线提醒
		m_bOnline = GetSettingFromIni(_T("Online"));
		//无提醒
		m_bSilence = GetSettingFromIni(_T("Silence"));
		//屏蔽群消息
		m_bShieldQunMessage=GetSettingFromIni(_T("ShieldQunMessage"));
	}
#endif
}

#if defined(_DEVICE_RESOLUTION_AWARE) //&& !defined(WIN32_PLATFORM_WFSP)
void FxMainWin::OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/)
{
    DRA::DisplayMode dp = DRA::GetDisplayMode();
	//DRA::RelayoutDialog(
	//	AfxGetInstanceHandle(), 
	//	this->m_hWnd, 
	//	dp != DRA::Portrait ? 
	//	MAKEINTRESOURCE(IDD_WMLF_MAIN_WIDE) : 
	//	MAKEINTRESOURCE(IDD_WMLF_MAIN));

    //if(dp == DRA::Portrait)
    {
        int xgrpInfo, ygrpInfo, wgrpInfo, hgrpInfo;
        int xIDC_HEAD_PIC, yIDC_HEAD_PIC, wIDC_HEAD_PIC, hIDC_HEAD_PIC;
        int xIDC_INFO_NAME, yIDC_INFO_NAME, wIDC_INFO_NAME, hIDC_INFO_NAME;
        int xIDC_INFO_SIGN, yIDC_INFO_SIGN, wIDC_INFO_SIGN, hIDC_INFO_SIGN;
        int xtvBuddy, ytvBuddy, wtvBuddy, htvBuddy;

	    RECT rcCtl;
	    ::GetClientRect(this->m_hWnd, &rcCtl);
        
        xgrpInfo = rcCtl.left;
        ygrpInfo = rcCtl.top;
        wgrpInfo = rcCtl.right - rcCtl.left;
        hgrpInfo = DRA::SCALEY(40);
#ifdef M8
        ygrpInfo += 24;
#endif
        xIDC_HEAD_PIC = xgrpInfo + DRA::SCALEX(4);
        yIDC_HEAD_PIC = ygrpInfo + DRA::SCALEY(4);
        wIDC_HEAD_PIC = DRA::SCALEX(32);
        hIDC_HEAD_PIC = DRA::SCALEY(32);
        
        xIDC_INFO_NAME = xIDC_HEAD_PIC + DRA::SCALEX(36);
        yIDC_INFO_NAME = yIDC_HEAD_PIC;
        wIDC_INFO_NAME = wgrpInfo - xIDC_INFO_NAME - DRA::SCALEY(4);
        hIDC_INFO_NAME = DRA::SCALEY(16);

        xIDC_INFO_SIGN = xIDC_INFO_NAME;
        yIDC_INFO_SIGN = yIDC_INFO_NAME + hIDC_INFO_NAME + DRA::SCALEY(2);
        wIDC_INFO_SIGN = wIDC_INFO_NAME;
        hIDC_INFO_SIGN = DRA::SCALEY(16);

        xtvBuddy = rcCtl.left;
        ytvBuddy = rcCtl.top + hgrpInfo + DRA::SCALEY(2);
        wtvBuddy = wgrpInfo;
        htvBuddy = rcCtl.bottom - ytvBuddy;

        //HWND hwndctl = ::GetDlgItem(this->m_hWnd, IDC_GRP_INFO);
        //::MoveWindow(hwndctl, xgrpInfo, ygrpInfo, wgrpInfo, hgrpInfo, false);

        HWND hwndctl = ::GetDlgItem(this->m_hWnd, IDC_HEAD_PIC);
        ::MoveWindow(hwndctl, xIDC_HEAD_PIC, yIDC_HEAD_PIC, wIDC_HEAD_PIC, hIDC_HEAD_PIC, false);

        hwndctl = ::GetDlgItem(this->m_hWnd, IDC_INFO_NAME);
        ::MoveWindow(hwndctl, xIDC_INFO_NAME, yIDC_INFO_NAME, wIDC_INFO_NAME, hIDC_INFO_NAME, false);

        hwndctl = ::GetDlgItem(this->m_hWnd, IDC_INFO_SIGN);
        ::MoveWindow(hwndctl, xIDC_INFO_SIGN, yIDC_INFO_SIGN, wIDC_INFO_SIGN, hIDC_INFO_SIGN, false);

        hwndctl = ::GetDlgItem(this->m_hWnd, IDC_TREE_BUDDY);
        ::MoveWindow(hwndctl, xtvBuddy, ytvBuddy, wtvBuddy, htvBuddy, false);
    }
}
#endif

void FxMainWin::OnClose()
{
    ShowWindow(SW_HIDE);
	if (m_BuddyOpt)
		delete m_BuddyOpt;
	m_BuddyOpt = NULL;
 
    CNotify::RemoveNotification();
	CDialog::OnClose();
}

//::GetCaretPos(&point);
BOOL FxMainWin::PreTranslateMessage(MSG* pMsg)
{
    /*
    将大部分原先在这里处理的消息移到DefWndProc中处理,以解决显示出菜单时无法收到消息的问题
    */
	if (pMsg->hwnd == view.m_hWnd)
	{
	    if (pMsg->message == WM_KEYDOWN) 
	    {
		    switch(pMsg->wParam)
		    {
		    case VK_RETURN:
                {
				    HTREEITEM hItem = view.GetSelectedItem();
				    if(NULL != hItem)
				    {
					    if(!view.ItemHasChildren(hItem))
					    {
						    return showMsgDlg(view.GetSelectedItem());
					    }
					    else
					    {
    #ifndef WIN32_PLATFORM_WFSP
						    return view.Expand(hItem,TVE_TOGGLE);
    #endif
					    }
				    }
				    break;
                }
	        default:
		        break;
		    }
	    } 
        else
		if (pMsg->message == WM_LBUTTONDOWN)
		{
			SHRGINFO shrg;
			shrg.cbSize = sizeof(shrg);
			shrg.hwndClient = pMsg->hwnd;
			shrg.ptDown.x = LOWORD(pMsg->lParam); 
			shrg.ptDown.y = HIWORD(pMsg->lParam); 
			shrg.dwFlags = SHRG_RETURNCMD; 
			CPoint point;
			point.x = shrg.ptDown.x;
			point.y = shrg.ptDown.y;
			if(::SHRecognizeGesture(&shrg) == GN_CONTEXTMENU)//长按键消息
			{ 
                if (showBuddyMenu(point)) return TRUE; 
            }
			else
			{	
                if(view.HitTest(point) == view.GetSelectedItem())
                    return showMsgDlg(point);
            }
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}


BOOL FxMainWin::SetUserOnlineState(int iState)
{
    CString strDesc;
    switch(iState)
    {
    case FX_STATUS_ONLINE:
        strDesc = "在线";
        break;
    case FX_STATUS_OFFLINE:
        strDesc = "隐身";
        break;
    case FX_STATUS_AWAY:
        strDesc = "离开";
        break;
    case FX_STATUS_BUSY:
        strDesc = "忙碌";
        break;
    default:
        strDesc = "未设置";
        break;
    }
#ifdef UNICODE  
    char *pBuf;
    HANDLE hClip=GlobalAlloc(GMEM_MOVEABLE,strDesc.GetLength()+1);
    pBuf=(char*)GlobalLock(hClip);
    strcpy(pBuf,(const char*)(strDesc.GetBuffer())); 
    GlobalUnlock(hClip);
#endif
    fx_set_user_state(iState, pBuf, Sys_EventListener, this);
    GlobalFree(hClip); 
    return TRUE;
}

BOOL FxMainWin::showBuddyMenu(CPoint point)
{
	UINT uFlags;
	HTREEITEM hItem = view.HitTest(point, &uFlags);

	if ((hItem != NULL) && (TVHT_ONITEM & uFlags))
		return showBuddyMenu(hItem);
	return FALSE;
}

BOOL FxMainWin::showBuddyMenu(HTREEITEM hItem)
{
	if(hItem != view.GetSelectedItem())
		return FALSE;

	RECT r;
	CMenu menu;
	UINT nIDRes; 

	view.GetItemRect(hItem, &r, TRUE);

	CPoint point(r.left, r.top);
#if !DEBUG_GUI
	if (view.GetParentItem(hItem))
		nIDRes = IDR_MENU_BUDDY;
	else
		return FALSE; // no impl the grup menu//nIDRes = IDR_MENU_GROUP;
#endif
	if (!menu.LoadMenu(nIDRes))
		return FALSE;

	CMenu *pPopupMenu = menu.GetSubMenu(0);
    view.ClientToScreen(&point);
	pPopupMenu->TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y + (r.bottom - r.top),this);

	return TRUE;
}
BOOL FxMainWin::showMsgDlg()
{
    HTREEITEM hItem = view.GetSelectedItem();
    if ((hItem != NULL) && !view.ItemHasChildren(hItem))
    {
        return showMsgDlg(hItem);
    } else {
        return FALSE;
    }
}
BOOL FxMainWin::showMsgDlg(CPoint point)
{
	UINT uFlags;
	HTREEITEM hItem = view.HitTest(point, &uFlags);

	if ((hItem != NULL) && (TVHT_ONITEM & uFlags))
		return showMsgDlg(hItem);
	return FALSE;
}

BOOL FxMainWin::showMsgDlg(HTREEITEM hItem)
{	
/***********************************************************
* 因为现在不一定是选择后打开聊天对话框,所以屏蔽这一句检查代码
* hiizsk 2009.07.14
	if(hItem != view.GetSelectedItem())
		return FALSE;
***********************************************************/
	RECT r;
	view.GetItemRect(hItem, &r, FALSE);

#if DEBUG_GUI
#else
	if (!view.GetParentItem(hItem))
		return FALSE;

	Account_Info *ac_info =(Account_Info*)view.GetItemData(hItem);
	if (!ac_info)
		return FALSE;

	RemoveFilker(ac_info->accountID);

	showMsgDlg(ac_info->accountID);
#endif
	return TRUE;
}

BOOL FxMainWin::showMsgDlg(long lAccountID)
{	
	m_pFxMsgDlgView->LoginOK(m_isLoginOK);
	m_pFxMsgDlgView->ShowWindow(SW_SHOW);
	m_pFxMsgDlgView->m_isShow= TRUE;
	m_pFxMsgDlgView->ShowChat(m_pFxMsgDlgView->ChatWith(lAccountID));

	return TRUE;
}

bool FxMainWin::hand_SystemNetErr(int errcode)
{
	this->m_isLoginOK = FALSE;
	m_pFxMsgDlgView->LoginOK(m_isLoginOK);
	GetDlgItem(IDC_NET_STATE)->ShowWindow(SW_SHOW);
	relogin_fetion();
	this->UpdateWindow();
	return true;
}

void FxMainWin::relogin_ok()
{
	this->m_isLoginOK = TRUE;
	m_pFxMsgDlgView->LoginOK(m_isLoginOK);
	
	//set the fetion system msg call back function
	fx_set_system_msg_cb (Sys_EventListener, this);

	GetDlgItem(IDC_NET_STATE)->ShowWindow(SW_HIDE);
	m_strNickNameShow = m_strNickName + GetUserStateString();
	this->UpdateData(FALSE);
}

void FxMainWin::relogin_fetion()
{
	m_strNickNameShow = m_strNickName + GetUserStateString();
	this->UpdateData(FALSE);

	KillTimer(TIMER_RELOGIN);

	HANDLE* hConnect = new HANDLE();
	if(NULL != hConnect)
	{
		DWORD dwStatus = 0; 
		ConnMgrEstablishConnectionSync(&m_ConnInfo, hConnect, 25000, &dwStatus);
		if(dwStatus==CONNMGR_STATUS_CONNECTED)
		{
			//网络连接成功
		}
		delete hConnect;
	}
	SetTimer(TIMER_RELOGIN, 1000*35, NULL);
	//fx_relogin(Relogin_EventListener, this);
	fx_relogin(Sys_EventListener, this, NULL, NULL);
}

void FxMainWin::update_account_info()
{
	Account_Info * ac_info = m_BuddyOpt->fetchNoUpdateAccount();
	if (ac_info)
	{
		fx_update_account_info_by_id(ac_info->accountID);
		m_BuddyOpt->setHaveUpdateAccount(ac_info);
	} else {
		KillTimer(TIMER_UPDATE_ACCOUNTINFO);
	}
}

void FxMainWin::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent)
	{
	case TIMER_NEWMSG:  //1/1秒刷新一次
		filker_newmsg();
		break;
	case TIMER_RELOGIN:
		relogin_fetion();
		break;
	case TIMER_ADD_ITEM_TO_TREE:
		KillTimer(TIMER_ADD_ITEM_TO_TREE);
		m_BuddyOpt->addItemToTree();
		SetTimer(TIMER_UPDATE_ACCOUNTINFO, 1000*3, NULL);
		break;
	case TIMER_UPDATE_ACCOUNTINFO:
		update_account_info();
		break;
	}
	CDialog::OnTimer(nIDEvent);
}


void FxMainWin::filker_newmsg()
{
	static bool state = false;
	int counts = filker.GetCount();
	for(int i = 0; i < counts; i++)
	{
		POSITION pos = filker.FindIndex(i);
		if (!pos)
			continue;
		HTREEITEM hItem = filker.GetAt(pos);
		if (state)
		{
			view.SetItemImage(hItem, I_FLICK, I_FLICK);
			view.SetItemImage(view.GetParentItem(hItem), I_FLICK, I_FLICK);
		} else {
			m_BuddyOpt->setOnlineState(hItem);
			view.SetItemImage(view.GetParentItem(hItem), I_QUN, I_QUN);
		}

	}
	state = !state;
}


#define MSG_OK       1
#define MSG_FAIL     2
#define MSG_TIMEOUT  3
#define MSG_FAIL_LIMIT  4


void FxMainWin::hand_MsgSend_Event(int message, int fx_msg, long who)
{
switch(message)
	{
		case FX_SMS_OK: 
		case FX_DIA_SEND_OK: 
		case FX_QUN_SEND_OK: 
		case FX_QUN_SMS_OK: 
			handle_sendmsg( MSG_OK, fx_msg, who);
			if(!fx_msg)
				return;
			fx_destroy_msg((Fetion_MSG *)fx_msg);
			break;
		case FX_SMS_FAIL: 
		case FX_DIA_SEND_FAIL: 
		case FX_QUN_SEND_FAIL: 
		case FX_QUN_SMS_FAIL: 
			handle_sendmsg( MSG_FAIL, fx_msg, who);
			if(!fx_msg)
				return;
			fx_destroy_msg((Fetion_MSG *)fx_msg);
			break;
		case FX_SMS_TIMEOUT: 
		case FX_DIA_SEND_TIMEOUT: 
		case FX_QUN_SEND_TIMEOUT: 
		case FX_QUN_SMS_TIMEOUT: 
			handle_sendmsg( MSG_TIMEOUT, fx_msg, who);
			//time out should not to destroy msg, beacuse the system will resend by itself..
			break;

		case FX_SMS_FAIL_LIMIT: 
		case FX_QUN_SMS_FAIL_LIMIT: 
			handle_sendmsg( MSG_FAIL_LIMIT , fx_msg, who);
			fx_destroy_msg((Fetion_MSG *)fx_msg);
			break;
	}

}

void FxMainWin::handle_sendmsg(int msgflag, int fx_msg, long account_id)
{
	if(!fx_msg)
		return;

	POSITION pos = NULL; 
	int i = 0;
	Fetion_MSG *fxMsg = (Fetion_MSG *) fx_msg;
	char *msg = fx_msg_no_format(fxMsg->message); 
	CString newmsg;
	CString head;
	
	switch(msgflag)
	{
		case MSG_OK:
			pos = timeOutMsgVector.Find(fx_msg);
			if (pos)
			{
				timeOutMsgVector.RemoveAt(pos);
				//newmsg = "<b style=\"color:rgb(170,0,255);\">" +tr("auto resend ok:") + "</b>" + newmsg.fromUtf8(msg);
				head = CString(_T("自动重发OK"));
			}
			break;
		case MSG_FAIL:
			pos = timeOutMsgVector.Find(fx_msg);
			if (pos)
			{
				timeOutMsgVector.RemoveAt(pos);
			}
			//newmsg = "<b style=\"color:red;\">"+tr("send fail:") +"</b>"+ newmsg.fromUtf8(msg);
			head = CString(_T("发送失败"));
			break;

		case MSG_FAIL_LIMIT:
			pos = timeOutMsgVector.Find(fx_msg);
			if (pos)
			{
				timeOutMsgVector.RemoveAt(pos);
			}
			//newmsg = "<b style=\"color:red;\">"+tr("send sms fail by limit:") +"</b>"+ newmsg.fromUtf8(msg);
			head = CString(_T("发送数目限制"));
			break;
		case MSG_TIMEOUT:
			timeOutMsgVector.InsertAfter(timeOutMsgVector.GetTailPosition(), fx_msg);
			//newmsg = "<b style=\"color:rgb(170,0,255);\">" +tr("send timeout:") +"</b>" + newmsg.fromUtf8(msg)+"<br><b style=\"color:rgb(170,0,255);\">" +tr("will auto resend")+"</b>";
			head = CString(_T("发送超时"));
			break;
	}
	if(!head.IsEmpty())
	{
		MSGLOGDB MsgLog;
		MsgLog.lID = account_id;
		MsgLog.strSender = head;
		MsgLog.MsgTime = GetCurrentTime();
		MsgLog.strMsg = ConvertUtf8ToUtf16(msg);
		g_pFxDB->AddMsgLog(&MsgLog);
		newmsg = FormatMsgLog(&MsgLog);
		addNewMessage(account_id, newmsg);
	}

	if(msg)
	{
		free(msg);
		msg = NULL;
	}
}

void FxMainWin::addNewMessage(long account_id, CString newmsg /* ="" */)
{
	const Fetion_Account * account = fx_get_account_by_id (account_id);
	if (!account)
		return;

    // 提醒用户
	char * showname = fx_get_account_show_name(account, FALSE);
    NotifyUser(1, account_id, ConvertUtf8ToUtf16(showname));
	if(showname)
		free(showname);

	HTREEITEM accountItem = m_BuddyOpt->findAccountItemFromAllGroup(account);

	m_BuddyOpt->setOnlineState(accountItem);

	if(FALSE == m_pFxMsgDlgView->addNewMsg(account_id, newmsg))
	{
		return;
	}
	if(!m_pFxMsgDlgView->m_isShow)
	{
		RecoveryMenuBar();
	}

	if (accountItem)
	{
		if (filker.GetCount() == 0)
			SetTimer(TIMER_NEWMSG, 500,NULL);

        // 更正一个好友多条消息时要打开多次对话框才会停止闪烁的BUG
        if(filker.Find(accountItem) == NULL)
		    filker.InsertAfter(filker.GetTailPosition(), accountItem);
	}

}

void FxMainWin::addNewQunMessage(long qun_id,CString newmsg )
{
	const Fetion_Qun * qun = fx_get_qun_by_id(qun_id);
	if (!qun)
		return;
	
	// 提醒用户
	char * showname = fx_get_qun_show_name((Fetion_Qun*)qun);
	NotifyUser(3, qun_id, ConvertUtf8ToUtf16(showname));
	if(showname)
		free(showname);

	HTREEITEM accountItem = m_BuddyOpt->findQunItem(qun);

	//对象不匹配，注释掉防止出错
	//m_BuddyOpt->setOnlineState(accountItem);

	if(FALSE == m_pFxMsgDlgView->addNewMsg(qun_id, newmsg))
	{
		return;
	}
	if(!m_pFxMsgDlgView->m_isShow)
	{
		RecoveryMenuBar();
	}

	if (accountItem)
	{
		if (filker.GetCount() == 0)
			SetTimer(TIMER_NEWMSG, 500,NULL);

		// 更正一个好友多条消息时要打开多次对话框才会停止闪烁的BUG
		if(filker.Find(accountItem) == NULL)
			filker.InsertAfter(filker.GetTailPosition(), accountItem);
	}

}
void FxMainWin::OnStnClickedBtnAdd()
{
    CFindBuddyDlg* dlg = new CFindBuddyDlg();
    dlg->DoModal();
    delete dlg;
}
void FxMainWin::OnSendMsg()
{
    this->showMsgDlg();
}


// 消息提示
// EventType: 1-新消息 2-用户上线 3-群消息提醒
void FxMainWin::NotifyUser(int EventType, long lAccountID, CString szBuddyName)
{   
    if(m_bSilence)
        return;
	LPCWSTR strSoundPath;
    UINT Styles = 0;
    int iPeriod = 0;
    BOOL bVibrate = false;
	CString wavfile;
    switch(EventType)
    {
    case 1:

		{
			iPeriod = 1000; //毫秒
			bVibrate = this->m_bVibrate;
			wavfile = m_strStartupPath + CString(_T("\\message.wav"));
			if(!(::GetFileAttributes(wavfile) == 0xFFFFFFFF))
			{
				Styles=SND_FILENAME;
				strSoundPath=(LPCTSTR)wavfile;

			}
			else
			{
				strSoundPath = MAKEINTRESOURCE(IDR_MSGSOUND);
				Styles = SND_RESOURCE;
			}
			break;
		}

    case 2:

		{
			if(!m_bOnline)
				return ;
			wavfile =m_strStartupPath + CString(_T("\\online.wav"));
			if(!(::GetFileAttributes(wavfile) == 0xFFFFFFFF))
			{
				Styles=SND_FILENAME;
				strSoundPath=(LPCTSTR)wavfile;

			}
			else
			{
				strSoundPath = MAKEINTRESOURCE(IDR_ONLINESOUND);
				Styles = SND_RESOURCE;
			}
			CNotify::Nodify(this->m_hWnd, strSoundPath, 0, this->m_bSound,FALSE, Styles);//只声音提示，不震动
			return ;
		}

	case 3:
		{
			if(m_bShieldQunMessage)
				return;
			wavfile=m_strStartupPath+CString(_T("\\qun.wav"));
			if(!(::GetFileAttributes(wavfile) == 0xFFFFFFFF))
			{
				Styles=SND_FILENAME;
				strSoundPath=(LPCTSTR)wavfile;

			}
			else
			{
				strSoundPath = MAKEINTRESOURCE(IDR_MSGSOUND);
				Styles = SND_RESOURCE;
			}
		}

    }

#ifdef WIN32_PLATFORM_PSPC 
	SetSystemPowerState(NULL, POWER_STATE_ON, 0);
    if(this->IsTopParentActive())
	{
        CNotify::Nodify(this->m_hWnd, strSoundPath, iPeriod, this->m_bSound, this->m_bVibrate, Styles);
	}
    else
    {	
        CString strMsg = L"<font color=\"#0000FF\"><b>为了保护个人隐私，这里不会直接显示消息，请自行点击查看。</b></font>";
        // fixme: 这里要如何才能取得最后一条消息又不会导致消息被移除?
        // 下面这个代码是有问题的.取出消息后,再打开对话框将看不到新消息
        /*
        Fetion_MSG * fxMsg = NULL;
        //得到最后一条消息
        fxMsg = fx_get_msg(lAccountID);

        if(fxMsg == NULL)
            return;

	    char * msg_contain = fx_msg_no_format(fxMsg->message);

	    strMsg = ConvertUtf8ToUtf16(msg_contain) + CString(_T("\r\n"));
	    if (msg_contain)
		    free(msg_contain);

	    fxMsg = NULL;
        fxMsgPrev = NULL;
        */
        CNotify::CreateAndAddNotification(this->GetSafeHwnd(), szBuddyName, strMsg);
    }
#else
        CNotify::Nodify(this->m_hWnd, strSoundPath, iPeriod, this->m_bSound, this->m_bVibrate, Styles);
#endif
}

void FxMainWin::OnMainSetVibr()
{
    m_bVibrate = !m_bVibrate;
	SetSettingToIni(_T("Vibrate"), m_bVibrate);
}

void FxMainWin::OnMainSetSilence()
{
    m_bSilence = !m_bSilence;
	SetSettingToIni(_T("Silence"), m_bSilence);
}

void FxMainWin::OnUpdateMainSetSilence(CCmdUI *pCmdUI)
{
    pCmdUI->SetCheck(m_bSilence);
}

void FxMainWin::OnUpdateMainSetVibr(CCmdUI *pCmdUI)
{
    pCmdUI->SetCheck(m_bVibrate);
}

void FxMainWin::OnMainSetOnline()
{
	m_bOnline=!m_bOnline;
	SetSettingToIni(_T("Online"), m_bOnline);
}

void FxMainWin::OnUpdateMainSetOnline(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bOnline);
}

void FxMainWin::OnMainShieldQunMessage()
{
	m_bShieldQunMessage=!m_bShieldQunMessage;
	SetSettingToIni(_T("ShieldQunMessage"),m_bShieldQunMessage);
}

void FxMainWin::OnUpdateShieldQunMessage(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bShieldQunMessage);
}
// 获取启动路径
CString FxMainWin::GetStartupPath(void)
{
    if(m_strStartupPath == _T(""))
    {
        TCHAR cPath[MAX_PATH];   
        CString filePath;
        GetModuleFileName( NULL, cPath, MAX_PATH );
        filePath = cPath;
        filePath = filePath.Left(filePath.ReverseFind('\\'));
        m_strStartupPath = filePath;
    }
    return m_strStartupPath;
}

void FxMainWin::OnBdViewinfo()
{
    HTREEITEM hItem = view.GetSelectedItem();
    if ((hItem != NULL) && !view.ItemHasChildren(hItem))
    {
        showBuddyInfo(hItem);
    }
}

BOOL FxMainWin::showBuddyInfo(HTREEITEM hItem)
{
#if !DEBUG_GUI
	if(hItem != view.GetSelectedItem())
		return FALSE;

	if (!view.GetParentItem(hItem))
		return FALSE;

	Account_Info *ac_info =(Account_Info*)view.GetItemData(hItem);
	if (!ac_info)
		return FALSE;
    m_BuddyInfoDlg = new CBuddyInfoDlg(ac_info->accountID);
#else
    CBuddyInfoDlg* dlg = new CBuddyInfoDlg(123456);
#endif
    m_BuddyInfoDlg->DoModal();
    delete m_BuddyInfoDlg;
	m_BuddyInfoDlg = NULL;
    return TRUE;
}
void FxMainWin::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
    CDialog::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);

    pPopupMenu->CheckMenuItem(IDM_MAIN_SET_SILENCE, m_bSilence ? MF_CHECKED : MF_UNCHECKED);
    pPopupMenu->CheckMenuItem(IDM_MAIN_SET_NOSOUND, m_bSound ? MF_CHECKED : MF_UNCHECKED);
    pPopupMenu->CheckMenuItem(IDM_MAIN_SET_VIBR, m_bVibrate ? MF_CHECKED : MF_UNCHECKED);
    pPopupMenu->CheckMenuItem(IDM_MAIN_SET_ONLINE,m_bOnline ? MF_CHECKED : MF_UNCHECKED);
	pPopupMenu->CheckMenuItem(IDM_MAIN_SET_SHIELDQUNMESSAGE,m_bShieldQunMessage ? MF_CHECKED:MF_UNCHECKED);
    pPopupMenu->CheckMenuItem(IDM_MAIN_STATE_ONLINE, MF_UNCHECKED);
    pPopupMenu->CheckMenuItem(IDM_MAIN_STATE_HIDE, MF_UNCHECKED);
    pPopupMenu->CheckMenuItem(IDM_MAIN_STATE_BUSY, MF_UNCHECKED);
    pPopupMenu->CheckMenuItem(IDM_MAIN_STATE_AWAY, MF_UNCHECKED);
    int iState = fx_get_user_state();
    switch(iState)
    {
    case 0:
    case FX_STATUS_ONLINE:
        pPopupMenu->CheckMenuItem(IDM_MAIN_STATE_ONLINE, MF_CHECKED);
        break;
    case FX_STATUS_OFFLINE:
        pPopupMenu->CheckMenuItem(IDM_MAIN_STATE_HIDE, MF_CHECKED);
        break;
    case FX_STATUS_BUSY:
        pPopupMenu->CheckMenuItem(IDM_MAIN_STATE_BUSY, MF_CHECKED);
        break;
    case FX_STATUS_AWAY:
        pPopupMenu->CheckMenuItem(IDM_MAIN_STATE_AWAY, MF_CHECKED);
        break;
    }
}

// 处理移动组成功的消息
void FxMainWin::handle_MoveGroupOk(long account_id, int group_id)  
{
	this->m_BuddyOpt->delAccount_direct(account_id);  
	this->m_BuddyOpt->addAccountToGroup(fx_get_account_by_id (account_id)); 
}
void FxMainWin::OnAbout()
{
    CAbout dlg;
    dlg.DoModal();
}

void FxMainWin::OnBdDelete()
{
    HTREEITEM hItem = view.GetSelectedItem();
    if ((hItem != NULL) && !view.ItemHasChildren(hItem))
    {

	    Account_Info *ac_info =(Account_Info*)view.GetItemData(hItem);
	    if (!ac_info)
		    return;

        CString strMessage;
        strMessage.Format(_T("是否确定要删除 %s？删除后对方仍能发送消息给你，如果不希望收到对方的消息请将其加入黑名单。"), ac_info->accountName);
        if(AfxMessageBox(strMessage, MB_YESNO) == IDNO)
            return;

        fx_delete_buddy_by_id(ac_info->accountID, NULL, NULL);
    }
}

void FxMainWin::OnBdAddblacklist()
{
    HTREEITEM hItem = view.GetSelectedItem();
    if ((hItem != NULL) && !view.ItemHasChildren(hItem))
    {

	    Account_Info *ac_info =(Account_Info*)view.GetItemData(hItem);
	    if (!ac_info)
		    return;

        CString strMessage;
        strMessage.Format(_T("是否确定要将 %s 加入黑名单？对方将无法发送消息给你。"), ac_info->accountName);
        if(AfxMessageBox(strMessage, MB_YESNO) == IDNO)
            return;

        fx_addto_blacklist_by_id(ac_info->accountID, NULL, NULL);
    }
}

void FxMainWin::OnBdRmblacklist()
{
    HTREEITEM hItem = view.GetSelectedItem();
    if ((hItem != NULL) && !view.ItemHasChildren(hItem))
    {

	    Account_Info *ac_info =(Account_Info*)view.GetItemData(hItem);
	    if (!ac_info)
		    return;
        fx_removefrom_blacklist_by_id(ac_info->accountID, NULL, NULL);
    }
}

void FxMainWin::OnMainSetNosound()
{
    m_bSound = !m_bSound;
	SetSettingToIni(_T("Sound"), m_bSound);
}

BOOL FxMainWin::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
#ifdef WIN32_PLATFORM_PSPC
    switch(wParam)
    {
    case ID_NOTIFY:
        {
            NMSHN* pnmshn = (NMSHN*)lParam;
            if (pnmshn->hdr.code == SHNN_ACTIVATE)
            {
                //insert code here to handle VK_TTALK
                //only need to call SetWindowLong if the sinkwindow is a dialog
                this->SetForegroundWindow();
                CNotify::RemoveNotification();
                //return TRUE to inform the Shell that the button was handled
                return TRUE;
            }
            break;
        }
    default:
        break;
    }
#endif
    return CDialog::OnNotify(wParam, lParam, pResult);
}

/***********************************************************************************
* 增加在左软键的提取新消息按钮,替代了原先很少用到的发送消息.发送消息更适合用确认键
* hiizsk 2009.07.14
************************************************************************************/
void FxMainWin::OnMainShownewmsg()
{
    if(filker.GetCount() != 0)
    {
        HTREEITEM hItem = filker.GetHead();
        
        showMsgDlg(hItem);
    }
}

void FxMainWin::OnBdSendmsg()
{
    showMsgDlg();
}

void FxMainWin::OnMainAddbuddy()
{
    OnStnClickedBtnAdd();
}

LRESULT FxMainWin::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == WM_COMMAND)
    {
        switch (LOWORD(wParam))
        {
        case IDM_MAIN_STATE_ONLINE:
            SetUserOnlineState(FX_STATUS_ONLINE);
            break;
        case IDM_MAIN_STATE_HIDE:
            SetUserOnlineState(FX_STATUS_OFFLINE);
            break;
        case IDM_MAIN_STATE_BUSY:
            SetUserOnlineState(FX_STATUS_BUSY);
            break;
        case IDM_MAIN_STATE_AWAY:
            SetUserOnlineState(FX_STATUS_AWAY);
            break;
        case IDM_MAIN_MINIMAL:
            ShowWindow( SW_MINIMIZE );
            break;
        case IDM_MAIN_LOGOFF:
            ::exit(0);
            break;
        case IDM_MAIN_SHOWNEWMSG2:
            OnMainShownewmsg2();
            break;
        case IDM_MAIN_DIMISS:
            OnMainDimiss();
            break;
        default:
            break;
        }
    }
	if (handleFx_Sys_Event(message, wParam, lParam))
		return TRUE;  

    return CDialog::DefWindowProc(message, wParam, lParam);
}

// 通过桌面提醒进来的时候,需要移除提醒图标
void FxMainWin::OnMainShownewmsg2(void)
{
	this->SetForegroundWindow();
	OnMainShownewmsg();
    CNotify::RemoveNotification();
}

// 通过桌面提醒进来的时候,需要移除提醒图标
void FxMainWin::OnMainDimiss(void)
{
    CNotify::RemoveNotification();
}

void FxMainWin::OnBdMovegroup()
{
    OnBdViewinfo();
}

void FxMainWin::OnMainClean()
{
	// TODO: 在此添加命令处理程序代码
	CIniWR hIni;
	hIni.WritePrivateProfileString(m_mobile_no, _T("PassWord"), _T(""), m_strStartupPath + _T("\\Users\\") + m_mobile_no + _T("\\") + m_mobile_no + _T(".ini"));
}

void FxMainWin::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//解决按回车键导致程序退出的问题
	//CDialog::OnOK();
}

void FxMainWin::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	//解决SP手机按CANCEL键后自动退出问题，并改成最小化
	ShowWindow(SW_MINIMIZE);
	//CDialog::OnCancel();
}

CString FxMainWin::GetUserStateString(void)
{
	CString strUserState = _T("");;
    int iState = fx_get_user_state();
    switch(iState)
    {
    case 0:
    case FX_STATUS_ONLINE:
        strUserState = _T(" (在线)");
        break;
    case FX_STATUS_OFFLINE:
        strUserState = _T(" (隐身)");
        break;
    case FX_STATUS_BUSY:
        strUserState = _T(" (忙碌)");
        break;
    case FX_STATUS_AWAY:
        strUserState = _T(" (离开)");
        break;
	default:
		break;
    }
	if(FALSE == this->m_isLoginOK)
	{
        strUserState = _T(" (脱机)");
	}

	return strUserState;
}

UINT FxMainWin::GetSettingFromIni(LPCTSTR lpKeyName, int nDefault)
{
	CIniWR hIni;
	return hIni.GetPrivateProfileInt(_T("OPTION"), lpKeyName, nDefault, m_strStartupPath + _T("\\Users\\") + m_mobile_no + _T("\\") + m_mobile_no + _T(".ini"));
}

BOOL FxMainWin::SetSettingToIni(LPCTSTR lpKeyName, UINT uValue)
{
	CIniWR hIni;
	return hIni.WritePrivateProfileInt(_T("OPTION"), lpKeyName, uValue, m_strStartupPath + _T("\\Users\\") + m_mobile_no + _T("\\") + m_mobile_no + _T(".ini"));
}


void FxMainWin::handle_AddAccountApp(char* uri, char* showname)
{
    CString strMessage;
    strMessage.Format(_T("%s 想添加你为好友，是否同意？"), ConvertUtf8ToUtf16(showname));

    if(AfxMessageBox(strMessage, MB_YESNO | MB_ICONQUESTION | MB_APPLMODAL) == IDYES)
    {
        fx_handleContactRequest(uri, 1, 0, showname);
    }
}
void FxMainWin::RecoveryMenuBar()
{
	ShowWindow(TRUE);
	m_dlgCommandBar.InsertMenuBar(IDR_MAIN_MENU);
}
void FxMainWin::OnSendMyself()
{
	// TODO: 在此添加命令处理程序代码
	m_pFxMsgDlgView->LoginOK(m_isLoginOK);
	m_pFxMsgDlgView->ShowWindow(SW_SHOW);
	m_pFxMsgDlgView->m_isShow= TRUE;
	m_pFxMsgDlgView->ShowChat(m_pFxMsgDlgView->ChatWith(strtol(fx_get_usr_uid(), NULL,10), TRUE));
}

void FxMainWin::RemoveFilker(long lAccountID)
{
	const Fetion_Account * account = fx_get_account_by_id (lAccountID);
	if(NULL == account)
	{
		return;
	}

	HTREEITEM hItem = m_BuddyOpt->findAccountItemFromAllGroup(account);

	POSITION pos = filker.Find(hItem);
	if(pos)
	{
		filker.RemoveAt(pos);
	}

    // 更正点击后头像消息的BUG
	m_BuddyOpt->setOnlineState(hItem);
	view.SetItemImage(view.GetParentItem(hItem), I_QUN, I_QUN);

	if (filker.GetCount() == 0)
	{
		KillTimer(TIMER_NEWMSG);
	}
}

void FxMainWin::OnUpdateAllAccountinfo()
{
	// TODO: 在此添加命令处理程序代码
    CString strMessage;
    strMessage.Format(_T("当好友较多时，更新所有好友资料将会产生较大网络数据流量，且时间较长。如果只想更新单个好友资料，您可以选择单个好友“查看资料”。是否确定要更新所有好友资料？"));
    if(MessageBox(strMessage, _T("LibFetion"), MB_OKCANCEL | MB_ICONQUESTION) == IDCANCEL)
	{
		return;
	}
	m_BuddyOpt->WantUpdateAllAccountInfo();
	SetTimer(TIMER_UPDATE_ACCOUNTINFO, 1000*3, NULL);
}
