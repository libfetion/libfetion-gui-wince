/***************************************************************************
 *   Copyright (C) 2008 by DDD                                          *
 *   dedodong@163.com                                                     *
 *                                                                         *
 ***************************************************************************/
// WMLFDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WMLF.h"
#include "LoginDlg.h"
#include "LoginOptionDlg.h"
#include "About.h"

#include "convert_charset.h"
#include "IniWR.h"

#ifdef M8
#include "M8Misc.h"
#else
#include <tpcshell.h>
#include <connmgr.h>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLoginDlg 对话框

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoginDlg::IDD, pParent)
	, m_mobile_no(_T(""))
	, m_fetion_id(_T(""))
	, m_passwd(_T(""))
	, m_server_addr(_T(""))
	, m_login_state(_T(""))
	, m_LoginFlag(FALSE)
    , m_bRemPass(FALSE)
	, m_bLoginOffLine(FALSE)
    , m_bIsLoging(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//fx_set_https_func(WINCE_https);
	m_pConnInfo = NULL;
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDV_MaxChars(pDX, m_mobile_no, 11);
	DDX_Text(pDX, IDC_PWD, m_passwd);
	DDX_Text(pDX, IDC_LOGIN_STATE, m_login_state);
	DDX_Control(pDX, IDC_REMPASS, RemPass);
	DDX_Check(pDX, IDC_REMPASS, m_bRemPass);
	DDX_Check(pDX, IDC_LOGIN_OFFLINE, m_bLoginOffLine);
	DDX_Control(pDX, IDC_COMBO_USERS, m_cboUsersList);
	DDX_Control(pDX, IDC_COMBO_NET, m_cboNetList);
}

BEGIN_MESSAGE_MAP(CLoginDlg, CDialog)
#if defined(_DEVICE_RESOLUTION_AWARE) //&& !defined(WIN32_PLATFORM_WFSP)
	ON_WM_SIZE()
#endif
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_LOGIN, &CLoginDlg::OnBnClickedLogin)
//	ON_BN_CLICKED(IDC_REMACC, &CLoginDlg::OnBnClickedRemAccount)
//#ifdef WIN32_PLATFORM_WFSP
	ON_COMMAND(IDM_LOGIN, &CLoginDlg::OnLogin)
	ON_COMMAND(IDM_LOGIN_CANCEL, &CLoginDlg::OnLoginCancel)
	ON_COMMAND(IDM_LOGIN_REMPASS, &CLoginDlg::OnRemPassChanged)
	ON_COMMAND(IDM_LOGIN_OFFLINE, &CLoginDlg::OnOffLineChanged)
	ON_COMMAND(IDM_CANCEL, &CLoginDlg::OnIDM_Cancel)
//#endif // WIN32_PLATFORM_WFSP
ON_CBN_SELCHANGE(IDC_COMBO_USERS, &CLoginDlg::OnCbnSelchangeComboUsers)
    ON_WM_INITMENUPOPUP()
	ON_WM_DESTROY()
	ON_COMMAND(IDM_LOGIN_OPTION, &CLoginDlg::OnLoginOption)
	ON_COMMAND(IDM_CHANGE_NUM, &CLoginDlg::OnChangeNum)
	ON_COMMAND(IDM_ABOUT, &CLoginDlg::OnAbout)
END_MESSAGE_MAP()

// CLoginDlg 消息处理程序

BOOL CLoginDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

#ifdef M8
    AddMenuBarForM8(this->GetSafeHwnd(), IDR_LOGIN_MENU);
    FullScreen(this->GetSafeHwnd());
#else
	if (!m_dlgCommandBar.Create(this) ||
	    !m_dlgCommandBar.InsertMenuBar(IDR_LOGIN_MENU))
	{
		TRACE0("未能创建 CommandBar\n");
		return FALSE;      // 未能创建
	}
#endif
	// TODO: 在此添加额外的初始化代码
	
	InitUsersList();
	GetSelectedUserOption();
	InitNetList();

#ifdef WIN32_PLATFORM_WFSP
    //重写后退键，引发WM_HOTKEY消息
    (void)::SendMessage(SHFindMenuBar (m_hWnd), SHCMBM_OVERRIDEKEY, VK_TBACK,
        MAKELPARAM(SHMBOF_NODEFAULT | SHMBOF_NOTIFY,
        SHMBOF_NODEFAULT | SHMBOF_NOTIFY));
#endif

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

