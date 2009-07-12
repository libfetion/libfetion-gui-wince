#pragma once

class CNotify
{
public:
    CNotify(void);
    ~CNotify(void);
    // 播放指定的声音
    static void Nodify(HWND hwnd, LPCWSTR strPath, int iPeriod, bool bNoSound = false, bool bVibe = false, UINT Styles = 0);
private:
    static void StartViberate(bool bVibe);
    static void StopViberate(bool bVibe);
};
