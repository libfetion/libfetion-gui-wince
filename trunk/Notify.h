#pragma once

class CNotify
{
public:
    CNotify(void);
    ~CNotify(void);
    // 播放指定的声音
    static void Nodify(CString strPath, int iPeriod, bool bVibe);
private:
    static void StartViberate(bool bVibe);
    static void StopViberate(bool bVibe);
};