#if defined(_DEVICE_RESOLUTION_AWARE)// && !defined(WIN32_PLATFORM_WFSP)
void CLoginDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);	
	//DRA::RelayoutDialog(
	//	AfxGetInstanceHandle(), 
	//	this->m_hWnd, 
	//	DRA::GetDisplayMode() != DRA::Portrait ? 
	//		MAKEINTRESOURCE(IDD_WMLF_DIALOG_WIDE) : 
	//		MAKEINTRESOURCE(IDD_WMLF_DIALOG));
    int xIDC_LOGO, yIDC_LOGO, wIDC_LOGO, hIDC_LOGO;
    int xIDC_STATIC_ID, yIDC_STATIC_ID, wIDC_STATIC_ID, hIDC_STATIC_ID;
    int xIDC_STATIC_PWD, yIDC_STATIC_PWD, wIDC_STATIC_PWD, hIDC_STATIC_PWD;
    int xIDC_COMBO_USERS, yIDC_COMBO_USERS, wIDC_COMBO_USERS, hIDC_COMBO_USERS;
    int xIDC_PWD, yIDC_PWD, wIDC_PWD, hIDC_PWD;
    int xIDC_LOGIN_STATE, yIDC_LOGIN_STATE, wIDC_LOGIN_STATE, hIDC_LOGIN_STATE;
    int xIDC_LOGIN, yIDC_LOGIN, wIDC_LOGIN, hIDC_LOGIN;
    int xIDC_REMPASS, yIDC_REMPASS, wIDC_REMPASS, hIDC_REMPASS;
    int xIDC_LOGIN_OFFLINE, yIDC_LOGIN_OFFLINE, wIDC_LOGIN_OFFLINE, hIDC_LOGIN_OFFLINE;
    int xIDC_STATIC_NET, yIDC_STATIC_NET, wIDC_STATIC_NET, hIDC_STATIC_NET;
    int xIDC_COMBO_NET, yIDC_COMBO_NET, wIDC_COMBO_NET, hIDC_COMBO_NET;
    
    int iHeight, iWidth;
    int iMargin = DRA::SCALEX(5);
    RECT rcCtl;
    ::GetClientRect(this->m_hWnd, &rcCtl);
    
    iHeight = rcCtl.bottom - rcCtl.top;
    iWidth = rcCtl.right - rcCtl.left;

    xIDC_LOGO = rcCtl.left + iWidth / 2 - 60;//图像宽度120
    yIDC_LOGO = (rcCtl.bottom - rcCtl.top - 25 - DRA::SCALEY(150)) / 2;
    wIDC_LOGO = 120;
    hIDC_LOGO = 25;

    xIDC_STATIC_ID = rcCtl.left + DRA::SCALEX(10);
    yIDC_STATIC_ID = yIDC_LOGO + hIDC_LOGO + DRA::SCALEY(10);
    wIDC_STATIC_ID = DRA::SCALEX(60);
    hIDC_STATIC_ID = DRA::SCALEY(20);

    xIDC_STATIC_PWD = xIDC_STATIC_ID;
    yIDC_STATIC_PWD = yIDC_STATIC_ID + hIDC_STATIC_ID + iMargin;
    wIDC_STATIC_PWD = wIDC_STATIC_ID;
    hIDC_STATIC_PWD = hIDC_STATIC_ID;

    xIDC_STATIC_NET = xIDC_STATIC_ID;
    yIDC_STATIC_NET = yIDC_STATIC_PWD + hIDC_STATIC_PWD + iMargin;
    wIDC_STATIC_NET = wIDC_STATIC_ID;
    hIDC_STATIC_NET = hIDC_STATIC_ID;

	xIDC_COMBO_USERS = xIDC_STATIC_ID + wIDC_STATIC_ID;
    yIDC_COMBO_USERS = yIDC_STATIC_ID - 4;
    wIDC_COMBO_USERS = iWidth - xIDC_COMBO_USERS - DRA::SCALEX(10);
    hIDC_COMBO_USERS = hIDC_STATIC_ID * 5;

    xIDC_PWD = xIDC_COMBO_USERS;
    yIDC_PWD = yIDC_STATIC_PWD - 4;
    wIDC_PWD = wIDC_COMBO_USERS;
    hIDC_PWD = hIDC_STATIC_ID;

    xIDC_COMBO_NET = xIDC_COMBO_USERS;
    yIDC_COMBO_NET = yIDC_STATIC_NET - 4;
    wIDC_COMBO_NET = wIDC_COMBO_USERS;
    hIDC_COMBO_NET = hIDC_STATIC_ID * 5;

    xIDC_REMPASS = xIDC_STATIC_ID + iMargin;
    yIDC_REMPASS = yIDC_STATIC_NET + hIDC_STATIC_NET + iMargin;
    wIDC_REMPASS = DRA::SCALEX(80);
    hIDC_REMPASS = hIDC_STATIC_ID;

    xIDC_LOGIN_OFFLINE = xIDC_REMPASS + wIDC_REMPASS + iMargin + iMargin;
    yIDC_LOGIN_OFFLINE = yIDC_REMPASS;
    wIDC_LOGIN_OFFLINE = wIDC_REMPASS;
    hIDC_LOGIN_OFFLINE = hIDC_REMPASS;

	xIDC_LOGIN = xIDC_COMBO_USERS;
    yIDC_LOGIN = yIDC_REMPASS + hIDC_REMPASS + iMargin;
    wIDC_LOGIN = wIDC_STATIC_ID;
