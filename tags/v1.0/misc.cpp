/***************************************************************************
 *   Copyright (C) 2009 by DDD                                             *
 *   dedodong@163.com                                                      *
 *                                                                         *
 ***************************************************************************/
#include "stdafx.h"
#include "misc.h"

#include <afxinet.h>

#define BUFFER_SIZE 1024
CString GetHttpsWebData(CString Url)
{
	CString sContent;
	CString strHeaders = _T("Content-Type: application/x-www-form-urlencoded"); 
	INTERNET_PORT nPort = INTERNET_DEFAULT_HTTPS_PORT;
	CInternetSession session; 
	CString strServerName;
	CString strObject;
	DWORD dwServiceType;
	if (!AfxParseURL(Url, dwServiceType, strServerName, strObject, nPort) || (dwServiceType != AFX_INET_SERVICE_HTTPS))
	{
		BOOL bS = FALSE;
		return _T("");
	}
	session.EnableStatusCallback(TRUE);
	CHttpConnection* pServer = session.GetHttpConnection(strServerName, nPort);
	CHttpFile* pFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_GET, strObject, NULL, 1, NULL, NULL,
	INTERNET_FLAG_EXISTING_CONNECT|
	INTERNET_FLAG_SECURE|
	INTERNET_FLAG_IGNORE_CERT_CN_INVALID|
	INTERNET_FLAG_IGNORE_CERT_DATE_INVALID);
	//end Request 
	BOOL result = FALSE;
	try
	{
		result = pFile->SendRequest(strHeaders, (LPVOID)(LPCTSTR)Url, Url.GetLength()); 
	}
	catch(CInternetException * pException)
	{
		pException->m_dwError;
		pException->Delete();
	}
	//Get Response
	CString strGetData;
	TCHAR* szWEBPage = new TCHAR[BUFFER_SIZE+1];
	if(szWEBPage)
	{
		szWEBPage[0] = L'\0';
		TCHAR* sz     = new TCHAR[BUFFER_SIZE+1];
		TCHAR* szwBuf = new TCHAR[(BUFFER_SIZE+1)*2];
		sz[0] = L'\0';
		szwBuf[0] = L'\0';
		DWORD n = 0;
		printf("pFile:%d\n",pFile->GetLength());
		pFile->SetReadBufferSize(BUFFER_SIZE*2);
		while (pFile->ReadString(sz, BUFFER_SIZE))//每次一行数据
		{
			mbstowcs(szwBuf, (char*)sz, BUFFER_SIZE+1);
			n += _tcslen(szwBuf);
			if(n >= BUFFER_SIZE)
				break;
			_tcscat(szWEBPage, szwBuf);
		}
		delete [] sz;
		sz = NULL;
		delete [] szwBuf;
		szwBuf = NULL;
	}
	strGetData = szWEBPage;
	delete [] szWEBPage;
	szWEBPage = NULL;
	return strGetData;
}
