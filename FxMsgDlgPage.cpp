// FxMsgDlgPage.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WMLF.h"
#include "FxMsgDlgPage.h"
#include "FxMsgDlgView.h"
#include "FxMainWin.h"
#include "FxDatabase.h"
#ifdef WIN32_PLATFORM_WFSP
#include <tpcshell.h>
#endif

extern CFxDatabase * g_pFxDB;

#define USER_SCROLLTOLAST WM_USER+1000 //��Ϣ���������ײ�
#define SYSTEM_ID 10000

// CFxMsgDlgPage �Ի���

IMPLEMENT_DYNAMIC(CFxMsgDlgPage, CDialog)

CFxMsgDlgPage::CFxMsgDlgPage(long lAccountID, CWnd* pParent /*=NULL*/, BOOL bLoginOK /*=TRUE*/)
	: CDialog(CFxMsgDlgPage::IDD, pParent)
	, m_lAccountID(lAccountID)
	, m_isSendSMS(FALSE)
	, m_bMyself(FALSE)
	, m_bNotReadFlag(FALSE)
	, m_isLoginOK(bLoginOK)
	, m_bInit(FALSE)
{
	m_pParentWnd = pParent;
}

CFxMsgDlgPage::~CFxMsgDlgPage()
{
}

void CFxMsgDlgPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_MSG_BROWSER, m_msgBrowser);
    DDX_Text(pDX, IDC_SEND_MSG, m_msgSend);
    DDX_Control(pDX, IDC_MSG_BROWSER, m_browser);
    DDX_Control(pDX, IDC_SEND_MSG, m_send);
    DDX_Text(pDX, IDC_MSG_INFO, m_strInfo);
}


BEGIN_MESSAGE_MAP(CFxMsgDlgPage, CDialog)
	ON_WM_SIZE()
    ON_WM_TIMER()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(IDM_COPY1, &CFxMsgDlgPage::OnBrowserCopy)
	ON_COMMAND(IDM_COPY2, &CFxMsgDlgPage::OnSendCopy)
	ON_COMMAND(IDM_PASTE, &CFxMsgDlgPage::OnSendPaste)
END_MESSAGE_MAP()


// CFxMsgDlgPage ��Ϣ�������

CString CFxMsgDlgPage::GetTitle()
{
	if(m_bMyself)
	{
		m_account_name = _T("�Լ�");
		return m_account_name;
	}
	m_account = fx_get_account_by_id(m_lAccountID);
	m_Qun = fx_get_qun_by_id(m_lAccountID);
	
	//����ò���m_account ��˵����İ����..������Щ�Ϸ��������Ļ쵰!!!
	if (m_account!=NULL)
	{
		m_IsQun=FALSE;
		m_IsStranger=FALSE;
		char * showname = fx_get_account_show_name(m_account, FALSE);
		m_account_name = ConvertUtf8ToUtf16(showname);
		if(showname)
		{
			free(showname);
			showname = NULL;
		}
	}
	else if (NULL!=m_Qun)
	{
		m_IsQun=TRUE;
		m_IsStranger=FALSE;
		char * showname=fx_get_qun_show_name((Fetion_Qun*)m_Qun);
		m_account_name=ConvertUtf8ToUtf16(showname);
		if(showname)
		{
			free(showname);
			showname = NULL;
		}
	}
	else 
	{
		m_IsQun=FALSE;
		m_IsStranger=TRUE;
		m_account_name.Format(_T("%d"), m_lAccountID);
	}

	return m_account_name;
}

