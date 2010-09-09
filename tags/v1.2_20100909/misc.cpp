/***************************************************************************
 *   Copyright (C) 2009 by DDD                                             *
 *   dedodong@163.com                                                      *
 *                                                                         *
 ***************************************************************************/
#include "stdafx.h"
#include "misc.h"

#include <afxinet.h>

#define BUFFER_SIZE 1024
int GetHttpsWebData(CString Url, CString *web_data, CString *cookie_data)
{
	CString sContent;
	CString strHeaders = _T("Content-Type: application/x-www-form-urlencoded"); 
	INTERNET_PORT nPort = INTERNET_DEFAULT_HTTPS_PORT;
	CInternetSession session; 
	CString strServerName;
	CString strObject;
	DWORD dwServiceType;
	if (!AfxParseURL(Url, dwServiceType, strServerName, strObject, nPort) ||
		(dwServiceType != AFX_INET_SERVICE_HTTPS))
	{
		return -1;
	}

	session.EnableStatusCallback(TRUE);
	CHttpConnection* pServer = session.GetHttpConnection(strServerName, nPort);
	if (!pServer)
	{
		session.Close();
		return -1;
	}

	CHttpFile* pFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_GET, strObject,
										NULL, 1, NULL, NULL,	INTERNET_FLAG_SECURE);
	if (!pFile)
	{
		pServer->Close();
		session.Close();
		return -1;
	}

	//end Request 
	BOOL result = FALSE;

	try
	{
		//Get Response
		CString strGetData;
		CString strsz;

		result = pFile->SendRequest(strHeaders, (LPVOID)(LPCTSTR)Url, Url.GetLength()); 
		if (result == FALSE)
		{
			pFile->Close();
			pServer->Close();
			session.Close();
			return -1;
		}

		pFile->SetReadBufferSize(BUFFER_SIZE);

		while (pFile->ReadString(strGetData))
		{
			strsz += strGetData;
		}

		strGetData = ConvertUtf8ToUtf16((LPCSTR)strsz.GetBuffer());

		if (cookie_data)
		{
			CString strcookie;
			DWORD dw = 0;
			pFile->QueryInfo(HTTP_QUERY_SET_COOKIE, strcookie, &dw);
			*cookie_data = strcookie;
		}

		if (web_data)
		{
			*web_data = strGetData;
		}

	}

	catch(CInternetException * pException)
	{
		pException->m_dwError;
		pException->Delete();
		return -1;
	}


	pFile->Close();
	pServer->Close();
	session.Close();
	return 0;
}

BOOL IsNumber(CString strInput, int iLength)
{
	if(iLength > 0)
	{
		if(strInput.GetLength() != iLength)
		{
			return FALSE;
		}
	}
	CStringA strConvertA = ConvertUtf16ToUtf8(strInput);
	int iPosition = iLength - 1;
	char tempchar;
	while(iPosition >= 0)
	{
		tempchar = (strConvertA.GetAt(iPosition));
		iPosition--;
		if((tempchar < '0') || (tempchar > '9'))
		{
			return FALSE;
		}
	}

	return TRUE;
}