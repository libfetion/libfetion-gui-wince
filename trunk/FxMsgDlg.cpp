
/***************************************************************************
 *   Copyright (C) 2008 by DDD                                          *
 *   dedodong@163.com                                                     *
 *                                                                         *
 ***************************************************************************/
// FxMsgDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WMLF.h"
#include "FxMsgDlg.h"
#include "FxMainWin.h"
#include "BuddyInfoDlg.h"

#ifdef M8
#include "M8Misc.h"
#endif

#define USER_SCROLLTOLAST WM_USER+1000 //消息框滚动到最底部

// FxMsgDlg 对话框

IMPLEMENT_DYNAMIC(FxMsgDlg, CDialog)
FxMsgDlg::FxMsgDlg(long ac_id, CWnd* pParent /*= NULL*/, BOOL SendFlag /*= FALSE*/)
: CDialog(FxMsgDlg::IDD, pParent)
, account_id(ac_id)
, account_name(_T(""))
, m_isSendSMS(SendFlag)
, m_account(NULL)
, m_msgBrowser(_T(""))
, m_msgSend(_T(""))
, m_isLoginOK(FALSE)
, m_strInfo(_T(""))
{
	EnableAutomation();
}

FxMsgDlg::~FxMsgDlg()
{
}

void FxMsgDlg::OnFinalRelease()
{
	// 释放了对自动化对象的最后一个引用后，将调用
	// OnFinalRelease。基类将自动
	// 删除该对象。在调用该基类之前，请添加您的
	// 对象所需的附加清除代码。

	CDialog::OnFinalRelease();
}

void FxMsgDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_MSG_BROWSER, m_msgBrowser);
    DDX_Text(pDX, IDC_SEND_MSG, m_msgSend);
    DDX_Control(pDX, IDC_MSG_BROWSER, m_browser);
    DDX_Control(pDX, IDC_SEND_MSG, m_send);
    DDX_Text(pDX, IDC_MSG_INFO, m_strInfo);
}


BEGIN_MESSAGE_MAP(FxMsgDlg, CDialog)
#if defined(_DEVICE_RESOLUTION_AWARE) //&& !defined(WIN32_PLATFORM_WFSP)
	ON_WM_SIZE()
#endif
	ON_STN_CLICKED(IDC_SEND, &FxMsgDlg::OnStnClickedSend)
	ON_STN_CLICKED(IDC_CHANGE, &FxMsgDlg::OnStnClickedChange)
//#ifdef WIN32_PLATFORM_WFSP
	ON_COMMAND(IDR_MSG_SEND, &FxMsgDlg::OnMsgSend)
	ON_COMMAND(IDM_MSG_CHANGE_METHOD, &FxMsgDlg::OnMsgChangeMethod)
//#endif // WIN32_PLATFORM_WFSP
	ON_EN_CHANGE(IDC_SEND_MSG, &FxMsgDlg::OnEnChangeSendMsg)
//    ON_WM_ACTIVATE()
    ON_COMMAND(IDM_BD_VIEWINFO, &FxMsgDlg::OnBdViewinfo)
    ON_WM_TIMER()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(IDM_COPY1, &FxMsgDlg::OnBrowserCopy)
	ON_COMMAND(IDM_COPY2, &FxMsgDlg::OnSendCopy)
	ON_COMMAND(IDM_PASTE, &FxMsgDlg::OnSendPaste)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(FxMsgDlg, CDialog)
END_DISPATCH_MAP()

// 注意: 我们添加 IID_IFxMsgDlg 支持
//  以支持来自 VBA 的类型安全绑定。此 IID 必须同附加到 .IDL 文件中的
//  调度接口的 GUID 匹配。

// {9203B7B4-C330-4B3D-937C-E96AEC53DA7B}
static const IID IID_IFxMsgDlg =
{ 0x9203B7B4, 0xC330, 0x4B3D, { 0x93, 0x7C, 0xE9, 0x6A, 0xEC, 0x53, 0xDA, 0x7B } };

BEGIN_INTERFACE_MAP(FxMsgDlg, CDialog)
	INTERFACE_PART(FxMsgDlg, IID_IFxMsgDlg, Dispatch)
END_INTERFACE_MAP()


// FxMsgDlg 消息处理程序

