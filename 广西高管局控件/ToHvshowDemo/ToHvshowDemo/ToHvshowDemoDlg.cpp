
// ToHvshowDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ToHvshowDemo.h"
#include "ToHvshowDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CToHvshowDemoDlg dialog




CToHvshowDemoDlg::CToHvshowDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CToHvshowDemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CToHvshowDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TOHVSHOWCTRL1, m_OcxToHwShow);
	DDX_Control(pDX, IDC_PCORG, m_originalpic);
	DDX_Control(pDX, IDC_PCFACE, m_facepic);
	DDX_Control(pDX, IDC_PCPLATE, m_platepic);
	DDX_Control(pDX, IDC_PCBIN, m_binpic);
	DDX_Control(pDX, IDC_TOHVSHOWCTRL2, m_OcxToHwShow1);
}

BEGIN_MESSAGE_MAP(CToHvshowDemoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_LOGIN, &CToHvshowDemoDlg::OnBnClickedLogin)
	ON_BN_CLICKED(IDC_GETSTATUS, &CToHvshowDemoDlg::OnBnClickedGetstatus)
	ON_BN_CLICKED(IDC_SETDEFPATH, &CToHvshowDemoDlg::OnBnClickedSetdefpath)
	ON_BN_CLICKED(IDC_LOGINOUT, &CToHvshowDemoDlg::OnBnClickedLoginout)
	ON_BN_CLICKED(IDC_GETPLATE, &CToHvshowDemoDlg::OnBnClickedGetplate)
	ON_BN_CLICKED(IDC_GETPLATECOLOR, &CToHvshowDemoDlg::OnBnClickedGetplatecolor)
	ON_BN_CLICKED(IDC_GETADDINFO, &CToHvshowDemoDlg::OnBnClickedGetaddinfo)
	ON_BN_CLICKED(IDC_SAVECARFULLPIC, &CToHvshowDemoDlg::OnBnClickedSavecarfullpic)
	ON_BN_CLICKED(IDC_SAVEFACEPIC, &CToHvshowDemoDlg::OnBnClickedSavefacepic)
	ON_BN_CLICKED(IDC_SAVEPLATEPIC, &CToHvshowDemoDlg::OnBnClickedSaveplatepic)
	ON_BN_CLICKED(IDC_SAVEPLATEBINPIC, &CToHvshowDemoDlg::OnBnClickedSaveplatebinpic)
	ON_BN_CLICKED(IDC_SAVEBINBMPPIC, &CToHvshowDemoDlg::OnBnClickedSavebinbmppic)
	ON_BN_CLICKED(IDC_GETSNAPTIME, &CToHvshowDemoDlg::OnBnClickedGetsnaptime)
	ON_BN_CLICKED(IDC_GETSAVEPICPATH, &CToHvshowDemoDlg::OnBnClickedGetsavepicpath)
	ON_BN_CLICKED(IDC_IFFULLPIC, &CToHvshowDemoDlg::OnBnClickedIffullpic)
	ON_BN_CLICKED(IDC_IFPLATEPIC, &CToHvshowDemoDlg::OnBnClickedIfplatepic)
	ON_BN_CLICKED(IDC_IFBINPIC, &CToHvshowDemoDlg::OnBnClickedIfbinpic)
	ON_BN_CLICKED(IDC_IFFACEPIC, &CToHvshowDemoDlg::OnBnClickedIffacepic)
	ON_BN_CLICKED(IDC_IFAUTOSAVE, &CToHvshowDemoDlg::OnBnClickedIfautosave)
	ON_BN_CLICKED(IDC_IFPRINTLOG, &CToHvshowDemoDlg::OnBnClickedIfprintlog)
	ON_BN_CLICKED(IDC_GETPLATECONTROL, &CToHvshowDemoDlg::OnBnClickedGetplatecontrol)
	ON_BN_CLICKED(IDC_GETFULLCONTROL, &CToHvshowDemoDlg::OnBnClickedGetfullcontrol)
	ON_BN_CLICKED(IDC_GETFACECONTROL, &CToHvshowDemoDlg::OnBnClickedGetfacecontrol)
	ON_BN_CLICKED(IDC_GETPLATENOCONTROL, &CToHvshowDemoDlg::OnBnClickedGetplatenocontrol)
	ON_BN_CLICKED(IDC_SETPLATECONTROL, &CToHvshowDemoDlg::OnBnClickedSetplatecontrol)
	ON_BN_CLICKED(IDC_SETFULLCONTROL, &CToHvshowDemoDlg::OnBnClickedSetfullcontrol)
	ON_BN_CLICKED(IDC_SETFACECONTROL, &CToHvshowDemoDlg::OnBnClickedSetfacecontrol)
	ON_BN_CLICKED(IDC_SETPLATENOCONTROL, &CToHvshowDemoDlg::OnBnClickedSetplatenocontrol)
