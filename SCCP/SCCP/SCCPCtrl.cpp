// SCCPCtrl.cpp : Implementation of the CSCCPCtrl ActiveX Control class.

#include "stdafx.h"
//#define NETVIDEO
#ifndef NETVIDEO
#include <Vfw.h>
#pragma comment(lib,"vfw32")
#endif
#include "SCCP.h"
#include "SCCPCtrl.h"
#include "SCCPPropPage.h"
#include "IntelConfig.h"
#include "SadpDlg.h"
#include <atldbcli.h>
//LONG volatile g_callback = 0;
//LONG volatile g_Passproc = 0;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CSCCPCtrl, COleControl)



// Message map

BEGIN_MESSAGE_MAP(CSCCPCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
	ON_WM_DESTROY()
	ON_MESSAGE(WM_USER+1,OnRecieve)
//	ON_WM_CLOSE()
//ON_WM_CLOSE()
END_MESSAGE_MAP()



// Dispatch map

BEGIN_DISPATCH_MAP(CSCCPCtrl, COleControl)
	DISP_FUNCTION_ID(CSCCPCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CSCCPCtrl, "OpenNet", dispidOpenNet, OpenNet, VT_I4, VTS_BSTR VTS_I4)
	DISP_FUNCTION_ID(CSCCPCtrl, "OpenCom", dispidOpenCom, OpenCom, VT_I4, VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	DISP_FUNCTION_ID(CSCCPCtrl, "Close", dispidClose, Close, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CSCCPCtrl, "SetUp", dispidSetUp, SetUp, VT_I4, VTS_I4)
	DISP_FUNCTION_ID(CSCCPCtrl, "Config", dispidConfig, Config, VT_I4, VTS_I4)
	DISP_FUNCTION_ID(CSCCPCtrl, "StarVideo", dispidStarVideo, StarVideo, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CSCCPCtrl, "StopVideo", dispidStopVideo, StopVideo, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CSCCPCtrl, "GetStatus", dispidGetStatus, GetStatus, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CSCCPCtrl, "GetPlate", dispidGetPlate, GetPlate, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CSCCPCtrl, "GetPlateColor", dispidGetPlateColor, GetPlateColor, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CSCCPCtrl, "SaveImage", dispidSaveImage, SaveImage, VT_I4, VTS_BSTR VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	DISP_FUNCTION_ID(CSCCPCtrl, "SavePlateImage", dispidSavePlateImage, SavePlateImage, VT_I4, VTS_BSTR VTS_BSTR)
	DISP_PROPERTY_NOTIFY_ID(CSCCPCtrl, "RecvPlateImageFlag", dispidRecvPlateImageFlag, m_RecvPlateImageFlag, OnRecvPlateImageFlagChanged, VT_I4)
	DISP_PROPERTY_NOTIFY_ID(CSCCPCtrl, "RecvImageFlag", dispidRecvImageFlag, m_RecvImageFlag, OnRecvImageFlagChanged, VT_I4)
	DISP_PROPERTY_NOTIFY_ID(CSCCPCtrl, "RecvPlateBinImageFlag", dispidRecvPlateBinImageFlag, m_RecvPlateBinImageFlag, OnRecvPlateBinImageFlagChanged, VT_I4)
	DISP_PROPERTY_NOTIFY_ID(CSCCPCtrl, "LogFile", dispidLogFile, m_LogFile, OnLogFileChanged, VT_I4)
	DISP_FUNCTION_ID(CSCCPCtrl, "SavePlateBinImage", dispidSavePlateBinImage, SavePlateBinImage, VT_I4, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION_ID(CSCCPCtrl, "Trigger", dispidTrigger, Trigger, VT_I4, VTS_NONE)
	DISP_PROPERTY_NOTIFY_ID(CSCCPCtrl, "DeviceInitOk", dispidDeviceInitOk, m_DeviceInitOk, OnDeviceInitOkChanged, VT_BOOL)
	DISP_FUNCTION_ID(CSCCPCtrl, "GetIPAddr", dispidGetIPAddr, GetIPAddr, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CSCCPCtrl, "GetPort", dispidGetPort, GetPort, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CSCCPCtrl, "SetSFZ", dispidSetSFZ, SetSFZ, VT_I4, VTS_BSTR)
END_DISPATCH_MAP()



// Event map

BEGIN_EVENT_MAP(CSCCPCtrl, COleControl)
	EVENT_CUSTOM_ID("OnPlateReceive", eventidOnPlateReceive, OnPlateReceive, VTS_NONE)
END_EVENT_MAP()



// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CSCCPCtrl, 1)
	PROPPAGEID(CSCCPPropPage::guid)
END_PROPPAGEIDS(CSCCPCtrl)



// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CSCCPCtrl, "SCCP.SCCPCtrl.1",
	0x7a8c7463, 0x738c, 0x4f63, 0xab, 0x50, 0x31, 0x64, 0x23, 0x96, 0xf, 0x6f)



// Type library ID and version

IMPLEMENT_OLETYPELIB(CSCCPCtrl, _tlid, _wVerMajor, _wVerMinor)



// Interface IDs

const IID BASED_CODE IID_DSCCP =
		{ 0x804DAB8E, 0x7240, 0x41B9, { 0xB1, 0x12, 0x9E, 0x16, 0xF7, 0x7C, 0xD8, 0x12 } };
const IID BASED_CODE IID_DSCCPEvents =
		{ 0x81E3F915, 0x54F, 0x49B6, { 0xA3, 0x68, 0xFE, 0xB, 0x12, 0x3D, 0x5D, 0x9C } };



// Control type information

static const DWORD BASED_CODE _dwSCCPOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CSCCPCtrl, IDS_SCCP, _dwSCCPOleMisc)



// CSCCPCtrl::CSCCPCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CSCCPCtrl

BOOL CSCCPCtrl::CSCCPCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegApartmentThreading to 0.

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_SCCP,
			IDB_SCCP,
			afxRegApartmentThreading,
			_dwSCCPOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


void CALLBACK MessageCallback (LONG lCommand, NET_DVR_ALARMER *pAlarmer, char *pAlarmInfo, DWORD dwBufLen, void* pUser)
{
	CSCCPCtrl	*pMian	= (CSCCPCtrl*)pUser;
	if (pMian == NULL)
	{
		return ;
	}
	if (lCommand != COMM_UPLOAD_PLATE_RESULT)
	{
		//不是车牌直接返回
		pMian->WriteLogFile((CA2T)__FUNCTION__, TRUE, _T("回调不是车牌命令，直接返回！"));
		return ;
	}
	if (pAlarmInfo == NULL)
	{
		pMian->WriteLogFile((CA2T)__FUNCTION__, TRUE, _T("布控信息为空，直接返回！"));
		return;
	}
	LPNET_DVR_PLATE_RESULT	pPlate = (LPNET_DVR_PLATE_RESULT)pAlarmInfo;

	pMian->InsertNewPlate(pPlate);
	
}




// CSCCPCtrl::CSCCPCtrl - Constructor



CString		g_strSCCPIP = _T("127.0.0.1");
CString		g_strSCCPPort = _T("");


CSCCPCtrl::CSCCPCtrl()
{
	m_RecvPlateImageFlag		= 1;
	m_RecvPlateBinImageFlag		= 1;
	m_RecvImageFlag				= 1;
	m_DeviceInitOk				= TRUE;
	m_LogFile					= 0;
	m_hMutexLogFile				= NULL;
	m_hLogFile					= NULL;
	m_lAlarmHandle				= -1;
	m_lPreviewHandle			= -1;
	m_lUserID					= -1;
	m_strPort					= _T("");
	m_strIPAddr					= _T("");
	m_hSadpModule = NULL;
	m_pfnSadpStop = NULL;
	m_pfnSadpStart = NULL;
	m_pfnSadpSendUpdateIP = NULL;
	InitializeIIDs(&IID_DSCCP, &IID_DSCCPEvents);
	m_cBufferManager.InitBufferManage();
	if (m_LogFile == 1)
	{
		CreateLogFile();
	}
	InitCtrl();
}



// CSCCPCtrl::~CSCCPCtrl - Destructor

CSCCPCtrl::~CSCCPCtrl()
{
	// TODO: Cleanup your control's instance data here.
	if(m_hSadpModule != NULL)
	{
		FreeLibrary(m_hSadpModule);
		m_hSadpModule = NULL;
	}
}



// CSCCPCtrl::OnDraw - Drawing function

void CSCCPCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;

	// TODO: Replace the following code with your own drawing code.
	if (m_lPreviewHandle < 0)
	{
		// 没有预览的时候，显示黑色
		pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(BLACK_BRUSH)));
	}
	
}