BOOL FxMsgDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_browser.SetReadOnly();

#ifdef M8
    AddMenuBarForM8(this->GetSafeHwnd(), IDR_MSG_MENU);
    FullScreen(this->GetSafeHwnd());
#else
	if (!m_dlgCommandBar.Create(this) ||
		!m_dlgCommandBar.InsertMenuBar(IDR_MSG_MENU))
	{
		TRACE0("未能创建 CommandBar\n");
		return FALSE;      // 未能创建
	}
#endif // M8
	SetNewBuddy(account_id);
	return TRUE;  // return TRUE unless you set the focus to a control
}

BOOL FxMsgDlg::SetNewBuddy(long ac_id)
{
	account_id = ac_id;
#if DEBUG_GUI
#else
	m_account = fx_get_account_by_id(this->account_id);
	//begin a dialog init, if the account is mobile, this function will do nothing...
	if (!m_isSendSMS)
		fx_begin_dialog (this->account_id, NULL, NULL); 

	//如果得不到m_account 则说明是陌生人..就是那些老发垃圾广告的混蛋!!!
	if (m_account)
	{
		char * showname = fx_get_account_show_name(m_account, FALSE);
		account_name = ConvertUtf8ToUtf16(showname);
		if(showname)
			free(showname);
	} else {
		account_name.Format(_T("%d"), this->account_id);
	}
#endif
	//this->m_msgBrowser += _T("你正在与朋友聊天:") + account_name + _T("\r\n") ;
	this->UpdateData(FALSE);
	printfOnlineInfo();

	GetDlgItem(IDC_SEND_MSG)->SetFocus();
	
	if(this->m_isLoginOK)
	{
		this->GetDlgItem(IDC_NET_STATE)->ShowWindow(SW_HIDE);
		//this->GetDlgItem(IDC_SEND)->ShowWindow(SW_SHOW);
		this->GetDlgItem(IDC_SEND)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_SEND_MSG)->EnableWindow(TRUE);
	} else {
		//this->GetDlgItem(IDC_SEND)->ShowWindow(SW_HIDE);
		this->GetDlgItem(IDC_SEND)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_SEND_MSG)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_NET_STATE)->ShowWindow(SW_SHOW);
	}
	this->UpdateWindow();

    SetTimer(USER_SCROLLTOLAST, 500, 0);

	return TRUE;
}

#define SYSTEM_ID 10000

void FxMsgDlg::printfOnlineInfo()
{
	this->UpdateData();

	CString status;
	if (this->account_id == SYSTEM_ID)
	{
		this->m_msgBrowser += CString(_T("系统消息")) + CString(_T("\r\n"));
		return ;
	}

#if !DEBUG_GUI
	if(!m_account) {
		m_strInfo.Format(_T("与 %s 陌生人聊天中"), account_name);
		goto out;
	}
#endif

	if (!fx_is_pc_user_by_account (m_account)) {
        m_strInfo.Format(_T("与 %s 聊天中，他/她是短信用户"), account_name);
		//this->m_msgBrowser += _T("他/她是手机用户，你的消息将发送到对方手机上") + CString(_T("\r\n"));
		m_isSendSMS = true;
		goto out;
	}

	if(m_isSendSMS) {
		if(fx_get_refuse_sms_day(m_account) > 0)
            status = _T("他/她已离线");
			//status =_T("他/她已离线,不能立即收到你的消息。你发送的消息将被保存下来并延迟发送");
		else
            status = _T("将通过短信发送您的消息");
			//status =_T("你的信息将发生到对方的手机上");
	} else {
		if(fx_is_on_line_by_account (m_account)) {
            status = _T("您的消息将发送到电脑上");
			//status = _T("他/她在线，你的消息将发送到电脑上");
		} else {
			if(fx_get_refuse_sms_day(m_account) > 0)
                status = _T("他/她已离线。将延迟发送您的消息。");
				//status = _T("他/她已离线,不能立即收到你的消息。你发送的消息将被保存下来并延迟发送");
			else
                status = _T("他/她已离线。将通过短信发送您的消息。");
				//status = _T("他/她已离线,你的信息将发生到对方的手机上"); 
		} //end of !fx_is_on_line_by_account (m_account)) 
	} //end of !m_isSendSMS
    m_strInfo.Format(_T("与 %s 聊天中，%s"), account_name, status);
	//this->m_msgBrowser += status + _T("\r\n");
out:
	this->UpdateData(FALSE);
	this->addNewMsg();
}


