/***************************************************************************
 *   Copyright (C) 2009 by DDD                                          *
 *   dedodong@163.com                                                     *
 *                                                                         *
 ***************************************************************************/
#if !defined _MISC_WMLF__H
#define _MISC_WMLF__H

CString GetHttpsWebData(CString Url);

#ifdef __cplusplus 
extern "C" { 
#endif 


#include <Winreg.h>

#if 0
long Lib_ReadReg(bool is_id, CString &Buf);
long Lib_WriteReg(bool is_id, CString Buf);
#endif

long Lib_ReadReg(CString Key, CString &Buf);
long Lib_WriteReg(CString Key, CString Buf);

void   OpenRegistry(WCHAR *pFilePath,BOOL nFlag = 0);
long   ReadRegistry(WCHAR *pRegKey,BYTE *pReadBuf,DWORD nReadLen,DWORD dwType);
long   WriteRegistry(WCHAR *pRegKey,BYTE *pWriteBuf,DWORD nWriteLen,DWORD dwType);
void   CloseRegistry(BOOL nCloseFlag = 0);

#ifdef __cplusplus 
} 
#endif 
#endif