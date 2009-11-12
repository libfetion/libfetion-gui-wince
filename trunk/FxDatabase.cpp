#include "StdAfx.h"
#include "FxDatabase.h"

CFxDatabase * g_pFxDB = NULL;

CFxDatabase::CFxDatabase(void)
{
	m_szFile = _T("");
}

CFxDatabase::~CFxDatabase(void)
{
	//Close();
}

BOOL CFxDatabase::Init(LPCTSTR szFile)
{
	m_szFile = szFile;
	try
	{
		CppSQLite3DB db;
		db.open(m_szFile);
		db.close();
	}
	catch(CppSQLite3Exception e)
	{
		TRACE(e.errorMessage());
		return FALSE;
	}
	return TRUE;
}

BOOL CFxDatabase::CheckBuddyInfoTable(CppSQLite3DB &db)
{
	try
	{
		if(!db.tableExists(_T("BuddyInfo")))
		{
			CString sqlStr;
			sqlStr.Format(_T("create table BuddyInfo(ID int UNIQUE, LocalName char, NickName char, Impresa char);"));
			db.execDML(sqlStr);
		}
	}
	catch(CppSQLite3Exception e)
	{
		TRACE(e.errorMessage());
		return FALSE;
	}
	return TRUE;
}

BOOL CFxDatabase::CheckMsgLogTable(CppSQLite3DB &db)
{
	try
	{
		if(!db.tableExists(_T("MsgLog")))
		{
			CString sqlStr;
			sqlStr.Format(_T("create table MsgLog(ID int, Sender char, Year int, Month int, Day int, Hour int, Minute int, Second int, Msg char);"));
			db.execDML(sqlStr);
		}
	}
	catch(CppSQLite3Exception e)
	{
		TRACE(e.errorMessage());
		return FALSE;
	}
	return TRUE;
}

BOOL CFxDatabase::UpdateBuddyInfo(BUDDYINFODB * pBuddyInfo)
{

	try
	{
		CppSQLite3DB db;
		CString sqlStr;

		db.open(m_szFile);
		CheckBuddyInfoTable(db);
		sqlStr.Format(_T("select * from BuddyInfo where ID=%d;"), pBuddyInfo->lID);
		CppSQLite3Query q = db.execQuery(sqlStr);
		if(!q.eof())
		{
			sqlStr.Format(_T("update BuddyInfo set LocalName='%s', NickName='%s', Impresa='%s' where ID=%d;"), pBuddyInfo->strLocalName, pBuddyInfo->strNickName, pBuddyInfo->strImpresa, pBuddyInfo->lID);
		}
		else
		{
			sqlStr.Format(_T("insert into BuddyInfo VALUES(%d, '%s', '%s', '%s')"), pBuddyInfo->lID, pBuddyInfo->strLocalName, pBuddyInfo->strNickName, pBuddyInfo->strImpresa);
		}
		db.execDML(sqlStr);
		db.close();
	}
	catch(CppSQLite3Exception e)
	{
		TRACE(e.errorMessage());
		return FALSE;
	}
	return TRUE;
}

BOOL CFxDatabase::ReadBuddyInfo(BUDDYINFODB * pBuddyInfo)
{
	if(NULL == pBuddyInfo)
	{
		return FALSE;
	}

	pBuddyInfo->strLocalName = _T("");
	pBuddyInfo->strNickName = _T("");
	pBuddyInfo->strImpresa = _T("");

	try
	{
		CppSQLite3DB db;
		CString sqlStr;

		db.open(m_szFile);
		CheckBuddyInfoTable(db);
		sqlStr.Format(_T("select * from BuddyInfo where ID=%d;"), pBuddyInfo->lID);
		CppSQLite3Query q = db.execQuery(sqlStr);
		if(!q.eof())
		{
			pBuddyInfo->strLocalName = q.getStringField(1);
			pBuddyInfo->strNickName = q.getStringField(2);
			pBuddyInfo->strImpresa = q.getStringField(3);
		}
		db.close();
	}
	catch(CppSQLite3Exception e)
	{
		TRACE(e.errorMessage());
		return FALSE;
	}
	return TRUE;
}

BOOL CFxDatabase::AddMegLog(MSGLOGDB * pMsgLog)
{

	try
	{
		CppSQLite3DB db;
		CString sqlStr;

		db.open(m_szFile);
		CheckMsgLogTable(db);
		sqlStr.Format(_T("insert into MsgLog VALUES(%d, '%s', %d, %d, %d, %d, %d, %d, '%s');"), pMsgLog->lID, pMsgLog->strSender, pMsgLog->MsgTime.wYear, pMsgLog->MsgTime.wMonth, pMsgLog->MsgTime.wDay, pMsgLog->MsgTime.wHour, pMsgLog->MsgTime.wMinute, pMsgLog->MsgTime.wSecond, pMsgLog->strMsg);
		db.execDML(sqlStr);
		db.close();
	}
	catch(CppSQLite3Exception e)
	{
		TRACE(e.errorMessage());
		return FALSE;
	}
	return TRUE;
}

DWORD CFxDatabase::GetMegLogCount(int lID)
{
	try
	{
		CppSQLite3DB db;
		CString sqlStr;

		db.open(m_szFile);
		CheckMsgLogTable(db);
		sqlStr.Format(_T("select count(*) from MsgLog where ID=%d;"), lID);
		DWORD dwCount = db.execScalar(sqlStr);
		db.close();
		return dwCount;
	}
	catch(CppSQLite3Exception e)
	{
		TRACE(e.errorMessage());
		return 0;
	}
}

BOOL CFxDatabase::ReadMsgLog(MSGLOGFROMDB * pMsgLogFromDB)
{
	if(NULL == pMsgLogFromDB)
	{
		return FALSE;
	}

	pMsgLogFromDB->strMsg = _T("");

	try
	{
		CppSQLite3DB db;
		CString sqlStr;

		db.open(m_szFile);
		CheckMsgLogTable(db);
		sqlStr.Format(_T("select * from MsgLog where ID=%d;"), pMsgLogFromDB->lID);
		CppSQLite3Query q = db.execQuery(sqlStr);
		DWORD dwSeek = 0;
		DWORD dwCount = 0;
		MSGLOGDB MsgLog;
		while(!q.eof())
		{
			if(dwCount >= pMsgLogFromDB->dwCount)
			{
				break;
			}
			dwSeek++;
			if(dwSeek >= pMsgLogFromDB->dwFirst)
			{
				dwCount++;
				MsgLog.lID =  pMsgLogFromDB->lID;
				MsgLog.strSender = q.getStringField(1);
				MsgLog.MsgTime.wYear = q.getIntField(2);
				MsgLog.MsgTime.wMonth = q.getIntField(3);
				MsgLog.MsgTime.wDay = q.getIntField(4);
				MsgLog.MsgTime.wHour = q.getIntField(5);
				MsgLog.MsgTime.wMinute = q.getIntField(6);
				MsgLog.MsgTime.wSecond = q.getIntField(7);
				MsgLog.MsgTime.wMilliseconds = 0;
				MsgLog.strMsg = q.getStringField(8);
				pMsgLogFromDB->strMsg += FormatMsgLog(&MsgLog);
			}
			q.nextRow();
		}
		pMsgLogFromDB->dwCount = dwCount;
		db.close();
	}
	catch(CppSQLite3Exception e)
	{
		TRACE(e.errorMessage());
		return FALSE;
	}
	return TRUE;
}