#if defined(_DEVICE_RESOLUTION_AWARE)// && !defined(WIN32_PLATFORM_WFSP)
void FxMsgDlg::OnSize(UINT nType, int cx, int cy)
{
//#if 0
//	DRA::RelayoutDialog(
//		AfxGetInstanceHandle(), 
//		this->m_hWnd, 
//		DRA::GetDisplayMode() != DRA::Portrait ? 
//		MAKEINTRESOURCE(IDD_WMLF_MSG_WIDE) : 
//	MAKEINTRESOURCE(IDD_WMLF_MSG));
//#endif
    //DRA::DisplayMode dp = DRA::GetDisplayMode();

    //if(dp == DRA::Portrait)
    {
        int xIDC_MSG_INFO, yIDC_MSG_INFO, wIDC_MSG_INFO, hIDC_MSG_INFO;
        int xIDC_MSG_BROWSER, yIDC_MSG_BROWSER, wIDC_MSG_BROWSER, hIDC_MSG_BROWSER;
        int xIDC_CHANGE, yIDC_CHANGE, wIDC_CHANGE, hIDC_CHANGE;
        int xIDC_SEND, yIDC_SEND, wIDC_SEND, hIDC_SEND;
        int xIDC_SEND_MSG, yIDC_SEND_MSG, wIDC_SEND_MSG, hIDC_SEND_MSG;
        int iHeight, iWidth;
        int iMargin = DRA::SCALEY(2);

	    RECT rcCtl;
	    ::GetClientRect(this->m_hWnd, &rcCtl);
#ifdef M8
        rcCtl.top += 40;
#endif
        iHeight = rcCtl.bottom - rcCtl.top;
        iWidth = rcCtl.right - rcCtl.left;
        
        xIDC_MSG_INFO = rcCtl.left + iMargin;
        yIDC_MSG_INFO = rcCtl.top + iMargin;
        wIDC_MSG_INFO = iWidth - iMargin * 2;
        hIDC_MSG_INFO = DRA::SCALEY(32);

        xIDC_SEND_MSG = xIDC_MSG_INFO;
        hIDC_SEND_MSG = DRA::SCALEY(44);
        yIDC_SEND_MSG = iHeight - iMargin - hIDC_SEND_MSG;
        wIDC_SEND_MSG = wIDC_MSG_INFO;

        xIDC_CHANGE = xIDC_MSG_INFO;

#ifdef M8
        hIDC_CHANGE = 19;    //如果需要显示中间两个按钮的话,这里的值赋19
#else
        hIDC_CHANGE = 0;
#endif
        yIDC_CHANGE = yIDC_SEND_MSG - hIDC_CHANGE - iMargin;
        wIDC_CHANGE = 50;

        xIDC_SEND = rcCtl.right - iMargin - 80;
        yIDC_SEND = yIDC_CHANGE;
        wIDC_SEND = 78;
        hIDC_SEND = hIDC_CHANGE;

        xIDC_MSG_BROWSER = xIDC_MSG_INFO;
        yIDC_MSG_BROWSER = yIDC_MSG_INFO + hIDC_MSG_INFO + iMargin;
        wIDC_MSG_BROWSER = wIDC_MSG_INFO;
        hIDC_MSG_BROWSER = yIDC_CHANGE - (hIDC_CHANGE == 0 ? 0 : iMargin) - yIDC_MSG_BROWSER;


        HWND hwndctl = ::GetDlgItem(this->m_hWnd, IDC_MSG_INFO);
        ::MoveWindow(hwndctl, xIDC_MSG_INFO, yIDC_MSG_INFO, wIDC_MSG_INFO, hIDC_MSG_INFO, false);

        hwndctl = ::GetDlgItem(this->m_hWnd, IDC_MSG_BROWSER);
        ::MoveWindow(hwndctl, xIDC_MSG_BROWSER, yIDC_MSG_BROWSER, wIDC_MSG_BROWSER, hIDC_MSG_BROWSER, false);
        
        hwndctl = ::GetDlgItem(this->m_hWnd, IDC_CHANGE);
        ::MoveWindow(hwndctl, xIDC_CHANGE, yIDC_CHANGE, wIDC_CHANGE, hIDC_CHANGE, false);

        hwndctl = ::GetDlgItem(this->m_hWnd, IDC_SEND);
        ::MoveWindow(hwndctl, xIDC_SEND, yIDC_SEND, wIDC_SEND, hIDC_SEND, false);

        hwndctl = ::GetDlgItem(this->m_hWnd, IDC_SEND_MSG);
        ::MoveWindow(hwndctl, xIDC_SEND_MSG, yIDC_SEND_MSG, wIDC_SEND_MSG, hIDC_SEND_MSG, true);
    }
	CDialog::OnSize(nType, cx, cy);	
}
#endif


