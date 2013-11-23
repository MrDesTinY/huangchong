// IPAccessCfgDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ParkingDemo.h"
#include "IPAccessCfgDlg.h"
#include "ParkingDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


char *GetDeviceAddress(LPNET_DVR_IPDEVINFO_V31 lpDeviceInfo)
{
	if (lpDeviceInfo == NULL)
	{
		return NULL;
	}

	return (char *)(lpDeviceInfo->struIP.sIpV4);
}


//check IP address available
BOOL CheckIPStr(CString ip)
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

void SetDeviceAddress(LPNET_DVR_IPDEVINFO_V31 lpDevInfo, char *lpAddress)
{
	if (lpDevInfo != NULL && lpAddress != NULL)
	{
		if (CheckIPStr(lpAddress))
		{
			memcpy(lpDevInfo->struIP.sIpV4, lpAddress, 16);
		}
		else
		{
			memcpy(lpDevInfo->byDomain, lpAddress, MAX_DOMAIN_NAME);
		}	
	}
}


// CIPAccessCfgDlg 对话框

IMPLEMENT_DYNAMIC(CIPAccessCfgDlg, CDialog)

CIPAccessCfgDlg::CIPAccessCfgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIPAccessCfgDlg::IDD, pParent)
{
	m_dwDevIndex = 0;
	m_iCurSel = 0;
	m_iCurChanNO = 0;
	m_iCurChanIndex = 0;
	m_bCurEnable = FALSE;
	m_bAnalogChange = FALSE;
	memset(&m_struIPParaCfgV40, 0, sizeof(m_struIPParaCfgV40));
	//{{AFX_DATA_INIT(CDlgRemoteIPAccess)
	m_csDevIP = _T("172.7.39.1");
	m_uDevPort = 8000;
	m_csDevPwd = _T("12345");
	m_ceDevUserName = _T("admin");
	m_iIPDevChan = 1;
	m_bStreamUrlEnable = FALSE;

}

CIPAccessCfgDlg::~CIPAccessCfgDlg()
{
}

void CIPAccessCfgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CHANNEL_GROUP_NO, m_comboChannelGroupNO);
	DDX_Control(pDX, IDC_COMBO_STREAM_TYPE, m_comboStreamType);
	DDX_Control(pDX, IDC_COMBO_PROTYPE, m_comboProType);
	DDX_Control(pDX, IDC_LIST_IP_CHAN, m_listIPChan);
	DDX_Text(pDX, IDC_DEVICE_IP, m_csDevIP);
	DDX_Text(pDX, IDC_DEVICE_PORT, m_uDevPort);
	DDX_Text(pDX, IDC_DEVICE_PSW, m_csDevPwd);
	DDX_Text(pDX, IDC_DEVICE_USERNAME, m_ceDevUserName);
	DDX_Text(pDX, IDC_EDIT_CHAN, m_iIPDevChan);
}


BEGIN_MESSAGE_MAP(CIPAccessCfgDlg, CDialog)
	ON_NOTIFY(NM_CLICK, IDC_LIST_IP_CHAN, &CIPAccessCfgDlg::OnNMClickListIpChan)
	ON_CBN_SELCHANGE(IDC_COMBO_CHANNEL_GROUP_NO, &CIPAccessCfgDlg::OnCbnSelchangeComboChannelGroupNo)
	ON_CBN_SELCHANGE(IDC_COMBO_STREAM_TYPE, &CIPAccessCfgDlg::OnCbnSelchangeComboStreamType)
	ON_BN_CLICKED(IDOK, &CIPAccessCfgDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_ENABLE, &CIPAccessCfgDlg::OnBnClickedBtnEnable)
	ON_BN_CLICKED(IDC_BTN_ADD_IP_CHAN, &CIPAccessCfgDlg::OnBnClickedBtnAddIpChan)
	ON_BN_CLICKED(IDC_BTN_DELETE, &CIPAccessCfgDlg::OnBnClickedBtnDelete)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE_PRO_LIST, &CIPAccessCfgDlg::OnBnClickedButtonUpdateProList)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CIPAccessCfgDlg::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_BTN_REFRESH, &CIPAccessCfgDlg::OnBnClickedBtnRefresh)
END_MESSAGE_MAP()


// CIPAccessCfgDlg 消息处理程序

