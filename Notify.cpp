#include "StdAfx.h"
#include "Notify.h"

CNotify::CNotify(void)
{
}

CNotify::~CNotify(void)
{
}

// 播放指定的声音或振动
// 如果strPath不为空, 则播放声音
void CNotify::Nodify(CString strPath, int iPeriod, bool bVibe)
{
    StartViberate(bVibe);
    // TODO 播放声音指定的长度

    StopViberate(bVibe);
}

void CNotify::StartViberate(bool bVibe)
{
    if(!bVibe)
        return;
    // TODO 开始振动
}

void CNotify::StopViberate(bool bVibe)
{
    if(!bVibe)
        return;

    // TODO 停止振动
}