void FxMsgDlg::OnStnClickedChange()
{
	m_isSendSMS = !m_isSendSMS;
	printfOnlineInfo();
}


void FxMsgDlg::OnStnClickedSend()
{
	this->UpdateData();

	if (this->m_msgSend.IsEmpty())
		return;

	int sendFlag = 0;

	char* content = ConvertUtf16ToUtf8(m_msgSend);

	//fixme: offline usr can send offline dialog
	if (m_isSendSMS || !fx_is_pc_user_by_id(account_id))
		sendFlag = fx_send_sms( account_id, content, NULL, NULL);   
	else 
	{
		//note: just dialog_send need replace to html mark.  
		m_msgSend.Replace(_T("<"), _T("&lt;"));
		m_msgSend.Replace(_T(">"), _T("&gt;"));
		sendFlag = fx_dialog_send( account_id, content, NULL, NULL); 
	}
	if (content)
		delete [] content;
	
	CString show_msg;
	CString head;
	if(sendFlag) 
		head = _T("我(") + GetCurrentTimeString() + _T("):");
	else 
		head = _T("发送失败(") + GetCurrentTimeString() + _T("):");

	m_msgSend.Replace(_T("&lt;"), _T("<"));
	m_msgSend.Replace(_T("&gt;"), _T(">"));

	this->m_msgBrowser += head + m_msgSend + _T("\r\n\r\n");
	//	saveHistroyMsg(strtol(fx_get_usr_uid(), NULL, 10), account_id, show_msg.toUtf8().data(), NULL);
	//clean the send edit
	m_msgSend = _T("");
	//((CEdit*)GetDlgItem(IDC_MSG_BROWSER))->SetSel(dwLen,dwLen,TRUE);
	this->UpdateData(FALSE);
    ScrollToLast();
}

void FxMsgDlg::addNewMsg(CString msg /*= ""*/)
{
	if (msg.IsEmpty())
		getMsg(msg);

/*first: we update the data of control val, then we reset then,
so that, the m_send data will not be losted. */
	this->UpdateData();
	this->m_msgBrowser += msg;
	this->UpdateData(FALSE);
    ScrollToLast();

}

void FxMsgDlg::getMsg(CString &msg)
{ 
	//get fetion msg
	Fetion_MSG * fxMsg = NULL;

	while (fxMsg = fx_get_msg(this->account_id))
	{
		char * msg_contain = fx_msg_no_format(fxMsg->message); 

		msg +=  this->account_name + _T("(") + GetMsgTimeString(fxMsg->msgtime) + _T("):");
		msg += ConvertUtf8ToUtf16(msg_contain) + CString(_T("\r\n\r\n"));
		if (msg_contain)
			free(msg_contain);
		fx_destroy_msg (fxMsg);
		fxMsg = NULL;
	}

	//get tmp msg
	TMPMSG_Info* msg_info = NULL;
	FxMainWin* parent = (FxMainWin* )this->GetParent();
	if (!parent)
		return;
	POSITION pos = parent->tmpMsg.GetTailPosition();
	int count = parent->tmpMsg.GetCount();

	for (int i=0; i <count; i++)
	{
		POSITION tmp = pos;
		msg_info = (TMPMSG_Info*)parent->tmpMsg.GetPrev(pos);
		if (msg_info->accountID == this->account_id)
		{
			msg += msg_info->msg;
			parent->tmpMsg.RemoveAt(tmp);
			delete msg_info;
		}
	}
}
void FxMsgDlg::OnMsgSend()
{
	//::AfxMessageBox(_T("MsgSend"));
	OnStnClickedSend();
}