BOOL CIPAccessCfgDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_listIPChan.SetExtendedStyle(m_listIPChan.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
	char szLan[128] = {0};	
	m_listIPChan.InsertColumn(0, "序号", LVCFMT_LEFT, 40,-1);	
	m_listIPChan.InsertColumn(1,"通道号",LVCFMT_LEFT,80,-1);	
	m_listIPChan.InsertColumn(2, "是否在线",LVCFMT_LEFT,80,-1);	
	m_listIPChan.InsertColumn(3,"协议",LVCFMT_LEFT,100,-1);

	CheckInitParam();

	m_comboStreamType.SetCurSel(0);
	ShowInfoByGetStreamType(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

char* CIPAccessCfgDlg::ProIndexToStr(int iProType)
{
	switch(iProType)
	{
	case 0:
		
		break;
	default:
		break;
	}
	return m_szProType;	
}

/*********************************************************
  Function:	CheckInitParam
  Desc:		check and initialize the parameters
  Input:	none
  Output:	none
  Return:	none
**********************************************************/
void CIPAccessCfgDlg::CheckInitParam()
{
	int i = 0;

	//Update channel list: group[0]
	UpdateGroup(0);
	char szLan[128] = {0};	
	m_comboChannelGroupNO.ResetContent();
	for (i = 0; i < (int)m_struIPParaCfgV40.dwGroupNum; i++)
	{
		memset(szLan, 0, 128);
		sprintf(szLan, "%d", i);
		m_comboChannelGroupNO.AddString(szLan);
	}
	m_comboChannelGroupNO.SetCurSel(0);
	return;
}

void CIPAccessCfgDlg::UpdateGroup(int iGroupNO)
{
	CParkingDemoDlg *pDlg = (CParkingDemoDlg*)theApp.GetMainWnd();
	ASSERT(pDlg != NULL);
	if (iGroupNO < 0)
	{
		return;
	}
	ShowWindow(SW_SHOW);

	int iDev = pDlg->GetCurDeviceIndex();
	if (iDev == -1)
	{
		EnableWindow(FALSE);
		return;
	}
	EnableWindow(TRUE);

	m_dwDevIndex = iDev;

	CGuard lock(&pDlg->m_csLock);	

	m_dwStartChan = pDlg->m_vDevices[m_dwDevIndex].lpDeviceInfo.byStartChan;		
		
	m_iSelAccessType = 0;
	
	CString strTemp =_T("");
	CString strChanTmp = _T("");
	int i = 0;
	DWORD dwChanNum = 0;
	int iIndex = 0;
	DWORD dwIPChanIndex = 0;
	DWORD dwID = 0;
	m_iCurChanNO = -1;
	m_iCurSel = -1;
	EnableInfoCtrl(FALSE);
	SetCtrlStat(2);

	char szLan[128] = {0};
	m_listIPChan.DeleteAllItems();

	//Get IPParamCfg_V40
	//.....
	if (pDlg->DoGetDeviceResoureCfg(m_dwDevIndex, iGroupNO))
	{
		//OnButtonUpdateProList();

		memset(&m_struIPParaCfgV40, 0, sizeof(m_struIPParaCfgV40));
		memcpy(&m_struIPParaCfgV40, &pDlg->m_vDevices[m_dwDevIndex].struIPParaCfgV40, sizeof(m_struIPParaCfgV40));

		int iMaxSimulateChannelNO = m_struIPParaCfgV40.dwAChanNum + pDlg->m_vDevices[m_dwDevIndex].lpDeviceInfo.byStartChan;
		//***channel list: group[0]****//
		//***64 simulate channel***//
		for (i = 0; i < MAX_CHANNUM_V30; i++)
		{
			dwChanNum = i + pDlg->m_vDevices[m_dwDevIndex].lpDeviceInfo.byStartChan + MAX_CHANNUM_V30 * iGroupNO;
			if ((dwChanNum + 1) > iMaxSimulateChannelNO)
			{
				break;
			}

			strTemp.Format("%02d", iIndex+1);
			m_listIPChan.InsertItem(iIndex, strTemp, 0);
			strTemp.Format(ANALOG_C_FORMAT, dwChanNum);
			m_listIPChan.SetItemText(iIndex, 1, strTemp);
			if (m_struIPParaCfgV40.byAnalogChanEnable[i])
			{				
				m_listIPChan.SetItemText(i, 2, "可用");
				m_listIPChan.SetItemData(iIndex, ANALOG_CHAN_ENABLE*1000+i);
			}
			else
			{			
				m_listIPChan.SetItemText(i, 2, "不可用");
				m_listIPChan.SetItemData(iIndex, ANALOG_CHAN_DISABLE*1000+i);
			}
			iIndex++;
		}

		//***64 IP channel***//
		int iMaxIPChannelNO = m_struIPParaCfgV40.dwStartDChan + m_struIPParaCfgV40.dwDChanNum;
		for (i = 0; i < MAX_CHANNUM_V30; i++)
		{
			dwChanNum = i + m_struIPParaCfgV40.dwStartDChan + MAX_CHANNUM_V30 * iGroupNO;
			if ((dwChanNum + 1) > iMaxIPChannelNO)
			{
				break;
			}
            
			strTemp.Format("%02d", iIndex+1);
			m_listIPChan.InsertItem(iIndex, strTemp, 0);

			strTemp.Format(IP_CAMERA_NAME, dwChanNum - m_struIPParaCfgV40.dwStartDChan + 1);
			m_listIPChan.SetItemText(iIndex, 1, strTemp);
			ShowIsEnableOrOnlineInList(iIndex, 2, i);
		
			//4.the type of getting stream
			memset(szLan, 0, 128);
			sprintf(szLan, "%d", m_struIPParaCfgV40.struStreamMode[i].byGetStreamType);
			m_listIPChan.SetItemText(iIndex, 3, szLan);

			iIndex++;		
        }	

		GetDlgItem(IDC_BTN_SAVE)->EnableWindow(TRUE);
		EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BTN_SAVE)->EnableWindow(FALSE);
		if (NET_DVR_GetLastError() == NET_DVR_NOSUPPORT)
		{
			EnableWindow(FALSE);
		}
	}
}

void CIPAccessCfgDlg::OnNMClickListIpChan(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	POSITION  iPos = m_listIPChan.GetFirstSelectedItemPosition();
	if (iPos == NULL)
	{
		return;
	}
	m_iCurSel = m_listIPChan.GetNextSelectedItem(iPos);
	int iData = m_listIPChan.GetItemData(m_iCurSel);

	int iGroupNO = m_comboChannelGroupNO.GetCurSel();

	m_iCurChanNO = iData%1000 + iGroupNO * 64;

	m_iCurChanIndex = m_iCurChanNO - m_struIPParaCfgV40.dwStartDChan - iGroupNO * MAX_CHANNUM_V30; 

	SetCtrlStat(iData/1000);
	UpdateData(FALSE);

	*pResult = 0;
}

void CIPAccessCfgDlg::OnCbnSelchangeComboChannelGroupNo()
{
	int iCurrentGroup = m_comboChannelGroupNO.GetCurSel();
	UpdateGroup(iCurrentGroup);
}

void CIPAccessCfgDlg::OnCbnSelchangeComboStreamType()
{
	ShowInfoByGetStreamType(m_comboStreamType.GetCurSel());
}

void CIPAccessCfgDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//OnOK();
}

