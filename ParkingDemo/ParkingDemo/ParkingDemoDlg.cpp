
// ParkingDemoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ParkingDemo.h"
#include "ParkingDemoDlg.h"
#include "AddDeviceDlg.h"
#include "IPAccessCfgDlg.h"
#include "PlaceInfoDlg.h"
#include "plaympeg4.h"
#include <ws2tcpip.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void CALLBACK MessageCallback(LONG lCommand, NET_DVR_ALARMER *pAlarmer, char *pAlarmInfo, DWORD dwBufLen, void* pUser)
{
	UNREFERENCED_PARAMETER(pUser);		
	
	CParkingDemoDlg *pDlg = (CParkingDemoDlg*)theApp.GetMainWnd();
	ASSERT(pDlg != NULL);

	CGuard lock(&pDlg->m_csLock);	

	int i = 0;
	int iDeviceIndex = -1;
	int iDeviceNum = pDlg->m_vDevices.size();
	for (i=0; i<iDeviceNum; i++)
	{ 
		if (strcmp(pDlg->m_vDevices[i].szDVRIP, pAlarmer->sDeviceIP) == 0)
		{
			iDeviceIndex = i;
			break;
		}
	}	
	if (iDeviceIndex < 0)
	{
		return;
	}

	try
	{
		char *pAlarmMsg = new char[dwBufLen];
		if (pAlarmMsg == NULL)
		{
			return;
		}
		memcpy(pAlarmMsg, pAlarmInfo, dwBufLen);

		CString strPlateNO;
		int iPicType = 0;
		if (COMM_ITS_PARK_VEHICLE == lCommand)
		{
			memcpy(((LPNET_ITS_PARK_VEHICLE)pAlarmMsg)->struPlateInfo.sLicense, ((LPNET_ITS_PARK_VEHICLE)pAlarmInfo)->struPlateInfo.sLicense, MAX_LICENSE_LEN);
			((LPNET_ITS_PARK_VEHICLE)pAlarmMsg)->dwChanIndex = ((LPNET_ITS_PARK_VEHICLE)pAlarmInfo)->dwChanIndex;
			((LPNET_ITS_PARK_VEHICLE)pAlarmMsg)->byLocationStatus = ((LPNET_ITS_PARK_VEHICLE)pAlarmInfo)->byLocationStatus;
			((LPNET_ITS_PARK_VEHICLE)pAlarmMsg)->bylogicalLaneNum = ((LPNET_ITS_PARK_VEHICLE)pAlarmInfo)->bylogicalLaneNum;

			strPlateNO.Format("%s",((LPNET_ITS_PARK_VEHICLE)pAlarmMsg)->struPlateInfo.sLicense );	
			if (strPlateNO == "无车牌" || strPlateNO == "" )
			{
				iPicType = ((LPNET_ITS_PARK_VEHICLE)pAlarmMsg)->struPicInfo[0].byType;
				if (iPicType == 1)
				{
					((LPNET_ITS_PARK_VEHICLE)pAlarmMsg)->struPicInfo[0].dwDataLen = ((LPNET_ITS_PARK_VEHICLE)pAlarmInfo)->struPicInfo[0].dwDataLen;
					int iPicSize0 = ((LPNET_ITS_PARK_VEHICLE)pAlarmMsg)->struPicInfo[0].dwDataLen;
					memcpy(((LPNET_ITS_PARK_VEHICLE)pAlarmMsg)->struPicInfo[0].pBuffer, ((LPNET_ITS_PARK_VEHICLE)pAlarmInfo)->struPicInfo[0].pBuffer, iPicSize0);
					if (pDlg->m_checkPicData)
					{
						int iChanIndex = ((LPNET_ITS_PARK_VEHICLE)pAlarmMsg)->dwChanIndex;
						CGuard lockstatic(&pDlg->m_csLock);	
						pDlg->DrawWindowPicture(iChanIndex-1,iPicSize0,((LPNET_ITS_PARK_VEHICLE)pAlarmMsg)->struPicInfo[0].pBuffer);
					}
				}
			}			
			else
			{		
				iPicType = ((LPNET_ITS_PARK_VEHICLE)pAlarmMsg)->struPicInfo[1].byType;
				if (iPicType == 1)
				{
					((LPNET_ITS_PARK_VEHICLE)pAlarmMsg)->struPicInfo[1].dwDataLen = ((LPNET_ITS_PARK_VEHICLE)pAlarmInfo)->struPicInfo[1].dwDataLen;
					int iPicSize1 = ((LPNET_ITS_PARK_VEHICLE)pAlarmMsg)->struPicInfo[1].dwDataLen;
					memcpy(((LPNET_ITS_PARK_VEHICLE)pAlarmMsg)->struPicInfo[1].pBuffer, ((LPNET_ITS_PARK_VEHICLE)pAlarmInfo)->struPicInfo[1].pBuffer, iPicSize1);
					if (pDlg->m_checkPicData)
					{
						int iChanIndex = ((LPNET_ITS_PARK_VEHICLE)pAlarmMsg)->dwChanIndex;
						CGuard lockstatic(&pDlg->m_csLock);	
						pDlg->DrawWindowPicture(iChanIndex-1,iPicSize1,((LPNET_ITS_PARK_VEHICLE)pAlarmMsg)->struPicInfo[1].pBuffer);
					}
				}				
			}			
		
			int i = 0;
			DWORD dwCurIndex = ((LPNET_ITS_PARK_VEHICLE)pAlarmMsg)->dwChanIndex - 4;
			int ilogicalLaneNum = ((LPNET_ITS_PARK_VEHICLE)pAlarmMsg)->bylogicalLaneNum;
			for (i = 0; i < MAX_OUTPUT_WINDOW_NUM; i++)
			{
				if (dwCurIndex == pDlg->m_vDevices[iDeviceIndex].struPlayWindow[i].dwChanIndex )
				{
					if (ilogicalLaneNum == 0)
					{
						pDlg->m_vDevices[iDeviceIndex].struPlayWindow[i].bHasLeftVehicle = ((LPNET_ITS_PARK_VEHICLE)pAlarmMsg)->byLocationStatus; 						
						pDlg->m_vDevices[iDeviceIndex].struPlayWindow[i].strLeftPlate.Format("%s",((LPNET_ITS_PARK_VEHICLE)pAlarmMsg)->struPlateInfo.sLicense);
						::PostMessage(pDlg->m_hWnd, WM_UPDATE_OUTPUTWINDOW, WPARAM(i), LPARAM(0));
					}
					else if (ilogicalLaneNum == 1)
					{						
						pDlg->m_vDevices[iDeviceIndex].struPlayWindow[i].bHasRightVehicle = ((LPNET_ITS_PARK_VEHICLE)pAlarmMsg)->byLocationStatus;
						pDlg->m_vDevices[iDeviceIndex].struPlayWindow[i].strRightPlate.Format("%s",((LPNET_ITS_PARK_VEHICLE)pAlarmMsg)->struPlateInfo.sLicense);
						::PostMessage(pDlg->m_hWnd, WM_UPDATE_OUTPUTWINDOW, WPARAM(i), LPARAM(0));
					}

					break;
				}
			}	
		}
		

		delete [] pAlarmMsg;
		pAlarmMsg = NULL;			
	}
	catch (...)
	{
		OutputDebugString("New Alarm Exception!\n");
	}
	return;
}



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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

