
/***************************************************************************
 *   Copyright (C) 2008 by DDD                                          *
 *   dedodong@163.com                                                     *
 *                                                                         *
 ***************************************************************************/
#include "stdafx.h"
#include "convert_charset.h"
#include "ProvAndCityCode.h"

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

SYSTEMTIME GetCurrentTime()
{
	SYSTEMTIME LocalTime;
	GetLocalTime(&LocalTime);
	return LocalTime;
}

SYSTEMTIME GetMsgTime(char *msgtime)
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

	return SystemTimeLocal;
}

CString GetTimeString(SYSTEMTIME SystemTime)
{
	CString sShortTime;
	sShortTime.Format(_T("%d-%d-%d %02d:%02d:%02d"), SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour, SystemTime.wMinute, SystemTime.wSecond);

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
		pUtf16[i] = (TCHAR)_wtol(str5c);
	}
	pUtf16[strNUM.GetLength()/5] = '\0';
	res.ReleaseBuffer();
	return res;
}

CString FormatMsgLog(MSGLOGDB * pMsgLog)
{
	CString strMsgLog;
	strMsgLog = pMsgLog->strSender + _T("(") + GetTimeString(pMsgLog->MsgTime) + _T(")\r\n") + pMsgLog->strMsg;
	return strMsgLog;
}

void AddReturn(CString &strShow)
{
	if(strShow.GetLength() > 0)
	{
		strShow += _T("\r\n\r\n");
	}
}

// 根据代码获取省份名字
CString GetProvince(CString strProvinceCode)
{
    if(L"" == strProvinceCode)
        return L"其它";
    for(int i = 0; i < 34; i ++)
    {
        if(strProvinceCode == ProvinceArray[i].ProvinceCode)
        {
            return ProvinceArray[i].ProvinceName;
        }
    }
    return L"其它";

}

CString GetCity(int iCityCode)
{
    //默认如果是99表示其它
    if(99 == iCityCode)
        return L"其它";
    for(int i = 0; i < 340; i ++)
    {
        if(iCityCode == CityArray[i].CityCode)
        {
            return CityArray[i].CityName;
        }
    }
    return L"其它";
}

CString LunarAnimal[12] = 
{
L"鼠",
L"牛",
L"虎",
L"兔",
L"龙",
L"蛇",
L"马",
L"羊",
L"猴",
L"鸡",
L"狗",
L"猪"
};

CString GetLunarAnimal(int iIndex)
{
	if((iIndex < 1) || (iIndex >12))
	{
		return L"";
	}
	return LunarAnimal[iIndex - 1];
}

CString Horoscope[12] = 
{
L"摩羯座",
L"水瓶座",
L"双鱼座",
L"白羊座",
L"金牛座",
L"双子座",
L"巨蟹座",
L"狮子座",
L"处女座",
L"天秤座",
L"天蝎座",
L"射手座"
};

CString GetHoroscope(int iIndex)
{
	if((iIndex < 1) || (iIndex >12))
	{
		return L"";
	}
	return Horoscope[iIndex - 1];
}

CString Sex[3] = 
{
L"保密",
L"男",
L"女"
};

CString GetSex(int iIndex)
{
	if((iIndex < 0) || (iIndex >2))
	{
		return L"";
	}
	return Sex[iIndex];
}

CString BloodType[6] = 
{
L"未知",
L"A型",
L"B型",
L"AB型",
L"O型",
L"其它"
};

CString GetBloodType(int iIndex)
{
	if((iIndex < 0) || (iIndex >5))
	{
		return L"";
	}
	return BloodType[iIndex];
}
