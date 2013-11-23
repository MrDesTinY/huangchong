// OutPutDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ParkingDemo.h"
#include "ParkingDemoDlg.h"
#include "OutPutDlg.h"
#include "plaympeg4.h"
#include <gdiplus.h>
using namespace Gdiplus;



LONG lPort; //全局的播放库port号

void CALLBACK g_RealDataCallBack_V30(LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer,DWORD dwBufSize,void* dwUser)
{
	HWND hWnd=GetConsoleWindow();

	switch (dwDataType)
	{
	case NET_DVR_SYSHEAD: //系统头

		if (!PlayM4_GetPort(&lPort))  //获取播放库未使用的通道号
		{
			break;
		}
		//m_iPort = lPort; //第一次回调的是系统头，将获取的播放库port号赋值给全局port，下次回调数据时即使用此port号播放
		if (dwBufSize > 0)
		{
			if (!PlayM4_SetStreamOpenMode(lPort, STREAME_REALTIME))  //设置实时流播放模式
			{
				break;
			}
			if (!PlayM4_OpenStream(lPort, pBuffer, dwBufSize, 1024*1024)) //打开流接口
			{
				break;
			}
			if (!PlayM4_Play(lPort, hWnd)) //播放开始
			{
				break;
			}
		}
	case NET_DVR_STREAMDATA:   //码流数据
		if (dwBufSize > 0 && lPort != -1)
		{
			if (!PlayM4_InputData(lPort, pBuffer, dwBufSize))
			{
				break;
			} 
		}
	}
}


IMPLEMENT_DYNAMIC(COutPutDlg, CDialog)

COutPutDlg::COutPutDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COutPutDlg::IDD, pParent)
	, m_strLeftPlateNo(_T(""))
	, m_strRightPlateNo(_T(""))
	, m_iLeftPlaceNo(0)
	, m_iRightPlaceNo(0)
{
	m_bHasLeftVehicle = FALSE;
	m_bHasRightVehicle = FALSE;
	m_bSeleted = FALSE;
	m_iUserId = -1;
	m_iChanId = -1;
	m_lPlayHandle = -1;

	m_RedColor=RGB(255,0,0);        
	m_GreenColor=RGB(0,255,0);      
	m_BlackColor=RGB(255,255,255);
	m_RedBrush.CreateSolidBrush(m_RedColor);      
	m_GreenBrush.CreateSolidBrush(m_GreenColor);    
	
	g_DC = NULL;
	m_bCsDlg = FALSE;	

}

COutPutDlg::~COutPutDlg()
{
}

void COutPutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_LEFT_PLATE_NO, m_strLeftPlateNo);
	DDX_Text(pDX, IDC_STATIC_RIGHT_PLATE_NO, m_strRightPlateNo);
	DDX_Text(pDX, IDC_STATIC_LEFT_PLACE_NO, m_iLeftPlaceNo);
	DDX_Text(pDX, IDC_STATIC_RIGHT_PLACE_NO, m_iRightPlaceNo);
	DDX_Control(pDX, IDC_STATIC_PreView, m_Static_Pic);
}


