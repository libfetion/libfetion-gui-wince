// WMLF.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "WMLF.h"
#include "LoginDlg.h"
#include "FxMainWin.h"
#ifndef M8
#pragma comment(lib, "Ws2.lib")
//#include "ConnectTo.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWMLFApp

BEGIN_MESSAGE_MAP(CWMLFApp, CWinApp)
END_MESSAGE_MAP()


// CWMLFApp 构造
CWMLFApp::CWMLFApp()
	: CWinApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CWMLFApp 对象
CWMLFApp theApp;

////sub thread to run the EnableConnection
//UINT ConnThreadProc( LPVOID pParam )
//{
//	TCHAR ipAddr[20]; 
//	EstablishConnection(ipAddr);
//	return 0;   // thread completed successfully
//}

// CWMLFApp 初始化

BOOL CWMLFApp::InitInstance()
{
	
#if defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP)
    // 在应用程序初始化期间，应调用一次 SHInitExtraControls 以初始化
    // 所有 Windows Mobile 特定控件，如 CAPEDIT 和 SIPPREF。
    SHInitExtraControls();
 // WIN32_PLATFORM_PSPC || WIN32_PLATFORM_WFSP

    ////If it is already running, then focus on the window, and exit.
    HWND hWnd = FindWindow(_T("CFxMainWin"), _T("LibFetion"));  

    if (hWnd) 
    {
    //    // Set focus to foremost child window.
    //    // The "| 0x00000001" is used to bring any owned windows to the foreground and
    //    // activate them.
        SetForegroundWindow((HWND)((ULONG) hWnd | 0x00000001));

        return 0;
    } 
#endif
	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("LibFetion By DengDongdong"));

	//::AfxBeginThread(ConnThreadProc, NULL);
	if (!fx_init()) 
	{
		AfxMessageBox(_T("初始化LibFetion失败！！程序将退出"));
		return FALSE;
	}

	FxMainWin mainwind;
	m_pMainWnd = &mainwind;
	INT_PTR nResponse = mainwind.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此处放置处理何时用“确定”来关闭
		//  对话框的代码
	}


	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

int CWMLFApp::ExitInstance()
{
	::fx_loginout();
	::fx_terminate();
#ifndef M8
	//释放网络连接
//	CloseConnection();
#endif
	return CWinApp::ExitInstance();
}