// CSCCPCtrl::DoPropExchange - Persistence support

void CSCCPCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);
	// TODO: Call PX_ functions for each persistent custom property.
}



// CSCCPCtrl::OnResetState - Reset control to default state

void CSCCPCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}



// CSCCPCtrl::AboutBox - Display an "About" box to the user

void CSCCPCtrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_SCCP);
	dlgAbout.DoModal();
}

LRESULT CSCCPCtrl::OnRecieve(WPARAM wParam, LPARAM lParam)
{
	OnPlateReceive();
	return 0;
}

/** @fn		VOID CSCCPCtrl::SetUpAlarm()
 *	@brief	设置布防
 *	@return	LONG	0表示成功，否则失败
 */
LONG CSCCPCtrl::SetUpAlarm()
{
	BOOL	bRet = FALSE;
	//设置布防回调函数
	bRet = NET_DVR_SetDVRMessageCallBack_V30(MessageCallback, this);
	if (bRet == FALSE)
	{
		WriteLogFile((CA2T)__FUNCTION__, TRUE, _T("设置布防回调函数失败"));
		return RETVAL_SET_ALARM_CALL_BACK_FAIL;
	}
	//开启布防
	if (m_lUserID >= 0)
	{
		m_lAlarmHandle = NET_DVR_SetupAlarmChan_V30(m_lUserID);
		if (m_lAlarmHandle < 0)
		{
			WriteLogFile((CA2T)__FUNCTION__, TRUE, _T("开启布防失败"));
			return RETVAL_DEPLOYMEMT_FAIL;
		}
	}
	WriteLogFile((CA2T)__FUNCTION__, FALSE, _T("开启布防"));
	return RETVAL_OK;
}

/** @fn		LONG CSCCPCtrl::CleanAlarm()
 *	@brief	撤销布防
 *	@return	LONG	0表示成功， 否则失败
 */
LONG CSCCPCtrl::CleanAlarm()
{
	BOOL	bRet = FALSE;

	if (m_lAlarmHandle < 0)
	{
		return RETVAL_OK;
	}

	bRet = NET_DVR_CloseAlarmChan_V30(m_lAlarmHandle);
	if (bRet == FALSE)
	{
		WriteLogFile((CA2T)__FUNCTION__, TRUE, _T("撤销布防失败"));
		return RETVAL_CLEAN_ALARM_FAIL;
	}
	m_lAlarmHandle = -1;
	WriteLogFile((CA2T)__FUNCTION__, FALSE, _T("撤销布防"));
	return RETVAL_OK;
}

/** @fn		BOOL CSCCPCtrl::SavePic(LPCTSTR lpPath, LPCTSTR lpPicName, BYTE * pImage, DWORD dwPicDataLen)
 *	@brief	保存图片
 *	@param	[IN]	lpPath		图片路径
 *	@param	[IN]	lpPicName	图片名字
 *	@param	[IN]	pImage		图片内容
 *	@param	[IN]	dwPicDataLen图片长度
 *	@return	BOOL	true表示成功，否则失败
 */
BOOL CSCCPCtrl::SavePic(LPCTSTR lpPath, LPCTSTR lpPicName, BYTE * pImage, DWORD dwPicDataLen)
{
	if (pImage == NULL || dwPicDataLen <= 0 
		|| lpPath == NULL || lpPicName == NULL)
	{
		return FALSE;
	}
	
	CString csPath(lpPath);
	//如果以"\"结尾,删除"\"
	if (csPath.GetAt(csPath.GetLength() - 1) == _T('\\'))
	{
		csPath.Delete(csPath.GetLength() - 1);
	}
	//完整路径
	CString csFullPath;
	csFullPath.Format(_T("%s\\%s"), csPath, lpPicName);
	if (csFullPath.GetLength() >= MAX_PATH)
	{ 
		AfxMessageBox(_T("图片路径过长，抓图失败"));
		return FALSE;
	}
	//将路径名中的？改成#号
	csFullPath.Replace('?', '#');
	if (CreateFileDir(csFullPath) == FALSE)
	{
		return FALSE;
	}
	//写图片文件
	FILE* file;
	try
	{
		_tfopen_s(&file,csFullPath, _T("wb"));
		if (file == NULL) 
		{
			AfxMessageBox(_T("创建图片文件失败！"));
			return false;
		}
		fwrite(pImage, dwPicDataLen, 1, file);
		fclose(file);
	}
	catch (...)
	{
		if (file != NULL) 
		{
			fclose(file);
		}
		AfxMessageBox(_T("保存图片文件出错"));
		return false;
	}
	return true;
}

/** @fn		void CSCCPCtrl::InsertNewPlate(LPNET_DVR_PLATE_RESULT pPlateInfo)
 *	@brief	插入新车牌信息
 *	@param	[IN]	pPlateInfo		车牌信息
 *	@return	void
 */
void CSCCPCtrl::InsertNewPlate(LPNET_DVR_PLATE_RESULT pPlateInfo)
{
	if (pPlateInfo == NULL)
	{
		WriteLogFile((CA2T)__FUNCTION__, FALSE, _T("收到新的车牌信息为空,回调处"));
		return ;
	}
	if (m_cBufferManager.InsertPlateInfo(pPlateInfo) == true)
	{
		WriteLogFile((CA2T)__FUNCTION__, FALSE, _T("收到新的车牌信息并存入缓存,回调处"));
		::PostMessage(this->m_hWnd,WM_USER+1,NULL,NULL);
		//InterlockedIncrement(&g_callback);
		//Sleep(10);
		//if(g_callback == 1)
		//{
		//	
		//	InterlockedDecrement(&g_callback);
		//}
		return;
	}
	WriteLogFile((CA2T)__FUNCTION__, TRUE, _T("收到新的车牌信息但是存入缓存失败,回调处"));	
}

// CSCCPCtrl message handlers
/** @fn		LONG CSCCPCtrl::OpenNet(LPCTSTR pszIP, LONG nPort)
 *	@brief	登录设备
 *	@param	[IN]	pszIP	设备IP
 *	@param	[IN]	nPort	端口
 *	@return	0表示成功，否则失败
 */

