
// SccpTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SccpTest.h"
#include "SccpTestDlg.h"

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


// CSccpTestDlg dialog




CSccpTestDlg::CSccpTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSccpTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSccpTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SCCPCTRL1, m_ctlSccp);
}

BEGIN_MESSAGE_MAP(CSccpTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CSccpTestDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CSccpTestDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CSccpTestDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CSccpTestDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON8, &CSccpTestDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON6, &CSccpTestDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CSccpTestDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON5, &CSccpTestDlg::OnBnClickedButton5)
	ON_WM_CLOSE()
	ON_WM_MOVE()
	ON_BN_CLICKED(IDC_BTSETCODE, &CSccpTestDlg::OnBnClickedBtsetcode)
END_MESSAGE_MAP()


// CSccpTestDlg message handlers

BOOL CSccpTestDlg::OnInitDialog()
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
	m_isStart = false;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSccpTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSccpTestDlg::OnPaint()
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
HCURSOR CSccpTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSccpTestDlg::OnBnClickedButton1()
{
	m_ctlSccp.Config(1);
}

void CSccpTestDlg::OnBnClickedButton2()
{
	CString csIP;
	DWORD	dwPort;
	LONG	lRet = 0;

	GetDlgItemText(IDC_IPADDRESS1, csIP);
	dwPort = GetDlgItemInt(IDC_EDIT3);
	if(dwPort != 0 && 0!=strcmp((char*)(csIP.GetBuffer(0)),""))
	{
		lRet = m_ctlSccp.OpenNet(csIP, dwPort);
	}
	else
	{
		CString cstr = m_ctlSccp.GetIPAddr();
		long lport = m_ctlSccp.GetPort();
		lRet = m_ctlSccp.OpenNet(cstr,lport);
	}
	SetStatus(lRet);
	
}


void CSccpTestDlg::SetStatus(int iRet)
{
	if (iRet == 0xFFFF)
	{
		SetDlgItemText(IDC_EDIT_STATUS, _T("收到新的车牌"));
		return ;
	}
	CString strTmp;
	TCHAR	*szError[18]= {_T("正确"),_T("布防失败"),_T("设置报警回调函数失败"),_T("撤销布防失败"),_T("取消"),
						_T("错误"),_T("停止预览错误"),_T("登出设备失败"),_T("登录设备失败"),_T("设备没有登录"),_T("预览失败"),_T("无效参数"),_T("获取车牌信息失败"),
	_T("保存图片失败"),_T("没有图片"),_T("当前情况下不接受图片"),_T("抓拍失败"),_T("设备不正常")};

	if (iRet < 0)
	{
		strTmp.Format(_T("返回值 %d : %s"), iRet, szError[4 - iRet]);
	}
	else
	{
		strTmp.Format(_T("返回值 %d : %s"), iRet, szError[iRet]);
	}
	SetDlgItemText(IDC_EDIT_STATUS, strTmp);

}
void CSccpTestDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	LONG lRet;
	lRet = m_ctlSccp.Close();
	SetStatus(lRet);
}

void CSccpTestDlg::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here
	LONG	lRet;
	lRet = m_ctlSccp.SetUp(1);
	SetStatus(lRet);
}

void CSccpTestDlg::OnBnClickedButton8()
{
	// TODO: Add your control notification handler code here
	LONG  lRet;

	lRet = m_ctlSccp.GetStatus();
	SetStatus(lRet);
}

void CSccpTestDlg::OnBnClickedButton6()
{
	// TODO: Add your control notification handler code here
	LONG	lRet;

	lRet = m_ctlSccp.StarVideo();
m_isStart = true;
	SetStatus(lRet);

}

void CSccpTestDlg::OnBnClickedButton7()
{
	// TODO: Add your control notification handler code here
	LONG		lRet;

	lRet = m_ctlSccp.StopVideo();
	m_isStart = false;
	SetStatus(lRet);
}
BEGIN_EVENTSINK_MAP(CSccpTestDlg, CDialog)
	ON_EVENT(CSccpTestDlg, IDC_SCCPCTRL1, 1, CSccpTestDlg::OnPlateReceiveSccpctrl1, VTS_NONE)
END_EVENTSINK_MAP()

void CSccpTestDlg::OnPlateReceiveSccpctrl1()
{
	// TODO: Add your message handler code here
	SetStatus(0xFFFF);
	CString csPlateNum;
	CString csColor;
	CString csName;


	csPlateNum = m_ctlSccp.GetPlate();
	SetDlgItemText(IDC_EDIT_NUM, csPlateNum);
	csColor	  = m_ctlSccp.GetPlateColor();
	SetDlgItemText(IDC_EDIT_COLOR, csColor);
	csName = csPlateNum + _T("近景.jpg");

	m_ctlSccp.SaveImage(_T("./"), csName, 0, 0, 0, 0);
	csName = csPlateNum + _T("远景.jpg");
	m_ctlSccp.SaveImage(_T("./"), csName, 0, 0, 0, 1);
	csName = csPlateNum + _T("车牌.jpg");
	m_ctlSccp.SavePlateImage(_T("./"), csName);
	csName = csPlateNum + _T("二值.jpg");
	m_ctlSccp.SavePlateBinImage(_T("./"), csName);

}

void CSccpTestDlg::OnBnClickedButton5()
{
	// TODO: Add your control notification handler code here
	LONG  lRet;

	lRet = m_ctlSccp.Trigger();
	SetStatus(lRet);
	MessageBeep(MB_OK);
	Sleep(1000);
}

void CSccpTestDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	LONG lRet;
	lRet = m_ctlSccp.Close();
	SetStatus(lRet);

	CDialog::OnClose();
}

void CSccpTestDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
	if(m_isStart == true)
	{
		m_ctlSccp.StopVideo();
		m_ctlSccp.StarVideo();
	}

	// TODO: Add your message handler code here
}

void CSccpTestDlg::OnBnClickedBtsetcode()
{
	// TODO: Add your control notification handler code here
	CString csName;
	GetDlgItemText(IDC_EDCUSTOMCODE, csName);
	LONG lstate = m_ctlSccp.SetSFZ(csName);
	SetStatus(lstate);
}
