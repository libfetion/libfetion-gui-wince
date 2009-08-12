/***************************************************************************
 *   Copyright (C) 2009 by DDD                                             *
 *   dedodong@163.com                                                      *
 *                                                                         *
 ***************************************************************************/
#include "stdafx.h"
#include "misc.h"

#include <afxinet.h>

HKEY g_hKEY = NULL;

void OpenRegistry(WCHAR *pFilePath,BOOL nOpenFlag)
{
	long ret = ::RegOpenKeyEx(HKEY_CURRENT_USER,pFilePath, 0, KEY_ALL_ACCESS, &g_hKEY);
	if (ERROR_SUCCESS != ret)
	{
		DWORD dwPos = 0;
		long ret1 = RegCreateKeyEx

		(HKEY_CURRENT_USER,pFilePath,0,0,REG_OPTION_NON_VOLATILE,0,NULL, &g_hKEY,&dwPos);
		if(ERROR_SUCCESS != ret1)
		{
			g_hKEY = NULL;
			RETAILMSG(true,(L"RegCreateKeyEx failed. \r\n"));
		}
		else
		{
			RETAILMSG(true,(L"OK .g_hKEY = %x\r\n",g_hKEY));
		}
	}
}

long ReadRegistry(WCHAR *pRegKey,BYTE *pReadBuf,DWORD nReadLen,DWORD dwType)
{
	long ret = 0;
	if (NULL == g_hKEY)
	{
		ret = -1;
		RETAILMSG(true,(L" failed NULL == g_hKEY.\r\n"));
		return ret;
	}
	ret = RegQueryValueEx(g_hKEY,pRegKey,NULL,&dwType,pReadBuf,&nReadLen);
	if(ERROR_SUCCESS != ret)
	{
		ret = -1;
		RETAILMSG(true,(L"RegQueryValueEx failed.\r\n"));
	}
	return ret;
}

long WriteRegistry(WCHAR *pRegKey,BYTE *pWriteBuf,DWORD nWriteLen,DWORD dwType)
{
	long ret = 0;
	if(NULL == g_hKEY)
	{
		ret = -1;
		RETAILMSG(true,(L"g_hKEY == NULL.\r\n"));
		return ret;
	}
	ret = ::RegSetValueEx(g_hKEY,pRegKey,0,dwType,(LPBYTE) pWriteBuf,nWriteLen);
	if (ERROR_SUCCESS != ret)
	{
		ret = -1;
		RETAILMSG(true,(L"RegSetValueEx failed.\r\n"));
	}
	return ret;
}

void CloseRegistry(BOOL nCloseFlag)
{
	if (g_hKEY)
	{
		long ret=::RegCloseKey(g_hKEY);
		g_hKEY = NULL;
		RETAILMSG(true,(L"RegCloseKey OK.\r\n"));
	}
	else
	{
		RETAILMSG(true,(L"RegCloseKey failed\r\n"));
	}
}


#define REGMAXLEN 200
BYTE buf[REGMAXLEN];

//long Lib_ReadReg(bool is_id, CString &Buf)
long Lib_ReadReg(CString Key, CString &Buf)
{
	long ret;
#if 0
	WCHAR   p[20]   =   _T("x234");  
	WCHAR   Key[20]   =   _T("app");
	WCHAR   KeyPath[200]   =  ;
	
	char *p2= "123132xxx";
	
	
	CString test = _T("13123123213****ADSANADJSAD))Wqe");
	char* WRbuf = ConvertUtf16ToUtf8(test);
//::AfxMessageBox(CString(WRbuf));	
//memcpy(buf, WRbuf,test.GetLength());
//::AfxMessageBox(CString(buf));
	//memcpy(buf,p2,sizeof("123132xxx"));
	OpenRegistry( _T("SOFTWARE\\HITECH\\Apps\\LibFetion"));
	ret = WriteRegistry(Key,(BYTE*)WRbuf,test.GetLength(),REG_DWORD);
	//ret = WriteRegistry(Key,buf,sizeof(buf),REG_DWORD);
	CloseRegistry();


	memset(buf,0,REGMAXLEN);
	OpenRegistry(KeyPath);
	ret = ReadRegistry(Key,buf,REGMAXLEN-1,REG_DWORD);
	CloseRegistry();

	::AfxMessageBox(CString(buf));

	::AfxMessageBox(CString(p));

	memcpy(buf,p2,sizeof(p2));
	::AfxMessageBox(CString(buf));

	memset(buf,0,REGMAXLEN);
	
#endif

	OpenRegistry(_T("SOFTWARE\\HITECH\\Apps\\LibFetion"));
	memset(buf, 0, REGMAXLEN);
	ret = ReadRegistry((LPTSTR)(LPCTSTR)Key, buf, REGMAXLEN,REG_DWORD);
#if 0
	if (is_id)
		ret = ReadRegistry(_T("ID"),buf,REGMAXLEN,REG_DWORD);
	else
		ret = ReadRegistry(_T("PWD"),buf,REGMAXLEN,REG_DWORD);
#endif
	CloseRegistry();
	
	//::AfxMessageBox(CString(buf));

	Buf = CString(buf);
	return ret;
}

long Lib_WriteReg(CString Key, CString Buf)
{
	long ret;

	char* WRbuf = ConvertUtf16ToUtf8(Buf);

	OpenRegistry(_T("SOFTWARE\\HITECH\\Apps\\LibFetion"));

	ret = WriteRegistry((LPTSTR)(LPCTSTR)Key,(BYTE*)WRbuf,Buf.GetLength(),REG_DWORD);
#if 0
	if (is_id)
		ret = WriteRegistry(_T("ID"),(BYTE*)WRbuf,Buf.GetLength(),REG_DWORD);
	else
		ret = WriteRegistry(_T("PWD"),(BYTE*)WRbuf,Buf.GetLength(),REG_DWORD);
#endif
	CloseRegistry();

	if (WRbuf)
		delete [] WRbuf;
	return ret;
}


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
	if (!AfxParseURL(Url, dwServiceType, strServerName, strObject, nPort) ||dwServiceType != INTERNET_SERVICE_HTTP)
	{
		BOOL bS = FALSE;
	}
	session.EnableStatusCallback(TRUE);
	CHttpConnection* pServer = session.GetHttpConnection(strServerName, nPort);
	CHttpFile* pFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_GET, strObject, NULL, 1, NULL, NULL,
	INTERNET_FLAG_EXISTING_CONNECT|
	INTERNET_FLAG_SECURE|
	INTERNET_FLAG_IGNORE_CERT_CN_INVALID|
	INTERNET_FLAG_IGNORE_CERT_DATE_INVALID);
	//end Request 
	BOOL result = pFile->SendRequest(strHeaders, 
	(LPVOID)(LPCTSTR)Url, Url.GetLength()); 
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
