// FxMsgLogDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WMLF.h"
#include "FxMsgLogDlg.h"
#include "FxDatabase.h"
#include "FxMainWin.h"

extern CFxDatabase * g_pFxDB;

#define MSGLOG_COUNT_ONCE_READ	10
#define TIMER_LOAD_MSGLOG		35

// CFxMsgLogDlg 对话框

IMPLEMENT_DYNAMIC(CFxMsgLogDlg, CDialog)

CFxMsgLogDlg::CFxMsgLogDlg(long lAccountID, CString strName, CWnd* pParent /*=NULL*/)
	: CDialog(CFxMsgLogDlg::IDD, pParent)
	, m_lAccountID(lAccountID)
	, m_strAccountName(strName)
	, m_dwFirst(1)
	, m_dwReadCount(0)
	, m_dwCount(0)
{
	m_pParentWnd = pParent;
}

CFxMsgLogDlg::~CFxMsgLogDlg()
{
}

void CFxMsgLogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_MSG_LOG, m_EditMsgLog);
	DDX_Control(pDX, IDC_HEAD_PAGE, m_btHeadPage);
	DDX_Control(pDX, IDC_PREV_PAGE, m_btPrevPage);
	DDX_Control(pDX, IDC_NEXT_PAGE, m_btNextPage);
	DDX_Control(pDX, IDC_TAIL_PAGE, m_btTailPage);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_stTitle);
	DDX_Control(pDX, IDC_STATIC_PAGENUM, m_stPageNum);
}


BEGIN_MESSAGE_MAP(CFxMsgLogDlg, CDialog)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_HEAD_PAGE, &CFxMsgLogDlg::OnBnClickedHeadPage)
	ON_BN_CLICKED(IDC_PREV_PAGE, &CFxMsgLogDlg::OnBnClickedPrevPage)
	ON_BN_CLICKED(IDC_NEXT_PAGE, &CFxMsgLogDlg::OnBnClickedNextPage)
	ON_BN_CLICKED(IDC_TAIL_PAGE, &CFxMsgLogDlg::OnBnClickedTailPage)
	ON_WM_TIMER()
	ON_COMMAND(IDM_DELETE_ALL_MSGLOG, &CFxMsgLogDlg::OnDeleteAllMsglog)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(IDM_COPY1, &CFxMsgLogDlg::OnEditMsgLogCopy)
	ON_COMMAND(IDM_EXPORT_ALL_MSGLOG, &CFxMsgLogDlg::OnExportAllMsglog)
END_MESSAGE_MAP()


// CFxMsgLogDlg 消息处理程序

