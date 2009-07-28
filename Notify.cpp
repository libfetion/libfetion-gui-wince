#include "StdAfx.h"
#include "Notify.h"
#include "resourceppc.h"

#ifdef WIN32_PLATFORM_WFSP
#include "vibrate.h"
#endif

#ifdef WIN32_PLATFORM_PSPC
#include <Nled.h>
#if _WIN32_WCE<0x500
extern "C"
{
	BOOL NLedGetDeviceInfo(INT nID, PVOID pOutput);
	BOOL NLedSetDevice(INT nID, PVOID pOutput);
}
#endif
SHNOTIFICATIONDATA2* pNotification;
int iMsgCount;
#endif

static const GUID guidNotifyApp = 
{ 0x569440f0, 0xc5b4, 0x4ac2, { 0xa9, 0xab, 0xc6, 0x16, 0xd1, 0x9c, 0x1, 0x2f } };




void CNotify::CreateAndAddNotification(HWND hwnd, WCHAR* szTitle, CString szNotify)
{
#ifdef WIN32_PLATFORM_PSPC
    if(pNotification == NULL)
    {
        iMsgCount ++;
        //HICON hIcon =(HICON)LoadImage(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDI_ICON_NOTIFY),
        //    IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
        TCHAR szMsgTitle[128];
        TCHAR szMsgBody[1024];
        wsprintf(szMsgTitle, TEXT("收到来自 %s 的飞信消息"), szTitle);
#ifndef NOTIF_NUM_SOFTKEYS
        wsprintf(szMsgBody, TEXT("<html><body><form method=\"POST\" action=>%s <input type=button name='cmd:%u' value='查看'>&nbsp;<input type=button name='cmd:%u' value='取消'></body></html>"), szNotify, IDM_MAIN_SHOWNEWMSG2, IDM_MAIN_DIMISS);
#else
        wsprintf(szMsgBody, TEXT("<html><body><form method=\"POST\" action=>%s</body></html>"), szNotify);
#endif
	    pNotification =(SHNOTIFICATIONDATA2*)malloc( sizeof(SHNOTIFICATIONDATA2));
        ZeroMemory(pNotification, sizeof(SHNOTIFICATIONDATA2));
	    pNotification->dwID = ID_NOTIFY;
	    pNotification->clsid = guidNotifyApp;
	    pNotification->npPriority = SHNP_INFORM;
	    pNotification->csDuration = 20;
	    pNotification->hwndSink = hwnd;
	    pNotification->pszHTML = szMsgBody;
	    pNotification->hicon = LoadIcon(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDI_ICON_NOTIFY));
	    pNotification->cbStruct = sizeof(SHNOTIFICATIONDATA2);
	    pNotification->pszTitle = szMsgTitle;
        pNotification->grfFlags = SHNF_ALERTONUPDATE | SHNF_WANTVKTTALK | SHNF_DISPLAYON;
        pNotification->rgskn[0].pszTitle = TEXT("查看");
        pNotification->rgskn[0].skc.wpCmd = IDM_MAIN_SHOWNEWMSG2;
        pNotification->rgskn[0].skc.grfFlags = NOTIF_SOFTKEY_FLAGS_DISMISS;
        pNotification->rgskn[1].pszTitle = TEXT("忽略");
        pNotification->rgskn[1].skc.wpCmd = IDM_MAIN_DIMISS;
        pNotification->rgskn[1].skc.grfFlags = NOTIF_SOFTKEY_FLAGS_DISMISS;
	    LRESULT ret = SHNotificationAdd(pNotification);

    } else {
        iMsgCount ++;
        TCHAR szMsgBody[1024];
#ifndef NOTIF_NUM_SOFTKEYS
        wsprintf(szMsgBody, TEXT("<html><body><form method=\"POST\" action=>收到 %d 条消息！<input type=button name='cmd:%u' value='查看'>&nbsp;<input type=button name='cmd:%u' value='取消'></body></html>"), iMsgCount, IDM_MAIN_SHOWNEWMSG2, IDM_MAIN_DIMISS);
#else
        wsprintf(szMsgBody, TEXT("<html><body><form method=\"POST\" action=>收到 %d 条消息！</body></html>"), iMsgCount);
#endif
		pNotification->pszTitle = TEXT("LibFetion提醒");
        pNotification->pszHTML = szMsgBody;
		SHNotificationUpdate(SHNUM_TITLE | SHNUM_HTML, pNotification);
    }