void CIPAccessCfgDlg::OnBnClickedBtnEnable()
{
	char szLan[128] = {0};
	if (m_bCurEnable)
	{		
		m_listIPChan.SetItemText(m_iCurSel, 2, "禁用");
		m_listIPChan.SetItemData(m_iCurSel, ANALOG_CHAN_DISABLE*1000+m_iCurChanNO);
		m_struIPParaCfgV40.byAnalogChanEnable[m_iCurChanNO] = FALSE;
		SetCtrlStat(0);
	}
	else
	{		
		m_listIPChan.SetItemText(m_iCurSel, 2, "可用");
		m_listIPChan.SetItemData(m_iCurSel, ANALOG_CHAN_ENABLE*1000+m_iCurChanNO);
		m_struIPParaCfgV40.byAnalogChanEnable[m_iCurChanNO] = TRUE;
		SetCtrlStat(1);	
	}
	m_bAnalogChange = TRUE;
}

void CIPAccessCfgDlg::OnBnClickedBtnAddIpChan()
{
	UpdateData(TRUE);
	int i = 0;
	int j = 0;
	CString csTmp;
	BOOL bExistDev = FALSE;
	char szLan[128] = {0};
	int iGroupNO = m_comboChannelGroupNO.GetCurSel();
	int iIPChanIndex =  0; //Array index in v40.

	iIPChanIndex = m_iCurChanNO - m_struIPParaCfgV40.dwStartDChan - iGroupNO * MAX_CHANNUM_V30;

	//check params
	if (iIPChanIndex < 0)
	{
		//g_pMainDlg->AddLog(m_dwDevIndex, OPERATION_FAIL_T, "chan [%d] not ip Channel", m_iCurChanNO);
		return;
	}
	if (m_iCurSel < 0)
	{		
		//g_StringLanType(szLan, "请选择列表中的通道", "Please choose the channel in the list");
		AfxMessageBox("请选择列表中的通道");
		return;
	}
	if (m_iIPDevChan == 0 || memcmp(m_csDevIP.GetBuffer(0), "0.0.0.0", m_csDevIP.GetLength()) == 0)
	{
		//g_StringLanType(szLan, "参数错误", "Parameter error");
		AfxMessageBox("参数错误");
		return;
	}	

	m_struIPParaCfgV40.struStreamMode[iIPChanIndex].byGetStreamType = 0; //direct connect device.
	//clear device info
	memset(&(m_struIPParaCfgV40.struStreamMode[iIPChanIndex].uGetStream.struChanInfo), 0, sizeof(NET_DVR_IPCHANINFO));

	//set device id
	m_struIPParaCfgV40.struStreamMode[iIPChanIndex].uGetStream.struChanInfo.byEnable = FALSE; //Is online?
	m_struIPParaCfgV40.struStreamMode[iIPChanIndex].uGetStream.struChanInfo.byChannel = (BYTE)m_iIPDevChan;
	int iDevID = iIPChanIndex + iGroupNO * MAX_CHANNUM_V30 + 1;
	m_struIPParaCfgV40.struStreamMode[iIPChanIndex].uGetStream.struChanInfo.byIPID = iDevID % 256;
	m_struIPParaCfgV40.struStreamMode[iIPChanIndex].uGetStream.struChanInfo.byIPIDHigh = iDevID /256;

	//set device infomation
	SetDeviceAddress(&(m_struIPParaCfgV40.struIPDevInfo[iIPChanIndex]), m_csDevIP.GetBuffer(0));
	m_struIPParaCfgV40.struIPDevInfo[iIPChanIndex].byProType = (BYTE)m_comboProType.GetItemData(m_comboProType.GetCurSel());
	m_struIPParaCfgV40.struIPDevInfo[iIPChanIndex].byEnable = TRUE;
	memcpy(m_struIPParaCfgV40.struIPDevInfo[iIPChanIndex].sUserName, m_ceDevUserName.GetBuffer(0), NAME_LEN);
	memcpy(m_struIPParaCfgV40.struIPDevInfo[iIPChanIndex].sPassword, m_csDevPwd.GetBuffer(0), PASSWD_LEN);
	m_struIPParaCfgV40.struIPDevInfo[iIPChanIndex].wDVRPort = (WORD)m_uDevPort;

	//g_StringLanType(szLan, "可用", "Enable");
	m_listIPChan.SetItemText(m_iCurSel, 2, "可用");

	m_listIPChan.SetItemData(m_iCurSel, IP_CHAN_ENABLE*1000 + m_iCurChanNO);
	SetCtrlStat(IP_CHAN_ENABLE);

}