BOOL CFxMsgLogDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_EditMsgLog.SetReadOnly();
	
	if (!m_dlgCommandBar.Create(this) ||
		!m_dlgCommandBar.InsertMenuBar(IDR_MSGLOG_MENU))
	{
		TRACE0("未能创建 CommandBar\n");
		return FALSE;      // 未能创建
	}

	m_stTitle.SetWindowText(_T("与 ") + m_strAccountName + _T(" 的聊天记录"));

	// TODO:  在此添加额外的初始化
	SetTimer(TIMER_LOAD_MSGLOG, 1, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CFxMsgLogDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	int xIDC_STATIC_TITLE, yIDC_STATIC_TITLE, wIDC_STATIC_TITLE, hIDC_STATIC_TITLE;
	int xIDC_EDIT_MSG_LOG, yIDC_EDIT_MSG_LOG, wIDC_EDIT_MSG_LOG, hIDC_EDIT_MSG_LOG;
	int xIDC_HEAD_PAGE, yIDC_HEAD_PAGE, wIDC_HEAD_PAGE, hIDC_HEAD_PAGE;
	int xIDC_PREV_PAGE, yIDC_PREV_PAGE, wIDC_PREV_PAGE, hIDC_PREV_PAGE;
	int xIDC_NEXT_PAGE, yIDC_NEXT_PAGE, wIDC_NEXT_PAGE, hIDC_NEXT_PAGE;
	int xIDC_TAIL_PAGE, yIDC_TAIL_PAGE, wIDC_TAIL_PAGE, hIDC_TAIL_PAGE;
	int xIDC_STATIC_PAGENUM, yIDC_STATIC_PAGENUM, wIDC_STATIC_PAGENUM, hIDC_STATIC_PAGENUM;
	int iHeight, iWidth;
	int iMargin = DRA::SCALEY(2);

	RECT rcCtl;
	::GetClientRect(this->m_hWnd, &rcCtl);

	iHeight = rcCtl.bottom - rcCtl.top;
	iWidth = rcCtl.right - rcCtl.left;

	xIDC_STATIC_TITLE = rcCtl.left + iMargin;
	yIDC_STATIC_TITLE = rcCtl.top + iMargin;
	wIDC_STATIC_TITLE = iWidth - iMargin * 2;
	hIDC_STATIC_TITLE = DRA::SCALEY(16);

	xIDC_HEAD_PAGE = xIDC_STATIC_TITLE;
	hIDC_HEAD_PAGE = DRA::SCALEY(24);
	yIDC_HEAD_PAGE = iHeight - iMargin - hIDC_HEAD_PAGE;
	wIDC_HEAD_PAGE = DRA::SCALEY(24);

	xIDC_PREV_PAGE = xIDC_HEAD_PAGE + wIDC_HEAD_PAGE + iMargin;
	yIDC_PREV_PAGE = yIDC_HEAD_PAGE;
	wIDC_PREV_PAGE = wIDC_HEAD_PAGE;
	hIDC_PREV_PAGE = hIDC_HEAD_PAGE;

	yIDC_TAIL_PAGE = yIDC_HEAD_PAGE;
	wIDC_TAIL_PAGE = wIDC_HEAD_PAGE;
	xIDC_TAIL_PAGE = rcCtl.right - iMargin - wIDC_TAIL_PAGE;
	hIDC_TAIL_PAGE = hIDC_HEAD_PAGE;

	yIDC_NEXT_PAGE = yIDC_HEAD_PAGE;
	wIDC_NEXT_PAGE = wIDC_HEAD_PAGE;
	xIDC_NEXT_PAGE = xIDC_TAIL_PAGE - iMargin - wIDC_NEXT_PAGE;
	hIDC_NEXT_PAGE = hIDC_HEAD_PAGE;

	wIDC_STATIC_PAGENUM = DRA::SCALEY(72);
	xIDC_STATIC_PAGENUM = rcCtl.left + iWidth / 2 - wIDC_STATIC_PAGENUM / 2;
	yIDC_STATIC_PAGENUM = yIDC_HEAD_PAGE;
	hIDC_STATIC_PAGENUM = hIDC_HEAD_PAGE;

	xIDC_EDIT_MSG_LOG = xIDC_STATIC_TITLE;
	yIDC_EDIT_MSG_LOG = yIDC_STATIC_TITLE + hIDC_STATIC_TITLE + iMargin;
	wIDC_EDIT_MSG_LOG = wIDC_STATIC_TITLE;
	hIDC_EDIT_MSG_LOG = yIDC_HEAD_PAGE - iMargin - yIDC_EDIT_MSG_LOG;

	HWND hwndctl = ::GetDlgItem(this->m_hWnd, IDC_STATIC_TITLE);
	::MoveWindow(hwndctl, xIDC_STATIC_TITLE, yIDC_STATIC_TITLE, wIDC_STATIC_TITLE, hIDC_STATIC_TITLE, false);

	hwndctl = ::GetDlgItem(this->m_hWnd, IDC_HEAD_PAGE);
	::MoveWindow(hwndctl, xIDC_HEAD_PAGE, yIDC_HEAD_PAGE, wIDC_HEAD_PAGE, hIDC_HEAD_PAGE, false);

	hwndctl = ::GetDlgItem(this->m_hWnd, IDC_PREV_PAGE);
	::MoveWindow(hwndctl, xIDC_PREV_PAGE, yIDC_PREV_PAGE, wIDC_PREV_PAGE, hIDC_PREV_PAGE, false);

	hwndctl = ::GetDlgItem(this->m_hWnd, IDC_TAIL_PAGE);
	::MoveWindow(hwndctl, xIDC_TAIL_PAGE, yIDC_TAIL_PAGE, wIDC_TAIL_PAGE, hIDC_TAIL_PAGE, false);

	hwndctl = ::GetDlgItem(this->m_hWnd, IDC_NEXT_PAGE);
	::MoveWindow(hwndctl, xIDC_NEXT_PAGE, yIDC_NEXT_PAGE, wIDC_NEXT_PAGE, hIDC_NEXT_PAGE, false);

	hwndctl = ::GetDlgItem(this->m_hWnd, IDC_STATIC_PAGENUM);
	::MoveWindow(hwndctl, xIDC_STATIC_PAGENUM, yIDC_STATIC_PAGENUM, wIDC_STATIC_PAGENUM, hIDC_STATIC_PAGENUM, false);

	hwndctl = ::GetDlgItem(this->m_hWnd, IDC_EDIT_MSG_LOG);
	::MoveWindow(hwndctl, xIDC_EDIT_MSG_LOG, yIDC_EDIT_MSG_LOG, wIDC_EDIT_MSG_LOG, hIDC_EDIT_MSG_LOG, false);

}

