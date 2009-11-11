
#include "CppSQLite3U.h"

#pragma once

class CFxDatabase
{
public:
	CFxDatabase(void);
	~CFxDatabase(void);
	BOOL Init(LPCTSTR szFile);
	BOOL CheckBuddyInfoTable(CppSQLite3DB &db);
	BOOL UpdateBuddyInfo(BUDDYINFODB * pBuddyInfo);
	BOOL ReadBuddyInfo(BUDDYINFODB * pBuddyInfo);
protected:
	//CppSQLite3DB m_db;
	CString m_szFile;
};
