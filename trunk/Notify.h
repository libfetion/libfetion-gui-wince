#pragma once

class CNotify
{
public:
    CNotify(void);
    ~CNotify(void);
    // ����ָ��������
    static void Nodify(HWND hwnd, LPCWSTR strPath, int iPeriod, bool bVibe, UINT Styles);
private:
    static void StartViberate(bool bVibe);
    static void StopViberate(bool bVibe);
};