LONG CSCCPCtrl::OpenNet(LPCTSTR pszIP, LONG nPort)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	LONG		lRetVal = RETVAL_OK;
	
	if (m_DeviceInitOk == FALSE)
	{
		WriteLogFile((CA2T)__FUNCTION__, TRUE, _T("设备工作不正常"));
		return RETVAL_DEVICE_CONNECT_ERROR;
	}
	if (pszIP == NULL)
	{
		WriteLogFile((CA2T)__FUNCTION__,TRUE, _T("无效IP地址"));
		return RETVAL_INVALID_PARAM;
	}
	
	lRetVal = Close();
	if (lRetVal < RETVAL_OK)
	{
		return lRetVal;
	}

	memset(&m_stDeviceInfo, 0, sizeof(m_stDeviceInfo));
	m_lUserID = NET_DVR_Login_V30((CT2A)pszIP, (WORD)nPort, "admin", "12345", &m_stDeviceInfo);
	if (m_lUserID == -1)
	{
		WriteLogFile((CA2T)__FUNCTION__, TRUE, _T("登录设备失败"));
		return RETVAL_LOGIN_ERROR;
	}
	m_strIPAddr.Format(_T("%s"), pszIP);
	m_strPort.Format(_T("%d"), nPort);
	memset(&m_stDevLogInfo, 0, sizeof(m_stDevLogInfo));
	m_stDevLogInfo.lConnectType	= 0;
	m_stDevLogInfo.wDevPort		= (WORD)nPort;
	m_stDevLogInfo.iChanNum		= 1;
	sprintf_s(m_stDevLogInfo.chDevIP,16, "%s", (CT2A)pszIP);
	sprintf_s(m_stDevLogInfo.chUserName,NAME_LEN, "%s", "admin");
	sprintf_s(m_stDevLogInfo.chPassword,PASSWD_LEN, "%s", "12345");
	WriteLogFile((CA2T)__FUNCTION__, FALSE, _T("登录设备"));
	return SetUpAlarm();
}

/** @fn		LONG CSCCPCtrl::OpenCom(LONG nComPort, LONG nBaudRate, LONG nParity, LONG nByteSize, LONG nStopBit)
 *	@brief	串口配置（暂时不做）
 *	@param	[IN]	nCommPor	串口号
 *	@param	[IN]	nBaudRate	波特率
 *	@param	[IN]	nParity		校验值
 *	@param	[IN]	nByteSize	数据位
 *	@param	[IN]	nStopBit	停止位
 *	@return	LONG	成功返回0， 否则失败
 */
LONG CSCCPCtrl::OpenCom(LONG nComPort, LONG nBaudRate, LONG nParity, LONG nByteSize, LONG nStopBit)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	WriteLogFile((CA2T)__FUNCTION__, FALSE, _T("暂不支持该接口"));

	return RETVAL_OK;
}

/** @fn		LONG CSCCPCtrl::Close(void)
 *	@brief	关闭设备
 *	@return	0表示成功，否则失败
 */
LONG CSCCPCtrl::Close(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	LONG		lRetVal = RETVAL_OK;

	lRetVal	= StopVideo();
	if (lRetVal != RETVAL_OK)
	{
		return lRetVal;
	}
	if (m_lUserID < 0)
	{
		Invalidate(TRUE);
		return RETVAL_OK;		
	}
	BOOL  bRet = NET_DVR_Logout(m_lUserID);
	if (!bRet)
	{
		WriteLogFile((CA2T)__FUNCTION__, TRUE, _T("关闭设备失败"));
		return RETVAL_LOGOUT_ERROR;
	}
	m_lUserID = -1;
	Invalidate(TRUE);
	WriteLogFile((CA2T)__FUNCTION__, FALSE, _T("关闭设备"));
	return CleanAlarm();	
}

/** @fn		LONG CSCCPCtrl::SetUp(LONG nTypeId)
 *	@biref	显示高级设置对话框，
 *	@param	[IN]	nTypeId	设备类型
 *	@return	LONG	0表示成功，否则失败
 */
LONG CSCCPCtrl::SetUp(LONG nTypeId)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (m_lUserID < 0)
	{
		WriteLogFile((CA2T)__FUNCTION__, TRUE, _T("设备没有登录"));
		return RETVAL_DEVICE_NOT_LOGIN;
	}
	WriteLogFile((CA2T)__FUNCTION__, FALSE, _T("开启智能配置对话框"));
	CIntelConfig dlg;
	dlg.SetIntelCfgDeviceInfo(&m_stDevLogInfo);
	if (dlg.DoModal() == IDOK)
	{
		return RETVAL_OK;
	}
	return RETVAL_OK;

	
}

/** @fn		LONG CSCCPCtrl::Config(LONG nTypeId)
 *	@brief	显示基本设置对话框
 *	@param	[IN]	nTypeID	设备类型，暂时不用
 *	@return	LONG	0表示成功，否则失败
 */
LONG CSCCPCtrl::Config(LONG nTypeId)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	WriteLogFile((CA2T)__FUNCTION__, FALSE, _T("开启基本配置对话框"));
 	CSadpDlg	dlg(m_hSadpModule);
 
 	if (dlg.DoModal() == IDOK)
	{
		m_strIPAddr = dlg.GetSelDeviceIP();
		m_strPort.Format(_T("%d"), dlg.GetSelDevicePort());
	}
	
	return RETVAL_OK;
}


#ifndef NETVIDEO
int g_iStartVideoFlag = 0;
/** @fn		LONG CSCCPCtrl::StarVideo(void)
 *	@brief	显示视频流
 *	@return	0表示成功，否则失败
 */
LONG CSCCPCtrl::StarVideo(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	LONG	lRetVal = RETVAL_OK;

	//if (m_lUserID < 0)
	//{
	//	//设备没有登录
	//	WriteLogFile((CA2T)__FUNCTION__, TRUE, _T("设备没有登录"));
	//	return RETVAL_DEVICE_NOT_LOGIN;
	//}
	if(g_iStartVideoFlag == 1)
	{
		return RETVAL_STOP_REALPLAY_ERROR;
	}
	// 打开预览
	//NET_DVR_CLIENTINFO		stClientInfo = {0};

	//stClientInfo.hPlayWnd		= m_hWnd;
	//stClientInfo.lChannel		= 1;
	//stClientInfo.lLinkMode		= 0;
	//stClientInfo.sMultiCastIP	= NULL;

	//m_hPreviewHandle = NET_DVR_RealPlay_V30(m_lUserID, &stClientInfo, NULL, NULL, 0);
	CRect rect;
	::GetClientRect(GetSafeHwnd(),&rect);

	m_hPreviewHandle = capCreateCaptureWindow(L"My PC VGA",WS_CHILD|WS_VISIBLE,
																			0,0,rect.Width(),rect.Height(),
																			GetSafeHwnd(),0);

	if (m_hPreviewHandle == NULL)
	{
		WriteLogFile((CA2T)__FUNCTION__, TRUE, _T("创建捕获窗口失败"));
		return -14;
	}
	if(!capDriverConnect(m_hPreviewHandle,0))
	{
		WriteLogFile((CA2T)__FUNCTION__, TRUE, _T("连接设备失败"));
		return -15;
	}
	CAPDRIVERCAPS caps;
	capDriverGetCaps(m_hPreviewHandle,&caps,sizeof(CAPDRIVERCAPS));
	if(!caps.fCaptureInitialized)
	{
		WriteLogFile((CA2T)__FUNCTION__, TRUE, _T("获取设备能力失败"));
		return -16;
	}
	CAPSTATUS status;
	capGetStatus(m_hPreviewHandle,&status,sizeof(CAPSTATUS));
	//if(caps.fHasDlgVideoFormat)
	//{
	//	capDlgVideoFormat(m_hPreviewHandle); 
	//}
	CAPTUREPARMS capParms;
	capCaptureGetSetup(m_hPreviewHandle,&capParms,sizeof(CAPTUREPARMS));
	capParms.dwRequestMicroSecPerFrame = 40000;
	capCaptureSetSetup(m_hPreviewHandle,&capParms,sizeof(CAPTUREPARMS));
	capPreviewScale(m_hPreviewHandle,TRUE);
	capPreviewRate(m_hPreviewHandle,40);
	capPreview(m_hPreviewHandle,TRUE);
	WriteLogFile((CA2T)__FUNCTION__, FALSE, _T("开启视频流"));
	g_iStartVideoFlag = 1;
	return RETVAL_OK;
}