BEGIN_MESSAGE_MAP(COutPutDlg, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_NCPAINT()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// COutPutDlg 消息处理程序
// 选中此画面
void COutPutDlg::SelectThis(void)
{
	m_bSeleted = TRUE;
	RefreshThis();
	return;
}

// 取消选中此画面
void COutPutDlg::CancelSelect(void)
{
	m_bSeleted = FALSE;
	RefreshThis();
	return;
}

// 在此画面播放
void COutPutDlg::PlayThis(int iUserId, int iChanId)
{
	CParkingDemoDlg *pDlg = (CParkingDemoDlg*)theApp.GetMainWnd();
	ASSERT(pDlg != NULL);

	if (iUserId < 0 || iChanId < 1 || m_lPlayHandle != -1)
	{
		return;
	}
	m_iUserId = iUserId;
	m_iChanId = iChanId;


	//启动预览并设置回调数据流	
	NET_DVR_CLIENTINFO ClientInfo = {0};
	ClientInfo.hPlayWnd = GetDlgItem(IDC_STATIC_PreView)->GetSafeHwnd();//NULL;         //需要SDK解码时句柄设为有效值，仅取流不解码时可设为空
	ClientInfo.lChannel     = iChanId;       //预览通道号
	ClientInfo.lLinkMode    = 1;        //最高位(31)为0表示主码流，为1表示子码流0～30位表示连接方式：0－TCP方式；1－UDP方式；2－多播方式；3－RTP方式;
	ClientInfo.sMultiCastIP = NULL;     //多播地址，需要多播预览时配置

	BOOL bPreviewBlock = false;         //请求码流过程是否阻塞，0：否，1：是
	m_lPlayHandle = NET_DVR_RealPlay_V30(m_iUserId, &ClientInfo, NULL, NULL, 0);
	if (m_lPlayHandle < 0)
	{
		//TRACE("NET_DVR_RealPlay_V30 error\n");
		NET_DVR_Logout(m_iUserId);
		NET_DVR_Cleanup();
		return;
	}
	

	//RefreshThis();
	return;
}

// 停止此画面播放
void COutPutDlg::StopThis(void)
{
	if (m_lPlayHandle == -1)
	{
		return;
	}
	if(NET_DVR_StopRealPlay(m_lPlayHandle))
	{
		m_lPlayHandle = -1;
		m_iUserId = -1;
		m_iChanId = -1;
		RefreshThis();
	}
	return;
}

// 强制刷新窗口
void COutPutDlg::RefreshThis(void)
{
	InvalidateRect(NULL);
	SendMessage(WM_NCPAINT,0,0);
}

void COutPutDlg::UpdateWindow(void)
{
	UpdateData(FALSE);
	//Invalidate();
	GetDlgItem(IDC_STATIC_LEFT_PLACE_NO)->Invalidate();
	GetDlgItem(IDC_STATIC_RIGHT_PLACE_NO)->Invalidate();
}

void COutPutDlg::ResetWindow(void)
{	
	m_bHasLeftVehicle = FALSE;
	m_bHasRightVehicle = FALSE;
	m_strLeftPlateNo = "";
	m_strRightPlateNo = "";
	UpdateWindow();
}

HBRUSH COutPutDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (pWnd->GetDlgCtrlID() == IDC_STATIC_LEFT_PLACE_NO)
	{		
		if (m_bHasLeftVehicle)
		{
			pDC->SetTextColor(m_GreenColor);
			pDC->SetBkColor(m_RedColor);
			hbr=(HBRUSH)m_RedBrush;//控件的填充颜色
		}
		else
		{
			pDC->SetTextColor(m_RedColor);
			pDC->SetBkColor(m_GreenColor);
			hbr=(HBRUSH)m_GreenBrush;//GetStockObject(LTGRAY_BRUSH);//控件的填充颜色
		}
		
	}
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_RIGHT_PLACE_NO)
	{		
		if (m_bHasRightVehicle)
		{
			pDC->SetTextColor(m_GreenColor);
			pDC->SetBkColor(m_RedColor);
			hbr=(HBRUSH)m_RedBrush;//控件的填充颜色
		}
		else
		{
			pDC->SetTextColor(m_RedColor);
			pDC->SetBkColor(m_GreenColor);
			hbr=(HBRUSH)m_GreenBrush;//控件的填充颜色
		}
	}
    //Invalidate(NULL);
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

BOOL COutPutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	g_DC = m_Static_Pic.GetDC();
	if (NULL == g_DC)
	{
		AfxMessageBox("获取DC失败");
		return FALSE;
	}
	m_Static_Pic.GetClientRect(&g_rect);
	m_bCsDlg = InitializeCriticalSectionAndSpinCount(&g_cs, 4000);
	if (FALSE == m_bCsDlg)
	{
		AfxMessageBox("初始化DC临界区失败") ;
		return FALSE;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void COutPutDlg::OnDestroy()
{
	CDialog::OnDestroy();

	if (NULL != g_DC)
	{
		ReleaseDC(g_DC);
	}
	
	if (m_bCsDlg)
	{
		DeleteCriticalSection(&g_cs);
		m_bCsDlg = FALSE;
	}

}

void COutPutDlg::DrawPicture(int iPicSize, unsigned char* pic) 
{
	DWORD iBufferSize = iPicSize;
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, iBufferSize);
	void* pData = GlobalLock(hGlobal);
	if (pData == NULL)
	{
		GlobalUnlock(hGlobal);
		GlobalFree(hGlobal);
		AfxMessageBox("全局内存分配失败");
		return;
	}

	memcpy(pData,pic,iBufferSize);
	GlobalUnlock(hGlobal);
	IStream* pStream = NULL;
	if (CreateStreamOnHGlobal(hGlobal, TRUE, &pStream) == S_OK)
	{
// 		CImage image;
// 		image.Load(pStream);
// 		image.Draw(g_DC->GetSafeHdc(), 0,0,g_rect.Width(),g_rect.Height());

		Image* image = NULL;
		image = Image::FromStream(pStream);
		if (NULL == image)
		{
			AfxMessageBox("初始化image失败");
			return;
		}
		Graphics graphics(g_DC->m_hDC);
		graphics.DrawImage(image,0,0,g_rect.Width(), g_rect.Height());

	}
	if (pStream != NULL)
	{ 
		pStream->Release();
	}
	GlobalFree(hGlobal);
}

void COutPutDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
}



void COutPutDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting	
	UpdateWindow();	
}