
// NTPSyncDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NTPSyncDemo.h"
#include "NTPSyncDemoDlg.h"

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


// CNTPSyncDemoDlg dialog




CNTPSyncDemoDlg::CNTPSyncDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNTPSyncDemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNTPSyncDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_lbListBox);
}

BEGIN_MESSAGE_MAP(CNTPSyncDemoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CNTPSyncDemoDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CNTPSyncDemoDlg::OnBnClickedCancel)
	ON_MESSAGE(WM_NC,&CNTPSyncDemoDlg::OnNotifyIcon)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDCANCEL2, &CNTPSyncDemoDlg::OnBnClickedCancel2)
	ON_BN_CLICKED(IDCANCEL3, &CNTPSyncDemoDlg::OnBnClickedCancel3)
END_MESSAGE_MAP()


// CNTPSyncDemoDlg message handlers

BOOL CNTPSyncDemoDlg::OnInitDialog()
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
	CString strPath;
	GetModuleFileName(NULL/*AfxGetInstanceHandle()*/,strPath.GetBuffer(MAX_PATH+1),MAX_PATH); 
	strPath.ReleaseBuffer();
	int istrPathIndex = strPath.ReverseFind('\\');
	strPath.Delete(istrPathIndex,strPath.GetLength()-istrPathIndex);
	strPath.Append("\\Config.ini");
	m_pConfig = new CConfig(strPath);
	CString chIP = m_pConfig->ReadConfig("Setting","ServerIP","127.0.0.1");
	int iPort = m_pConfig->ReadConfig("Setting","ServerPort",50000);
	int iInterval = m_pConfig->ReadConfig("Setting","Interval",10);
	int iStatus = m_pConfig->ReadConfig("Setting","AutoRun",0);
	bool bStatus = (iStatus == 0?false:true);

	SetDlgItemText(IDC_EDIT1,chIP);
	SetDlgItemInt(IDC_EDIT2,iPort);
	SetDlgItemInt(IDC_EDIT3,iInterval);
	if(bStatus)
	{
		((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(1);
		RegisterRunKey();
	}
	else
	{
		((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(0);
		UnRegisterRunKey();
	}

	m_clsNTPClient.SetParams(chIP.GetBuffer(0), iPort, iInterval);
	m_clsNTPClient.Start(&m_lbListBox);
	m_status = TRUE;
	//PostMessage(SC_MINIMIZE);
	SetTimer(1,55,NULL);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CNTPSyncDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else if ((nID & 0xFFF0) == SC_MINIMIZE)
	{
		ToTray();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CNTPSyncDemoDlg::OnPaint()
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
HCURSOR CNTPSyncDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CNTPSyncDemoDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();


	char chIP[20];// = m_pConfig->ReadConfig("Setting","ServerIP","127.0.0.1");
	int iPort;// = m_pConfig->ReadConfig("Setting","ServerPort",50000);
	int iInterval;// = m_pConfig->ReadConfig("Setting","Interval",10);
	int iStatus;

	memset(chIP,0,sizeof(chIP));
	GetDlgItemText(IDC_EDIT1,chIP,sizeof(chIP));
	iPort = GetDlgItemInt(IDC_EDIT2);
	iInterval = GetDlgItemInt(IDC_EDIT3);
	iStatus = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck();

	m_pConfig->WriteConfig("Setting","ServerIP",chIP);
	char ch[20] = {0};
	_itoa_s(iPort,ch,10);
	m_pConfig->WriteConfig("Setting","ServerPort",ch);
	memset(ch,0,sizeof(ch));
	_itoa_s(iInterval,ch,10);
	m_pConfig->WriteConfig("Setting","Interval",ch);
	memset(ch,0,sizeof(ch));
	_itoa_s(iStatus,ch,10);
	m_pConfig->WriteConfig("Setting","AutoRun",ch);
	if(iStatus == 1)
	{
		RegisterRunKey();
	}
	else
	{
		UnRegisterRunKey();
	}
	m_clsNTPClient.SetParams(chIP, iPort, iInterval);
	//m_clsNTPClient.Start(&m_lbListBox);
}

void CNTPSyncDemoDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	m_clsNTPClient.Stop();
	Shell_NotifyIcon(NIM_DELETE, &m_NotifyIcon);
	OnCancel();
}

void CNTPSyncDemoDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	m_clsNTPClient.Stop();
	Shell_NotifyIcon(NIM_DELETE, &m_NotifyIcon);
	CDialog::OnClose();
}


LRESULT CNTPSyncDemoDlg::OnNotifyIcon(WPARAM wParam,LPARAM lParam)
{
	if (lParam == WM_LBUTTONUP)
	{ 
		// 左键单击弹出主界面
		ModifyStyleEx(0,WS_EX_TOPMOST);
		ShowWindow(SW_SHOW);
		Shell_NotifyIcon(NIM_DELETE, &m_NotifyIcon);
	}
	else if (lParam == WM_RBUTTONDOWN)
	{
		//右键单击弹出主菜单
	}
	return 0;
}


BOOL CNTPSyncDemoDlg::RegisterRunKey()
{
	CString   str,strPath;   
	HKEY   hRegKey;       
	str=_T("Software\\Microsoft\\Windows\\CurrentVersion\\Run"); //开机启动的注册表路径....... 

	//获得程序自己的路径........ 
	GetModuleFileName(NULL/*AfxGetInstanceHandle()*/,strPath.GetBuffer(MAX_PATH+1),MAX_PATH); 
	strPath.ReleaseBuffer(); 
	//AfxMessageBox(strPath);

	//打开注册表........ 
	if(RegOpenKey(HKEY_LOCAL_MACHINE,str,&hRegKey)!=ERROR_SUCCESS)      
	{
		return FALSE;    
	} 
	else    
	{    
		_splitpath_s(strPath.GetBuffer(0),NULL,0,NULL,0,str.GetBufferSetLength(MAX_PATH+1),MAX_PATH+1,NULL,0);       
		strPath.ReleaseBuffer();    
		str.ReleaseBuffer();   

		if(::RegSetValueEx(hRegKey,str,0,REG_SZ,  
			(CONST BYTE*)strPath.GetBuffer(0),strPath.GetLength())!=ERROR_SUCCESS)    
			return FALSE;    
		else           
			strPath.ReleaseBuffer();    
	}   
	return true; 
}

BOOL CNTPSyncDemoDlg::UnRegisterRunKey()
{
	CString   str,strPath;   
	HKEY   hRegKey;       
	str=_T("Software\\Microsoft\\Windows\\CurrentVersion\\Run"); //开机启动的注册表路径....... 

	//获得程序自己的路径........ 
	GetModuleFileName(NULL/*AfxGetInstanceHandle()*/,strPath.GetBuffer(MAX_PATH+1),MAX_PATH); 
	strPath.ReleaseBuffer(); 
	//AfxMessageBox(strPath);

	//打开注册表........ 
	if(RegOpenKey(HKEY_LOCAL_MACHINE,str,&hRegKey)!=ERROR_SUCCESS)      
	{
		return FALSE;    
	}
	else    
	{    
		_splitpath_s(strPath.GetBuffer(0),NULL,0,NULL,0,str.GetBufferSetLength(MAX_PATH+1),MAX_PATH+1,NULL,0);    
		strPath.ReleaseBuffer();    
		str.ReleaseBuffer();   
		
		if(::RegDeleteValue(hRegKey,str)!=ERROR_SUCCESS)    
			return FALSE;    
		else           
			strPath.ReleaseBuffer();    
	}   
	return true; 
}


void CNTPSyncDemoDlg::ToTray()
{
	m_NotifyIcon.cbSize=sizeof(NOTIFYICONDATA);
	m_NotifyIcon.hIcon=AfxGetApp()->LoadIcon(IDR_MAINFRAME);    // 装载程序图标
	m_NotifyIcon.hWnd=m_hWnd;
	lstrcpy(m_NotifyIcon.szTip,"NTP校时程序");            // 这里是鼠标移到托盘图标时的提示信息
	m_NotifyIcon.uCallbackMessage=WM_NC;
	m_NotifyIcon.uFlags=NIF_ICON | NIF_MESSAGE | NIF_TIP;
	Shell_NotifyIcon(NIM_ADD,&m_NotifyIcon);    // 添加托盘图标
	ShowWindow(SW_HIDE);
}

void CNTPSyncDemoDlg::OnTimer(UINT nIDEvent)
{
	if(nIDEvent == 1)
	{
		KillTimer(1);
		ToTray();
	}
}
void CNTPSyncDemoDlg::OnBnClickedCancel2()
{
	// TODO: Add your control notification handler code here
	if(m_status)
	{
		return;
	}
	else
	{
		m_clsNTPClient.Start(&m_lbListBox);
		m_status = TRUE;
	}
}

void CNTPSyncDemoDlg::OnBnClickedCancel3()
{
	// TODO: Add your control notification handler code here
	if(!m_status)
	{
		return;
	}
	else
	{
		m_clsNTPClient.Stop();
		m_status = FALSE;
	}
	
}