/** @fn		LONG CSCCPCtrl::StopVideo(void)
 *	@brief	关闭视频流
 *	@return	LONG	0表示成功，否则失败
 */
LONG CSCCPCtrl::StopVideo(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your dispatch handler code here
	if (m_hPreviewHandle < 0)
	{
		Invalidate(TRUE);
		return RETVAL_OK;
	}
	//BOOL	bRet = FALSE;
	//bRet = NET_DVR_StopRealPlay(m_lPreviewHandle);
	capPreview(m_hPreviewHandle,FALSE);
	capDriverDisconnect(m_hPreviewHandle);
	m_hPreviewHandle = NULL;
	//m_lPreviewHandle = -1;
	Invalidate(TRUE);
	WriteLogFile((CA2T)__FUNCTION__, FALSE, _T("关闭视频流"));
	g_iStartVideoFlag = 0;
	return RETVAL_OK;
	//WriteLogFile((CA2T)__FUNCTION__, TRUE, _T("关闭视频流失败"));
	//return RETVAL_STOP_REALPLAY_ERROR;
}

#else
/** @fn		LONG CSCCPCtrl::StarVideo(void)
 *	@brief	显示视频流
 *	@return	0表示成功，否则失败
 */
LONG CSCCPCtrl::StarVideo(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	LONG	lRetVal = RETVAL_OK;

	if (m_lUserID < 0)
	{
		//设备没有登录
		WriteLogFile((CA2T)__FUNCTION__, TRUE, _T("设备没有登录"));
		return RETVAL_DEVICE_NOT_LOGIN;
	}
	lRetVal = StopVideo();
	if (lRetVal < RETVAL_OK)
	{
		return lRetVal;
	}
	// 打开预览
	NET_DVR_CLIENTINFO		stClientInfo = {0};

	stClientInfo.hPlayWnd		= m_hWnd;
	stClientInfo.lChannel		= 1;
	stClientInfo.lLinkMode		= 0;
	stClientInfo.sMultiCastIP	= NULL;

	m_lPreviewHandle = NET_DVR_RealPlay_V30(m_lUserID, &stClientInfo, NULL, NULL, 0);
	if (m_lPreviewHandle < 0)
	{
		WriteLogFile((CA2T)__FUNCTION__, TRUE, _T("开启视频流失败"));
		return RETVAL_REALPLAY_ERROR;
	}
	WriteLogFile((CA2T)__FUNCTION__, FALSE, _T("开启视频流"));
	return RETVAL_OK;
}

/** @fn		LONG CSCCPCtrl::StopVideo(void)
 *	@brief	关闭视频流
 *	@return	LONG	0表示成功，否则失败
 */
LONG CSCCPCtrl::StopVideo(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your dispatch handler code here
	if (m_lPreviewHandle < 0)
	{
		Invalidate(TRUE);
		return RETVAL_OK;
	}
	BOOL	bRet = FALSE;
	bRet = NET_DVR_StopRealPlay(m_lPreviewHandle);
	if (bRet == TRUE)
	{
		m_lPreviewHandle = -1;
		Invalidate(TRUE);
		WriteLogFile((CA2T)__FUNCTION__, FALSE, _T("关闭视频流"));
		return RETVAL_OK;
	}
	WriteLogFile((CA2T)__FUNCTION__, TRUE, _T("关闭视频流失败"));
	return RETVAL_STOP_REALPLAY_ERROR;
}
#endif

/** @fn		LONG CSCCPCtrl::GetStatus(void)
 *	@brief	获取设备工作状态
 *	@return	LONG	0表示正常，否则失败
 */
LONG CSCCPCtrl::GetStatus(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your dispatch handler code here
	BOOL	bRet = FALSE;
	NET_DVR_DEVICEINFO_V30	stDeviceInfo;
	LONG		lUserID;
	
	if (m_lUserID < 0)
	{
		return RETVAL_ERROR;
	}
	memset(&stDeviceInfo, 0, sizeof(NET_DVR_DEVICEINFO_V30));
	lUserID = NET_DVR_Login_V30(m_stDevLogInfo.chDevIP, m_stDevLogInfo.wDevPort, "admin", "12345", &stDeviceInfo);
	if (lUserID == -1)
	{
		return RETVAL_ERROR;
	}
	bRet = NET_DVR_Logout(lUserID);
	if (bRet == TRUE)
	{
		return RETVAL_OK;
	}
	return RETVAL_ERROR;
}

/** @fn		BSTR CSCCPCtrl::GetPlate(void)
 *	@brief	获取车牌号
 *	@return	BSTR	车牌号
 */
BSTR CSCCPCtrl::GetPlate(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult = _T("F");
	CString strTmp;

	NET_DVR_PLATE_RESULT		stPlateInfo;
	memset(&stPlateInfo, 0, sizeof(stPlateInfo));
	if (m_cBufferManager.GetPlateInfo(&stPlateInfo) == true)
	{
		strResult.Format(_T("%s"),(CA2T)(stPlateInfo.struPlateInfo.sLicense));
		if (strResult != _T("无车牌") && strResult != _T(""))
		{
			strResult.Delete(0);
		}
		else
		{
			strResult = _T("F");
		}
	}
	CString csLog = _T("获取车牌号码:	");
	csLog += strResult;
	WriteLogFile((CA2T)__FUNCTION__, FALSE, csLog);
	return strResult.AllocSysString();
}


/** @fn		BSTR CSCCPCtrl::GetPlateColor(void)
 *	@brief	获取车牌颜色
 *	@return	BSTR	车牌颜色
 */
BSTR CSCCPCtrl::GetPlateColor(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult = _T("F");
	NET_DVR_PLATE_RESULT		stPlateInfo;
	char	*szPlateColor[5]	= {"蓝", "黄", "白", "黑", "F"};
	int		iIndex = 0;
	memset(&stPlateInfo, 0, sizeof(NET_DVR_PLATE_RESULT));
	
	if (m_cBufferManager.GetPlateInfo(&stPlateInfo) == true)
	{
		iIndex = stPlateInfo.struPlateInfo.byColor;
		if (iIndex >= 4)
		{
			iIndex = 4;
		}
		strResult.Format(_T("%s"), (CA2T)szPlateColor[iIndex]);
	}
	CString csLog = _T("获取车牌颜色:	");
	csLog += strResult;
	WriteLogFile((CA2T)__FUNCTION__, FALSE, csLog);
	return strResult.AllocSysString();
}

/** @fn		LONG CSCCPCtrl::SaveImage(LPCTSTR szPath, LPCTSTR szFileName, LONG nWidth, LONG nHeight, LONG nQuality, LONG nImageType)
 *	@biref	保存图片文件
 *	@param	[IN]	szPath		图片路径
 *	@param	[IN]	szFileName	图片名字
 *	@param	[IN]	nWidth		图片宽度（图片大小是固定的，改变图片质量要到智能配置出修改，暂时不用）
 *	@param	[IN]	nHeight		图片高度（图片大小是固定的，改变图片质量要到智能配置出修改，暂时不用）
 *	@param	[IN]	nQuality	图片质量（图片质量是固定的，改变图片质量要到智能配置出修改，暂时不用）
 *	@param	[IN]	nImageType	爆出图片类型 0-近景图，1-远景图
 *	@return	LONG	0表示成功，否则失败
 */