// CParkingDemoDlg 对话框


CParkingDemoDlg::CParkingDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CParkingDemoDlg::IDD, pParent)
	, m_iTotalDeviceIndex(1)	
	, m_lListenHandle(-1)
	, m_evParkInfo(FALSE,TRUE)
	, m_bListenning(FALSE)
	, m_iHostPort(7200)
	, m_iUploadInterval(200)
	, m_iUploadTimeOut(5000)
	, m_bSuspendUpload(FALSE)
	, m_lChannel(-1)
	, m_checkPicData(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	InitializeCriticalSection(&m_csLock);
	m_hParkThread = NULL;	
	m_IsThreadRun = FALSE;
}

CParkingDemoDlg::~CParkingDemoDlg(void)
{
	DeleteCriticalSection(&m_csLock);
}

void CParkingDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_DEVICE, m_treeDevices);
	DDX_Text(pDX, IDC_EDT_HOST_IP, m_strHostIP);
	DDX_Text(pDX, IDC_EDT_HOST_PORT, m_iHostPort);
	DDX_Text(pDX, IDC_EDT_UPLOAD_INTERVAL, m_iUploadInterval);
	DDX_Text(pDX, IDC_EDT_UPLOAD_TIMEOUT, m_iUploadTimeOut);
	DDX_Check(pDX, IDC_CK_STOP_UPLOAD, m_bSuspendUpload);
	DDX_Check(pDX, IDC_CHECK_PIC_DATA, m_checkPicData);
}

BEGIN_MESSAGE_MAP(CParkingDemoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CParkingDemoDlg::OnBnClickedOk)
	ON_NOTIFY(NM_RCLICK, IDC_TREE_DEVICE, &CParkingDemoDlg::OnNMRClickTreeDevice)
ON_NOTIFY(NM_DBLCLK, IDC_TREE_DEVICE, &CParkingDemoDlg::OnNMDblclkTreeDevice)
ON_NOTIFY(NM_CLICK, IDC_TREE_DEVICE, &CParkingDemoDlg::OnNMClickTreeDevice)
ON_COMMAND(ID_TREE_DEVICE_PLAY, &CParkingDemoDlg::OnTreeDevicePlay)
ON_COMMAND(ID_TREE_DEVICE_STOP_PLAY, &CParkingDemoDlg::OnTreeDeviceStopPlay)
ON_COMMAND(ID_TREE_DEVICE_DELETE, &CParkingDemoDlg::OnTreeDeviceDelete)
ON_COMMAND(ID_TREEDEVICE_LOGIN, &CParkingDemoDlg::OnTreedeviceLogin)
ON_COMMAND(ID_TREEDEVICE_LOGOUT, &CParkingDemoDlg::OnTreedeviceLogout)
ON_WM_CTLCOLOR()
ON_COMMAND(ID_TREEDEVICE_PROPERTY, &CParkingDemoDlg::OnTreedeviceProperty)
ON_MESSAGE(WM_UPDATE_OUTPUTWINDOW,OnUpdateOutPutWindow)
ON_COMMAND(ID_CHAN_PLACE_INFO, &CParkingDemoDlg::OnChanPlaceInfo)
ON_BN_CLICKED(IDC_BUTTON_ENABLE_UPLOAD_DATA, &CParkingDemoDlg::OnBnClickedButtonEnableUploadData)
ON_BN_CLICKED(IDC_BUTTON_UPLOAD_CFG, &CParkingDemoDlg::OnBnClickedButtonSetUploadCfg)
ON_WM_CLOSE()
ON_BN_CLICKED(IDC_CHECK_PIC_DATA, &CParkingDemoDlg::OnBnClickedCheckPicData)
END_MESSAGE_MAP()


// CParkingDemoDlg 消息处理程序

BOOL CParkingDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标


	m_treeDevices.ModifyStyle(0,TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS); 
	m_hRoot = m_treeDevices.InsertItem(_T("右键添加设备"));

	m_strHostIP = GetLocalIPAddr();
	// 初始化SDK
	if (!NET_DVR_Init())
	{
		return FALSE;
	}	

	int i;
	for (i = 0; i < MAX_OUTPUT_WINDOW_NUM; i++)
	{
		m_OutPutWindow[i].Create(IDD_OUTPUT_DIALOG , GetDlgItem(IDC_STATIC_AREA));
		m_OutPutWindow[i].m_iNum = i;		
	}	
 	SetOutputWindow(16);

	UpdateData(FALSE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CParkingDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CParkingDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CParkingDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CParkingDemoDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//OnOK();
}


/*************************************************
Function:    	 GetLocalIPAddr
Description:	 获取本机IP地址
Input:           void
Output:      	 void
Return:		     获取成功返回 点分十进制的IP串，否则 0
*************************************************/
CString CParkingDemoDlg::GetLocalIPAddr()
{
	WORD wVersionRequested = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(wVersionRequested, &wsaData) != 0)
	{
		return "";
	}
	char local[255] = {0};
	gethostname(local, sizeof(local));
	hostent* ph = gethostbyname(local);
	if (ph == NULL)
	{
		return "";
	}
	in_addr addr;
	memcpy(&addr, ph->h_addr_list[0], sizeof(in_addr)); // 这里仅获取第一个ip
	string localIP;
	localIP.assign(inet_ntoa(addr));
	WSACleanup();
	return localIP.c_str();
}

