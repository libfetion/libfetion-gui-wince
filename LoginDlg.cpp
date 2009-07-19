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

#include "convert_charset.h"

#ifdef M8
#include "M8Misc.h"
#else
#include <tpcshell.h>
#include <connmgr.h>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#if 0
extern CString WINCE_https (CString URL, int* netflag);
void WINCE_https (char **reslut, char* url, int* netflag)
{
	CString strGetData = WINCE_https (ConvertUtf8ToUtf16(url), netflag);

#if 0
	CString strHeaders = _T("Content-Type: application/x-www-form-urlencoded"); 
	// URL-encoded form variables - 
	CString strFormData = ConvertUtf8ToUtf16(url);

	INTERNET_PORT nPort = INTERNET_DEFAULT_HTTPS_PORT;
	CInternetSession session; 
	CHttpConnection* pConnection = session.GetHttpConnection(_T("221.130.45.201"),nPort); 

	CHttpFile* pFile = 
		pConnection->OpenRequest(CHttpConnection::HTTP_VERB_GET, 
		_T(""),NULL,1,NULL,NULL,
		INTERNET_FLAG_EXISTING_CONNECT|
		INTERNET_FLAG_SECURE|
		INTERNET_FLAG_IGNORE_CERT_CN_INVALID|
		INTERNET_FLAG_IGNORE_CERT_DATE_INVALID
		);   
	//Send Request 
	BOOL result = pFile->SendRequest(strHeaders, 
		(LPVOID)(LPCTSTR)strFormData, strFormData.GetLength()); 
	//Get Response
	;
	pFile->ReadString(strGetData);
	
	if (netflag)
		netflag = result;
#endif 
//	MessageBox(strGetData);
	*reslut = ConvertUtf16ToUtf8(strGetData);
	//*reslut = malloc(strGetData.GetLength()+1);

	
}

#endif

// CLoginDlg 对话框

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoginDlg::IDD, pParent)
	, m_fetion_id(_T(""))
	, m_passwd(_T(""))
	, m_server_addr(_T(""))
	, m_login_state(_T(""))
	, m_LoginFlag(FALSE)
    , m_bRemPass(FALSE)
    , m_bIsLoging(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//fx_set_https_func(WINCE_https);
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_FETION_ID, m_fetion_id);
    DDV_MaxChars(pDX, m_fetion_id, 9);
    DDX_Text(pDX, IDC_PWD, m_passwd);
    DDX_Text(pDX, IDC_LOGIN_STATE, m_login_state);
    DDX_Control(pDX, IDC_REMACC, RemAccount);
    DDX_Check(pDX, IDC_REMACC, m_bRemPass);
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
	ON_COMMAND(IDM_LOGIN_REMPASS, &CLoginDlg::OnRemPassChanged)
	ON_COMMAND(IDM_CANCEL, &CLoginDlg::OnIDM_Cancel)
    ON_UPDATE_COMMAND_UI(IDM_LOGIN_REMPASS, &CLoginDlg::OnRemPassUpdateUI)
//#endif // WIN32_PLATFORM_WFSP
ON_UPDATE_COMMAND_UI(IDM_LOGIN, &CLoginDlg::OnUpdateLogin)
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
	

	Lib_ReadReg(_T("PWD"), m_passwd);
	Lib_ReadReg(_T("ID"), m_fetion_id);
	Lib_ReadReg(_T("S_ADDR"), m_server_addr);

	if (!m_passwd.IsEmpty() || !m_fetion_id.IsEmpty())
	{
		m_bRemPass = true;

		this->UpdateData(FALSE);
	}
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
    int xIDC_FETION_ID, yIDC_FETION_ID, wIDC_FETION_ID, hIDC_FETION_ID;
    int xIDC_PWD, yIDC_PWD, wIDC_PWD, hIDC_PWD;
    int xIDC_LOGIN_STATE, yIDC_LOGIN_STATE, wIDC_LOGIN_STATE, hIDC_LOGIN_STATE;
    int xIDC_LOGIN, yIDC_LOGIN, wIDC_LOGIN, hIDC_LOGIN;
    int xIDC_REMACC, yIDC_REMACC, wIDC_REMACC, hIDC_REMACC;
    
    int iHeight, iWidth;
    int iMargin = DRA::SCALEX(5);
    RECT rcCtl;
    ::GetClientRect(this->m_hWnd, &rcCtl);
    
    iHeight = rcCtl.bottom - rcCtl.top;
    iWidth = rcCtl.right - rcCtl.left;

    xIDC_LOGO = rcCtl.left + iWidth / 2 - 60;//图像宽度120
    yIDC_LOGO = (rcCtl.bottom - rcCtl.top - 29 - DRA::SCALEY(120)) / 2;
    wIDC_LOGO = 120;
    hIDC_LOGO = 29;

    xIDC_STATIC_ID = rcCtl.left + DRA::SCALEX(10);
    yIDC_STATIC_ID = yIDC_LOGO + 29 + DRA::SCALEY(10);
    wIDC_STATIC_ID = DRA::SCALEX(68);
    hIDC_STATIC_ID = DRA::SCALEY(25);

    xIDC_STATIC_PWD = xIDC_STATIC_ID;
    yIDC_STATIC_PWD = yIDC_STATIC_ID + hIDC_STATIC_ID + iMargin;
    wIDC_STATIC_PWD = wIDC_STATIC_ID;
    hIDC_STATIC_PWD = hIDC_STATIC_ID;

    xIDC_FETION_ID = xIDC_STATIC_ID + wIDC_STATIC_ID + iMargin;
    yIDC_FETION_ID = yIDC_STATIC_ID;
    wIDC_FETION_ID = iWidth - xIDC_FETION_ID - iMargin;
    hIDC_FETION_ID = hIDC_STATIC_ID;

    xIDC_PWD = xIDC_FETION_ID;
    yIDC_PWD = yIDC_STATIC_PWD;
    wIDC_PWD = wIDC_FETION_ID;
    hIDC_PWD = hIDC_FETION_ID;


    xIDC_REMACC = xIDC_FETION_ID;
    yIDC_REMACC = yIDC_PWD + hIDC_PWD + iMargin;
    wIDC_REMACC = DRA::SCALEX(80);
    hIDC_REMACC = hIDC_FETION_ID;

    xIDC_LOGIN = xIDC_FETION_ID;
    yIDC_LOGIN = yIDC_REMACC + hIDC_REMACC + iMargin;
    wIDC_LOGIN = wIDC_STATIC_ID;