LONG CSCCPCtrl::SaveImage(LPCTSTR szPath, LPCTSTR szFileName, LONG nWidth, LONG nHeight, LONG nQuality, LONG nImageType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	LONG		retVal = RETVAL_OK;
	NET_DVR_PLATE_RESULT	stPlateInfo;

	if (szPath == NULL || szFileName == NULL)
	{
		WriteLogFile((CA2T)__FUNCTION__, TRUE, _T("无效参数"));
		return RETVAL_INVALID_PARAM;
	}
	if (m_RecvImageFlag != 1)
	{
		// 当期不接受图片
		WriteLogFile((CA2T)__FUNCTION__, FALSE, _T("从当前不接受图片"));
		return RETVAL_NOT_RECV_PIC;
	}
	memset(&stPlateInfo, 0, sizeof(NET_DVR_PLATE_RESULT));
	if (m_cBufferManager.GetPlateInfo(&stPlateInfo) == false)
	{
		WriteLogFile((CA2T)__FUNCTION__, FALSE, _T("获取图片失败"));;
		return RETVAL_GET_PLATE_INFO_ERROR;
	}



	CString csPath(szPath);
	//如果以"\"结尾,删除"\"
	if (csPath.GetAt(csPath.GetLength() - 1) == _T('\\'))
	{
		csPath.Delete(csPath.GetLength() - 1);
	}
	//完整路径
	CString csFullPath;
	csFullPath.Format(_T("%s\\%s"), csPath, szFileName);
	if (csFullPath.GetLength() >= MAX_PATH)
	{ 
		AfxMessageBox(_T("图片路径过长，抓图失败"));
		return FALSE;
	}
	//将路径名中的？改成#号
	csFullPath.Replace('?', '#');
	if (CreateFileDir(csFullPath) == FALSE)
	{
		return FALSE;
	}


	switch (nImageType)
	{
	case 0:	//保存近景图
		if (stPlateInfo.pBuffer1 != NULL && stPlateInfo.dwPicLen != 0)
		{

			if (ConvertImgBytes2HBITMAP(stPlateInfo.pBuffer1, stPlateInfo.dwPicLen, nWidth, nHeight, csFullPath.GetBuffer()) == FALSE)
			{
				WriteLogFile((CA2T)__FUNCTION__, TRUE, _T("保存近景图片失败"));
				retVal = RETVAL_SAVE_PIC_ERROR;
			}
			csFullPath.ReleaseBuffer();
		}
		else
		{
			WriteLogFile((CA2T)__FUNCTION__, FALSE, _T("没有近景图片"));
			retVal = RETVAL_HAVE_NO_IMAGE;
		}
		break;
	case 1:	// 保存远景图
		if (stPlateInfo.pBuffer5 != NULL && stPlateInfo.dwFarCarPicLen != 0)
		{
			if (ConvertImgBytes2HBITMAP(stPlateInfo.pBuffer5, stPlateInfo.dwFarCarPicLen, nWidth, nHeight, csFullPath.GetBuffer()) == FALSE)
			{
				WriteLogFile((CA2T)__FUNCTION__, TRUE, _T("保存远景图片失败"));
				retVal = RETVAL_SAVE_PIC_ERROR;
			}
			csFullPath.ReleaseBuffer();
		}
		else
		{
			WriteLogFile((CA2T)__FUNCTION__, FALSE, _T("没有远景图片"));
			retVal = RETVAL_HAVE_NO_IMAGE;
		}
		break;
	default:
		WriteLogFile((CA2T)__FUNCTION__, TRUE, _T("无效参数"));
		retVal = RETVAL_INVALID_PARAM;
		break;
	}
	if (retVal == RETVAL_OK)
	{
		CString		csLog;
		csLog.Format(_T("保存图片：图片类型 %d， 地址：%s\\%s"), nImageType,szPath, szFileName);
		WriteLogFile((CA2T)__FUNCTION__, FALSE, csLog);
	}
	return retVal;
}

/** @fn		LONG CSCCPCtrl::SavePlateImage(LPCTSTR szPath, LPCTSTR szFileName)
 *	@brief	保存车牌图片
 *	@param	[IN]	szPath		图片路径
 *	@param	[IN]	szFileName	图片名字
 *	@return LONG	0表示成功，否则失败
 */
LONG CSCCPCtrl::SavePlateImage(LPCTSTR szPath, LPCTSTR szFileName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	if (szPath == NULL || szFileName == NULL)
	{
		WriteLogFile((CA2T)__FUNCTION__, TRUE, _T("无效参数")); 
		return RETVAL_INVALID_PARAM;
	}
	if (m_RecvPlateImageFlag != 1)
	{
		WriteLogFile((CA2T)__FUNCTION__, FALSE, _T("当前不接受车牌图片"));
		return RETVAL_NOT_RECV_PIC;
	}

	NET_DVR_PLATE_RESULT		stPlateInfo;
	LONG						retVal = RETVAL_OK;

	memset(&stPlateInfo, 0, sizeof(stPlateInfo));
	if (m_cBufferManager.GetPlateInfo(&stPlateInfo) == false)
	{
		WriteLogFile((CA2T)__FUNCTION__, TRUE, _T("获取车牌图片失败"));
		return RETVAL_GET_PLATE_INFO_ERROR;
	}
	if (stPlateInfo.pBuffer2 != NULL && stPlateInfo.dwPicPlateLen != 0)
	{
		if (SavePic(szPath, szFileName, stPlateInfo.pBuffer2, stPlateInfo.dwPicPlateLen) == FALSE)
		{
			WriteLogFile((CA2T)__FUNCTION__, TRUE, _T("保存车牌图片失败"));
			retVal = RETVAL_SAVE_PIC_ERROR;
		}
	}
	else
	{
		WriteLogFile((CA2T)__FUNCTION__, FALSE, _T("没有车牌图片"));
		retVal = RETVAL_HAVE_NO_IMAGE;
	}
	if (retVal == RETVAL_OK)
	{
		CString		csLog;
		csLog.Format(_T("保存车牌图片： 地址：%s\\%s"), szPath, szFileName);
		WriteLogFile((CA2T)__FUNCTION__, FALSE, csLog);
	}
	return retVal;
}

/** @fn		LONG CSCCPCtrl::SavePlateBinImage(LPCTSTR szPath, LPCTSTR szFileName)
 *	@brief	保存车牌二值图
 *	@param	[IN]	szPath		图片路径
 *	@param	[IN]	szFileName	图片名字
 *	@return	LONG	0表示成功，否则失败
 */
LONG CSCCPCtrl::SavePlateBinImage(LPCTSTR szPath, LPCTSTR szFileName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (szPath == NULL || szFileName == NULL)
	{
		WriteLogFile((CA2T)__FUNCTION__, TRUE, _T("无效参数"));
		return RETVAL_INVALID_PARAM;
	}
	if (m_RecvPlateBinImageFlag != 1)
	{
		WriteLogFile((CA2T)__FUNCTION__, FALSE, _T("当前不接受车牌二值图"));
		return RETVAL_NOT_RECV_PIC;
	}

	NET_DVR_PLATE_RESULT		stPlateInfo;
	LONG						retVal = RETVAL_OK;

	memset(&stPlateInfo, 0, sizeof(stPlateInfo));
	if (m_cBufferManager.GetPlateInfo(&stPlateInfo) == false)
	{
		WriteLogFile((CA2T)__FUNCTION__, TRUE, _T("获取车牌二值图片失败"));
		return RETVAL_GET_PLATE_INFO_ERROR;
	}
	if (stPlateInfo.pBuffer3 != NULL && stPlateInfo.dwBinPicLen != 0)
	{
		if (SavePic(szPath, szFileName, stPlateInfo.pBuffer3, stPlateInfo.dwBinPicLen) == FALSE)
		{
			WriteLogFile((CA2T)__FUNCTION__, TRUE, _T("保存车牌二值图失败"));
			retVal = RETVAL_SAVE_PIC_ERROR;
		}
	}
	else
	{
		WriteLogFile((CA2T)__FUNCTION__, FALSE, _T("没有车牌二值图"));
		retVal = RETVAL_HAVE_NO_IMAGE;
	}

	if (retVal == RETVAL_OK)
	{
		CString		csLog;
		csLog.Format(_T("保存车牌二值图： 地址：%s\\%s"), szPath, szFileName);
		WriteLogFile((CA2T)__FUNCTION__, FALSE, csLog);
	}

	return retVal;
}