BOOL CParkingDemoDlg::CheckIPStr(CString ip)
{	//check IP address correct
	char str[20];
	strcpy(str,ip);
	char a;
	int dot=0;
	int a3,a2,a1,a0,i = 0;
	a3 = a2 = a1 = a0 = -1;
	if (strlen(str)==0)
		return TRUE;
	while (a=str[i++])
	{
		if ((a==' ')||(a=='.')||((a>='0')&&(a<='9')))
		{
			if (a=='.')
				dot++;
		}
		else
			return FALSE;
	}
	if (dot!=3)
	{
		return FALSE;
	}
	else
	{
		sscanf(str,"%d.%d.%d.%d",&a3,&a2,&a1,&a0);
		if ((a0>255)||(a1>255)||(a2>255)||(a3>255))
		{
			return FALSE;
		}
		if ((a0 < 0) || (a1 < 0) || (a2 < 0) || (a3 < 0))
		{
			return FALSE;
		}
	}
	return TRUE;
}
void CParkingDemoDlg::SetOutputWindow(int n)
{
	// 视频画面分割窗口数目
	int i;
	CRect rect;
	GetDlgItem(IDC_STATIC_AREA)->GetClientRect(&rect);	
	
	if (n > MAX_OUTPUT_WINDOW_NUM)
	{
		return;
	}

	switch (n)
	{
	case 1:	
		m_OutPutWindow[0].SetWindowPos(NULL,0,0,rect.Width(),rect.Height(),SWP_SHOWWINDOW);
		for (i = 1; i < MAX_OUTPUT_WINDOW_NUM ; i++)
		{
			m_OutPutWindow[i].SetWindowPos(NULL,0,0,0,0,SWP_HIDEWINDOW);
		}
		break;
	case 4:
		m_OutPutWindow[0].SetWindowPos(NULL,0,					0,						rect.Width()/2,		rect.Height()/2,SWP_SHOWWINDOW);
		m_OutPutWindow[1].SetWindowPos(NULL,rect.Width()/2,		0,						rect.Width()/2,		rect.Height()/2,SWP_SHOWWINDOW);
		m_OutPutWindow[2].SetWindowPos(NULL,0,					rect.Height()/2,		rect.Width()/2,		rect.Height()/2,SWP_SHOWWINDOW);
		m_OutPutWindow[3].SetWindowPos(NULL,rect.Width()/2,		rect.Height()/2,		rect.Width()/2,		rect.Height()/2,SWP_SHOWWINDOW);
		for (i = 4 ;i < MAX_OUTPUT_WINDOW_NUM ;i++)
		{
			m_OutPutWindow[i].SetWindowPos(NULL,0,0,0,0,SWP_HIDEWINDOW);
		}
		break;;
	case 9:
		m_OutPutWindow[0].SetWindowPos(NULL,0,					0,					rect.Width()/3,		rect.Height()/3,SWP_SHOWWINDOW);
		m_OutPutWindow[1].SetWindowPos(NULL,rect.Width()/3,		0,					rect.Width()/3,		rect.Height()/3,SWP_SHOWWINDOW);
		m_OutPutWindow[2].SetWindowPos(NULL,rect.Width()/3*2,	0,					rect.Width()/3,		rect.Height()/3,SWP_SHOWWINDOW);
		m_OutPutWindow[3].SetWindowPos(NULL,0,					rect.Height()/3,	rect.Width()/3,		rect.Height()/3,SWP_SHOWWINDOW);
		m_OutPutWindow[4].SetWindowPos(NULL,rect.Width()/3,		rect.Height()/3,	rect.Width()/3,		rect.Height()/3,SWP_SHOWWINDOW);
		m_OutPutWindow[5].SetWindowPos(NULL,rect.Width()/3*2,	rect.Height()/3,	rect.Width()/3,		rect.Height()/3,SWP_SHOWWINDOW);
		m_OutPutWindow[6].SetWindowPos(NULL,0,					rect.Height()/3*2,	rect.Width()/3,		rect.Height()/3,SWP_SHOWWINDOW);
		m_OutPutWindow[7].SetWindowPos(NULL,rect.Width()/3,		rect.Height()/3*2,	rect.Width()/3,		rect.Height()/3,SWP_SHOWWINDOW);
		m_OutPutWindow[8].SetWindowPos(NULL,rect.Width()/3*2,	rect.Height()/3*2,	rect.Width()/3,		rect.Height()/3,SWP_SHOWWINDOW);
		for (i = 9 ;i < MAX_OUTPUT_WINDOW_NUM ;i++)
		{
			m_OutPutWindow[i].SetWindowPos(NULL,0,0,0,0,SWP_HIDEWINDOW);
		}
		break;
	case 16:
		m_OutPutWindow[0].SetWindowPos(NULL,0,						0,					rect.Width()/4,	rect.Height()/4,SWP_SHOWWINDOW);
		m_OutPutWindow[1].SetWindowPos(NULL,rect.Width()/4,			0,					rect.Width()/4,	rect.Height()/4,SWP_SHOWWINDOW);
		m_OutPutWindow[2].SetWindowPos(NULL,rect.Width()/4*2,		0,					rect.Width()/4,	rect.Height()/4,SWP_SHOWWINDOW);
		m_OutPutWindow[3].SetWindowPos(NULL,rect.Width()/4*3,		0,					rect.Width()/4,	rect.Height()/4,SWP_SHOWWINDOW);
		m_OutPutWindow[4].SetWindowPos(NULL,0,						rect.Height()/4,	rect.Width()/4,	rect.Height()/4,SWP_SHOWWINDOW);
		m_OutPutWindow[5].SetWindowPos(NULL,rect.Width()/4,			rect.Height()/4,	rect.Width()/4,	rect.Height()/4,SWP_SHOWWINDOW);
		m_OutPutWindow[6].SetWindowPos(NULL,rect.Width()/4*2,		rect.Height()/4,	rect.Width()/4,	rect.Height()/4,SWP_SHOWWINDOW);
		m_OutPutWindow[7].SetWindowPos(NULL,rect.Width()/4*3,		rect.Height()/4,	rect.Width()/4,	rect.Height()/4,SWP_SHOWWINDOW);
		m_OutPutWindow[8].SetWindowPos(NULL,0,						rect.Height()/4*2,	rect.Width()/4,	rect.Height()/4,SWP_SHOWWINDOW);
		m_OutPutWindow[9].SetWindowPos(NULL,rect.Width()/4,			rect.Height()/4*2,	rect.Width()/4,	rect.Height()/4,SWP_SHOWWINDOW);
		m_OutPutWindow[10].SetWindowPos(NULL,rect.Width()/4*2,		rect.Height()/4*2,	rect.Width()/4,	rect.Height()/4,SWP_SHOWWINDOW);
		m_OutPutWindow[11].SetWindowPos(NULL,rect.Width()/4*3,		rect.Height()/4*2,	rect.Width()/4,	rect.Height()/4,SWP_SHOWWINDOW);
		m_OutPutWindow[12].SetWindowPos(NULL,0,						rect.Height()/4*3,	rect.Width()/4,	rect.Height()/4,SWP_SHOWWINDOW);
		m_OutPutWindow[13].SetWindowPos(NULL,rect.Width()/4,		rect.Height()/4*3,	rect.Width()/4,	rect.Height()/4,SWP_SHOWWINDOW);
		m_OutPutWindow[14].SetWindowPos(NULL,rect.Width()/4*2,		rect.Height()/4*3,	rect.Width()/4,	rect.Height()/4,SWP_SHOWWINDOW);
		m_OutPutWindow[15].SetWindowPos(NULL,rect.Width()/4*3,		rect.Height()/4*3,	rect.Width()/4,	rect.Height()/4,SWP_SHOWWINDOW);
		break;
	default:;		
	}
}



void CParkingDemoDlg::PlayAll(void)
{
	//预览播放
	CGuard lock(&m_csLock);	
	int iChanNum = m_vDevices[m_iCurDeviceIndex].lpDeviceInfo.byChanNum;
	int iIPChanNum = m_vDevices[m_iCurDeviceIndex].lpDeviceInfo.byIPChanNum;
	int iStartIPChan = m_vDevices[m_iCurDeviceIndex].lpDeviceInfo.byStartChan+31;

	if (m_vDevices[m_iCurDeviceIndex].iUserId < 0 || m_vDevices[m_iCurDeviceIndex].bIsPlaying == TRUE)
	{
		return;
	}

	int i;
	for(i = 1; i <= iIPChanNum; i++)
	{
		m_OutPutWindow[i-1].PlayThis(m_vDevices[m_iCurDeviceIndex].iUserId, iStartIPChan+i);		
	}
	m_vDevices[m_iCurDeviceIndex].bIsPlaying = TRUE;
}

