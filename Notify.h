#pragma once

#ifdef WIN32_PLATFORM_PSPC

#define ID_NOTIFY WM_USER+1000
// 为了与WM2003兼容
#ifndef NOTIF_NUM_SOFTKEYS

#define SHNF_HASMENU					0x00000040
#define SHNF_ALERTONUPDATE				0x00000200
#define SHNF_WANTVKTTALK				0x00000400
#define SHNN_ACTIVATE           (SHNN_FIRST-5)

#define NOTIF_SOFTKEY_FLAGS_DISMISS		0x0000
#define NOTIF_SOFTKEY_FLAGS_HIDE		0x0001
#define NOTIF_SOFTKEY_FLAGS_STAYOPEN	0x0002
#define NOTIF_SOFTKEY_FLAGS_SUBMIT_FORM	0x0004
#define NOTIF_SOFTKEY_FLAGS_DISABLED	0x0008

#define SHNUM_SOFTKEYS					0x0020
#define SHNUM_TODAYKEY					0x0040
#define SHNUM_TODAYEXEC					0x0080
#define SHNUM_SOFTKEYCMDS				0x0100
#define SHNUM_FLAGS						0x0200
struct SOFTKEYCMD
{
	WPARAM wpCmd;
	DWORD grfFlags;
};

struct SOFTKEYMENU
{
	HMENU hMenu;
	SOFTKEYCMD *prgskc;
	UINT cskc;
};

struct SOFTKEYNOTIFY
{
	LPCTSTR pszTitle;
	SOFTKEYCMD skc;
};

struct SHNOTIFICATIONDATA2: public SHNOTIFICATIONDATA
{
	union
	{
		SOFTKEYMENU skm;
		SOFTKEYNOTIFY rgskn[2];
	};
	PCTSTR pszTodaySK;
	PCTSTR pszTodayExec;
};
#else
#define SHNOTIFICATIONDATA2 SHNOTIFICATIONDATA
#endif
#endif

class CNotify
{
public:
    CNotify(void);
    ~CNotify(void);
    // 播放指定的声音
    static void Nodify(HWND hwnd, LPCWSTR strPath, int iPeriod, BOOL bNoSound = FALSE, BOOL bVibe = FALSE, UINT Styles = 0);

    static void CNotify::CreateAndAddNotification(HWND hwnd, CString szTitle, CString szNotify);
    static void RemoveNotification();

private:
    static void StartViberate(bool bVibe);
    static void StopViberate(bool bVibe);
};