void CFxMsgLogDlg::LoadMsgLog(void)
{
	m_dwCount = g_pFxDB->GetMsgLogCount(m_lAccountID);

	MSGLOGFROMDB MsgLogFromDB;
	MsgLogFromDB.lID = m_lAccountID;
	MsgLogFromDB.dwFirst = m_dwFirst;
	MsgLogFromDB.dwCount = MSGLOG_COUNT_ONCE_READ;

	g_pFxDB->ReadMsgLog(&MsgLogFromDB);
	m_EditMsgLog.SetWindowText(MsgLogFromDB.strMsg);
	m_EditMsgLog.SetSel(-1, 0);

	m_dwReadCount = MsgLogFromDB.dwCount;

	CString strPageNum;
	if(0 == m_dwReadCount)
	{
		strPageNum.Format(_T("0-0/%d"), m_dwCount);
	}
	else
	{
		strPageNum.Format(_T("%d-%d/%d"), m_dwFirst, m_dwFirst + m_dwReadCount - 1, m_dwCount);
	}
	m_stPageNum.SetWindowText(strPageNum);
}

void CFxMsgLogDlg::OnBnClickedHeadPage()
{
	// TODO: 在此添加控件通知处理程序代码
	DWORD dwFirst;
	dwFirst = 1;
	if(dwFirst != m_dwFirst)
	{
		m_dwFirst = dwFirst;
		LoadMsgLog();
	}
}

void CFxMsgLogDlg::OnBnClickedPrevPage()
{
	// TODO: 在此添加控件通知处理程序代码
	DWORD dwFirst;
	dwFirst = m_dwFirst;
	if(dwFirst > MSGLOG_COUNT_ONCE_READ)
	{
		dwFirst -= MSGLOG_COUNT_ONCE_READ;
	}
	if(dwFirst != m_dwFirst)
	{
		m_dwFirst = dwFirst;
		LoadMsgLog();
	}
}

void CFxMsgLogDlg::OnBnClickedNextPage()
{
	// TODO: 在此添加控件通知处理程序代码
	DWORD dwFirst;
	dwFirst = m_dwFirst + MSGLOG_COUNT_ONCE_READ;
	if((dwFirst > m_dwCount) && (dwFirst > MSGLOG_COUNT_ONCE_READ))
	{
		dwFirst -= MSGLOG_COUNT_ONCE_READ;
	}
	if(dwFirst != m_dwFirst)
	{
		m_dwFirst = dwFirst;
		LoadMsgLog();
	}
}

void CFxMsgLogDlg::OnBnClickedTailPage()
{
	// TODO: 在此添加控件通知处理程序代码
	DWORD dwFirst;
	dwFirst = m_dwFirst;
	while(dwFirst <= m_dwCount)
	{
		dwFirst += MSGLOG_COUNT_ONCE_READ;
	}
	if((dwFirst > m_dwCount) && (dwFirst > MSGLOG_COUNT_ONCE_READ))
	{
		dwFirst -= MSGLOG_COUNT_ONCE_READ;
	}
	if(dwFirst != m_dwFirst)
	{
		m_dwFirst = dwFirst;
		LoadMsgLog();
	}
}

void CFxMsgLogDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch(nIDEvent)
	{
	case TIMER_LOAD_MSGLOG:
		KillTimer(TIMER_LOAD_MSGLOG);
		m_dwFirst = 1;
		LoadMsgLog();
		break;

	default:
		break;
	}
	CDialog::OnTimer(nIDEvent);
}