//停止播放
void CParkingDemoDlg::StopAll(void)
{	
	CGuard lock(&m_csLock);	

	if (m_vDevices[m_iCurDeviceIndex].bIsPlaying == FALSE)//
	{
		return;
	}

	for (int i = 0; i < MAX_OUTPUT_WINDOW_NUM; i++)
	{
		m_OutPutWindow[i].StopThis();
	}
	m_vDevices[m_iCurDeviceIndex].bIsPlaying = FALSE;	
}


BOOL CParkingDemoDlg::UpdateUI(void)
{
	return TRUE;
}

#define IDM_MENU0 0
#define IDM_MENU1 1
#define IDM_MENU2 2
#define IDM_MENU3 3
#define IDM_ITEM0 10
#define IDM_ITEM1 11
#define IDM_ITEM2 12
#define IDM_ITEM3 13
#define IDM_ITEM4 14
#define IDM_ITEM5 15
#define IDM_ITEM6 16

void CParkingDemoDlg::OnNMRClickTreeDevice(NMHDR *pNMHDR, LRESULT *pResult)
{
	CAddDeviceDlg dlg;	
	CPoint point(0,0);
	GetCursorPos(&point);
	HTREEITEM hParent = NULL;
	if (NULL == m_treeDevices.GetRootItem())
	{
		return;
	}
	else
	{
		CRect rect(0,0,0,0);;
		ScreenToClient(&point);
		GetDlgItem(IDC_TREE_DEVICE)->GetWindowRect(&rect);
		ScreenToClient(&rect);
		point.x = point.x-rect.left;
		point.y = point.y-rect.top;


		UINT uFlags = 0;
		m_hItem = m_treeDevices.HitTest(point);//将点击的点转化为当前选项的句柄
		if (m_hItem == NULL)
		{
			return;
		}
		m_treeDevices.SelectItem(m_hItem);

		m_dwNodeData = (DWORD)m_treeDevices.GetItemData(m_hItem);
		m_strNodeText = m_treeDevices.GetItemText(m_hItem);
		int iType = int(m_treeDevices.GetItemData(m_hItem) / 1000);
		int iIndex = m_dwNodeData%1000;

		if (TREE_NODE_ROOT == iType) //根结点
		{ 		
			if (dlg.DoModal() == IDOK)
			{					
				DeviceInfo stDevice;	

				CGuard lock(&m_csLock);	

				int i = 0;
				for (i = 0; i < MAX_OUTPUT_WINDOW_NUM; i++)
				{
					stDevice.struPlayWindow[i].iLeftPlaceNo = dlg.m_iBeginPlaceNo + 2*i;
					stDevice.struPlayWindow[i].iRightPlaceNo = dlg.m_iBeginPlaceNo + 2*i + 1;
					m_OutPutWindow[i].m_iLeftPlaceNo = stDevice.struPlayWindow[i].iLeftPlaceNo;
					m_OutPutWindow[i].m_iRightPlaceNo = stDevice.struPlayWindow[i].iRightPlaceNo;	
					m_OutPutWindow[i].UpdateWindow();
				}				
				strcpy_s(stDevice.szDeviceName, sizeof(stDevice.szDeviceName), dlg.m_strDeviceName.GetBuffer());
				strcpy_s(stDevice.szDVRIP,sizeof(stDevice.szDVRIP),dlg.m_strDeviceIP.GetBuffer());
				stDevice.wDVRPort = dlg.m_lPort;
				strcpy_s(stDevice.szUserName,sizeof(stDevice.szUserName),dlg.m_strUserName.GetBuffer());
				strcpy_s(stDevice.szPassword,sizeof(stDevice.szPassword),dlg.m_strPassWord.GetBuffer());
				stDevice.iUserId = -1;
				stDevice.lAlarmHandle = -1;				
				ZeroMemory(&stDevice.lpDeviceInfo , sizeof(stDevice.lpDeviceInfo));	

				m_hDeviceItem = m_treeDevices.InsertItem(dlg.m_strDeviceName.GetBuffer(), m_hRoot);	//hDeviceItem为设备结点句柄
				stDevice.hDeviceItem = m_hDeviceItem;
				m_vDevices.push_back(stDevice);
				m_iTotalDeviceIndex++;
				
				m_treeDevices.SetItemData(m_hDeviceItem,1000+m_vDevices.size()-1);	
				m_treeDevices.Expand(m_hRoot,TVE_EXPAND);	

				m_hCurDeviceItem = m_hDeviceItem;
				m_iCurDeviceIndex = int(m_treeDevices.GetItemData(m_hCurDeviceItem) % 1000);

				//


				
			}		
		}
		else if (TREE_NODE_DEVICE == iType) //设备结点
		{ 
			m_iCurDeviceIndex = int(m_treeDevices.GetItemData(m_hItem) % 1000);
			m_hCurDeviceItem = m_hItem;
			m_iCurChanIndex = -1;
			m_hCurChanItem = NULL;
			
			CMenu TreeMenu, *pSubMenu;
			TreeMenu.LoadMenu(IDR_MENU_TREE_DEVICE);
			pSubMenu = TreeMenu.GetSubMenu(0);
			CPoint point(0,0);
			GetCursorPos(&point);
			pSubMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON, point.x, point.y, this);//显示弹出菜单，TPM_LEFTALIGN 放置弹出菜单，与鼠标左对齐。
		}
		else if (TREE_NODE_CHAN == iType) //设备结点
		{ 
			m_iCurChanIndex = iIndex;
			m_hCurChanItem = m_hItem;
			m_hParent = m_treeDevices.GetParentItem(m_hItem);
			if (m_hParent != NULL)
			{
				m_dwNodeData = m_treeDevices.GetItemData(m_hParent);
				iType = m_dwNodeData/1000;
				iIndex = m_dwNodeData%1000;
				if (1 == iType)  //设备结点
				{
					m_iCurDeviceIndex = iIndex;
					m_hCurDeviceItem = m_hParent;
				}
			}

			CMenu TreeMenu, *pSubMenu;
			TreeMenu.LoadMenu(IDR_MENU_TREE_CHAN);
			pSubMenu = TreeMenu.GetSubMenu(0);
			CPoint point(0,0);
			GetCursorPos(&point);
			pSubMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON, point.x, point.y, this);//显示弹出菜单，TPM_LEFTALIGN 放置弹出菜单，与鼠标左对齐。
		}		
		else
		{
			m_iCurDeviceIndex = -1;
			m_hCurDeviceItem = NULL;
			m_iCurChanIndex = -1;
			m_hCurChanItem = NULL;
		}
	}
	UpdateData(FALSE);
	*pResult = 0;
}

