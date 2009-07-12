// ConnectTo.h : 连接网络库
//

#ifndef M8
#include "connmgr.h"
#include "stdafx.h"
#pragma comment(lib, "Ws2.lib")
#include "winsock2.h"

//#if (WIN32_PLATFORM_PSPC>300 || WIN32_PLATFORM_WFSP )
#include <objbase.h>
#include <initguid.h>

typedef HRESULT (*CONNMGRCONNECTIONSTATUS)(HANDLE hConnection,DWORD *pdwStatus);
typedef HRESULT (*CONNMGRRELEASECONNECTION)(HANDLE hConnection,LONG lCache );
typedef HRESULT (*CONNMGRESTABLISHCONNECTION)(CONNMGR_CONNECTIONINFO *pConnInfo, HANDLE *phConnection,DWORD dwTimeout,DWORD *pdwStatus);
//#endif

HANDLE g_phWebConnection;

//////////////////////////////////////////////////////////////////////////
//函数：EstablishDatacall
//作用：连接网络，返回是否成功
//参数：IPout，连接得到的IP地址
//返回值：TRUE成功，FALSE失败
//////////////////////////////////////////////////////////////////////////
BOOL EstablishConnection(TCHAR *IPout)
{
	CHAR    szHostname[255];      
	TCHAR   IP[17];
	HOSTENT *pHostEnt=NULL;
	int      nAdapter = 0;
	IN_ADDR *tsim=NULL;
	BOOL	   tried2Connect=FALSE;

	IP[0]=0;	// Clear the IP Address
	if (IPout!=NULL) IPout[0]=0;
tryagain:
	nAdapter=0;
	gethostname( szHostname, sizeof( szHostname ));
	pHostEnt = gethostbyname( szHostname );
	while ( pHostEnt!=NULL && pHostEnt->h_addr_list[nAdapter] )      
	{
		// in case a device has multiple ethernet cards
		// i.e. 802.11, Bluetooth, USB-Cradle
		// we need to go though all pHostEnt->h_addr_list[nAdapter]
		tsim=(IN_ADDR *)pHostEnt->h_addr_list[nAdapter];
		if (tsim->S_un.S_un_b.s_b1==192 || 
			tsim->S_un.S_un_b.s_b1==169 || 
			tsim->S_un.S_un_b.s_b1==127 || 
			tsim->S_un.S_un_b.s_b1==255) 
			// If you want to make sure you have a real Internet
			// connection you cannot bet on IpAddresses starting with
			// 127 and 255. 192 and 169 are local IP addresses and
			// might be routed or proxied
			nAdapter++;      
		else
		{
			wsprintf(IP,TEXT("%d.%d.%d.%d"),
				tsim->S_un.S_un_b.s_b1,
				tsim->S_un.S_un_b.s_b2,
				tsim->S_un.S_un_b.s_b3,
				tsim->S_un.S_un_b.s_b4);
			if (IPout!=NULL)
				wsprintf(IPout,IP);
			break;
		}
	}

	// the next lines only work with Pocket PC Phone 
	// and Smartphone
#if (WIN32_PLATFORM_PSPC>300 || WIN32_PLATFORM_WFSP )
	// Pocket PC Phone Edition has set WIN32_PLATFORM_PSPC to 310
	if (IP[0]==0 && tried2Connect==FALSE)
	{
		CONNMGRCONNECTIONSTATUS	g_hConnMgrConnectionStatus = NULL;
		CONNMGRESTABLISHCONNECTION g_hConnMgrEstablishConnectionSync=NULL;
		// It is good practice to load the cellcore.dll
		// dynamically to be able to compile the code even for
		// older platforms
		HINSTANCE	hcellDll = LoadLibrary(TEXT("cellcore.dll"));
		if (hcellDll)
		{
			// We need the Status and a call to establish the 
			// connection
			g_hConnMgrConnectionStatus =(CONNMGRCONNECTIONSTATUS)GetProcAddress(hcellDll,TEXT("ConnMgrConnectionStatus")); 
			// The next line is just for debugging. You will have
			// to decide what you want to do if this call fails
			DWORD a=GetLastError();
			g_hConnMgrEstablishConnectionSync =(CONNMGRESTABLISHCONNECTION)GetProcAddress(hcellDll,TEXT("ConnMgrEstablishConnectionSync")); 
			a=GetLastError();

			// Here comes the main code:
			// First we check if we might have a connection
			DWORD  pdwStatus;
			(* g_hConnMgrConnectionStatus)(&g_phWebConnection,&pdwStatus);
			if (pdwStatus==CONNMGR_STATUS_CONNECTED)
			{
				//We are already connected!
				//This code should never run since we should
				//have a valid IP already.
				//If you still get here, you probably have
				//stale connection. 
			}
			else
			{
				//We are not connected, so lets try:
				//The CONNECTIONINFO is the structure that 
				//tells Connection Manager how we want
				//to connect
				CONNMGR_CONNECTIONINFO sConInfo;
				memset(&sConInfo,0, sizeof(CONNMGR_CONNECTIONINFO));
				sConInfo.cbSize=sizeof(CONNMGR_CONNECTIONINFO);
				// We want to use the guidDestNet parameter
				sConInfo.dwParams=CONNMGR_PARAM_GUIDDESTNET;
				// This is the highest data priority.
				sConInfo.dwPriority=CONNMGR_PRIORITY_USERINTERACTIVE;
				sConInfo.dwFlags=0;
				// Lets be nice and share the connection with
				// other applications
				sConInfo.bExclusive=FALSE;
				sConInfo.bDisabled=FALSE;
                GUID IID_DestNetInternet = {0x436ef144, 0xb4fb, 0x4863, 0xa0, 0x41, 0x8f, 0x90, 0x5a, 0x62, 0xc5, 0x72};
				sConInfo.guidDestNet=IID_DestNetInternet;  //连接Net网络
				//sConInfo.guidDestNet=IID_DestNetWAP;  //连接Wap网络

				// We want to wait until the connection was
				// established successful but not longer then
				// 60 seconds. You can use
				// ConnMgrEstablishConnection to establish
				// an asynchronous connection. 
				if ((*g_hConnMgrEstablishConnectionSync)(&sConInfo,&g_phWebConnection,60000,&pdwStatus)==S_OK)
				{
					//We are successfully connected!
					//Now lets try to get the new IP address?
					tried2Connect=TRUE;
					goto tryagain;
				}
				else
				{
					tried2Connect=FALSE;
					//Doh! Connection failed!
				}
			}
		}
	}
#endif
	return tried2Connect;
}

//////////////////////////////////////////////////////////////////////////
//函数：CloseConnection
//作用：释放网络连接
//////////////////////////////////////////////////////////////////////////
void CloseConnection()
{
	// 释放网络连接
	//close   connect   
	CONNMGRRELEASECONNECTION   g_hConnMgrReleaseConnection=NULL;   
	HINSTANCE   hcellDll   =   LoadLibrary(TEXT("cellcore.dll"));   

	if   (hcellDll)   
	{   
		//release   connection   
		g_hConnMgrReleaseConnection   =(CONNMGRRELEASECONNECTION )GetProcAddress(hcellDll,TEXT("ConnMgrReleaseConnection"));     
		(*g_hConnMgrReleaseConnection)(g_phWebConnection,   1);   
	}
}

#endif