void CIPAccessCfgDlg::OnBnClickedBtnDelete()
{
	char szLan[128] = {0};
	int iGroupNO = m_comboChannelGroupNO.GetCurSel();

	int iIPChanIndex = m_iCurChanNO - m_struIPParaCfgV40.dwStartDChan - iGroupNO * MAX_CHANNUM_V30;//Current ip channel array index
	if (iIPChanIndex < 0 && iIPChanIndex >= MAX_CHANNUM_V30)
	{
		//g_pMainDlg->AddLog(m_dwDevIndex, OPERATION_FAIL_T, "chanindex[%d] err ip Channel", m_iCurChanNO);
		return;
	}

	m_struIPParaCfgV40.struStreamMode[iIPChanIndex].byGetStreamType = 0; //direct connect device.
	//clear ip channel info
	memset(&(m_struIPParaCfgV40.struStreamMode[iIPChanIndex].uGetStream.struChanInfo), 0, sizeof(NET_DVR_IPCHANINFO));
	//set device infomation
	memset(&(m_struIPParaCfgV40.struIPDevInfo[iIPChanIndex]), 0, sizeof(NET_DVR_IPDEVINFO_V31));

	//g_StringLanType(szLan, "不可用", "Disable");
	m_listIPChan.SetItemText(m_iCurSel, 2, "不可用");
	m_listIPChan.SetItemData(m_iCurSel, IP_CHAN_ENABLE*1000+m_iCurChanNO);

	SetCtrlStat(IP_CHAN_DISABLE);
}

void CIPAccessCfgDlg::OnBnClickedButtonUpdateProList()
{
	CParkingDemoDlg* pDlg = (CParkingDemoDlg*)theApp.GetMainWnd();
	int iDeviceIndex = pDlg->GetCurDeviceIndex();

	CGuard lock(&pDlg->m_csLock);

	int iUserId = pDlg->m_vDevices[iDeviceIndex].iUserId;
	int i = 0;
	char szLan[1024] = {0};

	m_comboProType.ResetContent();
	if (NET_DVR_GetIPCProtoList(iUserId, &m_struProtoList))
	{
		for (i = 0; i < (int)m_struProtoList.dwProtoNum; i++)
		{
			memset(szLan, 0, DESC_LEN);
			memcpy(szLan, m_struProtoList.struProto[i].byDescribe, DESC_LEN);
			m_comboProType.AddString(szLan);	
			m_comboProType.SetItemData(i, m_struProtoList.struProto[i].dwType);
		}
		m_comboProType.SetCurSel(0);
	}  
	else
	{
		//default	
		m_comboProType.AddString("0");
		m_comboProType.SetItemData(0, 0);		
		GetDlgItem(IDC_COMBO_PROTYPE)->EnableWindow(TRUE);
	}

	m_comboProType.SetCurSel(0);
}