END_MESSAGE_MAP()


// CToHvshowDemoDlg message handlers

BOOL CToHvshowDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	if(m_OcxToHwShow.GetRecvSnapImageFlag())
	{
		((CButton*)GetDlgItem(IDC_IFFULLPIC))->SetCheck(1);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_IFFULLPIC))->SetCheck(0);
	}

	if(m_OcxToHwShow.GetRecvPlateImageFlag())
	{
		((CButton*)GetDlgItem(IDC_IFPLATEPIC))->SetCheck(1);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_IFPLATEPIC))->SetCheck(0);
	}

	if(m_OcxToHwShow.GetRecvPlateBinImageFlag())
	{
		((CButton*)GetDlgItem(IDC_IFBINPIC))->SetCheck(1);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_IFBINPIC))->SetCheck(0);
	}

	if(m_OcxToHwShow.GetRecvSnapDriverImageFlag())
	{
		((CButton*)GetDlgItem(IDC_IFFACEPIC))->SetCheck(1);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_IFFACEPIC))->SetCheck(0);
	}

	if(m_OcxToHwShow.GetAutoSaveFlag())
	{
		((CButton*)GetDlgItem(IDC_IFAUTOSAVE))->SetCheck(1);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_IFAUTOSAVE))->SetCheck(0);
	}

	if(m_OcxToHwShow.GetLogFile())
	{
		((CButton*)GetDlgItem(IDC_IFPRINTLOG))->SetCheck(1);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_IFPRINTLOG))->SetCheck(0);
	}
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CToHvshowDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CToHvshowDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CString csSavePicPath = m_OcxToHwShow.GetImageFileName(0);
		Show_picture(m_originalpic,csSavePicPath);
		csSavePicPath = m_OcxToHwShow.GetImageFileName(3);
		Show_picture(m_facepic,csSavePicPath);
		csSavePicPath = m_OcxToHwShow.GetImageFileName(1);
		Show_picture(m_platepic,csSavePicPath);
		csSavePicPath = m_OcxToHwShow.GetImageFileName(4);
		Show_picture(m_binpic,csSavePicPath);
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CToHvshowDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CToHvshowDemoDlg::OnBnClickedLogin()
{
	// TODO: Add your control notification handler code here
	CString csIPAdd;
	GetDlgItemText(IDC_DEVICEADD1,csIPAdd);
	if(csIPAdd.GetLength() != 0)
	{
		m_OcxToHwShow1.ConnectTo(csIPAdd);
	}
	m_OcxToHwShow1.SetAutoSaveFlag(TRUE);
	m_OcxToHwShow1.SetRecvSnapImageFlag(TRUE);
	GetDlgItemText(IDC_DEVICEADD,csIPAdd);
	if(csIPAdd.GetLength() != 0)
	{
		m_OcxToHwShow.ConnectTo(csIPAdd);
	}
	SetDlgItemText(IDC_RESULT,_T("无反馈"));
}

