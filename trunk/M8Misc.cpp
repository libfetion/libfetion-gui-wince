/***************************************************************************
 *   Add by hiizsk 2009.07.12                                              *
 *                                                                         *
 ***************************************************************************/

#include "stdafx.h"
#include "M8Misc.h"
 
int AddMenuBarForM8(HWND hwnd, UINT idMenu)
{ 
    HWND b=NULL;
    HWND hBar=NULL;

#if defined(_WIN32_WCE) && (_WIN32_WCE < 201)  
    hBar = this->m_poCommandBar->m_hCommandBar;
    if (hBar !=NULL)
    {
        ::DestroyWindow(hBar);
        delete this->m_poCommandBar;
        this->m_poCommandBar = NULL;
    }
#endif
    HINSTANCE hInst = AfxGetInstanceHandle();

    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(icex);
    icex.dwICC  = ICC_BAR_CLASSES | ICC_COOL_CLASSES;
    InitCommonControlsEx(&icex);

    b = (HWND)::CommandBands_Create(hInst, hwnd, ID_BANDS, 
        RBS_BANDBORDERS|RBS_VARHEIGHT,NULL);
    if (b==NULL) return -1;

    //theApp.m_hRebars= b;
    // Will be making 3 bands (1 for the menu, and 2 for the toolbars)

    REBARBANDINFO rbi[1];

    memset(&rbi,0,sizeof(REBARBANDINFO));
    rbi[0].cbSize = sizeof(REBARBANDINFO);
    rbi[0].fMask = RBBIM_STYLE|RBBIM_ID|RBBIM_SIZE;
    rbi[0].fStyle = RBBS_NOGRIPPER;
    rbi[0].wID = ID_MENUBAND;
    rbi[0].cx = 480;


    // Now ad all three bar to the CommandBenad

    if(!::CommandBands_AddBands(b, hInst, 1, rbi))
        return -1;

    // Get a Handle to the first bar (Bar 0), aw we are going to insert a Menu

    // into it

    hBar = ::CommandBands_GetCommandBar(b,0);
    ASSERT(hBar != NULL);
    ::CommandBar_InsertMenubar(hBar, hInst, idMenu, 0);


#if !defined(_WIN32_WCE_PSPC)
    CommandBands_AddAdornments(b, hInst, 0, NULL);
#endif
}

void FullScreen(HWND hwnd)
{
    int iFullWidth = GetSystemMetrics(SM_CXSCREEN);

    int iFullHeight = GetSystemMetrics(SM_CYSCREEN);

    ::SetWindowPos(hwnd, HWND_TOPMOST, 0, 40, iFullWidth, iFullHeight - 40, SWP_NOOWNERZORDER|SWP_SHOWWINDOW);
}