void CIPAccessCfgDlg::OnBnClickedBtnSave()
{
	char szLan[128] = {0};

	CParkingDemoDlg* pDlg = (CParkingDemoDlg*)theApp.GetMainWnd();
	int iDeviceIndex = pDlg->GetCurDeviceIndex();

	CGuard lock(&pDlg->m_csLock);
	int iUserId = pDlg->m_vDevices[iDeviceIndex].iUserId;

	if (memcmp(pDlg->m_vDevices[iDeviceIndex].struIPParaCfgV40.byAnalogChanEnable, m_struIPParaCfgV40.byAnalogChanEnable, MAX_ANALOG_CHANNUM) != 0)
	{		
		if (IDOK != MessageBox("修改模拟通道使能状态,设备将自动重启!","Warning",IDOK))
		{
			return;
		}
	}
	memcpy(&pDlg->m_vDevices[iDeviceIndex].struIPParaCfgV40, &m_struIPParaCfgV40, sizeof(m_struIPParaCfgV40));
	if (pDlg->DoSetIPAccessCfg(m_dwDevIndex))
	{
		AfxMessageBox("IPC配置设置成功!请刷新!");	
	}
	else
	{
		AfxMessageBox("IPC配置设置失败!");
	}
}

void CIPAccessCfgDlg::OnBnClickedBtnRefresh()
{
	CheckInitParam();
}

void CIPAccessCfgDlg::ShowStreamUrl(BOOL bShow)
{
// 	GetDlgItem(IDC_STATIC_STREAM_SERVER_URL)->ShowWindow(bShow);
// 	GetDlgItem(IDC_EDIT_STREAM_SERVER_URL)->ShowWindow(bShow);
// 	GetDlgItem(IDC_CHECK_URL_ENABLE)->ShowWindow(bShow);
// 	GetDlgItem(IDC_BTN_STREAM_URL_SET)->ShowWindow(bShow);
// 	GetDlgItem(IDC_STATIC_URLCFG)->ShowWindow(bShow);
// 	GetDlgItem(IDC_COMBO_TRANS_PROTOCALTYPE_URL)->ShowWindow(bShow);
// 	GetDlgItem(IDC_EDIT_CHANNELNUM_URL)->ShowWindow(bShow);
// 	GetDlgItem(IDC_EDIT_DEVICEID_URL)->ShowWindow(bShow);
// 	GetDlgItem(IDC_STATIC_TRANSPORTPROTOCAL_TYPE_URL)->ShowWindow(bShow);
// 	GetDlgItem(IDC_STATIC_DVEICEID_URL)->ShowWindow(bShow);
// 	GetDlgItem(IDC_STATIC_CHANNEL_NUM_URL)->ShowWindow(bShow);
}


void CIPAccessCfgDlg::ShowInfoByGetStreamType(int iType)
{
	int iGroupNO = m_comboChannelGroupNO.GetCurSel();
	int iChannelIndex = m_iCurChanNO - m_struIPParaCfgV40.dwStartDChan - iGroupNO * MAX_CHANNUM_V30;

	switch (iType)
	{
	case 0:  //direct  connect device
		ShowDeviceInfo(TRUE);
// 		m_pDlgIPAccessDDNS->ShowWindow(SW_HIDE);
// 		m_pDlgIPAccessIPServer->ShowWindow(SW_HIDE);
// 		m_pDlgIPAccessStreamServer->ShowWindow(SW_HIDE);
// 		m_pDlgIPAccessCfgEasyDDNS->ShowWindow(SW_HIDE);
		ShowStreamUrl(FALSE);
		break;
	case 1: //Get stream from stream server.
		ShowDeviceInfo(FALSE);
// 		m_pDlgIPAccessStreamServer->ShowWindow(SW_SHOW);
// 		m_pDlgIPAccessIPServer->ShowWindow(SW_HIDE);
// 		m_pDlgIPAccessDDNS->ShowWindow(SW_HIDE);
// 		m_pDlgIPAccessCfgEasyDDNS->ShowWindow(SW_HIDE);
 		ShowStreamUrl(FALSE);
// 		m_pDlgIPAccessStreamServer->m_iChannelIndex = iChannelIndex;
		break;
	case 2: //Get stream from IP server.
		ShowDeviceInfo(FALSE);
// 		m_pDlgIPAccessStreamServer->ShowWindow(SW_HIDE);
// 		m_pDlgIPAccessIPServer->ShowWindow(SW_SHOW);
// 		m_pDlgIPAccessDDNS->ShowWindow(SW_HIDE);
// 		m_pDlgIPAccessCfgEasyDDNS->ShowWindow(SW_HIDE);
		ShowStreamUrl(FALSE);
		break;
	case 3: //Get stream from IP server and stream server.
		ShowDeviceInfo(FALSE);
// 		m_pDlgIPAccessStreamServer->ShowWindow(SW_HIDE);
// 		m_pDlgIPAccessIPServer->ShowWindow(SW_HIDE);
// 		m_pDlgIPAccessDDNS->ShowWindow(SW_SHOW);
// 		m_pDlgIPAccessCfgEasyDDNS->ShowWindow(SW_HIDE);
		ShowStreamUrl(FALSE);
		break;
	case 4: //Get stream from IP server and stream server.
		ShowDeviceInfo(TRUE);//
// 		m_pDlgIPAccessStreamServer->ShowWindow(SW_HIDE);
// 		m_pDlgIPAccessIPServer->ShowWindow(SW_HIDE);
// 		m_pDlgIPAccessDDNS->ShowWindow(SW_HIDE);
// 		m_pDlgIPAccessCfgEasyDDNS->ShowWindow(SW_HIDE);
		ShowStreamUrl(TRUE);
		break;
	case 5: //Get stream from EasyDDNS.
		ShowDeviceInfo(FALSE);
// 		m_pDlgIPAccessStreamServer->ShowWindow(SW_HIDE);
// 		m_pDlgIPAccessIPServer->ShowWindow(SW_HIDE);
// 		m_pDlgIPAccessDDNS->ShowWindow(SW_HIDE);
// 		m_pDlgIPAccessCfgEasyDDNS->ShowWindow(SW_SHOW);
		ShowStreamUrl(FALSE);
		break;
	default: //same as case 0;
		ShowDeviceInfo(TRUE);
// 		m_pDlgIPAccessDDNS->ShowWindow(SW_HIDE);
// 		m_pDlgIPAccessIPServer->ShowWindow(SW_HIDE);
// 		m_pDlgIPAccessStreamServer->ShowWindow(SW_HIDE);
// 		m_pDlgIPAccessCfgEasyDDNS->ShowWindow(SW_HIDE);
		ShowStreamUrl(FALSE);
		break;
	}
}

