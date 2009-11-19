// LoginOptionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WMLF.h"
#include "LoginOptionDlg.h"
#include "IniWR.h"
#include <tpcshell.h>


// CLoginOptionDlg 对话框

IMPLEMENT_DYNAMIC(CLoginOptionDlg, CDialog)

CLoginOptionDlg::CLoginOptionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoginOptionDlg::IDD, pParent)
	, m_strFetionNo(_T(""))
	, m_mobile_no(_T(""))
	, m_bHaveEdit(FALSE)
	, m_strInfo(_T(""))
{

}

CLoginOptionDlg::~CLoginOptionDlg()
{
}

void CLoginOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDV_MaxChars(pDX, m_mobile_no, 11);
	DDX_Control(pDX, IDC_COMBO_USERS2, m_cboUsersList2);
	DDX_Text(pDX, IDC_FETION_NO, m_strFetionNo);
	DDX_Text(pDX, IDC_STATIC_INFO, m_strInfo);
}


BEGIN_MESSAGE_MAP(CLoginOptionDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_USERS2, &CLoginOptionDlg::OnCbnSelchangeComboUsers2)
	ON_COMMAND(IDM_EDIT, &CLoginOptionDlg::OnEdit)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CLoginOptionDlg 消息处理程序

BOOL CLoginOptionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	if (!m_dlgCommandBar.Create(this) ||
	    !m_dlgCommandBar.InsertMenuBar(IDR_LOGIN_OPTION_MENU))
	{
		TRACE0("未能创建 CommandBar\n");
		return FALSE;      // 未能创建
	}
#ifdef WIN32_PLATFORM_WFSP
    //重写后退键，引发WM_HOTKEY消息
    (void)::SendMessage(SHFindMenuBar (m_hWnd), SHCMBM_OVERRIDEKEY, VK_TBACK,
        MAKELPARAM(SHMBOF_NODEFAULT | SHMBOF_NOTIFY,
        SHMBOF_NODEFAULT | SHMBOF_NOTIFY));
#endif

	InitUsersList();
	GetSelectedUserOption();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CLoginOptionDlg::InitUsersList(void)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	
	m_cboUsersList2.LimitText(11);
	m_cboUsersList2.ResetContent();
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
					m_cboUsersList2.AddString(strFileName);
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
	if(m_cboUsersList2.GetCount() > 0)
	{
		m_cboUsersList2.SetCurSel(0);
	}

	CIniWR hIni;
	CString strLastUser;
	hIni.GetString(_T("OPTION"), _T("LastUser"), strLastUser.GetBuffer(MAX_PATH), MAX_PATH, m_strStartupPath + _T("\\LibFetion.ini"));
	strLastUser.ReleaseBuffer();
	
	for(int i = 0; i < m_cboUsersList2.GetCount(); i++)
	{
		CString strMobileNo;
		m_cboUsersList2.GetLBText(i, strMobileNo);
		if(0 == strLastUser.Compare(strMobileNo))
		{
			m_cboUsersList2.SetCurSel(i);
			break;
		}
	}
}

void CLoginOptionDlg::OnCbnSelchangeComboUsers2()
{
	// TODO: 在此添加控件通知处理程序代码
	GetSelectedUserOption();
}

void CLoginOptionDlg::GetSelectedUserOption(void)
{
	if(m_cboUsersList2.GetCount() == 0)
	{
		return;
	}
	
	CString strMobileNo;

	m_cboUsersList2.GetLBText(m_cboUsersList2.GetCurSel(), strMobileNo);
	if(strMobileNo.GetLength() != 11)
	{
		return;
	}

	CIniWR hIni;
	CString strRead;

	hIni.GetString(strMobileNo, _T("FetionID"), strRead.GetBuffer(MAX_PATH), MAX_PATH, m_strStartupPath + _T("\\Users\\") + strMobileNo + _T("\\") + strMobileNo + _T(".ini"));
	strRead.ReleaseBuffer();
	m_strFetionNo = strRead;

	this->UpdateData(FALSE);
}

void CLoginOptionDlg::OnEdit()
{
	// TODO: 在此添加命令处理程序代码
	UpdateData();
	CString strUser;
	m_cboUsersList2.GetWindowTextW(strUser.GetBuffer(12), 12);
	strUser.ReleaseBuffer();
	if(strUser.GetLength() != 11)
	{
		m_strInfo = _T("请输入正确的手机号码");
		UpdateData(FALSE);
		return;
	}
	if((m_strFetionNo.GetLength() != 9) && (m_strFetionNo.GetLength() != 0))
	{
		m_strInfo = _T("请输入正确的飞信号码");
		UpdateData(FALSE);
		return;
	}
	m_mobile_no = strUser;
	WriteLoginOptionToIni();
	m_bHaveEdit = TRUE;
	SendMessage(WM_CLOSE,0, 0);
}

void CLoginOptionDlg::WriteLoginOptionToIni()
{
	CString strMobileNo = m_mobile_no;
	CIniWR hIni;
	hIni.WritePrivateProfileString(_T("OPTION"), _T("LastUser"), strMobileNo, m_strStartupPath + _T("\\LibFetion.ini"));

	CreateDirectory(m_strStartupPath + _T("\\Users"), NULL);
	CreateDirectory(m_strStartupPath + _T("\\Users\\") + strMobileNo, NULL);

	hIni.WritePrivateProfileString(strMobileNo, _T("FetionID"), m_strFetionNo, m_strStartupPath + _T("\\Users\\") + strMobileNo + _T("\\") + strMobileNo + _T(".ini"));
}

void CLoginOptionDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	int xIDC_STATIC_AD_TITLE2, yIDC_STATIC_AD_TITLE2, wIDC_STATIC_AD_TITLE2, hIDC_STATIC_AD_TITLE2;
	int xIDC_STATIC_MOBILE, yIDC_STATIC_MOBILE, wIDC_STATIC_MOBILE, hIDC_STATIC_MOBILE;
	int xIDC_COMBO_USERS2, yIDC_COMBO_USERS2, wIDC_COMBO_USERS2, hIDC_COMBO_USERS2;
	int xIDC_STATIC_FETION, yIDC_STATIC_FETION, wIDC_STATIC_FETION, hIDC_STATIC_FETION;
	int xIDC_FETION_NO, yIDC_FETION_NO, wIDC_FETION_NO, hIDC_FETION_NO;
	int xIDC_STATIC_INFO, yIDC_STATIC_INFO, wIDC_STATIC_INFO, hIDC_STATIC_INFO;
	int iHeight, iWidth;
	int iMargin = DRA::SCALEY(5);

	RECT rcCtl;
	::GetClientRect(this->m_hWnd, &rcCtl);

	iHeight = rcCtl.bottom - rcCtl.top;
	iWidth = rcCtl.right - rcCtl.left;

	xIDC_STATIC_AD_TITLE2 = rcCtl.left + iMargin * 2;
	yIDC_STATIC_AD_TITLE2 = rcCtl.top + iMargin * 2;
	wIDC_STATIC_AD_TITLE2 = iWidth - iMargin * 4;
	hIDC_STATIC_AD_TITLE2 = DRA::SCALEY(50);

	xIDC_STATIC_MOBILE = xIDC_STATIC_AD_TITLE2;
	yIDC_STATIC_MOBILE = yIDC_STATIC_AD_TITLE2 + hIDC_STATIC_AD_TITLE2 + iMargin;
	wIDC_STATIC_MOBILE = DRA::SCALEY(60);
	hIDC_STATIC_MOBILE = DRA::SCALEY(20);

	xIDC_COMBO_USERS2 = xIDC_STATIC_MOBILE + wIDC_STATIC_MOBILE + iMargin;
	yIDC_COMBO_USERS2 = yIDC_STATIC_MOBILE - 4;
	wIDC_COMBO_USERS2 = iWidth - xIDC_COMBO_USERS2 - iMargin * 2;
	hIDC_COMBO_USERS2 = hIDC_STATIC_MOBILE * 5;

	xIDC_STATIC_FETION = xIDC_STATIC_MOBILE;
	yIDC_STATIC_FETION = yIDC_STATIC_MOBILE + hIDC_STATIC_MOBILE + iMargin;
	wIDC_STATIC_FETION = wIDC_STATIC_MOBILE;
	hIDC_STATIC_FETION = hIDC_STATIC_MOBILE;

	xIDC_FETION_NO = xIDC_COMBO_USERS2;
	yIDC_FETION_NO = yIDC_STATIC_FETION - 4;
	wIDC_FETION_NO = wIDC_COMBO_USERS2;
	hIDC_FETION_NO = hIDC_STATIC_MOBILE;

	xIDC_STATIC_INFO = xIDC_STATIC_AD_TITLE2;
	yIDC_STATIC_INFO = yIDC_STATIC_FETION + hIDC_STATIC_MOBILE + iMargin;
	wIDC_STATIC_INFO = wIDC_STATIC_AD_TITLE2;
	hIDC_STATIC_INFO = hIDC_STATIC_MOBILE;

	HWND hwndctl = ::GetDlgItem(this->m_hWnd, IDC_STATIC_AD_TITLE2);
	::MoveWindow(hwndctl, xIDC_STATIC_AD_TITLE2, yIDC_STATIC_AD_TITLE2, wIDC_STATIC_AD_TITLE2, hIDC_STATIC_AD_TITLE2, false);

	hwndctl = ::GetDlgItem(this->m_hWnd, IDC_STATIC_MOBILE);
	::MoveWindow(hwndctl, xIDC_STATIC_MOBILE, yIDC_STATIC_MOBILE, wIDC_STATIC_MOBILE, hIDC_STATIC_MOBILE, false);

	hwndctl = ::GetDlgItem(this->m_hWnd, IDC_COMBO_USERS2);
	::MoveWindow(hwndctl, xIDC_COMBO_USERS2, yIDC_COMBO_USERS2, wIDC_COMBO_USERS2, hIDC_COMBO_USERS2, false);

	hwndctl = ::GetDlgItem(this->m_hWnd, IDC_STATIC_FETION);
	::MoveWindow(hwndctl, xIDC_STATIC_FETION, yIDC_STATIC_FETION, wIDC_STATIC_FETION, hIDC_STATIC_FETION, false);

	hwndctl = ::GetDlgItem(this->m_hWnd, IDC_FETION_NO);
	::MoveWindow(hwndctl, xIDC_FETION_NO, yIDC_FETION_NO, wIDC_FETION_NO, hIDC_FETION_NO, false);

	hwndctl = ::GetDlgItem(this->m_hWnd, IDC_STATIC_INFO);
	::MoveWindow(hwndctl, xIDC_STATIC_INFO, yIDC_STATIC_INFO, wIDC_STATIC_INFO, hIDC_STATIC_INFO, false);
}

LRESULT CLoginOptionDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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