#if defined(M8)
    hIDC_LOGIN = hIDC_STATIC_ID;// 应要求,隐藏按钮,如果需要按钮, 赋值 hIDC_STATIC_ID;
#else
    hIDC_LOGIN = 0;
#endif

    xIDC_LOGIN_STATE = xIDC_STATIC_ID;
    yIDC_LOGIN_STATE = yIDC_LOGIN + hIDC_LOGIN;
    wIDC_LOGIN_STATE = iWidth - 2 * iMargin;
    hIDC_LOGIN_STATE = hIDC_STATIC_ID;

    HWND hwndctl = ::GetDlgItem(this->m_hWnd, IDC_LOGO );
    ::MoveWindow(hwndctl, xIDC_LOGO, yIDC_LOGO , wIDC_LOGO , hIDC_LOGO , false);
    
    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_STATIC_ID);
    ::MoveWindow(hwndctl, xIDC_STATIC_ID, yIDC_STATIC_ID, wIDC_STATIC_ID, hIDC_STATIC_ID, false);

    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_STATIC_PWD);
    ::MoveWindow(hwndctl, xIDC_STATIC_PWD, yIDC_STATIC_PWD, wIDC_STATIC_PWD, hIDC_STATIC_PWD, false);

    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_STATIC_NET);
    ::MoveWindow(hwndctl, xIDC_STATIC_NET, yIDC_STATIC_NET, wIDC_STATIC_NET, hIDC_STATIC_NET, false);

	hwndctl = ::GetDlgItem(this->m_hWnd, IDC_COMBO_USERS);
    ::MoveWindow(hwndctl, xIDC_COMBO_USERS, yIDC_COMBO_USERS, wIDC_COMBO_USERS, hIDC_COMBO_USERS, false);

    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_PWD);
    ::MoveWindow(hwndctl, xIDC_PWD, yIDC_PWD, wIDC_PWD, hIDC_PWD, false);

    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_COMBO_NET);
    ::MoveWindow(hwndctl, xIDC_COMBO_NET, yIDC_COMBO_NET, wIDC_COMBO_NET, hIDC_COMBO_NET, false);

	hwndctl = ::GetDlgItem(this->m_hWnd, IDC_LOGIN);
    ::MoveWindow(hwndctl, xIDC_LOGIN, yIDC_LOGIN, wIDC_LOGIN, hIDC_LOGIN, false);

    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_REMPASS);
    ::MoveWindow(hwndctl, xIDC_REMPASS, yIDC_REMPASS, wIDC_REMPASS, hIDC_REMPASS, false);

    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_LOGIN_OFFLINE);
    ::MoveWindow(hwndctl, xIDC_LOGIN_OFFLINE, yIDC_LOGIN_OFFLINE, wIDC_LOGIN_OFFLINE, hIDC_LOGIN_OFFLINE, false);

	hwndctl = ::GetDlgItem(this->m_hWnd, IDC_LOGIN_STATE);
    ::MoveWindow(hwndctl, xIDC_LOGIN_STATE, yIDC_LOGIN_STATE, wIDC_LOGIN_STATE, hIDC_LOGIN_STATE, true);
}
#endif

