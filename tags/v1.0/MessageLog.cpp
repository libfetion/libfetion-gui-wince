#include "StdAfx.h"
#include "MessageLog.h"

CMessageLog::CMessageLog(void)
{
    m_MsgLogs = NULL;
}

CMessageLog::~CMessageLog(void)
{
    ClearMsgLog();
}

bool CMessageLog::StoreMsgLog(long accountID, CString Message)
{
    MSGLOG* curMsgLog;

    curMsgLog = this->m_MsgLogs;
    while(NULL != curMsgLog)
	{
		if(curMsgLog->AccountID == accountID)
        {
            curMsgLog->Message = Message;
            return true;
        } 
		else
		{
			if(NULL == curMsgLog->Next)
			{
				break;
			}
			curMsgLog = curMsgLog->Next;
        }
    } 

    MSGLOG* newMsgLog = new MSGLOG();
    newMsgLog->AccountID = accountID;
    newMsgLog->Message = Message;
    newMsgLog->Next = NULL;
    if(NULL == this->m_MsgLogs)
        this->m_MsgLogs = newMsgLog;
    else
        curMsgLog->Next = newMsgLog;
    return true;
}

CString CMessageLog::LoadMsgLog(long accountID)
{

    MSGLOG* curMsgLog = this->m_MsgLogs;
    while(NULL != curMsgLog)
    {
        if(curMsgLog->AccountID == accountID)
        {
            return curMsgLog->Message;
        } else {
            curMsgLog = curMsgLog->Next;
        }
    }
    return _T("");
}
void CMessageLog::ClearMsgLog(void)
{
    MSGLOG* curMsgLog = this->m_MsgLogs;
    MSGLOG* nextMsgLog;
    while(NULL != curMsgLog)
    {
        nextMsgLog = curMsgLog->Next;
        delete curMsgLog;
        curMsgLog = nextMsgLog;
    }
}