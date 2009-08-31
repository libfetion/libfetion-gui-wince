#pragma once


struct MSGLOG
{
    long AccountID;
    CString Message;
    MSGLOG* Next;
} ;

class CMessageLog
{
public:
    CMessageLog(void);
    ~CMessageLog(void);

    MSGLOG* m_MsgLogs;

    void ClearMsgLog(void);
    bool StoreMsgLog(long accountID, CString Message);
    CString LoadMsgLog(long accountID);
};