void CIPAccessCfgDlg::ShowDeviceInfo(BOOL bShow)
{
	//static
	GetDlgItem(IDC_STATIC_DEVICE)->ShowWindow(bShow);
	GetDlgItem(IDC_STATIC_IP)->ShowWindow(bShow);
	GetDlgItem(IDC_STATIC_PORT)->ShowWindow(bShow);
	GetDlgItem(IDC_STATIC_CHANNEL)->ShowWindow(bShow);
	GetDlgItem(IDC_STATIC_USER)->ShowWindow(bShow);
	GetDlgItem(IDC_STATIC_PASS)->ShowWindow(bShow);
	GetDlgItem(IDC_STATIC_DEV_TYPE)->ShowWindow(bShow);

	//edit
	GetDlgItem(IDC_DEVICE_IP)->ShowWindow(bShow);
	GetDlgItem(IDC_DEVICE_PORT)->ShowWindow(bShow);
	GetDlgItem(IDC_EDIT_CHAN)->ShowWindow(bShow);
	GetDlgItem(IDC_DEVICE_USERNAME)->ShowWindow(bShow);
	GetDlgItem(IDC_DEVICE_PSW)->ShowWindow(bShow);
	GetDlgItem(IDC_COMBO_PROTYPE)->ShowWindow(bShow);
	GetDlgItem(IDC_BUTTON_UPDATE_PRO_LIST)->ShowWindow(bShow);
	//GetDlgItem(IDC_BUTTON_CUSTOM_PRO)->ShowWindow(bShow);

	//btn
	GetDlgItem(IDC_BTN_ENABLE)->ShowWindow(bShow);
	GetDlgItem(IDC_BTN_ADD_IP_CHAN)->ShowWindow(bShow);
	GetDlgItem(IDC_BTN_DELETE)->ShowWindow(bShow);
}

void g_StringLanType(char *szDstLan, char *szLanCn, char *szLanEn)
{
//#ifdef DEMO_LAN_CN
//	sprintf(szDstLan, "%s", szLanCn);	
	strcpy(szDstLan, szLanCn);	
//#else	
//	sprintf(szDstLan, "%s", szLanEn);
//#endif
	return;
	WORD wLangID=(WORD)(PRIMARYLANGID(::GetSystemDefaultLangID()));
	switch(wLangID)
	{
	case LANG_ENGLISH:
		sprintf(szDstLan, "%s", szLanEn);
		break;
	case LANG_CHINESE:
		sprintf(szDstLan, "%s", szLanCn);
		break;
	default:
		sprintf(szDstLan, "%s", szLanEn);
		break;
	}	
}