void CToHvshowDemoDlg::OnBnClickedGetstatus()
{
	// TODO: Add your control notification handler code here
	if(m_OcxToHwShow.GetStatus() == 0)
	{
		SetDlgItemText(IDC_RESULT,_T("设备正常"));
		SetDlgItemText(IDC_STATUS,_T("设备正常"));
	}
	else
	{
		SetDlgItemText(IDC_RESULT,_T("设备异常"));
		SetDlgItemText(IDC_STATUS,_T("设备异常"));
	}
}

void CToHvshowDemoDlg::OnBnClickedSetdefpath()
{
	// TODO: Add your control notification handler code here
	CString csDefPath;
	GetDlgItemText(IDC_DEFPATH,csDefPath);
	m_OcxToHwShow.SetImgSavePath(csDefPath);
	SetDlgItemText(IDC_RESULT,_T("无反馈"));
}

void CToHvshowDemoDlg::OnBnClickedLoginout()
{
	// TODO: Add your control notification handler code here
	m_OcxToHwShow.Disconnect();
	m_OcxToHwShow1.Disconnect();
	SetDlgItemText(IDC_RESULT,_T("无反馈"));
}

void CToHvshowDemoDlg::OnBnClickedGetplate()
{
	// TODO: Add your control notification handler code here
	CString csPlate = m_OcxToHwShow.GetPlate();
	SetDlgItemText(IDC_PLATE,csPlate);
	SetDlgItemText(IDC_RESULT,_T("获取车牌号"));
}

void CToHvshowDemoDlg::OnBnClickedGetplatecolor()
{
	// TODO: Add your control notification handler code here
	CString csPlateColor = m_OcxToHwShow.GetPlateColor();
	SetDlgItemText(IDC_PLATECOLOR,csPlateColor);
	SetDlgItemText(IDC_RESULT,_T("获取车牌颜色"));
}

void CToHvshowDemoDlg::OnBnClickedGetaddinfo()
{
	// TODO: Add your control notification handler code here
	CString csAddInfo = m_OcxToHwShow.GetPlateInfo();
	SetDlgItemText(IDC_ADDINFO,csAddInfo);
	SetDlgItemText(IDC_RESULT,_T("获取附加信息"));
}

void CToHvshowDemoDlg::OnBnClickedSavecarfullpic()
{
	// TODO: Add your control notification handler code here
	CString csCarFullPath;
	GetDlgItemText(IDC_CARFULLPIC,csCarFullPath);
	if(m_OcxToHwShow.SaveSnapImage(csCarFullPath) == 0)
	{
		SetDlgItemText(IDC_RESULT,_T("保存成功"));
	}
	else
	{
		SetDlgItemText(IDC_RESULT,_T("保存失败"));
	}
}

void CToHvshowDemoDlg::OnBnClickedSavefacepic()
{
	// TODO: Add your control notification handler code here
	CString csFacePath;
	GetDlgItemText(IDC_FACEPIC,csFacePath);
	if(m_OcxToHwShow.SaveSnapImage2(csFacePath) == 0)
	{
		SetDlgItemText(IDC_RESULT,_T("保存成功"));
	}
	else
	{
		SetDlgItemText(IDC_RESULT,_T("保存失败"));
	}
}

void CToHvshowDemoDlg::OnBnClickedSaveplatepic()
{
	// TODO: Add your control notification handler code here
	CString csPlatePath;
	GetDlgItemText(IDC_PLATEPIC,csPlatePath);
	if(m_OcxToHwShow.SavePlateImage(csPlatePath) == 0)
	{
		SetDlgItemText(IDC_RESULT,_T("保存成功"));
	}
	else
	{
		SetDlgItemText(IDC_RESULT,_T("保存失败"));
	}
}

void CToHvshowDemoDlg::OnBnClickedSaveplatebinpic()
{
	// TODO: Add your control notification handler code here
	CString csPlateBinPath;
	GetDlgItemText(IDC_PLATEBINPIC,csPlateBinPath);
	if(m_OcxToHwShow.SavePlateBinImage(csPlateBinPath) == 0)
	{
		SetDlgItemText(IDC_RESULT,_T("保存成功"));
	}
	else
	{
		SetDlgItemText(IDC_RESULT,_T("保存失败"));
	}
}

