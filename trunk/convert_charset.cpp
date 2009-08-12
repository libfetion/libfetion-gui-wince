
/***************************************************************************
 *   Copyright (C) 2008 by DDD                                          *
 *   dedodong@163.com                                                     *
 *                                                                         *
 ***************************************************************************/
#include "stdafx.h"
#include "convert_charset.h"

char* ConvertUtf16ToUtf8(CString strUtf16) 
{ 

	int len = WideCharToMultiByte(CP_UTF8, 0, (LPCTSTR)strUtf16, -1, NULL, 0, NULL, NULL); 

	char *szUtf8 = new char[len + 1]; 
	memset(szUtf8, 0, (len + 1)*sizeof(char)); 

	WideCharToMultiByte (CP_UTF8, 0, (LPCTSTR)strUtf16, -1, szUtf8, len, NULL, NULL); 

	szUtf8[len] = '\0';

	return szUtf8;
} 


CString ConvertUtf8ToUtf16(const char * utf8) 
{ 
	CString res;
	int len = MultiByteToWideChar(CP_UTF8/*CP_ACP*/, 0, utf8, -1, NULL, 0); 

	WCHAR * szUtf16=new WCHAR[len + 1]; 
	memset(szUtf16, 0, (len + 1)*sizeof(WCHAR)); 

	MultiByteToWideChar(CP_UTF8/*CP_ACP*/, 0, utf8, -1, szUtf16, len); 

	szUtf16[len] = '\0';

	res = szUtf16;
	delete [] szUtf16;
	
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
	sShortTime.Format(_T("%02d:%02d:%02d"), LocalTime.wHour, LocalTime.wMinute, LocalTime.wSecond);
	return sShortTime;
}

CString GetMsgTimeString(char *msgtime)
{
	CString sShortTime;
	char DayOfWeek[10] = {0};
	char Month[10] = {0};
	char GMT[10] = {0};
	int Day, Year, Hour, Minute, Second;
	TIME_ZONE_INFORMATION TimeZoneInformation;
	int MinuteWithBias;

	sscanf(msgtime, "%s %d %s %d %d:%d:%d %s", DayOfWeek, &Day, Month, &Year, &Hour, &Minute, &Second, GMT);
	MinuteWithBias = Hour * 60 + Minute;

	GetTimeZoneInformation(&TimeZoneInformation);
	MinuteWithBias -= TimeZoneInformation.Bias;
	if(MinuteWithBias < 0)
	{
		MinuteWithBias += 24 * 60;
	}
	if(MinuteWithBias > 24 * 60)
	{
		MinuteWithBias -= 24 * 60;
	}
	Hour = MinuteWithBias / 60;
	Minute = MinuteWithBias - Hour * 60;

	sShortTime.Format(_T("%02d:%02d:%02d"), Hour, Minute, Second);

	return sShortTime;
}