void CIPAccessCfgDlg::ShowIsEnableOrOnlineInList(int iRowIndex, int iColumnIndex, int iArrayIndex)
{
	char szLan[128] = {0};
	BOOL bEnable = FALSE;

	//3.whether enable?
	switch (m_struIPParaCfgV40.struStreamMode[iArrayIndex].byGetStreamType)
	{
	case 0: //direct device
		bEnable = m_struIPParaCfgV40.struStreamMode[iArrayIndex].uGetStream.struChanInfo.byIPID;
		break;
	case 1:
		bEnable = m_struIPParaCfgV40.struStreamMode[iArrayIndex].uGetStream.struPUStream.struStreamMediaSvrCfg.byValid;
		break;
	case 2:
		bEnable = m_struIPParaCfgV40.struStreamMode[iArrayIndex].uGetStream.struIPServerStream.byEnable;
		break;
	case 3:
		bEnable = m_struIPParaCfgV40.struStreamMode[iArrayIndex].uGetStream.struDDNSStream.byEnable;
		break;
	case 4:
		bEnable = m_struIPParaCfgV40.struStreamMode[iArrayIndex].uGetStream.struStreamUrl.byEnable;
		break;
	default:
		break;
	}

	if (bEnable)
	{
		m_listIPChan.SetItemData(iRowIndex, IP_CHAN_ENABLE*1000+iArrayIndex + m_struIPParaCfgV40.dwStartDChan);
	} 
	else
	{
		m_listIPChan.SetItemData(iRowIndex, IP_CHAN_DISABLE*1000+iArrayIndex + m_struIPParaCfgV40.dwStartDChan);
	}

	if (0 == m_struIPParaCfgV40.struStreamMode[iArrayIndex].byGetStreamType && bEnable)
	{
		if (m_struIPParaCfgV40.struStreamMode[iArrayIndex].uGetStream.struChanInfo.byEnable)
		{
			//g_StringLanType(szLan, "在线", "online");
			//GetIsEnableString(bEnable, "在线");
			m_listIPChan.SetItemText(iRowIndex, iColumnIndex, "在线");
		}
		else
		{
			//g_StringLanType(szLan, "离线", "offline");
			//GetIsEnableString(bEnable, "离线");
			m_listIPChan.SetItemText(iRowIndex, iColumnIndex, "离线");
		}
	}
	else
	{
		//GetIsEnableString(bEnable, szLan);
		if (bEnable)
		{
			//g_StringLanType(pStrEnable, "可用", "Enable");
			m_listIPChan.SetItemText(iRowIndex, iColumnIndex, "可用");
		}
		else
		{
			//g_StringLanType(pStrEnable, "不可用", "Disable");
			m_listIPChan.SetItemText(iRowIndex, iColumnIndex, "不可用");
		}
	}
	//m_listIPChan.SetItemText(iRowIndex, iColumnIndex, szLan);	
}

void CIPAccessCfgDlg::GetIsEnableString(BOOL bEnable, char *pStrEnable)
{
	if (bEnable)
	{
		g_StringLanType(pStrEnable, "可用", "Enable");
	}
	else
	{
		g_StringLanType(pStrEnable, "不可用", "Disable");
	}
}

void CIPAccessCfgDlg::SetCtrlStat(int iStat)
{
	DWORD dwID = 0;
	char szLan[128] = {0};
	int i = 0;
	switch(iStat)
	{
	case ANALOG_CHAN_DISABLE://analog disable
		GetDlgItem(IDC_BTN_ADD_IP_CHAN)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_DELETE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_ENABLE)->EnableWindow(TRUE);		
		GetDlgItem(IDC_BTN_ENABLE)->SetWindowText("启用");
		EnableInfoCtrl(FALSE);
		m_bCurEnable = FALSE;
		SetChannelInfobyStreamType(TRUE);
		break;
	case ANALOG_CHAN_ENABLE://analog enable channel
		GetDlgItem(IDC_BTN_ADD_IP_CHAN)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_DELETE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_ENABLE)->EnableWindow(TRUE);
		//g_StringLanType(szLan, "禁用", "Disable");	
		GetDlgItem(IDC_BTN_ENABLE)->SetWindowText("禁用");
		EnableInfoCtrl(FALSE);
		m_bCurEnable = TRUE;
		SetChannelInfobyStreamType(TRUE);
		break;
	case IP_CHAN_DISABLE://digital disable channel
		GetDlgItem(IDC_BTN_ADD_IP_CHAN)->EnableWindow(TRUE);
		//g_StringLanType(szLan, "添加", "Add");
		GetDlgItem(IDC_BTN_ADD_IP_CHAN)->SetWindowText("添加");
		GetDlgItem(IDC_BTN_DELETE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_ENABLE)->EnableWindow(FALSE);
		//GetDlgItem(IDC_BTN_MOD)->EnableWindow(FALSE);
		EnableInfoCtrl(TRUE);
		m_bCurEnable = FALSE;
		SetChannelInfobyStreamType(FALSE);
		break;
	case IP_CHAN_ENABLE://digital enable
		GetDlgItem(IDC_BTN_ADD_IP_CHAN)->EnableWindow(TRUE);
		//g_StringLanType(szLan, "修改", "Modify");
		GetDlgItem(IDC_BTN_ADD_IP_CHAN)->SetWindowText("修改");
		GetDlgItem(IDC_BTN_DELETE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_ENABLE)->EnableWindow(FALSE);

		//GetDlgItem(IDC_BTN_MOD)->EnableWindow(TRUE);
		EnableInfoCtrl(TRUE);
		m_bCurEnable = TRUE;
		SetChannelInfobyStreamType(FALSE);
		break;
	default:
		break;
	}

	//Set info by the type of getting stream.
	UpdateData(FALSE);
}