/** @fn		LONG CSCCPCtrl::Trigger(void)
 *	@brief	模拟外部触发，强制识别车牌，该功能只能在非视频模式下才生效
 *	@return	LONG	0表示成功，否则失败
 */
LONG CSCCPCtrl::Trigger(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (m_lUserID < 0)
	{
		WriteLogFile((CA2T)__FUNCTION__, TRUE, _T("设备没有登录"));
		return  RETVAL_DEVICE_NOT_LOGIN;
	}

	NET_DVR_SNAPCFG			stSnapCfg = {0};
	BOOL					bRet	  = FALSE;
	
	stSnapCfg.byRelatedDriveWay	= 1;
	stSnapCfg.bySnapTimes		= 1;
	stSnapCfg.dwSize			= sizeof(stSnapCfg);
	
	bRet = NET_DVR_ContinuousShoot(m_lUserID, &stSnapCfg);
	if (bRet == FALSE)
	{
		WriteLogFile((CA2T)__FUNCTION__, TRUE, _T("模拟外部触发失败"));
		return RETVAL_SHOOT_ERROR;
	}
	WriteLogFile((CA2T)__FUNCTION__, FALSE, _T("模拟外部触发"));
	//int i = 0;
	//while(g_callback == 0&&i < 5000)
	//{
	//	Sleep(1);
	//	i++;
	//}
	//InterlockedDecrement(&g_callback);
	//OnPlateReceive();
	return RETVAL_OK;
}


/** @fn		void CSCCPCtrl::OnDeviceInitOkChanged(void)
 *	@brief	修改控件DeviceInitOk属性
 *	@return	void
 */
void CSCCPCtrl::OnDeviceInitOkChanged(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString csLog;
	csLog.Format(_T("修改DeviceInitOk属性： %s"), m_DeviceInitOk ? _T("TRUE") : _T("FALSE"));
	WriteLogFile((CA2T)__FUNCTION__, FALSE, csLog);
	SetModifiedFlag();
}


/** @fn		void CSCCPCtrl::OnRecvPlateImageFlagChanged(void)
 *	@brief	修改控件RecvPlateImageFlag属性
 *	@return	void
 */
void CSCCPCtrl::OnRecvPlateImageFlagChanged(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString csLog;
	csLog.Format(_T("修改RecvPlateImageFlag属性： %d"), m_RecvPlateImageFlag);
	WriteLogFile((CA2T)__FUNCTION__, FALSE, csLog);

	SetModifiedFlag();
}

/** @fn		void CSCCPCtrl::OnRecvImageFlagChanged(void)
 *	@brief	修改控件RecvImageFlag属性
 *	@return	void
 */
void CSCCPCtrl::OnRecvImageFlagChanged(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString csLog;
	csLog.Format(_T("修改RecvImageFlag属性： %d"), m_RecvImageFlag);
	WriteLogFile((CA2T)__FUNCTION__, FALSE, csLog);

	SetModifiedFlag();
}

/** @fn		void CSCCPCtrl::OnRecvPlateBinImageFlagChanged(void)
 *	@brief	修改控件RecvPlateBinImage属性
 *	@return	void
 */
void CSCCPCtrl::OnRecvPlateBinImageFlagChanged(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString csLog;
	csLog.Format(_T("修改RecvPlateBinImage属性： %d"), m_RecvPlateBinImageFlag);
	WriteLogFile((CA2T)__FUNCTION__, FALSE, csLog);

	SetModifiedFlag();
}

/** @fn		void CSCCPCtrl::OnLogFileChanged(void)
 *	@brief	修改控件LogFile属性
 *	@return	void
 */
void CSCCPCtrl::OnLogFileChanged(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your property handler code here
	if (m_LogFile == 1)
	{
		CreateLogFile();
	}
	SetModifiedFlag();
}

/** @fn		VOID CSCCPCtrl::WriteLogFile(LPCTSTR szLog) 
 *	@brief	写日志
 *	@param	[IN]	szFun	函数名
 *	@param	[IN]	bError	是否为错误
 *	@param	[IN]	szLog	日志内容
 *	@return	void
 */
VOID CSCCPCtrl::WriteLogFile(LPCTSTR szFun, BOOL bError, LPCTSTR szLog) 
{
	if (m_LogFile == 0 || m_hLogFile == NULL || m_hMutexLogFile == NULL)
	{
		return ;
	}

	CTime	cCurrentTime = CTime::GetCurrentTime();
	CString csLog = _T("");
	CString	csTmp = _T("");
	DWORD	dwLen = 0;
	char	szPrint[512];

	csLog = cCurrentTime.Format(_T("%Y-%m-%d %H:%M:%S"));
	if (bError)
	{
		csTmp.Format(_T("\t%s:ERROR:\t%s"), szFun, szLog);
	}
	else
	{
		csTmp.Format(_T("\t%s:\t%s"), szFun, szLog);
	}
	csLog += csTmp;
	sprintf_s(szPrint,512, "%s\r\n", (CT2A)csLog);

	if (WaitForSingleObject(m_hMutexLogFile, INFINITE) != WAIT_OBJECT_0)
	{
		return;
	}
	if (WriteFile(m_hLogFile, szPrint, strlen(szPrint) + 1, &dwLen, NULL) == FALSE)
	{
		ReleaseMutex(m_hMutexLogFile);
		return;
	}
	ReleaseMutex(m_hMutexLogFile);
}

/** @fn		BOOL CSCCPCtrl::CreateLogFile()
 *	@brief	创建日志文件
 *	@return	BOOL	true表示正确，否则失败
 */
BOOL CSCCPCtrl::CreateLogFile()
{
	if (m_hLogFile != NULL)
	{
		return TRUE;
	}

	CTime	cCurrentTime = CTime::GetCurrentTime();
	CString	csPath;
	int			iPos = 0;
	csPath.Format(_T("%s%4d_%2d_%2d_log.txt"),LOG_FILE_PATH, cCurrentTime.GetYear(), cCurrentTime.GetMonth(), cCurrentTime.GetDay());
	while((iPos=csPath.Find(_T('\\'),iPos+1)) != -1)
	{
		CreateDirectory(csPath.Left(iPos),NULL);
	}
	
	m_hLogFile = CreateFile(csPath, 
								GENERIC_WRITE,
								FILE_SHARE_READ,
								NULL,
								OPEN_ALWAYS,
								FILE_ATTRIBUTE_NORMAL,
								NULL);
	if (m_hLogFile == INVALID_HANDLE_VALUE)
	{
		m_hLogFile = NULL;
		return FALSE;
	}
	
	//移动文件指针到最后
	DWORD	dwRet = 0;
	dwRet = SetFilePointer(m_hLogFile, 0, NULL, FILE_END);
	if (dwRet == 0xFFFFFFFF)
	{
		CloseHandle(m_hLogFile);
		m_hLogFile = NULL;
		return FALSE;
	}
	if (m_hMutexLogFile == NULL)
	{
		m_hMutexLogFile = CreateMutex(NULL, FALSE, NULL);
		if (m_hMutexLogFile == NULL)
		{
			return FALSE;
		}
	}
	return TRUE;
}