void CFxMsgLogDlg::OnDeleteAllMsglog()
{
	// TODO: 在此添加命令处理程序代码
    CString strMessage;
    strMessage.Format(_T("该操作将删除与此好友的所有聊天记录，且不可恢复，您确定要如此操作？"));
    if(MessageBox(strMessage, _T("LibFetion"), MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2) == IDCANCEL)
	{
		return;
	}
	g_pFxDB->DeleteAllMsgLog(m_lAccountID);
	LoadMsgLog();
}

void CFxMsgLogDlg::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: 在此处添加消息处理程序代码
	int iID;
	iID = pWnd->GetDlgCtrlID();
	if(iID == m_EditMsgLog.GetDlgCtrlID())
	{
			CMenu *pMenu;

            pMenu=new CMenu;
            
            pMenu->LoadMenu( IDR_MSG_POP_MENU );  //加载菜单资源
            CMenu *pMenuPop=pMenu->GetSubMenu(0); //资源菜单的第一项
			pMenuPop->TrackPopupMenu( TPM_LEFTALIGN, point.x, point.y, this);
            delete pMenu;
	}
}

void CFxMsgLogDlg::OnEditMsgLogCopy()
{
	// TODO: 在此添加命令处理程序代码
	m_EditMsgLog.Copy();
}

void CFxMsgLogDlg::OnExportAllMsglog()
{
	// TODO: 在此添加命令处理程序代码
    CString strMessage;
	CString strPathName;
	strPathName = ((FxMainWin*)((CFxMsgDlgView*)m_pParentWnd)->m_pParentWnd)->m_strStartupPath
		+ _T("\\Users\\")
		+ ((FxMainWin*)((CFxMsgDlgView*)m_pParentWnd)->m_pParentWnd)->m_strAccountID
		+ _T("\\log")
		+ ((FxMainWin*)((CFxMsgDlgView*)m_pParentWnd)->m_pParentWnd)->m_strAccountID;
	if(fx_is_pc_user_by_id(m_lAccountID))
	{
		strPathName.Format(_T("%s_%u.txt"), strPathName, m_lAccountID);
	}
	else
	{
		char* original = fx_get_original_ID(m_lAccountID);
		strPathName.Format(_T("%s_%s.txt"), strPathName,  ConvertUtf8ToUtf16(original));
		free(original);
	}
    strMessage.Format(_T("该操作将导出与此好友的所有聊天记录到文件 %s 中，原有文件会被覆盖掉，请确认？"), strPathName);
    if(MessageBox(strMessage, _T("LibFetion"), MB_OKCANCEL | MB_ICONQUESTION) == IDCANCEL)
	{
		return;
	}

	m_dwCount = g_pFxDB->GetMsgLogCount(m_lAccountID);

	MSGLOGFROMDB MsgLogFromDB;
	MsgLogFromDB.lID = m_lAccountID;
	MsgLogFromDB.dwFirst = 1;
	MsgLogFromDB.dwCount = m_dwCount;

	g_pFxDB->ReadMsgLog(&MsgLogFromDB);
	MsgLogFromDB.strMsg;

	HANDLE hFile = CreateFile(strPathName, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(INVALID_HANDLE_VALUE == hFile)
	{
		CString strText = _T("创建文件失败。");
		MessageBox(strText, _T("LibFetion"), MB_OK | MB_ICONWARNING);
	}
	else
	{
		DWORD dwWrite = 0;
		CString strHead;
		strHead.Format(_T("LibFetion聊天记录导出文件，共%u条记录。\r\n\r\n"), MsgLogFromDB.dwCount);
		CStringA strAMsg = ConvertUtf16ToUtf8(strHead + MsgLogFromDB.strMsg);
		if(!WriteFile(hFile, strAMsg.GetBuffer(), strAMsg.GetLength(), &dwWrite, NULL))
		{
			DWORD dwError = GetLastError();
			CloseHandle(hFile);

			CString strText;
			strText.Format(_T("写文件失败，错误代码%u。"), dwError);
			MessageBox(strText, _T("LibFetion"), MB_OK | MB_ICONWARNING);
		}
		else
		{
			CloseHandle(hFile);

			CString strText = _T("导出聊天记录成功。");
			MessageBox(strText, _T("LibFetion"), MB_OK | MB_ICONINFORMATION);
		}
	}

	return;
}
