// AuthCodeDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WMLF.h"
#include "AuthCodeDlg.h"
#include <initguid.h>
#include <Imaging.h>

#ifdef WIN32_PLATFORM_WFSP
#include <tpcshell.h>
#endif
// CAuthCodeDlg �Ի���

IMPLEMENT_DYNAMIC(CAuthCodeDlg, CDialog)

CAuthCodeDlg::CAuthCodeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAuthCodeDlg::IDD, pParent)
	, m_verfy(NULL)
	, m_strInputAuthCode(_T(""))
{

}

CAuthCodeDlg::~CAuthCodeDlg()
{
}

void CAuthCodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_AUTH_TEXT, m_stAuthText);
	DDX_Text(pDX, IDC_EDIT_INPUT_AUTH_CODE, m_strInputAuthCode);
}


BEGIN_MESSAGE_MAP(CAuthCodeDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CAuthCodeDlg ��Ϣ�������

BOOL CAuthCodeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	if (!m_dlgCommandBar.Create(this) ||
		!m_dlgCommandBar.InsertMenuBar(IDR_MENU_AUTH_CODE))
	{
		TRACE0("δ�ܴ��� CommandBar\n");
		return FALSE;      // δ�ܴ���
	}
#ifdef WIN32_PLATFORM_WFSP
    //��д���˼�������WM_HOTKEY��Ϣ
    (void)::SendMessage(SHFindMenuBar (m_hWnd), SHCMBM_OVERRIDEKEY, VK_TBACK,
        MAKELPARAM(SHMBOF_NODEFAULT | SHMBOF_NOTIFY,
        SHMBOF_NODEFAULT | SHMBOF_NOTIFY));
#endif

	m_stAuthText.SetWindowText(ConvertUtf8ToUtf16(m_verfy->text));

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}



void CAuthCodeDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()

	LPVOID pvData = NULL;
	//�ڶ��Ϸ���ռ�
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, m_verfy->pic_len);   
	_ASSERTE(NULL != hGlobal);
	//�����ѿռ�,��ȡָ��ѿռ��ָ��
	pvData = GlobalLock(hGlobal);
	_ASSERTE(NULL != pvData);   
	//���Ƶ�����
	memcpy(pvData, m_verfy->pic, m_verfy->pic_len);
	GlobalUnlock(hGlobal);   
	LPSTREAM pstm = NULL;   
	//�ڶ��д���������
	HRESULT hr = CreateStreamOnHGlobal(hGlobal, TRUE, &pstm);   
	_ASSERTE(SUCCEEDED(hr) && pstm);   

	IImagingFactory *pImgFactory = NULL;
	IImage *pImage = NULL;  
	CoInitializeEx(NULL, COINIT_MULTITHREADED); 
	HBITMAP hResult =0;  
	if(SUCCEEDED(CoCreateInstance(CLSID_ImagingFactory,  
									NULL,  
									CLSCTX_INPROC_SERVER,  
									IID_IImagingFactory,  
									(void **)&pImgFactory)))  
	{  
		ImageInfo imageInfo;  
		//ֱ�ӽ������ݴ���ȥ
		if(SUCCEEDED(pImgFactory->CreateImageFromStream(pstm,&pImage))  
		&& SUCCEEDED(pImage->GetImageInfo(&imageInfo)))  
		{  
			CDC dcBitmap;  
			dcBitmap.CreateCompatibleDC(&dc);  
			hResult = CreateCompatibleBitmap(dc.GetSafeHdc(),imageInfo.Width,imageInfo.Height);  
			if(hResult)
			{  
				HGDIOBJ   hOldBitmap = dcBitmap.SelectObject(hResult);  
				pImage->Draw(dcBitmap.GetSafeHdc(),CRect(0,0,imageInfo.Width,imageInfo.Height),NULL);  

				int iMargin = DRA::SCALEX(5);
			    int xAUTH_CODE_PIC = iMargin;
				int yAUTH_CODE_PIC = iMargin;

				dc.BitBlt( xAUTH_CODE_PIC, yAUTH_CODE_PIC, imageInfo.Width, imageInfo.Height, &dcBitmap, 0, 0, SRCCOPY );
				dcBitmap.SelectObject(hOldBitmap);  
			}  
			pImage->Release();  
		} 
		pImgFactory->Release();  
	}
	CoUninitialize(); 

	GlobalFree(hGlobal); //�ͷ�ȫ���ڴ�
}

LRESULT CAuthCodeDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_CLOSE:
		break;
#ifdef WIN32_PLATFORM_WFSP
		//�ı���˼���Ϊ
	case WM_HOTKEY:
		if ((VK_TBACK == HIWORD(lParam)) && (MOD_KEYUP == LOWORD(lParam)))
		{
			SHSendBackToFocusWindow(message, wParam, lParam);
		}
		break;
#endif
	default:
		break;
	}

	return CDialog::WindowProc(message, wParam, lParam);
}


void CAuthCodeDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
    UpdateData();
	if(m_strInputAuthCode.IsEmpty())
	{
		return;
	}

	CDialog::OnOK();
}

void CAuthCodeDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
    int xIDC_STATIC_AUTH_TEXT, yIDC_STATIC_AUTH_TEXT, wIDC_STATIC_AUTH_TEXT, hIDC_STATIC_AUTH_TEXT;
    int xIDC_EDIT_INPUT_AUTH_CODE, yIDC_EDIT_INPUT_AUTH_CODE, wIDC_EDIT_INPUT_AUTH_CODE, hIDC_EDIT_INPUT_AUTH_CODE;
    int iHeight, iWidth;
    int iMargin = DRA::SCALEX(5);
    RECT rcCtl;
    ::GetClientRect(this->m_hWnd, &rcCtl);
    
    iHeight = rcCtl.bottom - rcCtl.top;
    iWidth = rcCtl.right - rcCtl.left;

	xIDC_STATIC_AUTH_TEXT = iMargin;
    yIDC_STATIC_AUTH_TEXT = (iHeight - iMargin * 4) / 3 + iMargin * 2;
    wIDC_STATIC_AUTH_TEXT = iWidth - iMargin * 2;
    hIDC_STATIC_AUTH_TEXT = (iHeight - iMargin * 4) / 3;

	xIDC_EDIT_INPUT_AUTH_CODE = iMargin;
    yIDC_EDIT_INPUT_AUTH_CODE = iHeight - (iHeight - iMargin * 4) / 3 - iMargin;
    wIDC_EDIT_INPUT_AUTH_CODE = DRA::SCALEX(100);
    hIDC_EDIT_INPUT_AUTH_CODE = DRA::SCALEX(20);

	HWND hwndctl = ::GetDlgItem(this->m_hWnd, IDC_STATIC_AUTH_TEXT );
    ::MoveWindow(hwndctl, xIDC_STATIC_AUTH_TEXT, yIDC_STATIC_AUTH_TEXT , wIDC_STATIC_AUTH_TEXT , hIDC_STATIC_AUTH_TEXT , false);
    
    hwndctl = ::GetDlgItem(this->m_hWnd, IDC_EDIT_INPUT_AUTH_CODE);
    ::MoveWindow(hwndctl, xIDC_EDIT_INPUT_AUTH_CODE, yIDC_EDIT_INPUT_AUTH_CODE, wIDC_EDIT_INPUT_AUTH_CODE, hIDC_EDIT_INPUT_AUTH_CODE, false);
}