void FxMsgDlg::OnMsgChangeMethod()
{
	//::AfxMessageBox(_T("OnMsgChangeMethod"));
	OnStnClickedChange();
}

void FxMsgDlg::OnEnChangeSendMsg()
{
	// TODO:  Èç¹û¸Ã¿Ø¼þÊÇ RICHEDIT ¿Ø¼þ£¬ÔòËü½«²»»á
	// ·¢ËÍ¸ÃÍ¨Öª£¬³ý·ÇÖØÐ´ CDialog::OnInitDialog()
	// º¯Êý²¢µ÷ÓÃ CRichEditCtrl().SetEventMask()£¬
	// Í¬Ê±½« ENM_CHANGE ±êÖ¾¡°»ò¡±ÔËËãµ½ÑÚÂëÖÐ¡£

	// TODO:  ÔÚ´ËÌí¼Ó¿Ø¼þÍ¨Öª´¦Àí³ÌÐò´úÂë
	//OnMsgChangeMethod();
}
#ifdef WIN32_PLATFORM_WFSP
#include "tpcshell.h"
#endif

void FxMsgDlg::OnCancel()
{
	
#ifdef WIN32_PLATFORM_WFSP
	// 在这里修改后退键的行为为删除EditBox中的内容，而不是退出模态对话框
	//如果文本框为多行的则必须加入下面这行代码，单行的则不用~
	::SendMessage(GetDlgItem(IDC_SEND_MSG)->m_hWnd, EM_SETEXTENDEDSTYLE, 0, ES_EX_CLEARONBACKPRESSHOLD);
	SHSendBackToFocusWindow(WM_HOTKEY, 2, MAKELPARAM(MOD_KEYUP, VK_TBACK));
#else
	CDialog::OnCancel();
#endif // WIN32_PLATFORM_W

}

// 将聊天记录滚动到最下面
void FxMsgDlg::ScrollToLast(void)
{
	int dwLen = m_msgBrowser.GetLength();
	this->m_browser.SetSel (dwLen,dwLen);
}

void FxMsgDlg::OnBdViewinfo()
{
#if DEBUG_GUI
    CBuddyInfoDlg* dlg = new CBuddyInfoDlg(123456);
#else
    CBuddyInfoDlg* dlg = new CBuddyInfoDlg(this->m_account->id);
#endif
    dlg->DoModal();
    delete dlg;
}

void FxMsgDlg::OnTimer(UINT_PTR nIDEvent)
{
    switch(nIDEvent)
	{
    case USER_SCROLLTOLAST:
        ScrollToLast();
        KillTimer(nIDEvent);
        break;
    default:
        break;
    }
    CDialog::OnTimer(nIDEvent);
}

void FxMsgDlg::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: 在此处添加消息处理程序代码
	int iID;
	iID = pWnd->GetDlgCtrlID();
	if(iID == m_browser.GetDlgCtrlID())
	{
			CMenu *pMenu;

            pMenu=new CMenu;
            
            pMenu->LoadMenu( IDR_MSG_POP_MENU );  //加载菜单资源
            CMenu *pMenuPop=pMenu->GetSubMenu(0); //资源菜单的第一项
			pMenuPop->TrackPopupMenu( TPM_LEFTALIGN, point.x, point.y, this);
            delete pMenu;
	}
	if(iID == m_send.GetDlgCtrlID())
	{
			CMenu *pMenu;

            pMenu=new CMenu;
            
            pMenu->LoadMenu( IDR_MSG_POP_MENU );  //加载菜单资源
            CMenu *pMenuPop=pMenu->GetSubMenu(1); //资源菜单的第二项
			pMenuPop->TrackPopupMenu( TPM_LEFTALIGN, point.x, point.y, this);
            delete pMenu;
	}
}

void FxMsgDlg::OnBrowserCopy()
{
	// TODO: 在此添加命令处理程序代码
	m_browser.Copy();
}

void FxMsgDlg::OnSendCopy()
{
	// TODO: 在此添加命令处理程序代码
	m_send.Copy();
}

void FxMsgDlg::OnSendPaste()
{
	// TODO: 在此添加命令处理程序代码
	m_send.Paste();
}