#endif
	return;	
}

// function removed all nodes in the list
// called when the programme exits
void CNotify::RemoveNotification()
{
#ifdef WIN32_PLATFORM_PSPC
    if(pNotification != NULL)
    {
	    SHNotificationRemove(&guidNotifyApp, pNotification->dwID);
        //FreeNotificationData(pNotification);
	    free(pNotification);
        iMsgCount = 0;
        pNotification = NULL;
    }
#endif
}



#define TIMER_STOPVIBRATE 30


#ifdef WIN32_PLATFORM_WFSP
//SP用于停止振动
void CALLBACK StopVib(
					  HWND hwnd, 
					  UINT uMsg, 
					  UINT idEvent, 
					  DWORD dwTime 
					  )
{
	//停止振动
	VibrateStop();
}
#endif


#ifdef WIN32_PLATFORM_PSPC
//PPC振动函数
int m_LedNum = 0;
bool m_bLedInited = false;

void StartVirbate( )
{
	NLED_SETTINGS_INFO settings ; 

	memset(&settings,0,sizeof(NLED_SETTINGS_INFO));
    NLED_COUNT_INFO nci;

    if(!m_bLedInited)
    {
        // FIXME 这里获取振动LED的代码应该没有问题,但是如果ledNum不为0,就不能振动,
        if(NLedGetDeviceInfo(NLED_COUNT_INFO_ID, (PVOID) &nci))
        {
            m_LedNum = (int)nci.cLeds - 1;
        }
        m_bLedInited = true;
    }
    settings.LedNum = m_LedNum;
	settings.OffOnBlink = 1;
	NLedSetDevice(NLED_SETTINGS_INFO_ID, &settings);
		settings.LedNum=0;
	NLedSetDevice(NLED_SETTINGS_INFO_ID,&settings);
}

void StopVirbate(HWND hwnd, UINT idEvent, UINT_PTR, DWORD )
{
	NLED_SETTINGS_INFO settings; 

	memset(&settings,0,sizeof(NLED_SETTINGS_INFO));
	settings.LedNum= m_LedNum; 
	settings.OffOnBlink= 0;
	NLedSetDevice(NLED_SETTINGS_INFO_ID, &settings);
	settings.LedNum= 0;
	NLedSetDevice(NLED_SETTINGS_INFO_ID, &settings);
  KillTimer(hwnd, TIMER_STOPVIBRATE);
}
#endif

CNotify::CNotify(void)
{
}

CNotify::~CNotify(void)
{
}

// 播放指定的声音或振动
// 如果strPath不为空, 则播放声音
void CNotify::Nodify(HWND hwnd, LPCWSTR strPath, int iPeriod, bool bNoSound, bool bVibr, UINT Styles)
{
    if(bVibr)
    {
#ifdef WIN32_PLATFORM_PSPC
        StartVirbate();
        SetTimer(hwnd, TIMER_STOPVIBRATE, iPeriod, StopVirbate);
#endif
#ifdef WIN32_PLATFORM_WFSP
    Vibrate (0, NULL, FALSE, INFINITE);
    SetTimer(hwnd, TIMER_STOPVIBRATE, iPeriod, StopVib);
#endif
    }
    if(!bNoSound)
    {
        //播放声音
        PlaySound (strPath, AfxGetApp()->m_hInstance, Styles | SND_ASYNC);
    }

}

void CNotify::StartViberate(bool bVibe)
{
    if(!bVibe)
        return;
    // TODO 开始振动
}

void CNotify::StopViberate(bool bVibe)
{
    if(!bVibe)
        return;

    // TODO 停止振动
}