/** @fn		void CSCCPCtrl::OnDestroy()
 *	@brief	销毁消息
 *	@return	void
 */
void CSCCPCtrl::OnDestroy()
{
	//if(m_hThread!=NULL)
	//{
	//	CloseHandle(m_hThread);
	//	m_hThread = NULL;
	//}
	m_pfnSadpStop();
	Close();
	m_LogFile = 0;
	Sleep(10);
	if (m_hMutexLogFile != NULL)
	{
		CloseHandle(m_hMutexLogFile);
		m_hMutexLogFile = NULL;
	}
	if (m_hLogFile != NULL)
	{
		CloseHandle(m_hLogFile);
		m_hLogFile = NULL;
	}
	Gdiplus::GdiplusShutdown(m_gdiplusToken);


	COleControl::OnDestroy();
}

/** @fn		BOOL CSCCPCtrl::CreateFileDir(LPCTSTR lpPath)
 *	@brief	创建指定的文件夹
 *	@param	[IN]	lpPath	文件路径
 *	@return	BOOL	TRUE表示成功，否则失败
 */
BOOL CSCCPCtrl::CreateFileDir(LPCTSTR lpPath)
{
	if (lpPath == NULL)
	{
		return FALSE;
	}
	int			iPos=0;
	CString		strFilePath;

	strFilePath.Format(_T("%s"), lpPath);
	while((iPos=strFilePath.Find(_T('\\'),iPos+1)) != -1)
	{
		CreateDirectory(strFilePath.Left(iPos),NULL);
	}

	if (_taccess(strFilePath,0) == FALSE)
	{
		if (DeleteFile(strFilePath) == FALSE)
		{
			CString	csTmp = _T("删除原来已经存在的文件失败：");
			csTmp += strFilePath;
			WriteLogFile((CA2T)__FUNCTION__, TRUE, csTmp);
			return FALSE;
		}
	}
	return TRUE;
}

void __stdcall fSadpDataCallBack1(char* series,
								  char* deviceid,
								  char* hwaddr,
								  char* praddr,
								  char* subnetmask,
								  unsigned int dev_type,
								  unsigned int port,
								  unsigned int enccnt,
								  unsigned int hdiskcnt,
								  char* sfwversion, 
								  char* dspversion, 
								  char* starttime,
								  int result);

/** @fn		BSTR CSCCPCtrl::GetIPAddr(void)
 *	@brief	获取设备IP
 *	@return	返回IP地址
 */
BSTR CSCCPCtrl::GetIPAddr(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//if( 0 != strcmp((char*)(g_strSCCPIP.GetBuffer(0)),"") && 0 != strcmp((char*)(g_strSCCPIP.GetBuffer(0)),(char*)(m_strIPAddr.GetBuffer(0))))
	//{
	//	m_strIPAddr = g_strSCCPIP;
	//}
	//else
	//{
	CString strIPAddr = _T("127.0.0.1");
	m_strIPAddr = _T("127.0.0.1");
	//g_strSCCPIP = _T("");
	//}
	int i = 0;
	CString strSCCPIP = g_strSCCPIP;
	m_pfnSadpStart(fSadpDataCallBack1);
	while (0 == strcmp((char*)(m_strIPAddr.GetBuffer(0)),(char*)(strIPAddr.GetBuffer(0))) && i <= 100 )
	{
		//m_strIPAddr.ReleaseBuffer();
		//strIPAddr.ReleaseBuffer();
		//while(i<=5)
		//{
		//	m_pfnSadpStart(fSadpDataCallBack1);
		//	i++;
		//	continue;
		//}
		//m_pfnSadpStart(fSadpDataCallBack1);
		
		m_strIPAddr = g_strSCCPIP;
		Sleep(10);
		i++;
	}
	//Sleep(100);
	//if(0 != strcmp((char*)(g_strSCCPIP.GetBuffer(0)),(char*)(strSCCPIP.GetBuffer(0))))
	//{
	//	//g_strSCCPIP.ReleaseBuffer();
	//	//strSCCPIP.ReleaseBuffer();
	//	m_strIPAddr = g_strSCCPIP;
	//}
	m_pfnSadpStop();
	return m_strIPAddr.AllocSysString();
}

/** @fn		BSTR CSCCPCtrl::GetPort(void)
 *	@brief	获取端口号
 *	@return	返回设备端口
 */
BSTR CSCCPCtrl::GetPort(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	
	//m_strPort = _T("8000");
	// TODO: Add your dispatch handler code here
	if(0 != strcmp((char *)(g_strSCCPPort.GetBuffer(0)),""))
	{
		m_strPort = g_strSCCPPort;
	}
	else
	{
		m_strPort = _T("8000");
	}
	//g_strSCCPPort.ReleaseBuffer();
	//m_strPort.ReleaseBuffer();
	return m_strPort.AllocSysString();
}

//DWORD CSCCPCtrl::FunProc(LPVOID lpParameter)
//{
//	CSCCPCtrl *pCSCCP = (CSCCPCtrl*)lpParameter;
//	while(1)
//	{
//		while(g_Passproc == 1)
//		{
//			InterlockedDecrement(&g_Passproc);
//			//pCSCCP->OnPlateReceive();
//			pCSCCP->PostMessage(WM_USER+1,NULL,NULL);
//		}
//	}
//}


/** @fn		void CSCCPCtrl::InitCtrl()
 *	@brief	初始化控件
 *	@return	对Sadp控件函数导入
 */

void CSCCPCtrl::InitCtrl()
{
	CString strModuleName = _T("Sadp.dll");
	m_hSadpModule = LoadLibrary(strModuleName);
	if (m_hSadpModule == NULL)
	{
		return ;
	}

	m_pfnSadpSendUpdateIP = (PFN_SADP_SENDUPDATEIP)GetProcAddress(m_hSadpModule, "SADP_SendUpdateIp");
	if (m_pfnSadpSendUpdateIP == NULL)
	{
		return ;
	}

	m_pfnSadpStart	= (PFN_SADP_START)GetProcAddress(m_hSadpModule, "SADP_Start");
	if (m_pfnSadpStart == NULL)
	{
		return ;
	}
	m_pfnSadpStop = (PFN_SADP_STOP)GetProcAddress(m_hSadpModule, "SADP_Stop");
	if (m_pfnSadpStop == NULL)
	{
		return ;
	}

	//m_pfnSadpStart(fSadpDataCallBack1);

	//初始化GDI
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

	//m_hThread=CreateThread(NULL,0,FunProc,this,0,NULL);

	
}

/** @fn		void __stdcall fSadpDataCallBack1(char* series,
																	char* deviceid,
																	char* hwaddr,
																	char* praddr,
																	char* subnetmask,
																	unsigned int dev_type,
																	unsigned int port,
																	unsigned int enccnt, 
																	unsigned int hdiskcnt,
																	char* sfwversion,
																	char* dspversion, 
																	char* starttime,
																	int result)
 *	@brief	Sadp回调函数
 *	@return	回调获取设备参数
 */

