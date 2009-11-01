// FxMsgDlgView.cpp : 实现文件
//

#include "stdafx.h"
#include "WMLF.h"
#include "FxMsgDlgPage.h"
#include "FxMsgDlgView.h"
#include "FxMainWin.h"


// CFxMsgDlgView 对话框

IMPLEMENT_DYNAMIC(CFxMsgDlgView, CDialog)

CFxMsgDlgView::CFxMsgDlgView(CWnd* pParent /*=NULL*/)
	: CDialog(CFxMsgDlgView::IDD, pParent)
{
	m_pParentWnd = pParent;
	m_isLoginOK = FALSE;
	m_isShow = FALSE;
}

CFxMsgDlgView::~CFxMsgDlgView()
{
	TCITEM Item;
	CFxMsgDlgPage * pMsgPage = NULL;
	for(int i = 0; i<m_TabChat.GetItemCount(); i++)
	{
		Item.mask = TCIF_PARAM;
		m_TabChat.GetItem(i, &Item);
		pMsgPage = (CFxMsgDlgPage*)Item.lParam;
		if(NULL != pMsgPage)
		{
			delete pMsgPage;
		}
	}
	m_TabChat.DeleteAllItems();
}

void CFxMsgDlgView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_CHAT, m_TabChat);
}


BEGIN_MESSAGE_MAP(CFxMsgDlgView, CDialog)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_CHAT, &CFxMsgDlgView::OnTcnSelchangeTabChat)
	ON_COMMAND(IDM_MSG_CHANGE_METHOD, &CFxMsgDlgView::OnMsgChangeMethod)
	ON_COMMAND(IDR_MSG_SEND, &CFxMsgDlgView::OnMsgSend)
    ON_COMMAND(IDM_BD_VIEWINFO, &CFxMsgDlgView::OnBdViewinfo)
	ON_COMMAND(IDM_END_TALK, &CFxMsgDlgView::OnEndTalk)
	ON_COMMAND(IDM_NEXT_TALK, &CFxMsgDlgView::OnNextTalk)
END_MESSAGE_MAP()


// CFxMsgDlgView 消息处理程序

BOOL CFxMsgDlgView::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	if (!m_dlgCommandBar.Create(this) ||
		!m_dlgCommandBar.InsertMenuBar(IDR_MSG_MENU))
	{
		TRACE0("未能创建 CommandBar\n");
		return FALSE;      // 未能创建
	}

	m_ImageList.Create(IDB_BITMAP_NOTREAD, 16, 1, RGB(255,255,255));
	m_TabChat.SetImageList(&m_ImageList);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CFxMsgDlgView::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	ShowWindow(SW_HIDE);
	m_isShow = FALSE;
	((FxMainWin*)m_pParentWnd)->RecoveryMenuBar();

	//CDialog::OnOK();
}

void CFxMsgDlgView::ShowMenuBar()
{
	m_dlgCommandBar.InsertMenuBar(IDR_MSG_MENU);
}
void CFxMsgDlgView::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CRect rc;
	GetClientRect(rc);
	m_TabChat.MoveWindow(rc);

	m_TabChat.GetClientRect(rc);
	rc.top += 24;
	rc.bottom -= 2;
	rc.left += 2;
	rc.right -= 2;

	TCITEM Item;
	CFxMsgDlgPage * pMsgPage = NULL;

	Item.mask = TCIF_PARAM;

	for(int i = 0; i<m_TabChat.GetItemCount(); i++)
	{
		m_TabChat.GetItem(i, &Item);
		pMsgPage = (CFxMsgDlgPage*)Item.lParam;
		if(NULL != pMsgPage)
		{
			pMsgPage->MoveWindow(rc);
		}
	}
}

CFxMsgDlgPage * CFxMsgDlgView::ChatWith(long lAccountID, BOOL bMyself)
{
	int nItem = 0;
	CFxMsgDlgPage * pMsgPage = SeekPage(lAccountID, nItem);
	if(NULL == pMsgPage)
	{
		pMsgPage = AddNewChat(lAccountID, bMyself);
	}
	ASSERT(NULL != pMsgPage);

	return pMsgPage;
}

CFxMsgDlgPage * CFxMsgDlgView::SeekPage(long lAccountID, int & nItem)
{
	CFxMsgDlgPage * pSeekMsgPage = NULL;
	TCITEM Item;
	CFxMsgDlgPage * pMsgPage = NULL;
	for(int i = 0; i<m_TabChat.GetItemCount(); i++)
	{
		Item.mask = TCIF_PARAM;
		m_TabChat.GetItem(i, &Item);
		pMsgPage = (CFxMsgDlgPage*)Item.lParam;
		if((NULL != pMsgPage) && (lAccountID == pMsgPage->m_lAccountID))
		{
			pSeekMsgPage = pMsgPage;
			nItem = i;
			break;
		}
	}
	return pSeekMsgPage;
}

CFxMsgDlgPage * CFxMsgDlgView::AddNewChat(long lAccountID, BOOL bMyself)
{
	CFxMsgDlgPage * pMsgPage = new CFxMsgDlgPage(lAccountID, this);
	ASSERT(NULL != pMsgPage);
	pMsgPage->m_bMyself = bMyself;
	CString strTitle = pMsgPage->GetTitle();

	TCITEM Item;
	Item.mask = TCIF_TEXT | TCIF_PARAM;
	Item.pszText = strTitle.GetBuffer();
	Item.lParam = (LPARAM)pMsgPage;
	m_TabChat.InsertItem(m_TabChat.GetItemCount(), &Item);

	int nItem = pMsgPage->Create(IDD_WMLF_MSG_PAGE,&m_TabChat);
	pMsgPage->ShowWindow(SW_HIDE);
	CRect rc;
	m_TabChat.GetClientRect(rc);
	rc.top += 24;
	rc.bottom -= 2;
	rc.left += 2;
	rc.right -= 2;
	pMsgPage->MoveWindow(rc);
	return pMsgPage;
}

