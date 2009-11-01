
/***************************************************************************
 *   Copyright (C) 2008 by DDD                                          *
 *   dedodong@163.com                                                     *
 *                                                                         *
 ***************************************************************************/
#include "stdafx.h"
#include "convert_charset.h"

CStringA ConvertUtf16ToUtf8(CString strUtf16) 
{ 
	CStringA res;
	int len = WideCharToMultiByte(CP_UTF8, 0, (LPCTSTR)strUtf16, -1, NULL, 0, NULL, NULL); 

	char *szUtf8 = res.GetBufferSetLength(len + 1);
	memset(szUtf8, 0, (len + 1)*sizeof(char)); 

	WideCharToMultiByte (CP_UTF8, 0, (LPCTSTR)strUtf16, -1, szUtf8, len, NULL, NULL); 

	szUtf8[len] = '\0';

	res.ReleaseBuffer();
	return res;
} 


CString ConvertUtf8ToUtf16(const char * utf8) 
{ 
	CString res;
	if(NULL == utf8)
	{
		res = _T("");
		return res;
	}
	int len = MultiByteToWideChar(CP_UTF8/*CP_ACP*/, 0, utf8, -1, NULL, 0); 

	WCHAR * szUtf16 = res.GetBufferSetLength(len + 1); 
	memset(szUtf16, 0, (len + 1)*sizeof(WCHAR)); 

	MultiByteToWideChar(CP_UTF8/*CP_ACP*/, 0, utf8, -1, szUtf16, len); 

	szUtf16[len] = '\0';

	res.ReleaseBuffer();
	return res;
}

char* ConvertUtf8ToGBK( char* strUtf8)
{
	if(!strUtf8)
		return NULL;
	int len=MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)strUtf8, -1, NULL,0); 
	unsigned short * wszGBK = new unsigned short[len+1]; 
	memset(wszGBK, 0, len * 2 + 2); 
	MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)strUtf8, -1, (LPWSTR)wszGBK, len); 
	len = WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, NULL, 0, NULL, NULL); 
	char *szGBK=new char[len + 1]; 
	memset(szGBK, 0, len + 1); 
	WideCharToMultiByte (CP_ACP, 0, (LPCWSTR)wszGBK, -1, szGBK, len, NULL,NULL); 
	
	delete[] wszGBK; 
	return szGBK; 
}

char* ConvertGBKToUtf8(char* strGBK) 
{ 
#if 1
int len=MultiByteToWideChar(CP_ACP, 0, (LPCSTR)strGBK, -1, NULL,0); 
unsigned short * wszUtf8 = new unsigned short[len+1]; 
memset(wszUtf8, 0, len * 2 + 2); 
MultiByteToWideChar(CP_ACP, 0, (LPCSTR)strGBK, -1, (LPWSTR)wszUtf8, len); 
len = WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)wszUtf8, -1, NULL, 0, NULL, NULL); 
char *szUtf8=new char[len + 1]; 
memset(szUtf8, 0, len + 1); 
WideCharToMultiByte (CP_UTF8, 0, (LPCWSTR)wszUtf8, -1, szUtf8, len, NULL,NULL); 

delete[] wszUtf8; 

return szUtf8; 
#endif
	return NULL;
//delete[] szUtf8; 
} 

CString GetCurrentTimeString()
{
	CString sShortTime;
	SYSTEMTIME LocalTime;
	GetLocalTime(&LocalTime);
	sShortTime.Format(_T("%d-%d-%d %02d:%02d:%02d"), LocalTime.wYear, LocalTime.wMonth, LocalTime.wDay, LocalTime.wHour, LocalTime.wMinute, LocalTime.wSecond);
	return sShortTime;
}

CString GetMsgTimeString(char *msgtime)
{
	CString sShortTime;
	char DayOfWeek[10] = {0};
	char szMonth[10] = {0};
	char GMT[10] = {0};
	int Day, Year, Month, Hour, Minute, Second;
	SYSTEMTIME SystemTimeUTC, SystemTimeLocal;
	FILETIME FileTimeUTC, FileTimeLocal;

	sscanf(msgtime, "%s %d %s %d %d:%d:%d %s", DayOfWeek, &Day, szMonth, &Year, &Hour, &Minute, &Second, GMT);

	{
		if (0 == strcmp(szMonth, "Jan"))
			Month = 1;
		else if (0 == strcmp(szMonth, "Feb"))
			Month = 2;
		else if (0 == strcmp(szMonth, "Mar"))
			Month = 3;
		else if (0 == strcmp(szMonth, "Apr"))
			Month = 4;
		else if (0 == strcmp(szMonth, "May"))
			Month = 5;
		else if (0 == strcmp(szMonth, "Jun"))
			Month = 6;
		else if (0 == strcmp(szMonth, "Jul"))
			Month = 7;
		else if (0 == strcmp(szMonth, "Aug"))
			Month = 8;
		else if (0 == strcmp(szMonth, "Sep"))
			Month = 9;
		else if (0 == strcmp(szMonth, "Oct"))
			Month = 10;
		else if (0 == strcmp(szMonth, "Nov"))
			Month = 11;
		else if (0 == strcmp(szMonth, "Dec"))
			Month = 12;
		else
			Month = 0;
	}

	ZeroMemory(&SystemTimeUTC, sizeof(SYSTEMTIME));
	SystemTimeUTC.wYear = Year;
	SystemTimeUTC.wMonth = Month;
	SystemTimeUTC.wDay = Day;
	SystemTimeUTC.wHour = Hour;
	SystemTimeUTC.wMinute = Minute;
	SystemTimeUTC.wSecond = Second;
	SystemTimeUTC.wMilliseconds = 0;

	SystemTimeToFileTime(&SystemTimeUTC, &FileTimeUTC);
	FileTimeToLocalFileTime(&FileTimeUTC, &FileTimeLocal);
	FileTimeToSystemTime(&FileTimeLocal, &SystemTimeLocal);

	sShortTime.Format(_T("%d-%d-%d %02d:%02d:%02d"), SystemTimeLocal.wYear, SystemTimeLocal.wMonth, SystemTimeLocal.wDay, SystemTimeLocal.wHour, SystemTimeLocal.wMinute, SystemTimeLocal.wSecond);

	return sShortTime;
}

CString ConvertUtf16ToNUM(CString strUtf16)
{
	CString res = _T("");
	CString str5c;
	for(int i = 0; i< strUtf16.GetLength(); i++)
	{
		str5c.Format(_T("%05d"), strUtf16.GetAt(i));
		res += str5c;
	}
	return res;
}

CString ConvertNUMToUft16(CString strNUM)
{
	CString res = _T("");
	PTCHAR pNUM;
	PTCHAR pUtf16;
	TCHAR tc5c[6];
	CString str5c;
	pNUM = strNUM.GetBuffer();
	pUtf16 = res.GetBufferSetLength(strNUM.GetLength()/5 + 1);
	for(int i = 0; i<(strNUM.GetLength()/5); i++)
	{
		wmemcpy(tc5c, pNUM + i * 5, 5);
		tc5c[6] = '\0';
		str5c = tc5c;
		pUtf16[i] = _wtol(str5c);
	}
	pUtf16[strNUM.GetLength()/5] = '\0';
	res.ReleaseBuffer();
	return res;
}