void CIPAccessCfgDlg::EnableInfoCtrl(BOOL bEnable)
{
	GetDlgItem(IDC_DEVICE_IP)->EnableWindow(bEnable);
	GetDlgItem(IDC_DEVICE_PORT)->EnableWindow(bEnable);
	GetDlgItem(IDC_DEVICE_PSW)->EnableWindow(bEnable);
	GetDlgItem(IDC_DEVICE_USERNAME)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_CHAN)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_UPDATE_PRO_LIST)->EnableWindow(bEnable);
	//GetDlgItem(IDC_BUTTON_CUSTOM_PRO)->EnableWindow(bEnable);
}

void CIPAccessCfgDlg::SetChannelInfobyStreamType(BOOL bAnalog)
{
	CParkingDemoDlg *pDlg = (CParkingDemoDlg*)theApp.GetMainWnd();
	ASSERT(pDlg != NULL);

	if (m_iCurChanNO < 0)
	{
		return;
	}

	if (bAnalog)
	{
		ShowInfoByGetStreamType(0);
		m_comboStreamType.EnableWindow(FALSE);
		return;
	}

	m_comboStreamType.EnableWindow(TRUE);
	int iGroupNO = m_comboChannelGroupNO.GetCurSel();
	int iIPChanIndex =  0; //Array index in v40.
	iIPChanIndex = m_iCurChanNO - m_struIPParaCfgV40.dwStartDChan - iGroupNO * MAX_CHANNUM_V30;
	if (iIPChanIndex < 0)
	{		
		return;
	}

	int iStreamType  = m_struIPParaCfgV40.struStreamMode[iIPChanIndex].byGetStreamType;
	ShowInfoByGetStreamType(iStreamType);

	//the combo of Stream type.
	m_comboStreamType.SetCurSel(iStreamType);

	//set information
	switch(iStreamType)
	{
	case 0: //direct get stream from the device
		if (iIPChanIndex >= 0)
		{
			int dwID = m_struIPParaCfgV40.struStreamMode[iIPChanIndex].uGetStream.struChanInfo.byIPID + \
				256 * m_struIPParaCfgV40.struStreamMode[iIPChanIndex].uGetStream.struChanInfo.byIPIDHigh;
			if (dwID > 0 && dwID <= MAX_IP_DEVICE_V40)
			{
				if (m_struProtoList.dwProtoNum < IPC_PROTOCOL_NUM)
				{
					for (int i = 0; i<m_struProtoList.dwProtoNum; i++)
					{
						if (m_struIPParaCfgV40.struIPDevInfo[dwID-1].byProType == m_struProtoList.struProto[i].dwType)
						{
							m_comboProType.SetCurSel(i); //protocol	
							break;
						}
					}
				}
				m_csDevIP.Format("%s", GetDeviceAddress(&(m_struIPParaCfgV40.struIPDevInfo[dwID-1]))); //IP
				m_uDevPort = m_struIPParaCfgV40.struIPDevInfo[dwID-1].wDVRPort;
				m_iIPDevChan = m_struIPParaCfgV40.struStreamMode[iIPChanIndex].uGetStream.struChanInfo.byChannel;
			}
		}
		break;
	case 1: //Get stream from stream server.  
		//m_pDlgIPAccessStreamServer->UpdateParams();
		break;
	case 2: //Get stream from IP server.
		//m_pDlgIPAccessIPServer->UpdateParams();
		break;
	case 3: //Get stream from IP server and stream server.
		//m_pDlgIPAccessDDNS->UpdateParams();
		break;
	case 4: //Get stream from stream server by url.
		//m_csStreamServerUrl.Format("%s", m_struIPParaCfgV40.struStreamMode[iIPChanIndex].uGetStream.struStreamUrl.strURL);
		m_bStreamUrlEnable = m_struIPParaCfgV40.struStreamMode[iIPChanIndex].uGetStream.struStreamUrl.byEnable; 
		break;
	case 5: //Get stream from EasyDDNS.
		//m_pDlgIPAccessCfgEasyDDNS->UpdateParams();
		break;
	default: 
		break;
	}
	UpdateData(FALSE);
}