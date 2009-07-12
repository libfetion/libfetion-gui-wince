#include "StdAfx.h"
#include "Notify.h"

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
#endif

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
        if(NLedGetDeviceInfo(NLED_COUNT_INFO_ID, (PVOID) &nci))
        {
            m_LedNum = (int)nci.cLeds - 1;
        }
    }
    settings.LedNum = m_LedNum;
	settings.OffOnBlink= 1;
	NLedSetDevice(NLED_SETTINGS_INFO_ID, &settings);
}

void StopVirbate(HWND hwnd, UINT idEvent, UINT_PTR, DWORD )
{
	NLED_SETTINGS_INFO settings; 

	memset(&settings,0,sizeof(NLED_SETTINGS_INFO));
	settings.LedNum= m_LedNum; 
	settings.OffOnBlink= 0;
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
void CNotify::Nodify(HWND hwnd, LPCWSTR strPath, int iPeriod, bool bVibr, UINT Styles)
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
    //播放声音
    PlaySound (strPath, AfxGetApp()->m_hInstance, Styles | SND_ASYNC);

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
