
// SyncTimeTestMFCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SyncTimeTestMFC.h"
#include "SyncTimeTestMFCDlg.h"
#include "VPRCtrlInterface.h"


typedef bool (CALLBACK *pfuncSyncTime)(EquType i_equType, LONG lUserID);
typedef bool (CALLBACK *pfuncVPR_CameraInit)(EquType i_equType,
					   int& io_uID,					
					   char * i_pchPort,
					   HWND i_hWndHandle,
					   UINT i_uMsg
					   );
typedef bool (CALLBACK *pfuncVPR_Init)(EquType i_equType);
typedef bool (CALLBACK *pfuncVPR_CameraQuit)(EquType i_equType,int *i_uID);
typedef bool (CALLBACK *pfuncVPR_Quit)(EquType i_equType);

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


// CSyncTimeTestMFCDlg dialog




CSyncTimeTestMFCDlg::CSyncTimeTestMFCDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSyncTimeTestMFCDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSyncTimeTestMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSyncTimeTestMFCDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CSyncTimeTestMFCDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CSyncTimeTestMFCDlg message handlers

BOOL CSyncTimeTestMFCDlg::OnInitDialog()
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

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSyncTimeTestMFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSyncTimeTestMFCDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSyncTimeTestMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




void CSyncTimeTestMFCDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	int iCameraID;
	EquType etCamere = HD976;
	HANDLE hOut;
	char chCameraPort[255] = "10.100.40.22:8000<admin,12345>";
	HINSTANCE hInstance=LoadLibrary("VPRCtrl.dll");
	if(!hInstance)
	{
		printf("载入动态库失败！！！");
		return;
	}
	pfuncVPR_CameraInit pfuncvc = (pfuncVPR_CameraInit)GetProcAddress(hInstance,"VPR_CameraInit");
	if(!pfuncvc)
	{
		printf("找不到函数符号pfuncSyncTime！！！");
		return;
	}
	pfuncSyncTime pfuncst = (pfuncSyncTime)GetProcAddress(hInstance,"DEV_SyncTime");
	if(!pfuncst)
	{
		printf("找不到函数符号pfuncSyncTime！！！");
		return;
	}
	pfuncVPR_Init pfuncvi = (pfuncVPR_Init)GetProcAddress(hInstance,"VPR_Init");
	if(!pfuncvi)
	{
		printf("找不到函数符号pfuncVPR_Init！！！");
		return;
	}
	pfuncVPR_Quit pfuncvq = (pfuncVPR_Quit)GetProcAddress(hInstance,"VPR_Quit");
	if(!pfuncvq)
	{
		printf("找不到函数符号pfuncVPR_Quit！！！");
		return;
	}
	pfuncVPR_CameraQuit pfuncvcq = (pfuncVPR_CameraQuit)GetProcAddress(hInstance,"VPR_CameraQuit");
	if(!pfuncvcq)
	{
		printf("找不到函数符号pfuncVPR_CameraQuit！！！");
		return;
	}
	//if(!pfuncvi(etCamere))
	//{
	//	printf("VPR初始化失败！！！");
	//	return;
	//}
	if(!pfuncvc(etCamere,iCameraID,chCameraPort,(HWND)0x99999,0))
	{
		printf("相机初始化失败！！！");
		return;
	}
	if(!pfuncst(etCamere,iCameraID))
	{
		printf("相机校时失败！！！");
		return;
	}
	if(!pfuncvcq(etCamere,&iCameraID))
	{
		printf("相机退出失败！！！");
		return;
	}
	if(!pfuncvq(etCamere))
	{
		printf("VPR退出失败！！！");
		return;
	}
	FreeLibrary(hInstance);
}
