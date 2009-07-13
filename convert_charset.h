
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


char* ConvertUtf8ToGBK( char* strUtf8);

char* ConvertGBKToUtf8(char* strGBK) ;

WCHAR* ConvertUtf8ToUtf16(const char * utf8);
char* ConvertUtf16ToUtf8(CString strUtf16);


#ifdef __cplusplus 
} 
#endif 
#endif