BOOL CFxMsgDlgPage::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_bInit = TRUE;
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_browser.SetReadOnly();
	//begin a dialog init, if the account is mobile, this function will do nothing...
	if (!m_isSendSMS)
		fx_begin_dialog (m_lAccountID, NULL, NULL); 

	LoginOK(m_isLoginOK);
	m_send.SetFocus();

	//this->UpdateWindow();

    SetTimer(USER_SCROLLTOLAST, 500, 0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CFxMsgDlgPage::ShowOnlineInfo()
{
	UpdateData();

	if(!m_isLoginOK)
	{
		m_strInfo.Format(_T("�����ѻ���������������..."));
		UpdateData(FALSE);
		return;
	}

	if(m_bMyself)
	{
		m_strInfo.Format(_T("����������..."));
		UpdateData(FALSE);
		return;
	}

	CString status;
	if (m_lAccountID == SYSTEM_ID)
	{
		m_msgBrowser += CString(_T("ϵͳ��Ϣ")) + CString(_T("\r\n"));
		UpdateData(FALSE);
		ScrollToLast();
		return;
	}

	if(m_IsStranger)
	{
		m_strInfo.Format(_T("�� %s İ����������"), m_account_name);
		goto out;
	}
	if(m_IsQun)
	{
		m_strInfo.Format(_T("�� %s Ⱥ��������"),m_account_name);
		goto out;
	}

	if (!fx_is_pc_user_by_account (m_account)) {
        m_strInfo.Format(_T("�� %s �����У���/���Ƕ����û�"), m_account_name);
		//this->m_msgBrowser += _T("��/�����ֻ��û��������Ϣ�����͵��Է��ֻ���") + CString(_T("\r\n"));
		m_isSendSMS = TRUE;
		goto out;
	}

	if(m_isSendSMS)
	{
		if(fx_get_refuse_sms_day(m_account) > 0)
            status = _T("��/�������ߡ�");
			//status =_T("��/��������,���������յ������Ϣ���㷢�͵���Ϣ���������������ӳٷ���");
		else
            status = _T("��ͨ�����ŷ���������Ϣ��");
			//status =_T("�����Ϣ���������Է����ֻ���");
	}
	else
	{
		if(fx_is_on_line_by_account (m_account))
		{
            status = _T("��/�����ߡ�");
			//status = _T("��/�����ߣ������Ϣ�����͵�������");
		}
		else
		{
			if(fx_get_refuse_sms_day(m_account) > 0)
			{
                status = _T("��/�������ߣ����ӳٷ���������Ϣ��");
				//status = _T("��/��������,���������յ������Ϣ���㷢�͵���Ϣ���������������ӳٷ���");
			}
			else
			{
                status = _T("��/�������ߣ���ͨ�����ŷ���������Ϣ��");
				//status = _T("��/��������,�����Ϣ���������Է����ֻ���"); 
			}
		} //end of !fx_is_on_line_by_account (m_account)) 
	} //end of !m_isSendSMS
    m_strInfo.Format(_T("�� %s �����У�%s"), m_account_name, status);
	//this->m_msgBrowser += status + _T("\r\n");
out:
	UpdateData(FALSE);
	addNewMsg();
}

void CFxMsgDlgPage::OnSize(UINT nType, int cx, int cy)
{
    {
        int xIDC_MSG_INFO, yIDC_MSG_INFO, wIDC_MSG_INFO, hIDC_MSG_INFO;
        int xIDC_MSG_BROWSER, yIDC_MSG_BROWSER, wIDC_MSG_BROWSER, hIDC_MSG_BROWSER;
        int xIDC_SEND_MSG, yIDC_SEND_MSG, wIDC_SEND_MSG, hIDC_SEND_MSG;
        int iHeight, iWidth;
        int iMargin = DRA::SCALEY(2);

	    RECT rcCtl;
	    ::GetClientRect(this->m_hWnd, &rcCtl);

		iHeight = rcCtl.bottom - rcCtl.top;
        iWidth = rcCtl.right - rcCtl.left;
        
        xIDC_MSG_INFO = rcCtl.left + iMargin;
        yIDC_MSG_INFO = rcCtl.top + iMargin;
        wIDC_MSG_INFO = iWidth - iMargin * 2;
        hIDC_MSG_INFO = DRA::SCALEY(26);

        xIDC_SEND_MSG = xIDC_MSG_INFO;
        hIDC_SEND_MSG = DRA::SCALEY(44);
		if(iHeight < 200)
		{
			hIDC_SEND_MSG = DRA::SCALEY(30);
		}
        yIDC_SEND_MSG = iHeight - iMargin - hIDC_SEND_MSG;
        wIDC_SEND_MSG = wIDC_MSG_INFO;

        xIDC_MSG_BROWSER = xIDC_MSG_INFO;
        yIDC_MSG_BROWSER = yIDC_MSG_INFO + hIDC_MSG_INFO + iMargin;
        wIDC_MSG_BROWSER = wIDC_MSG_INFO;
        hIDC_MSG_BROWSER = yIDC_SEND_MSG - yIDC_MSG_BROWSER - iMargin;


        HWND hwndctl = ::GetDlgItem(this->m_hWnd, IDC_MSG_INFO);
        ::MoveWindow(hwndctl, xIDC_MSG_INFO, yIDC_MSG_INFO, wIDC_MSG_INFO, hIDC_MSG_INFO, false);

        hwndctl = ::GetDlgItem(this->m_hWnd, IDC_MSG_BROWSER);
        ::MoveWindow(hwndctl, xIDC_MSG_BROWSER, yIDC_MSG_BROWSER, wIDC_MSG_BROWSER, hIDC_MSG_BROWSER, false);
        
        hwndctl = ::GetDlgItem(this->m_hWnd, IDC_SEND_MSG);
        ::MoveWindow(hwndctl, xIDC_SEND_MSG, yIDC_SEND_MSG, wIDC_SEND_MSG, hIDC_SEND_MSG, true);
    }
	CDialog::OnSize(nType, cx, cy);	
}

void CFxMsgDlgPage::OnMsgChangeMethod()
{
	if(m_bMyself)
	{
		return;
	}
	m_isSendSMS = !m_isSendSMS;
	ShowOnlineInfo();
}

void CFxMsgDlgPage::OnMsgSend()
{
	this->UpdateData();

	if (this->m_msgSend.IsEmpty())
		return;

	int sendFlag = 0;
	MSGLOGDB MsgLog;
	MsgLog.lID = m_lAccountID;

	CStringA content = ConvertUtf16ToUtf8(m_msgSend);

	if(m_bMyself)
	{
		CStringA mobile_no = ConvertUtf16ToUtf8(((FxMainWin*)((CFxMsgDlgView*)m_pParentWnd)->m_pParentWnd)->m_mobile_no);
		sendFlag = fx_send_sms_by_mobile_no(mobile_no.GetBuffer(), content.GetBuffer(), NULL, NULL); 
	}
	//fixme: offline usr can send offline dialog
	else
	{
		if (m_isSendSMS || !fx_is_pc_user_by_id(m_lAccountID))
		{
			sendFlag = fx_send_sms( m_lAccountID, content.GetBuffer(), NULL, NULL);
		}
		else 
		{
			//note: just dialog_send need replace to html mark.  
			m_msgSend.Replace(_T("<"), _T("&lt;"));
			m_msgSend.Replace(_T(">"), _T("&gt;"));
			sendFlag = fx_dialog_send( m_lAccountID, content.GetBuffer(), NULL, NULL); 
		}
	}
	
	CString show_msg;
	CString head;
	if(sendFlag) 
		head = ((FxMainWin*)((CFxMsgDlgView*)m_pParentWnd)->m_pParentWnd)->m_strNickName;
	else 
		head = _T("����ʧ��");

	m_msgSend.Replace(_T("&lt;"), _T("<"));
	m_msgSend.Replace(_T("&gt;"), _T(">"));

	MsgLog.strSender = head;
	MsgLog.MsgTime = GetCurrentTime();
	MsgLog.strMsg = m_msgSend;

	AddReturn(m_msgBrowser);
	m_msgBrowser += FormatMsgLog(&MsgLog);
	//	saveHistroyMsg(strtol(fx_get_usr_uid(), NULL, 10), account_id, show_msg.toUtf8().data(), NULL);
	//clean the send edit
	m_msgSend = _T("");
	//((CEdit*)GetDlgItem(IDC_MSG_BROWSER))->SetSel(dwLen,dwLen,TRUE);
	this->UpdateData(FALSE);
    ScrollToLast();
	g_pFxDB->AddMsgLog(&MsgLog);
}

void CFxMsgDlgPage::ScrollToLast(void)
{
	int dwLen = m_msgBrowser.GetLength();
	this->m_browser.SetSel(dwLen,dwLen);
}

void CFxMsgDlgPage::addNewMsg(CString msg /*= ""*/)
{
	if (msg.IsEmpty())
		getMsg(msg);

/*first: we update the data of control val, then we reset then,
so that, the m_send data will not be losted. */
	if(m_bInit)
	{
		UpdateData();
	}

	if (msg.IsEmpty())
	{
		return;
	}
	AddReturn(m_msgBrowser);
	m_msgBrowser += msg;
	if(m_bInit)
	{
		UpdateData(FALSE);
		ScrollToLast();
	}
}

void CFxMsgDlgPage::getMsg(CString &msg)
{ 
	//get fetion msg
	Fetion_MSG * fxMsg = NULL;
	MSGLOGDB MsgLog;
	MsgLog.lID = m_lAccountID;

	while (fxMsg = fx_get_msg(m_lAccountID))
	{
		CString strSender;
		char * msg_contain = fx_msg_no_format(fxMsg->message); 
		if(!fxMsg->ext_id)
		{
			strSender =  m_account_name;
		}
		else
		{
			const Fetion_Account * sender_account = fx_get_account_by_id(fxMsg->ext_id);
			char * sender_name=fx_get_account_show_name(sender_account,FALSE);
			CString temp;
			temp.Format(_T("%d"),fxMsg->ext_id);
			if(sender_name!=NULL)
				strSender=ConvertUtf8ToUtf16(sender_name);
			else
				strSender=temp;
			if(sender_name)
			{
				free(sender_name);
				sender_name = NULL;
			}
		}
		MsgLog.strSender = strSender;
		MsgLog.MsgTime = GetMsgTime(fxMsg->msgtime);
		MsgLog.strMsg = ConvertUtf8ToUtf16(msg_contain);
		g_pFxDB->AddMsgLog(&MsgLog);
		AddReturn(msg);
		msg += FormatMsgLog(&MsgLog);
		if (msg_contain)
		{
			free(msg_contain);
			msg_contain = NULL;
		}
		fx_destroy_msg (fxMsg);
		fxMsg = NULL;
	}
}

void CFxMsgDlgPage::OnTBack()
{
#ifdef WIN32_PLATFORM_WFSP
	// �������޸ĺ��˼�����ΪΪɾ��EditBox�е����ݣ��������˳�ģ̬�Ի���
	//����ı���Ϊ���е����������������д��룬���е�����~
	::SendMessage(m_send.GetSafeHwnd(), EM_SETEXTENDEDSTYLE, 0, ES_EX_CLEARONBACKPRESSHOLD);
	SHSendBackToFocusWindow(WM_HOTKEY, VK_TBACK, MAKELPARAM(MOD_KEYUP, VK_TBACK));
#endif // WIN32_PLATFORM_WFSP
}

void CFxMsgDlgPage::OnTimer(UINT_PTR nIDEvent)
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

void CFxMsgDlgPage::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: �ڴ˴������Ϣ����������
	int iID;
	iID = pWnd->GetDlgCtrlID();
	if(iID == m_browser.GetDlgCtrlID())
	{
			CMenu *pMenu;

            pMenu=new CMenu;
            
            pMenu->LoadMenu( IDR_MSG_POP_MENU );  //���ز˵���Դ
            CMenu *pMenuPop=pMenu->GetSubMenu(0); //��Դ�˵��ĵ�һ��
			pMenuPop->TrackPopupMenu( TPM_LEFTALIGN, point.x, point.y, this);
            delete pMenu;
	}
	if(iID == m_send.GetDlgCtrlID())
	{
			CMenu *pMenu;

            pMenu=new CMenu;
            
            pMenu->LoadMenu( IDR_MSG_POP_MENU );  //���ز˵���Դ
            CMenu *pMenuPop=pMenu->GetSubMenu(1); //��Դ�˵��ĵڶ���
			pMenuPop->TrackPopupMenu( TPM_LEFTALIGN, point.x, point.y, this);
            delete pMenu;
	}
}

void CFxMsgDlgPage::OnBrowserCopy()
{
	// TODO: �ڴ���������������
	m_browser.Copy();
}

void CFxMsgDlgPage::OnSendCopy()
{
	// TODO: �ڴ���������������
	m_send.Copy();
}

void CFxMsgDlgPage::OnSendPaste()
{
	// TODO: �ڴ���������������
	m_send.Paste();
}
BOOL CFxMsgDlgPage::PreTranslateMessage(MSG* pMsg)
{

	if(pMsg->message==WM_KEYDOWN)         
	{
		if (pMsg->hwnd == m_browser.m_hWnd)
		{
			switch(pMsg->wParam)
			{
			case VK_RETURN: 
				//�������������Ϣ����ʱ����һ��ȷ�ϼ��������л���������Ϣ��
				m_send.SetFocus();
				return TRUE;
			}
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CFxMsgDlgPage::LoginOK(BOOL bLoginOK)
{
	m_isLoginOK = bLoginOK;
	if(m_isLoginOK)
	{
		m_send.SetReadOnly(FALSE);
	}
	else
	{
		m_send.SetReadOnly(TRUE);
	}
	ShowOnlineInfo();
}