void CParkingDemoDlg::OnNMDblclkTreeDevice(NMHDR *pNMHDR, LRESULT *pResult)
{
	m_hSelect = m_treeDevices.GetSelectedItem();
	if (NULL == m_hSelect)//it works to double click device tree only while preview
	{
		return;
	}
	m_dwNodeData = (DWORD)m_treeDevices.GetItemData(m_hSelect);
	m_strNodeText = m_treeDevices.GetItemText(m_hSelect);
	m_hParent = NULL;
	int iType = m_dwNodeData/1000;
	int iIndex = m_dwNodeData%1000;

	switch (iType)
	{
	case TREE_NODE_ROOT:  //root
		break;
	case TREE_NODE_DEVICE:   //Double click device node
		m_iCurDeviceIndex = iIndex;
		m_hCurDeviceItem = m_hSelect;
		m_iCurChanIndex = -1;
		m_hCurChanItem = NULL;
		if (GetCurDeviceIndex() != -1)
		{			
			PlayAll();
		}		
		m_treeDevices.Expand(m_hSelect, TVE_COLLAPSE);//expend reverse operation
		//m_treeDevices.Expand(hSelect,TVE_EXPAND);
		break;
	case TREE_NODE_CHAN:   //Double click device node		
		m_iCurChanIndex = iIndex;
		m_hCurChanItem = m_hSelect;
		m_hParent = m_treeDevices.GetParentItem(m_hSelect);
		if (m_hParent != NULL)
		{
			m_dwNodeData = m_treeDevices.GetItemData(m_hParent);
			iType = m_dwNodeData/1000;
			iIndex = m_dwNodeData%1000;
			if (1 == iType)  //设备结点
			{
				m_iCurDeviceIndex = iIndex;
				m_hCurDeviceItem = m_hParent;
			}
		}		
	default:
		{
			m_iCurDeviceIndex = -1;
			m_hCurDeviceItem = NULL;
			m_iCurChanIndex = -1;
			m_hCurChanItem = NULL;
		}
		break;
	}
	*pResult = 0;
}

void CParkingDemoDlg::OnNMClickTreeDevice(NMHDR *pNMHDR, LRESULT *pResult)
{
	CPoint pt(0,0);
	GetCursorPos(&pt);

	CRect rect(0,0,0,0);
	ScreenToClient(&pt);
	GetDlgItem(IDC_TREE_DEVICE)->GetWindowRect(&rect);
	ScreenToClient(&rect);

	pt.x = pt.x-rect.left;
	pt.y = pt.y-rect.top;

	UINT uFlags = 0;
	m_hSelect = m_treeDevices.HitTest(pt,&uFlags);
	if (m_hSelect == NULL)
	{
		m_iCurDeviceIndex = -1;
		m_hCurDeviceItem = NULL;		
		//m_hCurChanItem = NULL;
		//AfxMessageBox(_T( "please click the right node!"));
		return;
	}

	m_dwNodeData = (DWORD)m_treeDevices.GetItemData(m_hSelect);
	m_strNodeText = m_treeDevices.GetItemText(m_hSelect);

	int iType = m_dwNodeData/1000;
	int iIndex = m_dwNodeData%1000;
	if (TREE_NODE_DEVICE == iType) //单击设备结点
	{
		m_iCurDeviceIndex = iIndex;
		m_hCurDeviceItem = m_hSelect;
		//m_iCurChanIndex = -1;
		//m_hCurChanItem = NULL;
	}
	else if (TREE_NODE_CHAN == iType) //单击通道结点
	{
		m_iCurChanIndex = iIndex;
		m_hCurChanItem = m_hSelect;
		m_hParent = m_treeDevices.GetParentItem(m_hSelect);
		if (m_hParent != NULL)
		{
			m_dwNodeData = m_treeDevices.GetItemData(m_hParent);
			iType = m_dwNodeData/1000;
			iIndex = m_dwNodeData%1000;
			if (1 == iType)  //设备结点
			{
				m_iCurDeviceIndex = iIndex;
				m_hCurDeviceItem = m_hParent;
			}
		}
	}	
	else
	{
		m_iCurDeviceIndex = -1;
		m_hCurDeviceItem = NULL;
		m_iCurChanIndex = -1;
		m_hCurChanItem = NULL;
	}
	*pResult = 0;
}

void CParkingDemoDlg::DeleteTreeChanNode(HTREEITEM hDeviceItem)  //删除选中设备下的IPC
{	
	ASSERT(hDeviceItem != NULL);
	HTREEITEM hTempItem;
	HTREEITEM hChanItem = m_treeDevices.GetChildItem(hDeviceItem);
	while(hChanItem != NULL)
	{
		hTempItem = m_treeDevices.GetNextSiblingItem(hChanItem);
		m_treeDevices.DeleteItem(hChanItem);
		hChanItem = hTempItem;
	}
}

int CParkingDemoDlg::GetCurDeviceIndex(void)
{
	if (m_iCurDeviceIndex < 0)
	{
		return -1;
	}	
	return m_iCurDeviceIndex;
}

void CParkingDemoDlg::LoginAll()
{
	DeviceInfo d;
	CString strTmp;
	CGuard lock(&m_csLock);	
	if (m_iCurDeviceIndex != -1)
	{
		d = m_vDevices[m_iCurDeviceIndex];
	}	
	if (d.iUserId >= 0)//判断已经登录的情况
	{
		return;
	}

	int uid = NET_DVR_Login_V30(d.szDVRIP, d.wDVRPort,d.szUserName,d.szPassword, &m_vDevices[m_iCurDeviceIndex].lpDeviceInfo); //备用IP:172.7.15.101 172.7.15.67
	if (uid != -1)
	{
		//登陆成功	
		//LOG_INFO("登录设备%s成功,lUserId = %d",d.szDVRIP,uid);
		m_vDevices[m_iCurDeviceIndex].iUserId= uid;	

		int i;

		for (i = 1; i <= m_vDevices[m_iCurDeviceIndex].lpDeviceInfo.byIPChanNum; i++)
		{
			m_vDevices[m_iCurDeviceIndex].struPlayWindow[i-1].dwChanIndex = i;
			HTREEITEM hSubItem = m_treeDevices.InsertItem("",m_hCurDeviceItem);			
			strTmp.Format("IPCamera%02d",i);
			m_treeDevices.SetItemText(hSubItem, strTmp.GetBuffer());
			m_treeDevices.SetItemData(hSubItem, 2*1000 + i);
		}
		m_treeDevices.Expand(m_hCurDeviceItem,TVE_EXPAND);

		m_vDevices[m_iCurDeviceIndex].lAlarmHandle = NET_DVR_SetupAlarmChan_V30(uid);
		if (m_vDevices[m_iCurDeviceIndex].lAlarmHandle < 0)
		{
			strTmp.Format(_T("布防失败！错误类型%i"),NET_DVR_GetLastError());
			AfxMessageBox(strTmp);
			return;
		}
	}
	else 
	{
		strTmp.Format(_T("登陆失败！错误类型%i"),NET_DVR_GetLastError());
		AfxMessageBox(strTmp);
	}
}

void CParkingDemoDlg::LogoutAll()
{
	CString strTmp;
	CGuard lock(&m_csLock);	
	int iDeviceIndex = m_iCurDeviceIndex;  
	int iChanIndex = m_iCurChanIndex;
	int iLoginState = m_vDevices[iDeviceIndex].iUserId;

	if (iLoginState < 0)
	{
		AfxMessageBox(_T("该设备未登录或已注销!"));
	}
	else if (m_vDevices[iDeviceIndex].lAlarmHandle != -1)
	{
		NET_DVR_CloseAlarmChan_V30(m_vDevices[iDeviceIndex].lAlarmHandle); //撤防
		m_vDevices[iDeviceIndex].lAlarmHandle = -1;		
	}
	else
	{
		strTmp.Format(_T("撤防失败！error code %d", NET_DVR_GetLastError()));
		AfxMessageBox(strTmp);
	}	

	if (NET_DVR_Logout(m_vDevices[iDeviceIndex].iUserId))//注销
	{
		m_vDevices[iDeviceIndex].iUserId = -1;
		ResetOutPutWindow();
		DeleteTreeChanNode(m_vDevices[iDeviceIndex].hDeviceItem);
	} 
	else
	{
		strTmp.Format(_T("注销失败！error code %d", NET_DVR_GetLastError()));
		AfxMessageBox(strTmp);	
	}		
}