void  My_EventListener (int message, WPARAM wParam, LPARAM lParam, void* args);
int My_getDataFromHttps(char* url, char*web_data, char* cookie);
void CLoginDlg::OnBnClickedLogin()
{
	CStringA fetion_id;
	CStringA pwd;
	CStringA server_addr;
	CIniWR hIni;
	CString strUser;

	if(m_bIsLoging) {
		return;
    }
    m_bIsLoging = TRUE;

#if DEBUG_GUI
	this->m_LoginFlag = TRUE;
	::SendMessage(this->m_hWnd, WM_CLOSE,0, 0);
	return;
#endif
	
	this->m_LoginFlag = FALSE;
	
	this->UpdateData();

	m_cboUsersList.GetWindowTextW(strUser.GetBuffer(12), 12);
	strUser.ReleaseBuffer();
	m_mobile_no = strUser;

	if(this->m_mobile_no.IsEmpty()) {
		this->m_login_state = _T("请输入飞信号或手机号码");
		goto fail;
	}
	

	if (this->m_mobile_no.GetLength() != 11 && this->m_mobile_no.GetLength() != 9 ) {
		this->m_login_state = _T("请输入正确的飞信号或手机号码");
		goto fail;
	}

	if(this->m_passwd.IsEmpty()) {
		this->m_login_state = _T("请输入密码");
		goto fail;
	}	
#ifndef M8
    if(!EstablishConnection())
    {
        this->m_login_state = _T("网络连接错误...");
        goto fail;
    }
#endif

	m_fetion_id = m_mobile_no;
	/*
	m_fetion_id = GetFetionNoFromIni(m_mobile_no);
	if(m_fetion_id.GetLength() != 9)
	{
		this->m_login_state = _T("从服务器获取登陆信息...");
		this->UpdateData(FALSE);
		this->UpdateWindow();

		int netflag = 0;
		m_fetion_id = GetFetionNoFromHttpsWeb(m_mobile_no, m_passwd, netflag);
		if(this->m_fetion_id.IsEmpty())
		{
			switch(netflag)
			{
			case	404:
				this->m_login_state = _T("SSL失败");
				break;
			case	301:
				this->m_login_state = _T("验证用户信息错误");
				break;
			case	401:
				this->m_login_state = _T("验证用户信息失败");
				break;
			case	402:
				this->m_login_state = _T("SSL数据错误");
				break;
			default:
				this->m_login_state = _T("未知错误");
				break;
			}
			goto fail;
		}
	}

	WriteLoginUserToIni();
*/
	this->m_login_state = _T("登录中...");
	this->UpdateData(FALSE);
	this->UpdateWindow();

	fetion_id = ConvertUtf16ToUtf8(m_fetion_id);
	pwd = ConvertUtf16ToUtf8(m_passwd);
	
/*
	if (!this->m_server_addr.IsEmpty()) 
	{ 
		//we should read the fetion server address if we set it before.
		server_addr = ConvertUtf16ToUtf8(m_server_addr);
		fx_set_serve_address(server_addr.GetBuffer());
	}
*/
	if(m_bLoginOffLine)
	{
		fx_set_login_status(FX_STATUS_OFFLINE);
	}

	fx_set_Https_CB(My_getDataFromHttps);
	fx_login(fetion_id.GetBuffer(), pwd.GetBuffer(),(My_EventListener), this);

	m_dlgCommandBar.InsertMenuBar(IDR_LOGIN_CANCEL_MENU);

    goto lfinally;
fail:
    m_bIsLoging =FALSE;
	m_dlgCommandBar.InsertMenuBar(IDR_LOGIN_MENU);

lfinally:
	this->UpdateData(FALSE);
	return ;
}


void  My_EventListener (int message, WPARAM wParam, LPARAM lParam, void* args)
{
	if(!args)
		return;
	
	CLoginDlg *loginDlg = (CLoginDlg *)args;
	::SendMessage(loginDlg->m_hWnd, message + WM_USER, wParam, lParam);
}

