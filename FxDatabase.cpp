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
		if(!db.tableExists(L"BuddyInfo"))
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
