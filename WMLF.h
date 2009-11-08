﻿// WMLF.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#ifdef WIN32_PLATFORM_WFSP
#include "Resourcesp.h"
#else
#include "Resourceppc.h"
#endif

// CWMLFApp:
// 有关此类的实现，请参阅 WMLF.cpp
//

class CWMLFApp : public CWinApp
{
public:
	CWMLFApp();
	
// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
public:
	virtual int ExitInstance();
};

extern CWMLFApp theApp;