BOOL CLoginDlg::handleFx_Login_Event(int message, WPARAM wParam, LPARAM lParam)
{
    CLoginDlg *loginDlg = this;
    bool bLoginFail = false;
	CIniWR hIni;

	if (message < WM_USER)
		return FALSE;

	switch(message - WM_USER)
	{
	case FX_LOGIN_URI_ERROR:
		loginDlg->m_login_state = (_T("飞信号错误"));
        bLoginFail = true;
		break;
		
	case FX_LOGIN_CONNECTING:
		loginDlg->m_login_state = (_T("正在连接服务器"));
		break;
		
	case FX_LOGIN_WAIT_AUTH:
		loginDlg->m_login_state = (_T("正在验证用户名和密码"));
		break;
		
	case FX_LOGIN_AUTH_OK:
		loginDlg->m_login_state = (_T("验证用户名和密码成功"));
		break;
		
	case FX_LOGIN_FAIL:
		loginDlg->m_login_state = (_T("登陆失败"));
        bLoginFail = true;
		break;

	case FX_LOGIN_NETWORK_ERROR:
		loginDlg->m_login_state = (_T("网络错误"));
        bLoginFail = true;
		break;

	case FX_LOGIN_UNKOWN_ERROR :   
		loginDlg->m_login_state = (_T("登陆出现未知错误"));
        bLoginFail = true;
		break;
		
	case FX_LOGIN_UNKOWN_USR:
		loginDlg->m_login_state = (_T("用户名错误"));
        bLoginFail = true;
		break;
		
	case FX_LOGIN_GCL_GETTING:
		loginDlg->m_login_state = (_T("正在获取联系人列表"));
		break;
		
	case FX_LOGIN_GCL_OK:   
		loginDlg->m_login_state = (_T("获取联系人列表成功"));
		break;
		
	case FX_LOGIN_GCL_FAIL:
		loginDlg->m_login_state = (_T("获取联系人列表失败"));
        bLoginFail = true;
		break;
		
	case FX_LOGIN_GP_GETTING:
		loginDlg->m_login_state = (_T("正在获取联系人信息"));
		break;
		
	case FX_LOGIN_GP_OK:
		loginDlg->m_login_state = (_T("获取联系人信息成功 "));
		break;

	case FX_LOGIN_GP_FAIL:
		loginDlg->m_login_state = (_T("获取联系人信息失败"));
        bLoginFail = true;
		break;
		
	case FX_LOGIN_OK :
		loginDlg->m_login_state = (_T("登陆成功"));
		loginDlg->m_LoginFlag = TRUE;
		//after login success, we should save the fetion server address.
		loginDlg->m_server_addr = ConvertUtf8ToUtf16(fx_get_serve_address());
		hIni.WritePrivateProfileString(_T("OPTION"), _T("ServerAddress"), loginDlg->m_server_addr, loginDlg->m_strStartupPath + _T("\\LibFetion.ini"));

		::SendMessage(loginDlg->m_hWnd, WM_CLOSE, (WPARAM)bLoginFail, 0);
		break;
	case FX_LOGIN_NEED_AUTH_CODE:
		loginDlg->m_login_state = (_T("请输入验证码"));
		break;
	default:
		return FALSE;
	}

	this->UpdateData(FALSE);
	//如果连接失败则允许再次连接
	if(bLoginFail)
	{
		m_bIsLoging =FALSE;
		m_dlgCommandBar.InsertMenuBar(IDR_LOGIN_MENU);
	}
		
	//safe to do for our process..
	//::SendMessage(loginDlg->m_hWnd, FX_LOGINSTATE_MSG, bLoginFail, 0);
	return TRUE;
}