void CFxMsgDlgView::ShowChat(CFxMsgDlgPage * pSeekMsgPage)
{
	if(NULL == pSeekMsgPage)
	{
		return;
	}
	TCITEM Item;
	CFxMsgDlgPage * pMsgPage = NULL;

	Item.mask = TCIF_PARAM;

	for(int i = 0; i<m_TabChat.GetItemCount(); i++)
	{
		m_TabChat.GetItem(i, &Item);
		pMsgPage = (CFxMsgDlgPage*)Item.lParam;
		if(NULL != pMsgPage)
		{
			if(pSeekMsgPage == pMsgPage)
			{
				m_TabChat.SetCurSel(i);
				if(pMsgPage->m_bNotReadFlag)
				{
					pMsgPage->m_bNotReadFlag = FALSE;
					SetNotReadFlag(i, pMsgPage->m_bNotReadFlag);
					((FxMainWin*)m_pParentWnd)->RemoveFilker(pMsgPage->m_lAccountID);
				}
				pMsgPage->ShowWindow(SW_SHOW);
			}
			else
			{
				pMsgPage->ShowWindow(SW_HIDE);
			}
		}
	}
}

CFxMsgDlgPage * CFxMsgDlgView::GetCurrentDlgPage()
{
	TCITEM Item;
	CFxMsgDlgPage * pMsgPage = NULL;

	Item.mask = TCIF_PARAM;
	m_TabChat.GetItem(m_TabChat.GetCurSel(), &Item);

	pMsgPage = (CFxMsgDlgPage*)Item.lParam;
	return pMsgPage;
}


void CFxMsgDlgView::OnTcnSelchangeTabChat(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	ShowChat(GetCurrentDlgPage());
	*pResult = 0;
}

void CFxMsgDlgView::OnMsgChangeMethod()
{
	GetCurrentDlgPage()->OnMsgChangeMethod();
}

void CFxMsgDlgView::OnMsgSend()
{
	GetCurrentDlgPage()->OnMsgSend();
}

void CFxMsgDlgView::OnBdViewinfo()
{
	if(GetCurrentDlgPage()->m_bMyself)
	{
		return;
	}
	((FxMainWin*)m_pParentWnd)->m_BuddyInfoDlg = new CBuddyInfoDlg(GetCurrentDlgPage()->m_lAccountID);
	((FxMainWin*)m_pParentWnd)->m_BuddyInfoDlg->DoModal();
    delete ((FxMainWin*)m_pParentWnd)->m_BuddyInfoDlg;
	((FxMainWin*)m_pParentWnd)->m_BuddyInfoDlg = NULL;

}

void CFxMsgDlgView::OnEndTalk()
{

	CFxMsgDlgPage * pMsgPage = GetCurrentDlgPage();
	int nItem = m_TabChat.GetCurSel();
	m_TabChat.DeleteItem(nItem);
	if(nItem >= m_TabChat.GetItemCount())
	{
		nItem --;
	}
	m_TabChat.SetCurSel(nItem);
	pMsgPage->ShowWindow(SW_HIDE);
	delete pMsgPage;
	pMsgPage = NULL;

	ShowChat(GetCurrentDlgPage());
	if(m_TabChat.GetItemCount() == 0)
	{
		OnOK();
		return;
	}
}

BOOL CFxMsgDlgView::addNewMsg(long lAccountID, CString msg)
{
	CFxMsgDlgPage * pMsgPage = ChatWith(lAccountID);
	if(NULL != pMsgPage)
	{
		pMsgPage->addNewMsg(msg);
		if(!m_isShow || (GetCurrentDlgPage() != pMsgPage))
		{
			int nItem = 0;
			SeekPage(lAccountID, nItem);
			pMsgPage->m_bNotReadFlag = TRUE;
			SetNotReadFlag(nItem, pMsgPage->m_bNotReadFlag);
			return TRUE;
		}
	}
	return FALSE;
}

void CFxMsgDlgView::updateAccountInfo(long lAccountID)
{
	int nItem = 0;
	CFxMsgDlgPage * pMsgPage = SeekPage(lAccountID, nItem);
	if(NULL != pMsgPage)
	{
		CString strTitle = pMsgPage->GetTitle();
		pMsgPage->ShowOnlineInfo();

		TCITEM Item;
		Item.mask = TCIF_TEXT;
		Item.pszText = strTitle.GetBuffer();
		m_TabChat.SetItem(nItem, &Item);
	}
}
void CFxMsgDlgView::OnNextTalk()
{
	// TODO: 在此添加命令处理程序代码
	if(m_TabChat.GetItemCount() == 0)
	{
		return;
	}
	int nItem = m_TabChat.GetCurSel();
	nItem ++;
	if(nItem >= m_TabChat.GetItemCount())
	{
		nItem = 0;
	}
	m_TabChat.SetCurSel(nItem);
	ShowChat(GetCurrentDlgPage());
}

void CFxMsgDlgView::SetNotReadFlag(int nItem, BOOL bNotRead)
{
	TCITEM Item;
	Item.mask = TCIF_IMAGE;
	Item.iImage = bNotRead?0:-1;
	m_TabChat.SetItem(nItem, &Item);
}