void __stdcall fSadpDataCallBack1(char* series,
								 char* deviceid,
								 char* hwaddr,
								 char* praddr,
								 char* subnetmask,
								 unsigned int dev_type,
								 unsigned int port,
								 unsigned int enccnt, 
								 unsigned int hdiskcnt,
								 char* sfwversion,
								 char* dspversion, 
								 char* starttime,
								 int result)
{
	if ((result == SADP_ADD || SADP_RESTART) && (enccnt == 2) && (dev_type == 22392))
	{
		g_strSCCPIP.Format(TEXT("%s"),CA2T(praddr));
		g_strSCCPPort.Format(TEXT("%d"), port);	
	}
}

/** @fn		int CSCCPCtrl::GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
 *	@brief	获取图像的类ID
 *	@param	[IN]	format	图像类型
 *	@param	[OUT]	pClsid	返回ID
 *	@return	返回类型id，否则返回-1
 */


int CSCCPCtrl::GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT  nNum = 0;          
	UINT  nSize = 0;         

	ImageCodecInfo* pImageCodecInfo = NULL;
	GetImageEncodersSize(&nNum, &nSize);
	if(nSize == 0)
	{
		return -1;  
	}
	pImageCodecInfo = (ImageCodecInfo*)(malloc(nSize));
	if(pImageCodecInfo == NULL)
	{
		return -1;
	}
	GetImageEncoders(nNum, nSize, pImageCodecInfo);
	for(UINT i = 0; i < nNum; ++i)
	{
		if( wcscmp(pImageCodecInfo[i].MimeType, format) == 0 )
		{
			*pClsid = pImageCodecInfo[i].Clsid;
			free(pImageCodecInfo);
			return i;  
		}    
	}
	free(pImageCodecInfo);
	return -1;  
}

/** @fn		BOOL CSCCPCtrl::ConvertImgBytes2HBITMAP(BYTE* pImageBuffer,DWORD dwImageDataLen, int iWidth, int iHeight, WCHAR *  szFilePath)
 *	@brief	保存图片
 *	@param	[IN]	pImageBuffer	图像数据地址
 *	@param	[IN]	dwImageDataLen	图像数据长度
 *	@param	[IN]	iWidth	图像宽度
 *	@param	[IN]	iHeight	图像高度
 *	@param	[IN]	szFilePath	保存路径
 *	@return	0表示成功，否则失败
 */

BOOL CSCCPCtrl::ConvertImgBytes2HBITMAP(BYTE* pImageBuffer,DWORD dwImageDataLen, int iWidth, int iHeight, WCHAR *  szFilePath)
{
	if (pImageBuffer == NULL || dwImageDataLen <= 0 )
	{
		return FALSE;
	}

	BOOL bResult = FALSE;
	HGLOBAL hImageMemory= GlobalAlloc(GMEM_MOVEABLE,dwImageDataLen);
	if (hImageMemory != NULL)
	{
		BYTE *pImageMemory= (BYTE*)GlobalLock(hImageMemory);
		if (pImageMemory != NULL)
		{
			memcpy_s(pImageMemory,dwImageDataLen,pImageBuffer,dwImageDataLen);
			GlobalUnlock(hImageMemory);

			IStream *pIStream;
			if (CreateStreamOnHGlobal(hImageMemory, false, &pIStream) == S_OK)
			{
				Bitmap  img(pIStream);
				Bitmap* pThumbnail = static_cast<Bitmap*>(img.GetThumbnailImage(iWidth, iHeight, NULL, NULL));
				if (pThumbnail != NULL)
				{
					CLSID pngClsid;

					GetEncoderClsid(L"image/jpeg", &pngClsid);


					Status status = pThumbnail->Save(szFilePath,&pngClsid,NULL);
					if (status != Ok)
					{   
						bResult =  FALSE;
					}
					else
					{
						bResult = TRUE;
					}

					delete pThumbnail;
				}
				pIStream->Release(); 
			}
		}
		GlobalFree(hImageMemory); 
	}
	return bResult;
}

LONG CSCCPCtrl::SetSFZ(LPCTSTR name)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// TODO: Add your dispatch handler code here
	DWORD dwReturn;
	NET_DVR_IMAGEOVERLAYCFG struImageOverlayCfg;
	if(m_lUserID>=0)
	{
//通道1设置
		ZeroMemory(&struImageOverlayCfg, sizeof(NET_DVR_IMAGEOVERLAYCFG));
		if(!NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_IMAGEOVERLAYCFG, 1, &struImageOverlayCfg, sizeof(NET_DVR_IMAGEOVERLAYCFG), &dwReturn))
		{
			DWORD dwret = NET_DVR_GetLastError();
			CString	csTmp;
			csTmp.Format(_T("获取通道1参数失败,错误码是：%d"),dwret);
			WriteLogFile((CA2T)__FUNCTION__, TRUE, csTmp);
			return 1;
		}
		else
		{
			USES_CONVERSION;
			CString strName;
			strName.Format(_T("%s"),name);
			struImageOverlayCfg.byOverlayInfo = 1;
			memcpy_s(struImageOverlayCfg.byMonitorInfo1,32,W2A(strName.GetBuffer(0)),32);
			strName.ReleaseBuffer();
			if(!NET_DVR_SetDVRConfig(m_lUserID,NET_DVR_SET_IMAGEOVERLAYCFG,1,&struImageOverlayCfg,sizeof(NET_DVR_IMAGEOVERLAYCFG)))
			{
				DWORD dwret = NET_DVR_GetLastError();
				CString	csTmp;
				csTmp.Format(_T("设置通道1参数失败,错误码是：%d"),dwret);
				WriteLogFile((CA2T)__FUNCTION__, TRUE, csTmp);
				return 1;
			}
			else
			{
				CString	csTmp;
				csTmp.Format(_T("设置通道1成功，值为："),name);
				WriteLogFile((CA2T)__FUNCTION__, TRUE, csTmp);
				//return 0;
			}
		}
//通道2设置
		ZeroMemory(&struImageOverlayCfg, sizeof(NET_DVR_IMAGEOVERLAYCFG));
		if(!NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_IMAGEOVERLAYCFG, 2, &struImageOverlayCfg, sizeof(NET_DVR_IMAGEOVERLAYCFG), &dwReturn))
		{
			DWORD dwret = NET_DVR_GetLastError();
			CString	csTmp;
			csTmp.Format(_T("获取通道2参数失败,错误码是：%d"),dwret);
			WriteLogFile((CA2T)__FUNCTION__, TRUE, csTmp);
			return 1;
		}
		else
		{
			USES_CONVERSION;
			CString strName;
			strName.Format(_T("%s"),name);
			struImageOverlayCfg.byOverlayInfo = 1;
			memcpy_s(struImageOverlayCfg.byMonitorInfo1,32,W2A(strName.GetBuffer(0)),32);
			strName.ReleaseBuffer();
			if(!NET_DVR_SetDVRConfig(m_lUserID,NET_DVR_SET_IMAGEOVERLAYCFG,2,&struImageOverlayCfg,sizeof(NET_DVR_IMAGEOVERLAYCFG)))
			{
				DWORD dwret = NET_DVR_GetLastError();
				CString	csTmp;
				csTmp.Format(_T("设置通道2参数失败,错误码是：%d"),dwret);
				WriteLogFile((CA2T)__FUNCTION__, TRUE, csTmp);
				return 1;
			}
			else
			{
				CString	csTmp;
				csTmp.Format(_T("设置通道2成功，值为："),name);
				WriteLogFile((CA2T)__FUNCTION__, TRUE, csTmp);
				return 0;
			}
		}
	}
	else
	{
		CString	csTmp;
		csTmp.Format(_T("设置失败，设备没有登录!"));
		WriteLogFile((CA2T)__FUNCTION__, TRUE, csTmp);
		return 1;
	}
}
