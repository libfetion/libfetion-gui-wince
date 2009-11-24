
/***************************************************************************
 *   Copyright (C) 2008 by DDD                                          *
 *   dedodong@163.com                                                     *
 *                                                                         *
 ***************************************************************************/
#if !defined _CONVERT_CHARSET__H
#define _CONVERT_CHARSET__H

#ifdef __cplusplus 
extern "C" { 
#endif 

#ifdef __cplusplus 
} 
#endif 

char* ConvertUtf8ToGBK( char* strUtf8);

char* ConvertGBKToUtf8(char* strGBK) ;

CString ConvertUtf8ToUtf16(const char * utf8);
CStringA ConvertUtf16ToUtf8(CString strUtf16);

SYSTEMTIME GetCurrentTime();
SYSTEMTIME GetMsgTime(char *msgtime);
CString GetTimeString(SYSTEMTIME SystemTime);

CString ConvertUtf16ToNUM(CString strUtf16);
CString ConvertNUMToUft16(CString strNUM);

CString FormatMsgLog(MSGLOGDB * pMsgLog);
void AddReturn(CString &strShow);

#endif