void CParkingDemoDlg::ResetOutPutWindow()
{
	int i = 0;
	for (i = 0; i < MAX_OUTPUT_WINDOW_NUM; i++)
	{
		m_OutPutWindow[i].ResetWindow();
	}
}
void CParkingDemoDlg::OnTreeDevicePlay()
{
	PlayAll();
}

void CParkingDemoDlg::OnTreeDeviceStopPlay()
{
	StopAll();
}
void CParkingDemoDlg::OnTreedeviceLogin()
{
	LoginAll();	
}

void CParkingDemoDlg::OnTreedeviceLogout()
{
	LogoutAll();	
}

void CParkingDemoDlg::OnTreeDeviceDelete()  //删除选中设备
{
	CGuard lock(&m_csLock);	
	if (m_iCurDeviceIndex != -1)
	{			
		m_vDevices.erase(m_vDevices.begin() + m_iCurDeviceIndex);
		m_treeDevices.DeleteItem(m_hDeviceItem);
	}		
	m_iCurDeviceIndex = -1;
	m_iCurChanIndex = -1;
	m_iTotalDeviceIndex--;
}



void CALLBACK g_ExceptionCallBack(DWORD dwType, LONG lUserID, LONG lHandle, void *pUser)
{
	char tempbuf[256] = {0};
	switch(dwType) 
	{
	case EXCEPTION_RECONNECT:    //预览时重连
		TRACE("----------reconnect--------%d\n", time(NULL));
		break;
	default:
		break;
	}
}

void CParkingDemoDlg::OnBnClickedButtonEnableUploadData()
{
	UpdateData(TRUE);
	if (m_bListenning)
	{
		StopListen();
		GetDlgItem(IDC_BUTTON_ENABLE_UPLOAD_DATA)->SetWindowText("开启牌识上传");
		m_bListenning = FALSE;
	} 
	else
	{
		StartListen();
		GetDlgItem(IDC_BUTTON_ENABLE_UPLOAD_DATA)->SetWindowText("关闭牌识上传");
		m_bListenning = TRUE;
	}
	UpdateData(FALSE);
}

void CParkingDemoDlg::StartListen() 
{
	UpdateData(TRUE);

	CString strIP = GetLocalIPAddr();	

	if(m_lListenHandle == -1)
	{

		m_lListenHandle = NET_DVR_StartListen_V30(strIP.GetBuffer(), m_iHostPort, MessageCallback, NULL);
		if(m_lListenHandle < 0)
		{
			m_lListenHandle = -1;
			AfxMessageBox("NET_DVR_StartListen_V30");
		}
		
	}
}

void CParkingDemoDlg::StopListen() 
{
	if(m_lListenHandle == -1)
	{
		return;
	}

	if(NET_DVR_StopListen_V30(m_lListenHandle))	
	{		
		m_lListenHandle = -1;	
	}
}


void CParkingDemoDlg::FreshParkData(int iIndex)
{
	if (m_iCurDeviceIndex < 0)
	{
		return;
	}	
	int i = iIndex;	

	CGuard lock(&m_csLock);	

	m_OutPutWindow[i].m_bHasLeftVehicle = m_vDevices[m_iCurDeviceIndex].struPlayWindow[i].bHasLeftVehicle;
	m_OutPutWindow[i].m_bHasRightVehicle = m_vDevices[m_iCurDeviceIndex].struPlayWindow[i].bHasRightVehicle;
	m_OutPutWindow[i].m_strLeftPlateNo = m_vDevices[m_iCurDeviceIndex].struPlayWindow[i].strLeftPlate;
	m_OutPutWindow[i].m_strRightPlateNo = m_vDevices[m_iCurDeviceIndex].struPlayWindow[i].strRightPlate;
	m_OutPutWindow[i].m_iLeftPlaceNo = m_vDevices[m_iCurDeviceIndex].struPlayWindow[i].iLeftPlaceNo;
	m_OutPutWindow[i].m_iRightPlaceNo = m_vDevices[m_iCurDeviceIndex].struPlayWindow[i].iRightPlaceNo;
	m_OutPutWindow[i].UpdateWindow();

}

LRESULT CParkingDemoDlg::OnUpdateOutPutWindow(WPARAM wParam,LPARAM lParam)
{
	//CGuard lock(&m_csLock);	

	int nIndex = (int)wParam;
	if(nIndex < 0 || nIndex > MAX_OUTPUT_WINDOW_NUM - 1)
	{
		return -1;
	}

	FreshParkData(nIndex);

	return 0;
}

HBRUSH CParkingDemoDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

void CParkingDemoDlg::OnTreedeviceProperty()
{
	CIPAccessCfgDlg dlg;
	dlg.DoModal();
}

BOOL CParkingDemoDlg::DoSetIPAccessCfg(int iDeviceIndex)
{
	BOOL bIPRet = FALSE;
	CGuard lock(&m_csLock);	
	bIPRet = NET_DVR_SetDVRConfig(m_vDevices[iDeviceIndex].iUserId, NET_DVR_SET_IPPARACFG_V40, m_vDevices[iDeviceIndex].iGroupNO, &m_vDevices[iDeviceIndex].struIPParaCfgV40, sizeof(m_vDevices[iDeviceIndex].struIPParaCfgV40));
	if (bIPRet)
	{		
		RefreshIPDevLocalCfg(iDeviceIndex);
	}
	return bIPRet;
}