#if defined(M8)
    hIDC_LOGIN = hIDC_STATIC_ID;// 应要求,隐藏按钮,如果需要按钮, 赋值 hIDC_STATIC_ID;
#else
    hIDC_LOGIN = 0;
#endif

    xIDC_LOGIN_STATE = xIDC_STATIC_ID;
    yIDC_LOGIN_STATE = yIDC_LOGIN + hIDC_LOGIN + iMargin;
    wIDC_LOGIN_STATE = iWidth - 2 * iMargin;
    hIDC_LOGIN_STATE = hIDC_STATIC_ID;

    HWND hwndctl = ::GetDlgItem(this->m_hWnd, IDC_LOGO );
    ::MoveWindow(hwndctl, xIDC_LOGO, yIDC_LOGO , wIDC_LOGO , hIDC_LOGO , false);
    
    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_STATIC_ID);
    ::MoveWindow(hwndctl, xIDC_STATIC_ID, yIDC_STATIC_ID, wIDC_STATIC_ID, hIDC_STATIC_ID, false);

    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_STATIC_PWD);
    ::MoveWindow(hwndctl, xIDC_STATIC_PWD, yIDC_STATIC_PWD, wIDC_STATIC_PWD, hIDC_STATIC_PWD, false);

    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_FETION_ID);
    ::MoveWindow(hwndctl, xIDC_FETION_ID, yIDC_FETION_ID, wIDC_FETION_ID, hIDC_FETION_ID, false);

    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_PWD);
    ::MoveWindow(hwndctl, xIDC_PWD, yIDC_PWD, wIDC_PWD, hIDC_PWD, false);
    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_LOGIN);
    ::MoveWindow(hwndctl, xIDC_LOGIN, yIDC_LOGIN, wIDC_LOGIN, hIDC_LOGIN, false);

    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_REMACC);
    ::MoveWindow(hwndctl, xIDC_REMACC, yIDC_REMACC, wIDC_REMACC, hIDC_REMACC, false);

    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_LOGIN_STATE);
    ::MoveWindow(hwndctl, xIDC_LOGIN_STATE, yIDC_LOGIN_STATE, wIDC_LOGIN_STATE, hIDC_LOGIN_STATE, true);
}
#endif

void  My_EventListener (int message, WPARAM wParam, LPARAM lParam, void* args);