void CToHvshowDemoDlg::OnBnClickedSavebinbmppic()
{
	// TODO: Add your control notification handler code here
	CString csPlateBinBMPPath;
	GetDlgItemText(IDC_BINBMPPIC,csPlateBinBMPPath);
	if(m_OcxToHwShow.SavePlateBin2BMP(csPlateBinBMPPath) == 0)
	{
		SetDlgItemText(IDC_RESULT,_T("保存成功"));
	}
	else
	{
		SetDlgItemText(IDC_RESULT,_T("保存失败"));
	}
}

void CToHvshowDemoDlg::OnBnClickedGetsnaptime()
{
	// TODO: Add your control notification handler code here
	LONG iType = -1;
	iType = GetDlgItemInt(IDC_SNAPTYPE);
	CString csSnapTime = m_OcxToHwShow.GetImageRecTime(iType);
	SetDlgItemText(IDC_SNAPTIME,csSnapTime);
	if(csSnapTime.GetLength() > 0)
	{
		SetDlgItemText(IDC_RESULT,_T("获取成功"));
	}
	else
	{
		SetDlgItemText(IDC_RESULT,_T("获取失败"));
	}
}

void CToHvshowDemoDlg::OnBnClickedGetsavepicpath()
{
	// TODO: Add your control notification handler code here
	LONG iType = -1;
	iType = GetDlgItemInt(IDC_SAVEPICTYPE);
	CString csSavePicPath = m_OcxToHwShow.GetImageFileName(iType);
	SetDlgItemText(IDC_SAVEPICPATH,csSavePicPath);
	SetDlgItemText(iType == 0?IDC_CARFULLPIC: \
							iType == 1?IDC_PLATEPIC: \
							iType == 2?IDC_PLATEBINPIC: \
							iType == 3?IDC_FACEPIC: \
							iType == 4?IDC_BINBMPPIC:IDC_SAVEPICPATH, \
							csSavePicPath);
	SetDlgItemText(IDC_RESULT,_T("无反馈"));
}

void CToHvshowDemoDlg::OnBnClickedIffullpic()
{
	// TODO: Add your control notification handler code here
	if(((CButton*)GetDlgItem(IDC_IFFULLPIC))->GetCheck() == 1)
	{
		m_OcxToHwShow.SetRecvSnapImageFlag(1);
	}
	else
	{
		m_OcxToHwShow.SetRecvSnapImageFlag(0);
	}
	SetDlgItemText(IDC_RESULT,_T("无反馈"));
}

void CToHvshowDemoDlg::OnBnClickedIfplatepic()
{
	// TODO: Add your control notification handler code here
	if(((CButton*)GetDlgItem(IDC_IFPLATEPIC))->GetCheck() == 1)
	{
		m_OcxToHwShow.SetRecvPlateImageFlag(1);
	}
	else
	{
		m_OcxToHwShow.SetRecvPlateImageFlag(0);
	}
	SetDlgItemText(IDC_RESULT,_T("无反馈"));
}

void CToHvshowDemoDlg::OnBnClickedIfbinpic()
{
	// TODO: Add your control notification handler code here
	if(((CButton*)GetDlgItem(IDC_IFBINPIC))->GetCheck() == 1)
	{
		m_OcxToHwShow.SetRecvPlateBinImageFlag(1);
	}
	else
	{
		m_OcxToHwShow.SetRecvPlateBinImageFlag(0);
	}
	SetDlgItemText(IDC_RESULT,_T("无反馈"));
}

void CToHvshowDemoDlg::OnBnClickedIffacepic()
{
	// TODO: Add your control notification handler code here
	if(((CButton*)GetDlgItem(IDC_IFBINPIC))->GetCheck() == 1)
	{
		m_OcxToHwShow.SetRecvSnapDriverImageFlag(1);
	}
	else
	{
		m_OcxToHwShow.SetRecvSnapDriverImageFlag(0);
	}
	SetDlgItemText(IDC_RESULT,_T("无反馈"));
}