BOOL CParkingDemoDlg::DoGetDeviceResoureCfg(int iDeviceIndex, int iChanIndex)
{
	int i = 0;
	CString csTemp;
	CGuard lock(&m_csLock);	
	LPNET_DVR_IPPARACFG_V40 lpIPAccessCfgV40 = &m_vDevices[iDeviceIndex].struIPParaCfgV40;
	DWORD dwReturned = 0;
	memset(lpIPAccessCfgV40, 0, sizeof(NET_DVR_IPPARACFG_V40));
	//2008-9-15 13:44 ip input configuration
	m_vDevices[iDeviceIndex].bIPRet = NET_DVR_GetDVRConfig(m_vDevices[iDeviceIndex].iUserId, NET_DVR_GET_IPPARACFG_V40, iChanIndex, lpIPAccessCfgV40, sizeof(NET_DVR_IPPARACFG_V40), &dwReturned);
	if (!m_vDevices[iDeviceIndex].bIPRet)
	{	///device no support ip access
		m_vDevices[iDeviceIndex].lFirstEnableChanIndex = 0;
		for (i = 0; i < MAX_CHANNUM_V30; i++)
		{	
			if (i < m_vDevices[iDeviceIndex].iAnalogChanNum)
			{
				m_vDevices[iDeviceIndex].struChanInfo[i].iDeviceIndex = iDeviceIndex;
				m_vDevices[iDeviceIndex].struChanInfo[i].iChanIndex = i;
				m_vDevices[iDeviceIndex].struChanInfo[i].iChannelNO = i + m_vDevices[iDeviceIndex].lpDeviceInfo.byStartChan;
				m_vDevices[iDeviceIndex].struChanInfo[i].bEnable = TRUE;
				m_vDevices[iDeviceIndex].struChanInfo[i].iChanType = DEMO_CHANNEL_TYPE_ANALOG;
				sprintf(m_vDevices[iDeviceIndex].struChanInfo[i].chChanName, ANALOG_C_FORMAT, i+m_vDevices[iDeviceIndex].lpDeviceInfo.byStartChan);	
			}
			else//clear the state of other channel
			{
				m_vDevices[iDeviceIndex].struChanInfo[i].iDeviceIndex = -1;
				m_vDevices[iDeviceIndex].struChanInfo[i].iChanIndex = -1;
				m_vDevices[iDeviceIndex].struChanInfo[i].bEnable = FALSE;
				sprintf(m_vDevices[iDeviceIndex].struChanInfo[i].chChanName, "");	
			}
		}

		m_vDevices[iDeviceIndex].iGroupNO = -1;
	} 
	else
	{
		m_vDevices[iDeviceIndex].iGroupNO = iChanIndex;
		RefreshIPDevLocalCfg(iDeviceIndex);
	}

	if (m_vDevices[iDeviceIndex].lpDeviceInfo.byZeroChanNum > 0)
	{
		for (i=0; i<m_vDevices[iDeviceIndex].lpDeviceInfo.byZeroChanNum; i++)
		{
			if (1)
			{
				m_vDevices[iDeviceIndex].struZeroChan[i].iDeviceIndex = iDeviceIndex;
				m_vDevices[iDeviceIndex].struZeroChan[i].iChanIndex = i+ZERO_CHAN_INDEX;
				sprintf(m_vDevices[iDeviceIndex].struZeroChan[i].chChanName, "ZeroChan%d", i);	
				//analog devices
				if (1)
				{
					m_vDevices[iDeviceIndex].struZeroChan[i].bEnable = TRUE;
					m_vDevices[iDeviceIndex].struZeroChan[i].dwImageType = CHAN_ORIGINAL;
					// g_struDeviceInfo[iDeviceIndex].iEnableChanNum ++;
				}
				else
				{
					m_vDevices[iDeviceIndex].struZeroChan[i].dwImageType = CHAN_OFF_LINE;
					m_vDevices[iDeviceIndex].struZeroChan[i].bEnable = FALSE;		
				}
			}
		}
	}
	return m_vDevices[iDeviceIndex].bIPRet;
}

void CParkingDemoDlg::RefreshIPDevLocalCfg(int iDeviceIndex)
{
	CGuard lock(&m_csLock);	
	//EnterCriticalSection(&m_csLock);
	LPNET_DVR_IPPARACFG_V40 lpIPAccessCfgV40 = &m_vDevices[iDeviceIndex].struIPParaCfgV40;
	DWORD dwChanShow = 0;
	int iIPChanIndex = 0;
	int i = 0;
	// 	for (i=0; i<MAX_IP_DEVICE; i++)
	// 	{
	// 		AddLog(-1, OPERATION_SUCC_T, "IP dev chan[%d] ip=%s name=%s", i,\
	// 		lpIPAccessCfg->struIPDevInfo[i].struIP.sIpV4, lpIPAccessCfg->struIPDevInfo[i].sUserName);
	// 	}

	//Update MAX IP channel number.
	m_vDevices[iDeviceIndex].lpDeviceInfo.byIPChanNum = lpIPAccessCfgV40->dwDChanNum;
	int iAnalogChanCount = 0;
	int iIPChanCount = 0;
	int iGroupNO   = m_vDevices[iDeviceIndex].iGroupNO;    //Group NO.
	int iGroupNum  = lpIPAccessCfgV40->dwGroupNum;
	int iIPChanNum = m_vDevices[iDeviceIndex].lpDeviceInfo.byIPChanNum;

	for (i = 0; i < MAX_CHANNUM_V30*2; i++)
	{
		//analog channel
		if (iAnalogChanCount < m_vDevices[iDeviceIndex].iAnalogChanNum)
		{
			dwChanShow = iAnalogChanCount + m_vDevices[iDeviceIndex].lpDeviceInfo.byStartChan;

			m_vDevices[iDeviceIndex].struChanInfo[i].iDeviceIndex = iDeviceIndex;
			m_vDevices[iDeviceIndex].struChanInfo[i].iChanIndex = i;
			m_vDevices[iDeviceIndex].struChanInfo[i].iChanType  = DEMO_CHANNEL_TYPE_ANALOG;
			m_vDevices[iDeviceIndex].struChanInfo[i].iChannelNO = dwChanShow;

			sprintf(m_vDevices[iDeviceIndex].struChanInfo[i].chChanName, ANALOG_C_FORMAT, dwChanShow);	
			//analog devices
			if (lpIPAccessCfgV40->byAnalogChanEnable[i])
			{
				m_vDevices[iDeviceIndex].struChanInfo[i].bEnable = TRUE;
				m_vDevices[iDeviceIndex].struChanInfo[i].dwImageType = CHAN_ORIGINAL;
				//g_struDeviceInfo[iDeviceIndex].iEnableChanNum ++;
			}
			else
			{
				m_vDevices[iDeviceIndex].struChanInfo[i].bEnable = FALSE;	
				m_vDevices[iDeviceIndex].struChanInfo[i].dwImageType = CHAN_OFF_LINE;
			}

			iAnalogChanCount++;
		}
		//IP channel  //(i>=MAX_ANALOG_CHANNUM && i<MAX_ANALOG_CHANNUM+g_struDeviceInfo[iDeviceIndex].iIPChanNum) )
		else if (iGroupNO >= 0 && ((iIPChanCount + iGroupNO * MAX_CHANNUM_V30)  <  iIPChanNum) && (iIPChanCount < MAX_CHANNUM_V30))   
		{
			dwChanShow = iIPChanCount + iGroupNO * MAX_CHANNUM_V30 + lpIPAccessCfgV40->dwStartDChan;

			m_vDevices[iDeviceIndex].struChanInfo[i].iChanType  = DEMO_CHANNEL_TYPE_IP;
			m_vDevices[iDeviceIndex].struChanInfo[i].iChannelNO = dwChanShow;

			iIPChanIndex = iIPChanCount;
			m_vDevices[iDeviceIndex].struChanInfo[i].iDeviceIndex = iDeviceIndex;
			m_vDevices[iDeviceIndex].struChanInfo[i].iChanIndex = i;	
			sprintf(m_vDevices[iDeviceIndex].struChanInfo[i].chChanName, DIGITAL_C_FORMAT, iIPChanCount + iGroupNO * MAX_CHANNUM_V30 + 1);				

			if (lpIPAccessCfgV40->struStreamMode[iIPChanIndex].uGetStream.struChanInfo.byIPID != 0 && lpIPAccessCfgV40->struStreamMode[iIPChanIndex].uGetStream.struChanInfo.byEnable == TRUE)
			{
				m_vDevices[iDeviceIndex].struChanInfo[i].bEnable = TRUE;//
				if (lpIPAccessCfgV40->struStreamMode[iIPChanIndex].uGetStream.struChanInfo.byEnable)
				{
					m_vDevices[iDeviceIndex].struChanInfo[i].dwImageType = CHAN_ORIGINAL;
				}
				else
				{
					m_vDevices[iDeviceIndex].struChanInfo[i].dwImageType = CHAN_OFF_LINE;
				}

				//	g_struDeviceInfo[iDeviceIndex].iEnableChanNum ++;
			}
			else
			{
				m_vDevices[iDeviceIndex].struChanInfo[i].dwImageType = CHAN_OFF_LINE;
				m_vDevices[iDeviceIndex].struChanInfo[i].bEnable = FALSE;	
				//g_struDeviceInfo[iDeviceIndex].struChanInfo[i].bAlarm = FALSE;
			}

			iIPChanCount++;
		}
		else
		{
			m_vDevices[iDeviceIndex].struChanInfo[i].iDeviceIndex = -1;
			m_vDevices[iDeviceIndex].struChanInfo[i].iChanIndex = -1;
			m_vDevices[iDeviceIndex].struChanInfo[i].iChanType  = DEMO_CHANNEL_TYPE_INVALID;
			m_vDevices[iDeviceIndex].struChanInfo[i].iChannelNO = -1;
			m_vDevices[iDeviceIndex].struChanInfo[i].bEnable = FALSE;	
			m_vDevices[iDeviceIndex].struChanInfo[i].bAlarm = FALSE;
			m_vDevices[iDeviceIndex].struChanInfo[i].bLocalManualRec = FALSE;
			m_vDevices[iDeviceIndex].struChanInfo[i].lRealHandle = -1;
			sprintf(m_vDevices[iDeviceIndex].struChanInfo[i].chChanName, "");	
		}
	}

	for (i = 0; i < MAX_CHANNUM_V30*2; i++)
	{
		if (m_vDevices[iDeviceIndex].struChanInfo[i].bEnable)
		{
			m_vDevices[iDeviceIndex].lFirstEnableChanIndex = i;
			break;
		}
	}

	if (m_vDevices[iDeviceIndex].lpDeviceInfo.byZeroChanNum > 0)
	{
		//get配置
	}
}