void CLoginDlg::OnBnClickedLogin()
{
    if(m_bIsLoging) {
        /* 最近的库里增加了取消登录的函数，所以我们可以在这里调用它，
           当第二次点击登录按钮时可以实现取消登录的功能 */
        fx_cancel_login();
        return ;
    }
    m_bIsLoging = true;

#if DEBUG_GUI
	this->m_LoginFlag = TRUE;
	::SendMessage(this->m_hWnd, WM_CLOSE,0, 0);
	return;
#endif
	
	this->m_LoginFlag = FALSE;
	
	this->UpdateData();
    OnBnClickedRemAccount();

	if(this->m_fetion_id.IsEmpty()) {
		this->m_login_state = _T("请输入飞信号码");
		goto fail;
	}
	
	//fetion号长度为9位， 手机号长度为11位
	if (this->m_fetion_id.GetLength()  != 9 && this->m_fetion_id.GetLength() != 11) {
		this->m_login_state = _T("请输入正确的飞信号码");
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
    this->m_login_state = _T("登陆中...");
	//fx_set_serve_address("221.130.45.208:8080");

	char* fetion_id = ConvertUtf16ToUtf8(m_fetion_id);
	char* pwd = ConvertUtf16ToUtf8(m_passwd);
	

	if (!this->m_server_addr.IsEmpty()) 
	{ 
		//we should read the fetion server address if we set it before.
		char* server_addr = ConvertUtf16ToUtf8(m_server_addr);
		fx_set_serve_address(server_addr);
		if (server_addr)
			delete [] server_addr;
	}

	fx_login(fetion_id, pwd,(My_EventListener), this);

	if (fetion_id)
		delete [] fetion_id;
	if (pwd)
		delete [] pwd;
    goto lfinally;
fail:
    m_bIsLoging = false;

lfinally:
	this->UpdateData(FALSE);
	return ;
}


void  My_EventListener (int message, WPARAM wParam, LPARAM lParam, void* args)
{
	CLoginDlg *loginDlg = NULL;
	
	if(!args)
		return;
    
    bool bLoginFail = false;

	loginDlg = (CLoginDlg *)args;
    
	switch(message)
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
		Lib_WriteReg(_T("S_ADDR"), loginDlg->m_server_addr);

		::SendMessage(loginDlg->m_hWnd, WM_CLOSE, (WPARAM)bLoginFail, 0);

		break;
	}

	//safe to do for our process..
	::SendMessage(loginDlg->m_hWnd, FX_LOGINSTATE_MSG, bLoginFail, 0);
}

LRESULT CLoginDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case FX_LOGINSTATE_MSG:
		this->UpdateData(FALSE);
        //如果连接失败则允许再次连接
        if((bool)(LOWORD(wParam)))
            m_bIsLoging = false;
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
		if (VK_TBACK == HIWORD(lParam))
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
		case VK_RETURN: 
			OnBnClickedLogin();
			return TRUE;
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

long Lib_ReadReg(CString RegKey, CString &Buf);

void CLoginDlg::OnBnClickedRemAccount()
{
	if (m_bRemPass)
	{	
		//this->UpdateData();
		Lib_WriteReg(_T("PWD"), m_passwd);
		Lib_WriteReg(_T("ID"), m_fetion_id);
	} else {
		Lib_WriteReg(_T("ID"), _T(""));
		Lib_WriteReg(_T("PWD"), _T(""));
	}
}


void CLoginDlg::OnLogin()
{
	OnBnClickedLogin();
}

void CLoginDlg::OnIDM_Cancel()
{
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
	this->m_login_state=_T("连接网络...");
	this->UpdateData(FALSE);
	this->UpdateWindow();

    CONNMGR_CONNECTIONINFO ConnInfo = { 0 };
    ConnInfo.cbSize      = sizeof(ConnInfo);
    ConnInfo.dwParams    = CONNMGR_PARAM_GUIDDESTNET; 
    ConnInfo.dwPriority  = CONNMGR_PRIORITY_USERINTERACTIVE; 
    {
        GUID IID_DestNetInternet = {0x436ef144, 0xb4fb, 0x4863, 0xa0, 0x41, 0x8f, 0x90, 0x5a, 0x62, 0xc5, 0x72};
        ConnInfo.guidDestNet = IID_DestNetInternet; 
    }

    DWORD dwStatus = 0; 
    ConnMgrEstablishConnectionSync(&ConnInfo, hConnect, 25000, &dwStatus);
	DWORD dwStartTime = GetTickCount();
	BOOL bRet = FALSE;
	while ( GetTickCount ()-dwStartTime < 15* 1000 )
	{
		if ( m_hConnection )
		{
			DWORD dwStatus = 0;
			HRESULT hr = ConnMgrConnectionStatus ( hConnect, &dwStatus );
			if ( SUCCEEDED(hr) )
			{
				if ( dwStatus == CONNMGR_STATUS_CONNECTED )
				{
					return TRUE;
				}
			}
		}
		Sleep(100);
	}
	return FALSE;

}
#endif

void CLoginDlg::OnRemPassChanged()
{
    UpdateData();
    m_bRemPass = !m_bRemPass;
    UpdateData(FALSE);
}

void CLoginDlg::OnRemPassUpdateUI(CCmdUI* cmdui)
{
    cmdui->SetCheck(m_bRemPass);
}
void CLoginDlg::OnUpdateLogin(CCmdUI *pCmdUI)
{
    if(m_bIsLoging)
        pCmdUI->SetText(_T("取消登录"));
    else
        pCmdUI->SetText(_T("登录"));
}