void CToHvshowDemoDlg::OnBnClickedIfautosave()
{
	// TODO: Add your control notification handler code here
	if(((CButton*)GetDlgItem(IDC_IFAUTOSAVE))->GetCheck() == 1)
	{
		m_OcxToHwShow.SetAutoSaveFlag(TRUE);
	}
	else
	{
		m_OcxToHwShow.SetAutoSaveFlag(FALSE);
	}
	SetDlgItemText(IDC_RESULT,_T("无反馈"));
}

void CToHvshowDemoDlg::OnBnClickedIfprintlog()
{
	// TODO: Add your control notification handler code here
	if(((CButton*)GetDlgItem(IDC_IFPRINTLOG))->GetCheck() == 1)
	{
		m_OcxToHwShow.SetLogFile(1);
	}
	else
	{
		m_OcxToHwShow.SetLogFile(0);
	}
	SetDlgItemText(IDC_RESULT,_T("无反馈"));
}

void CToHvshowDemoDlg::OnBnClickedGetplatecontrol()
{
	// TODO: Add your control notification handler code here
	CString csPlateControl;
	csPlateControl = m_OcxToHwShow.GetShowPlateImage();
	SetDlgItemText(IDC_LBPLATECONTROL,csPlateControl);
	SetDlgItemText(IDC_RESULT,_T("无反馈"));
}

void CToHvshowDemoDlg::OnBnClickedGetfullcontrol()
{
	// TODO: Add your control notification handler code here
	CString csFullControl;
	csFullControl = m_OcxToHwShow.GetShowFirstImage();
	SetDlgItemText(IDC_LBFULLCONTROL,csFullControl);
	SetDlgItemText(IDC_RESULT,_T("无反馈"));
}

void CToHvshowDemoDlg::OnBnClickedGetfacecontrol()
{
	// TODO: Add your control notification handler code here
	CString csFaceControl;
	csFaceControl = m_OcxToHwShow.GetShowSecondImage();
	SetDlgItemText(IDC_LBFACECONTROL,csFaceControl);
	SetDlgItemText(IDC_RESULT,_T("无反馈"));
}

void CToHvshowDemoDlg::OnBnClickedGetplatenocontrol()
{
	// TODO: Add your control notification handler code here
	CString csPlateNoControl;
	csPlateNoControl = m_OcxToHwShow.GetShowPlateResult();
	SetDlgItemText(IDC_LBPLATENOCONTROL,csPlateNoControl);
	SetDlgItemText(IDC_RESULT,_T("无反馈"));
}

void CToHvshowDemoDlg::OnBnClickedSetplatecontrol()
{
	// TODO: Add your control notification handler code here
	CString csPlateControl;
	GetDlgItemText(IDC_EPLATECONTROL,csPlateControl);
	m_OcxToHwShow.SetShowPlateImage(csPlateControl);
	SetDlgItemText(IDC_RESULT,_T("无反馈"));
}

void CToHvshowDemoDlg::OnBnClickedSetfullcontrol()
{
	// TODO: Add your control notification handler code here
	CString csFullControl;
	GetDlgItemText(IDC_EFULLCONTROL,csFullControl);
	m_OcxToHwShow.SetShowFirstImage(csFullControl);
	SetDlgItemText(IDC_RESULT,_T("无反馈"));
}

void CToHvshowDemoDlg::OnBnClickedSetfacecontrol()
{
	// TODO: Add your control notification handler code here
	CString csFaceControl;
	GetDlgItemText(IDC_EFACECONTROL,csFaceControl);
	m_OcxToHwShow.SetShowSecondImage(csFaceControl);
	SetDlgItemText(IDC_RESULT,_T("无反馈"));
}

void CToHvshowDemoDlg::OnBnClickedSetplatenocontrol()
{
	// TODO: Add your control notification handler code here
	CString csPlateNoControl;
	GetDlgItemText(IDC_EPLATENOCONTROL,csPlateNoControl);
	m_OcxToHwShow.SetShowPlateResult(csPlateNoControl);
	SetDlgItemText(IDC_RESULT,_T("无反馈"));
}