void CParkingDemoDlg::OnChanPlaceInfo()
{
	CPlaceInfoDlg dlg;
	CGuard lock(&m_csLock);	

	int iDeviceIndex = GetCurDeviceIndex();
	int iChanIndex = m_iCurChanIndex;
	dlg.m_iChanNo = iChanIndex;
	dlg.m_iLeftPlaceNo = m_vDevices[iDeviceIndex].struPlayWindow[iChanIndex-1].iLeftPlaceNo;
	dlg.m_iRightPlaceNo = m_vDevices[iDeviceIndex].struPlayWindow[iChanIndex-1].iRightPlaceNo;
	UpdateData(FALSE);
	
	if (dlg.DoModal() == IDOK)
	{
		UpdateData(TRUE);
		m_vDevices[iDeviceIndex].struPlayWindow[iChanIndex-1].iLeftPlaceNo = dlg.m_iLeftPlaceNo;
		m_vDevices[iDeviceIndex].struPlayWindow[iChanIndex-1].iRightPlaceNo = dlg.m_iRightPlaceNo;
		m_OutPutWindow[iChanIndex-1].m_iLeftPlaceNo = dlg.m_iLeftPlaceNo;
		m_OutPutWindow[iChanIndex-1].m_iRightPlaceNo = dlg.m_iRightPlaceNo;
		m_OutPutWindow[iChanIndex-1].UpdateWindow();
	}
}

void CParkingDemoDlg::OnBnClickedButtonSetUploadCfg()
{
	UpdateData(TRUE);
	if (GetCurDeviceIndex() < 0)
	{
		AfxMessageBox("请选择设备！");
		return;
	}
	if (!CheckIPStr(m_strHostIP))
	{
		AfxMessageBox("IP格式设置有误！");
		return;
	}

	OnBtnGet();

	memcpy(m_struUploadCfg.struRemoteDataHost1.struHostAddr.sIpV4,m_strHostIP.GetBuffer(),sizeof(m_struUploadCfg.struRemoteDataHost1.struHostAddr.sIpV4));
	m_struUploadCfg.struRemoteDataHost1.wHostPort = m_iHostPort;
	m_struUploadCfg.struRemoteDataHost1.wUploadInterval = m_iUploadInterval;
	m_struUploadCfg.struRemoteDataHost1.dwUploadTimeOut = m_iUploadTimeOut;
	m_struUploadCfg.struRemoteDataHost1.byUploadStrategy = 0;//0-最新数据优先上传
	m_struUploadCfg.struRemoteDataHost1.bySuspendUpload = m_bSuspendUpload;
	m_struUploadCfg.struRemoteDataHost1.dwDataType |= 0x01;//卡口数据	

	CString str;
	LONG lChannel = -1;
	BOOL bRet =FALSE;
	bRet = NET_DVR_SetDVRConfig(m_vDevices[m_iCurDeviceIndex].iUserId, NET_ITS_SET_UPLOAD_CFG, lChannel, &m_struUploadCfg, sizeof(NET_ITS_UPLOAD_CFG));
	if (!bRet)
	{		
		str.Format("设置上传信息失败,Error code = %d",NET_DVR_GetLastError());
		AfxMessageBox(str);
		return;
	}	

	if(MessageBox(_T("设置成功，重启设备生效，\n将耗时2-3分钟，确定重启？"),_T("提示"),MB_OKCANCEL) == IDOK)
	{
		if (NET_DVR_RebootDVR(m_vDevices[m_iCurDeviceIndex].iUserId))
		{			
			str.Format("等待重启完成，重新运行软件!");
			AfxMessageBox(str);
		}
	}
}

void CParkingDemoDlg::OnBtnGet() 
{
	// TODO: Add your control notification handler code here
    int count = sizeof(m_struUploadCfg);
	memset(&m_struUploadCfg, 0, sizeof(m_struUploadCfg));
	DWORD dwReturn = 0;
	BOOL bRet =FALSE;
	bRet = NET_DVR_GetDVRConfig(m_vDevices[m_iCurDeviceIndex].iUserId, NET_ITS_GET_UPLOAD_CFG, m_lChannel, &m_struUploadCfg,sizeof(m_struUploadCfg), &dwReturn);
	if (!bRet)
	{
		CString str;
		str.Format("获取设备上传参数失败,Error code = %d",NET_DVR_GetLastError());
		AfxMessageBox(str);
	}
	UpdateData(FALSE);
}

void CParkingDemoDlg::DrawWindowPicture(int iWndIndex,int iPicSize, unsigned char* pic) 
{
	// TODO: Add your control notification handler code here
	if (iWndIndex < 0 || iWndIndex >= MAX_OUTPUT_WINDOW_NUM || iPicSize <= 0 || !pic)
	{
		return;
	}
	m_OutPutWindow[iWndIndex].DrawPicture(iPicSize,pic);	
	
}
void CParkingDemoDlg::OnBnClickedCheckPicData()
{
	UpdateData(TRUE);
}
