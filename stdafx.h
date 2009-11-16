﻿// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
//  特定于项目的包含文件

#pragma once

#pragma comment(linker, "/nodefaultlib:libc.lib")
#pragma comment(linker, "/nodefaultlib:libcd.lib")
#pragma comment(lib, "winsock.lib")
#pragma comment(lib, "cellcore.lib")
#pragma comment(lib, "./lib/LibFetion.lib") 


// 注意 - 这个值与作为目标的 Windows CE OS 版本的关联性并不强
#define WINVER _WIN32_WCE

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// 从 Windows 头中排除极少使用的资料
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 某些 CString 构造函数将是显式的
#ifdef _CE_DCOM
#define _ATL_APARTMENT_THREADED
#endif

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <ceconfig.h>
#if defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP)
#define SHELL_AYGSHELL
#endif

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>		// MFC 对 Internet Explorer 4 公共控件的支持
#endif



#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <pm.h>				// 电源管理

#if defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP) || defined(M8)
#ifndef _DEVICE_RESOLUTION_AWARE
#define _DEVICE_RESOLUTION_AWARE
#endif
#endif

#ifdef _DEVICE_RESOLUTION_AWARE
#include "DeviceResolutionAware.h"
#endif

#if defined(SHELL_AYGSHELL) || defined(M8)
#include <aygshell.h>
#pragma comment(lib, "aygshell.lib") 
#endif // SHELL_AYGSHELL

#if (_WIN32_WCE < 0x500) && ( defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP) )
	#pragma comment(lib, "ccrtrtti.lib")
	#ifdef _X86_	
		#if defined(_DEBUG)
			#pragma comment(lib, "libcmtx86d.lib")
		#else
			#pragma comment(lib, "libcmtx86.lib")
		#endif
	#endif
#endif

#define DEBUG_GUI 0
#include <altcecrt.h>

#include "libfetion/libfetion.h"
#include "misc.h"

typedef struct _Group_Info
{
	CString groupName;
	long groupID;
	int online_no;
}Group_Info;

typedef struct _Account_Info
{
	CString accountName;
	long accountID;
	int onlinestate;
	BOOL isUpdate;
}Account_Info;

typedef struct _Qun_Info
{
	CString qunName;
	long qunID;
}Qun_Info;

typedef struct _BUDDYINFODB
{
	long lID;
	CString strLocalName;
	CString strNickName;
	CString strImpresa;
}BUDDYINFODB;

typedef struct _MSGLOGDB
{
	long lID;
	CString strSender;
	SYSTEMTIME MsgTime;
	CString strMsg;
}MSGLOGDB;

typedef struct _MSGLOGFROMDB
{
	long lID;
	DWORD dwFirst;
	DWORD dwCount;
	CString strMsg;
}MSGLOGFROMDB;

#include "convert_charset.h"