//判断文件是否存在
BOOL   CToHvshowDemoDlg::IsFileExists(CString csPath)   
{   
	WIN32_FIND_DATA   wfd;   
	BOOL   bRet;   
	HANDLE   hFind;   
	hFind   =   FindFirstFile(csPath,   &wfd);   
	bRet   =   hFind   !=   INVALID_HANDLE_VALUE;   
	FindClose(hFind);   
	return   bRet;   
} 
//显示图片函数
void CToHvshowDemoDlg::Show_picture(CStatic &PicShow,CString csPath)
{
	CRect rect;
	PicShow.GetClientRect(&rect);     //m_picture为Picture Control控件变量，获得控件的区域对象
	if(!IsFileExists(csPath))        //判断图片路径是否存在
	{
		//PicShow.SetBitmap(NULL);
		//PicShow.Invalidate(FALSE);
		//PicShow.Invalidate(TRUE);
		CDC* pDC = PicShow.GetWindowDC();
		CBrush brush = GetSysColor(COLOR_BTNFACE);
		pDC->FillRect(&rect,&brush);
		ReleaseDC(pDC);
		return;
	}

	CImage image;       //使用图片类
	image.Load(csPath);   //装载路径下图片信息到图片类
	CDC* pDC = PicShow.GetWindowDC();    //获得显示控件的DC
	pDC->SetStretchBltMode(STRETCH_HALFTONE);//不失真
	image.Draw( pDC -> m_hDC,rect);      //图片类的图片绘制Draw函数
	ReleaseDC(pDC);
}


BEGIN_EVENTSINK_MAP(CToHvshowDemoDlg, CDialog)
	ON_EVENT(CToHvshowDemoDlg, IDC_TOHVSHOWCTRL1, 1, CToHvshowDemoDlg::OnReceivePlateTohvshowctrl1, VTS_NONE)
	ON_EVENT(CToHvshowDemoDlg, IDC_TOHVSHOWCTRL2, 1, CToHvshowDemoDlg::OnReceivePlateTohvshowctrl2, VTS_NONE)
END_EVENTSINK_MAP()

void CToHvshowDemoDlg::OnReceivePlateTohvshowctrl1()
{
	// TODO: Add your message handler code here
	SetDlgItemText(IDC_RESULT,_T("触发事件1激活"));
	CString csSavePicPath = m_OcxToHwShow.GetImageFileName(0);
	Show_picture(m_originalpic,csSavePicPath);
	csSavePicPath = m_OcxToHwShow.GetImageFileName(3);
	Show_picture(m_facepic,csSavePicPath);
	csSavePicPath = m_OcxToHwShow.GetImageFileName(1);
	Show_picture(m_platepic,csSavePicPath);
	csSavePicPath = m_OcxToHwShow.GetImageFileName(4);
	Show_picture(m_binpic,csSavePicPath);
	Sleep(600);
	SetDlgItemText(IDC_RESULT,_T("无反馈"));
}


void CToHvshowDemoDlg::OnReceivePlateTohvshowctrl2()
{
	// TODO: Add your message handler code here
	SetDlgItemText(IDC_RESULT,_T("触发事件2激活"));
	CString csSavePicPath = m_OcxToHwShow1.GetImageFileName(0);
	Show_picture(m_originalpic,csSavePicPath);
	csSavePicPath = m_OcxToHwShow1.GetImageFileName(3);
	Show_picture(m_facepic,csSavePicPath);
	csSavePicPath = m_OcxToHwShow1.GetImageFileName(1);
	Show_picture(m_platepic,csSavePicPath);
	csSavePicPath = m_OcxToHwShow1.GetImageFileName(4);
	Show_picture(m_binpic,csSavePicPath);
	Sleep(600);
	SetDlgItemText(IDC_RESULT,_T("无反馈"));
}
