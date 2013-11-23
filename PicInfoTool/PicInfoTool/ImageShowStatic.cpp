// ImageShowStatic.cpp : implementation file
//

#include "stdafx.h"
//#include "Dra_Bmptest.h"
#include "ImageShowStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#include "ImageDataManager.h"

/////////////////////////////////////////////////////////////////////////////
// CImageShowStatic

CImageShowStatic::CImageShowStatic()
{
	m_pImageData = NULL;
	int   nColors   =   (1 << 24); 
	m_pBmpInfo   =   (BITMAPINFO*)new BYTE[sizeof(BITMAPINFOHEADER)   +  nColors*sizeof(RGBQUAD)];
}

CImageShowStatic::~CImageShowStatic()
{	
	m_pImageData = NULL;
	delete [] m_pBmpInfo;
	m_pBmpInfo = NULL;
}


BEGIN_MESSAGE_MAP(CImageShowStatic, CStatic)
	//{{AFX_MSG_MAP(CImageShowStatic)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageShowStatic message handlers

void CImageShowStatic::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// 进入这里后，先擦出原数据显示，再根据图像数据指针画图像显示
	CRect rect1;
	GetClientRect(rect1);
	/*******************************************************/
	//绘制背景色 congjianting 7.06
	COLORREF  color = GetSysColor(COLOR_3DFACE); //system color
	HBRUSH hBrush=::CreateSolidBrush( color/*RGB(255, 255, 255)*/ );
	::FillRect(dc.m_hDC, rect1, hBrush);
	DeleteObject(hBrush);
	/*******************************************************/


	//CImageDataManager *pImageData = (CImageDataManager *)GetImagePointer();

	if (m_pImageData!=NULL && m_iImageWidth > 0 && m_iImageHeight > 0)
	{
		// 定义BITMAP头信息

		memset(m_pBmpInfo,0,sizeof(BITMAPINFO));
		//BITMAPINFO   *pBmpInfo   =   NULL; 
		//pBmpInfo   =   (BITMAPINFO*) new   BYTE[sizeof(BITMAPINFOHEADER)   +  nColors*sizeof(RGBQUAD)];
		
		m_pBmpInfo->bmiHeader.biSize   =   sizeof(BITMAPINFOHEADER); 
		m_pBmpInfo->bmiHeader.biWidth   =   m_iImageWidth; 
		m_pBmpInfo->bmiHeader.biHeight   =   (-1)*m_iImageHeight; 
		m_pBmpInfo->bmiHeader.biPlanes   =   1; 
		m_pBmpInfo->bmiHeader.biBitCount   =   24;       //*位色 
		m_pBmpInfo->bmiHeader.biCompression   =   BI_RGB; 
		m_pBmpInfo->bmiHeader.biSizeImage   =   0; //The size, in bytes, of the image. This may be set to zero for BI_RGB bitmaps.
		m_pBmpInfo->bmiHeader.biXPelsPerMeter   =   dc.GetDeviceCaps(LOGPIXELSX); 
		m_pBmpInfo->bmiHeader.biYPelsPerMeter   =   dc.GetDeviceCaps(LOGPIXELSY);
		m_pBmpInfo->bmiHeader.biClrUsed   =   0; //If this value is zero,
		//the bitmap uses the maximum number of colors corresponding to the value of the biBitCount member for the compression mode specified by biCompression.
		m_pBmpInfo->bmiHeader.biClrImportant   =   0; 

		//double dHWRatio = m_iImageHeight/(m_iImageWidth*1.0);

		::SetStretchBltMode(dc.m_hDC, COLORONCOLOR);
		::StretchDIBits( dc.m_hDC, 0, 0, 
						  rect1.Width(), rect1.Height(), 0, 0,
						  m_iImageWidth, m_iImageHeight,
						  m_pImageData,
						  m_pBmpInfo,
						  DIB_RGB_COLORS,
						  SRCCOPY
						);
		//if()

	}
	else //如果图像数据指针为空，则擦出图像区域
	{
		// do nothing
	}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 显示一幅图像
// 	HBITMAP hBitmap;
// 	hBitmap =  (HBITMAP)::LoadImage( AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP1),IMAGE_BITMAP,0,0, 
// 		                             LR_CREATEDIBSECTION ); // IDB_BMP_BK是位图在RC里的ID
// 	ModifyStyle(0, BS_OWNERDRAW);  // 修改CStatic样式，改成自绘制，这一步很重要
// 	
// 	CRect rc; 
// 	GetClientRect(&rc); 
// 	
// 	BITMAP bm; // 位图结构体，用于接受位图的信息
// 	HDC hdcImage = CreateCompatibleDC(NULL); // 创建兼容DC
// 	SelectObject(hdcImage, hBitmap); // 将加载进来的位图句柄选入兼容DC
// 	GetObject(hBitmap, sizeof(bm), &bm);  // 将位图信息填入BITMAP结构体
// 	
// 	StretchBlt( GetDC()->GetSafeHdc(),rc.left,rc.top,rc.Width(),rc.Height(),hdcImage,       
// 		        0,0,bm.bmWidth,bm.bmHeight,SRCCOPY ); // 拷贝位图到CStatic
// 	DeleteObject(hBitmap);
//     DeleteDC(hdcImage);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	// Do not call CStatic::OnPaint() for painting messages
}

void CImageShowStatic::OnDraw()
{
	OnPaint();
}
// 设置图像数据管理类实例指针函数
void  CImageShowStatic::SetImagePointer(unsigned char  *pImage,int width,int height) 
{
	m_pImageData = pImage;
	m_iImageWidth = width;
	m_iImageHeight = height;
}

// 获取图像数据管理类实例指针函数
unsigned char*  CImageShowStatic::GetImagePointer()    
{
	return m_pImageData;
}       