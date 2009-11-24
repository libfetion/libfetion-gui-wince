
#include "CppSQLite3U.h"

#pragma once

class CFxDatabase
{
public:
	CFxDatabase(void);
	~CFxDatabase(void);
	BOOL Init(LPCTSTR szFile);
	BOOL CheckBuddyInfoTable(CppSQLite3DB * pdb);
	BOOL CheckMsgLogTable(CppSQLite3DB &db);
	BOOL UpdateBuddyInfo(BUDDYINFODB * pBuddyInfo);
	BOOL ReadBuddyInfo(BUDDYINFODB * pBuddyInfo);
	BOOL ReadBuddyInfoBegin();
	BOOL ReadBuddyInfoEnd();
	BOOL AddMegLog(MSGLOGDB * pMsgLog);
	DWORD GetMegLogCount(long lID);
	BOOL ReadMsgLog(MSGLOGFROMDB * pMsgLogFromDB);
	BOOL DeleteAllMsgLog(long lID);
protected:
	CppSQLite3DB * m_pdbReadBuddyInfo;
	CString m_szFile;
};