LRESULT CLoginDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (handleFx_Login_Event(message, wParam, lParam))
		return TRUE;

	switch(message)
	{
	case FX_LOGINSTATE_MSG:
		this->UpdateData(FALSE);
        //如果连接失败则允许再次连接
        if(TRUE == (LOWORD(wParam)))
		{
            m_bIsLoging =FALSE;
			m_dlgCommandBar.InsertMenuBar(IDR_LOGIN_MENU);
		}
		break;

	case WM_CLOSE:
		if (!this->m_LoginFlag) 
		{
			//AfxMessageBox(_T("close"));
			fx_terminate();
			::exit(0);
		}
		break;
#ifdef WIN32_PLATFORM_WFSP
		//改变后退键行为
	case WM_HOTKEY:
		if ((VK_TBACK == HIWORD(lParam)) && (MOD_KEYUP == LOWORD(lParam)))
		{
			SHSendBackToFocusWindow (message, wParam, lParam);
		}
		break;
#endif
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

	
BOOL CLoginDlg::PreTranslateMessage(MSG* pMsg)
{

	if(pMsg->message==WM_KEYDOWN)         
	{
		switch(pMsg->wParam)
		{
#if 0
		case VK_RETURN: 
			OnBnClickedLogin();
			return TRUE;
#endif
		case VK_ESCAPE: 
			return TRUE;
			break;
#if 0
#ifdef WIN32_PLATFORM_WFSP
			;
#else
		case VK_ESCAPE: 
			return TRUE;
			break;
#endif // WIN32_PLATFORM_W
#endif
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CLoginDlg::OnLogin()
{
	OnBnClickedLogin();
}

void CLoginDlg::OnLoginCancel()
{
    if(m_bIsLoging)
	{
		Sleep(500);
        /* 最近的库里增加了取消登录的函数，所以我们可以在这里调用它*/
	    m_bIsLoging = FALSE;
        fx_cancel_login();
		this->m_login_state = _T("登录被取消");
		m_dlgCommandBar.InsertMenuBar(IDR_LOGIN_MENU);
    }
	this->UpdateData(FALSE);
}

void CLoginDlg::OnIDM_Cancel()
{
	if(m_bIsLoging)
	{
		fx_cancel_login();
		this->m_bIsLoging = FALSE;
	}
	DeleteNetListDataPrt();
	SendMessage(WM_CLOSE,0, 0);
}

#ifdef WIN32_PLATFORM_WFSP
#include "tpcshell.h"
#endif

void CLoginDlg::OnCancel()
{
	CDialog::OnCancel();
//#if 0
//#ifdef WIN32_PLATFORM_WFSP
//	SHSendBackToFocusWindow(WM_HOTKEY, 2, MAKELPARAM(MOD_KEYUP, VK_TBACK));
//#else
//	CDialog::OnCancel();
//#endif // WIN32_PLATFORM_W
//#endif
}

#ifndef M8
// 自动连接网络
BOOL CLoginDlg::EstablishConnection(void)
{
    HANDLE* hConnect = new HANDLE();
	if(NULL == hConnect)
	{
		return FALSE;
	}
	this->m_login_state=_T("连接网络...");
	this->UpdateData(FALSE);
	this->UpdateWindow();

	if(!GetSelectedNet(m_pConnInfo->guidDestNet))
	{
		return FALSE;
	}

    DWORD dwStatus = 0; 
    ConnMgrEstablishConnectionSync(m_pConnInfo, hConnect, 25000, &dwStatus);
	delete hConnect;
	if(dwStatus==CONNMGR_STATUS_CONNECTED)//网络连接成功
	{
		this->m_login_state=_T("网络连接成功...");
		this->UpdateData(FALSE);
		this->UpdateWindow();
		Sleep(500);//等待0.5S，让设备准备好
		return TRUE;
	}
	else
		return FALSE;
}
#endif

void CLoginDlg::OnRemPassChanged()
{
    UpdateData();
    m_bRemPass = !m_bRemPass;
    UpdateData(FALSE);
}

void CLoginDlg::OnOffLineChanged()
{
    UpdateData();
    m_bLoginOffLine = !m_bLoginOffLine;
    UpdateData(FALSE);
}

int My_getDataFromHttps(char* url, char** data, char** cookie);
{
	char *web_data = NULL, *cookie_data = NULL;
	CString strWeb, strCookie;
	CStringA strWebA, strCookieA;

	if (GetHttpsWebData(ConvertUtf8ToUtf16(url), &strWeb, &strCookie) || strWeb.IsEmpty())
		return 0;

	strWebA = ConvertUtf16ToUtf8(strWeb);
	strCookieA = ConvertUtf16ToUtf8(strCookie);

	web_data = (char*)malloc(strWebA.GetLength() + 1);
	if (!web_data)
		return -1;

	cookie_data = (char*)malloc(strCookieA.GetLength() + 1);
	if (!cookie_data)
	{
		free(web_data);
		return -1;
	}

	memset(web_data, 0, strWebA.GetLength() + 1);
	memset(cookie_data, 0, strCookieA.GetLength() + 1);

	strncpy(web_data, (char*)strWebA.GetBuffer(), strWebA.GetLength());
	strncpy(cookie_data, (char*)strCookieA.GetBuffer(), strCookieA.GetLength());

	if (data)
		*data = web_data;
	else
		free(web_data);

	if (cookie)
		*cookie = cookie_data;
	else
		free(cookie_data);

	return 0;
}

void CLoginDlg::InitUsersList(void)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	
	m_cboUsersList.LimitText(11);
	m_cboUsersList.ResetContent();
	hFind = FindFirstFile(m_strStartupPath + _T("\\Users\\*.*"), &FindFileData);

	if(INVALID_HANDLE_VALUE != hFind)
	{
		while(1)
		{
			CString strFileName = FindFileData.cFileName;
			
			if(FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
			{
				if(strFileName.GetLength() == 11)
				{
					m_cboUsersList.AddString(strFileName);
				}
			}
			if(!FindNextFile(hFind, &FindFileData))
			{
				if(GetLastError() == ERROR_NO_MORE_FILES)
				{
					break;
				}
			}
		}
		FindClose(hFind);
		hFind = INVALID_HANDLE_VALUE;
	}
	if(m_cboUsersList.GetCount() > 0)
	{
		m_cboUsersList.SetCurSel(0);
	}

	CIniWR hIni;
	CString strLastUser;
	hIni.GetString(_T("OPTION"), _T("LastUser"), strLastUser.GetBuffer(MAX_PATH), MAX_PATH, m_strStartupPath + _T("\\LibFetion.ini"));
	strLastUser.ReleaseBuffer();
	
	for(int i = 0; i < m_cboUsersList.GetCount(); i++)
	{
		CString strMobileNo;
		m_cboUsersList.GetLBText(i, strMobileNo);
		if(0 == strLastUser.Compare(strMobileNo))
		{
			m_cboUsersList.SetCurSel(i);
			break;
		}
	}
	hIni.GetString(_T("OPTION"), _T("ServerAddress"), m_server_addr.GetBuffer(MAX_PATH), MAX_PATH, m_strStartupPath + _T("\\LibFetion.ini"));
	m_server_addr.ReleaseBuffer();
}

void CLoginDlg::GetSelectedUserOption(void)
{
	if(m_cboUsersList.GetCount() == 0)
	{
		return;
	}
	
	CString strMobileNo;

	m_cboUsersList.GetLBText(m_cboUsersList.GetCurSel(), strMobileNo);
	if(strMobileNo.GetLength() != 11)
	{
		return;
	}

	CIniWR hIni;
	int nDefault = 0;

	m_bRemPass = hIni.GetPrivateProfileInt(_T("OPTION"), _T("RemberPassWord"), nDefault, m_strStartupPath + _T("\\Users\\") + strMobileNo + _T("\\") + strMobileNo + _T(".ini"));
	m_bLoginOffLine = hIni.GetPrivateProfileInt(_T("OPTION"), _T("LoginOffLine"), nDefault, m_strStartupPath + _T("\\Users\\") + strMobileNo + _T("\\") + strMobileNo + _T(".ini"));
	if(m_bRemPass)
	{
		CString strRead;
		hIni.GetString(strMobileNo, _T("PassWord"), strRead.GetBuffer(MAX_PATH), MAX_PATH, m_strStartupPath + _T("\\Users\\") + strMobileNo + _T("\\") + strMobileNo + _T(".ini"));
		strRead.ReleaseBuffer();
		m_passwd = ConvertNUMToUft16(strRead);
	}
	else
	{
		m_passwd = _T("");
	}

	this->UpdateData(FALSE);
}

void CLoginDlg::OnCbnSelchangeComboUsers()
{
	// TODO: 在此添加控件通知处理程序代码
	GetSelectedUserOption();
}

CString CLoginDlg::GetFetionNoFromIni(CString strMobileNo)
{
	CIniWR hIni;
	CString strRead;

	hIni.GetString(strMobileNo, _T("FetionID"), strRead.GetBuffer(MAX_PATH), MAX_PATH, m_strStartupPath + _T("\\Users\\") + strMobileNo + _T("\\") + strMobileNo + _T(".ini"));
	strRead.ReleaseBuffer();
	if(strRead.GetLength() == 9)
	{
		return strRead;
	}
	else
	{
		return _T("");
	}
}

void CLoginDlg::WriteLoginUserToIni()
{
	CString strMobileNo = m_mobile_no;
	CIniWR hIni;
	hIni.WritePrivateProfileString(_T("OPTION"), _T("LastUser"), strMobileNo, m_strStartupPath + _T("\\LibFetion.ini"));
	CString strNet;
	m_cboNetList.GetLBText(m_cboNetList.GetCurSel(), strNet);
	hIni.WritePrivateProfileString(_T("OPTION"), _T("Network"), strNet, m_strStartupPath + _T("\\LibFetion.ini"));

	CreateDirectory(m_strStartupPath + _T("\\Users"), NULL);
	CreateDirectory(m_strStartupPath + _T("\\Users\\") + strMobileNo, NULL);

	hIni.WritePrivateProfileString(strMobileNo, _T("FetionID"), m_fetion_id, m_strStartupPath + _T("\\Users\\") + strMobileNo + _T("\\") + strMobileNo + _T(".ini"));
	hIni.WritePrivateProfileInt(_T("OPTION"), _T("RemberPassWord"), m_bRemPass, m_strStartupPath + _T("\\Users\\") + strMobileNo + _T("\\") + strMobileNo + _T(".ini"));
	
	CString strWrite;
	if(m_bRemPass)
	{
		strWrite = ConvertUtf16ToNUM(m_passwd);
	}
	else
	{
		strWrite = _T("");
	}
	hIni.WritePrivateProfileString(strMobileNo, _T("PassWord"), strWrite, m_strStartupPath + _T("\\Users\\") + strMobileNo + _T("\\") + strMobileNo + _T(".ini"));

	hIni.WritePrivateProfileInt(_T("OPTION"), _T("LoginOffLine"), m_bLoginOffLine, m_strStartupPath + _T("\\Users\\") + strMobileNo + _T("\\") + strMobileNo + _T(".ini"));
}

void CLoginDlg::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
	CDialog::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);

	UpdateData();
	pPopupMenu->CheckMenuItem(IDM_LOGIN_REMPASS, m_bRemPass ? MF_CHECKED : MF_UNCHECKED);
	pPopupMenu->CheckMenuItem(IDM_LOGIN_OFFLINE, m_bLoginOffLine ? MF_CHECKED : MF_UNCHECKED);
}

void CLoginDlg::InitNetList(void)
{
	HRESULT hResult = E_FAIL;
	CONNMGR_DESTINATION_INFO DestInfo;
	int index = 0; 
	int nIndex = 0;
	hResult = ConnMgrEnumDestinations(index++, &DestInfo);
	while(SUCCEEDED(hResult)) 
	{

		CONNMGR_DESTINATION_INFO * lpDestInfo = new CONNMGR_DESTINATION_INFO;
		*lpDestInfo = DestInfo;
		nIndex = m_cboNetList.AddString(DestInfo.szDescription);
		if(nIndex >= 0)
		{
			m_cboNetList.SetItemDataPtr(nIndex, lpDestInfo);
		}
		hResult = ConnMgrEnumDestinations(index++, &DestInfo);
	}

	if(m_cboNetList.GetCount() > 0)
	{
		m_cboNetList.SetCurSel(0);
	}

	CIniWR hIni;
	CString strNetwork;
	hIni.GetString(_T("OPTION"), _T("Network"), strNetwork.GetBuffer(MAX_PATH), MAX_PATH, m_strStartupPath + _T("\\LibFetion.ini"));
	strNetwork.ReleaseBuffer();
	
	for(int i = 0; i < m_cboNetList.GetCount(); i++)
	{
		CString strNet;
		m_cboNetList.GetLBText(i, strNet);
		if(0 == strNetwork.Compare(strNet))
		{
			m_cboNetList.SetCurSel(i);
			break;
		}
	}
}

BOOL CLoginDlg::GetSelectedNet(GUID &IID_DestNetInternet)
{
	if(m_cboNetList.GetCount() == 0)
	{
		return FALSE;
	}
	
	CONNMGR_DESTINATION_INFO * lpDestInfo = (CONNMGR_DESTINATION_INFO*)m_cboNetList.GetItemDataPtr(m_cboNetList.GetCurSel());
	if(NULL == lpDestInfo)
	{
		return FALSE;
	}
	IID_DestNetInternet = lpDestInfo->guid;
	return TRUE;
}

void CLoginDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	DeleteNetListDataPrt();
}

void CLoginDlg::DeleteNetListDataPrt(void)
{
	for(int i = 0; i < m_cboNetList.GetCount(); i++)
	{
		CONNMGR_DESTINATION_INFO * lpDestInfo = (CONNMGR_DESTINATION_INFO*)m_cboNetList.GetItemDataPtr(i);
		delete [] lpDestInfo;
		lpDestInfo = NULL;
		m_cboNetList.SetItemDataPtr(i, NULL);
	}
}

void CLoginDlg::OnLoginOption()
{
	// TODO: 在此添加命令处理程序代码
	CLoginOptionDlg dlg;
	dlg.m_strStartupPath = m_strStartupPath;
	dlg.DoModal();
	if(dlg.m_bHaveEdit)
	{
		InitUsersList();
		GetSelectedUserOption();
	}
}

void CLoginDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialog::OnOK();
}

void CLoginDlg::OnChangeNum()
{
	// TODO: 在此添加命令处理程序代码
	if(m_cboUsersList.GetCount() > 0)
	{
		int iSel = m_cboUsersList.GetCurSel();
		iSel ++;
		if(iSel >= m_cboUsersList.GetCount())
		{
			iSel = 0;
		}
		m_cboUsersList.SetCurSel(iSel);
		GetSelectedUserOption();
	}
}

void CLoginDlg::OnAbout()
{
	// TODO: 在此添加命令处理程序代码
    CAbout dlg;
    dlg.DoModal();
}
