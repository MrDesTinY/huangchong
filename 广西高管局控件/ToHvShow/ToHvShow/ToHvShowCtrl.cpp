// ToHvShowCtrl.cpp : Implementation of the CToHvShowCtrl ActiveX Control class.

#include "stdafx.h"
#include "ToHvShow.h"
#include "ToHvShowCtrl.h"
#include "ToHvShowPropPage.h"
std::vector<pDeviceInfo> CToHvShowCtrl::m_vecDeviceInfoTable;
//LONG CToHvShowCtrl::m_lCurrentUid;//标识当前触发设备Uid
//LONG CToHvShowCtrl::m_lLoginUid;//标识当前登录设备Uid，用于布防。
//CRITICAL_SECTION CToHvShowCtrl::m_csVecLock;
//HANDLE CToHvShowCtrl::m_hLogFile = NULL;					///< 日志文件句柄
//HANDLE CToHvShowCtrl::m_hMutexLogFile = NULL;			///< 日志文件锁
//LONG CToHvShowCtrl::m_LogFile = 1;//是否打印日志到文件中。
//LONG volatile g_callback = 0;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CToHvShowCtrl, COleControl)



// Message map

BEGIN_MESSAGE_MAP(CToHvShowCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
	ON_WM_DESTROY()
	ON_MESSAGE(WM_USER+100,OnRecieve)
END_MESSAGE_MAP()



// Dispatch map

BEGIN_DISPATCH_MAP(CToHvShowCtrl, COleControl)
	DISP_PROPERTY_NOTIFY_ID(CToHvShowCtrl, "RecvSnapImageFlag", dispidRecvSnapImageFlag, m_RecvSnapImageFlag, OnRecvSnapImageFlagChanged, VT_I4)
	DISP_PROPERTY_NOTIFY_ID(CToHvShowCtrl, "RecvPlateImageFlag", dispidRecvPlateImageFlag, m_RecvPlateImageFlag, OnRecvPlateImageFlagChanged, VT_I4)
	DISP_PROPERTY_NOTIFY_ID(CToHvShowCtrl, "RecvPlateBinImageFlag", dispidRecvPlateBinImageFlag, m_RecvPlateBinImageFlag, OnRecvPlateBinImageFlagChanged, VT_I4)
	DISP_PROPERTY_NOTIFY_ID(CToHvShowCtrl, "RecvSnapDriverImageFlag", dispidRecvSnapDriverImageFlag, m_RecvSnapDriverImageFlag, OnRecvSnapDriverImageFlagChanged, VT_I4)
	DISP_PROPERTY_NOTIFY_ID(CToHvShowCtrl, "AutoSaveFlag", dispidAutoSaveFlag, m_AutoSaveFlag, OnAutoSaveFlagChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY_ID(CToHvShowCtrl, "ShowPlateImage", dispidShowPlateImage, m_ShowPlateImage, OnShowPlateImageChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY_ID(CToHvShowCtrl, "ShowFirstImage", dispidShowFirstImage, m_ShowFirstImage, OnShowFirstImageChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY_ID(CToHvShowCtrl, "ShowSecondImage", dispidShowSecondImage, m_ShowSecondImage, OnShowSecondImageChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY_ID(CToHvShowCtrl, "ShowPlateResult", dispidShowPlateResult, m_ShowPlateResult, OnShowPlateResultChanged, VT_BSTR)
	DISP_FUNCTION_ID(CToHvShowCtrl, "ConnectTo", dispidConnectTo, ConnectTo, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CToHvShowCtrl, "GetStatus", dispidGetStatus, GetStatus, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CToHvShowCtrl, "Disconnect", dispidDisconnect, Disconnect, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CToHvShowCtrl, "SetImgSavePath", dispidSetImgSavePath, SetImgSavePath, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CToHvShowCtrl, "GetPlate", dispidGetPlate, GetPlate, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CToHvShowCtrl, "GetPlateColor", dispidGetPlateColor, GetPlateColor, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CToHvShowCtrl, "GetPlateInfo", dispidGetPlateInfo, GetPlateInfo, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CToHvShowCtrl, "SaveSnapImage", dispidSaveSnapImage, SaveSnapImage, VT_I4, VTS_BSTR)
	DISP_FUNCTION_ID(CToHvShowCtrl, "SaveSnapImage2", dispidSaveSnapImage2, SaveSnapImage2, VT_I4, VTS_BSTR)
	DISP_FUNCTION_ID(CToHvShowCtrl, "SavePlateImage", dispidSavePlateImage, SavePlateImage, VT_I4, VTS_BSTR)
	DISP_FUNCTION_ID(CToHvShowCtrl, "SavePlateBinImage", dispidSavePlateBinImage, SavePlateBinImage, VT_I4, VTS_BSTR)
	DISP_FUNCTION_ID(CToHvShowCtrl, "SavePlateBin2BMP", dispidSavePlateBin2BMP, SavePlateBin2BMP, VT_I4, VTS_BSTR)
	DISP_FUNCTION_ID(CToHvShowCtrl, "GetImageRecTime", dispidGetImageRecTime, GetImageRecTime, VT_BSTR, VTS_I4)
	DISP_FUNCTION_ID(CToHvShowCtrl, "GetImageFileName", dispidGetImageFileName, GetImageFileName, VT_BSTR, VTS_I4)
	DISP_FUNCTION_ID(CToHvShowCtrl, "GetImageDataFromMem", dispidGetImageDataFromMem, GetImageDataFromMem, VT_BSTR, VTS_I4 VTS_PVARIANT VTS_PI4)
	DISP_PROPERTY_NOTIFY_ID(CToHvShowCtrl, "LogFile", dispidLogFile, m_LogFile, OnLogFileChanged, VT_I4)
END_DISPATCH_MAP()



// Event map

BEGIN_EVENT_MAP(CToHvShowCtrl, COleControl)
	EVENT_CUSTOM_ID("OnReceivePlate", eventidOnReceivePlate, OnReceivePlate, VTS_NONE)
END_EVENT_MAP()



// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CToHvShowCtrl, 1)
	PROPPAGEID(CToHvShowPropPage::guid)
END_PROPPAGEIDS(CToHvShowCtrl)



// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CToHvShowCtrl, "TOHVSHOW.ToHvShowCtrl.1",
	0xb38e8e73, 0x1451, 0x470f, 0x8e, 0xf, 0xe7, 0x1b, 0x9a, 0xfb, 0x3f, 0x53)



// Type library ID and version

IMPLEMENT_OLETYPELIB(CToHvShowCtrl, _tlid, _wVerMajor, _wVerMinor)



// Interface IDs

const IID BASED_CODE IID_DToHvShow =
		{ 0xC683820E, 0x70D4, 0x43E5, { 0xA0, 0x3D, 0xBB, 0xB9, 0x45, 0xF2, 0xEB, 0x2A } };
const IID BASED_CODE IID_DToHvShowEvents =
		{ 0xFB8A8CB8, 0x869A, 0x4457, { 0x8A, 0x97, 0xE6, 0x8E, 0x58, 0xD2, 0xA8, 0xA0 } };



// Control type information

static const DWORD BASED_CODE _dwToHvShowOleMisc =
/*	OLEMISC_INVISIBLEATRUNTIME |*/
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CToHvShowCtrl, IDS_TOHVSHOW, _dwToHvShowOleMisc)



// CToHvShowCtrl::CToHvShowCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CToHvShowCtrl

BOOL CToHvShowCtrl::CToHvShowCtrlFactory::UpdateRegistry(BOOL bRegister)
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
			IDS_TOHVSHOW,
			IDB_TOHVSHOW,
			afxRegApartmentThreading,
			_dwToHvShowOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}



// CToHvShowCtrl::CToHvShowCtrl - Constructor

CToHvShowCtrl::CToHvShowCtrl()
{
	InitializeIIDs(&IID_DToHvShow, &IID_DToHvShowEvents);
	// TODO: Initialize your control's instance data here.
	m_iBinBMPSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*2 + 20*112;//BMP 指定大小
	m_pchPlateBinBMP = new char[m_iBinBMPSize];
	m_fChangingCtrlName = false;
	m_hWndParent = NULL;
	m_hMutexLogFile = NULL;
	m_hLogFile = NULL;
	m_LogFile = 1;
#ifndef ONE_DEVICE
	InitializeCriticalSectionAndSpinCount(&m_csVecLock,4000);//分配临界区
	m_lCurrentUid = -1;
	m_lLoginUid = -1;
#else
	m_csPort = _T("");
	m_csIPAddr = _T("");
	m_lUid = -1;
	m_lAlarmHandle = -1;
	m_cBufferManager.InitBufferManage();
#endif
	m_csImagePath = _T("C:\\Image");
	//初始化GDI
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
	if(m_pClientSite)
	{
		EnumAllControlNames();//获取所有控件名称
	}
	if (m_LogFile == 1)
	{
		CreateLogFile();
	}
}



// CToHvShowCtrl::~CToHvShowCtrl - Destructor

CToHvShowCtrl::~CToHvShowCtrl()
{
	// TODO: Cleanup your control's instance data here.
	if(m_pchPlateBinBMP)
	{
		delete [] m_pchPlateBinBMP;
		m_pchPlateBinBMP = NULL;
	}
#ifndef ONE_DEVICE
	EnterCriticalSection(&m_csVecLock);
	m_vecDeviceInfoTable.clear();//析构所有设备对象
	LeaveCriticalSection(&m_csVecLock);
	DeleteCriticalSection(&m_csVecLock);//释放临界区
#endif
}



// CToHvShowCtrl::OnDraw - Drawing function

void CToHvShowCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;

	// TODO: Replace the following code with your own drawing code.
	//pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	//pdc->Ellipse(rcBounds);
}



// CToHvShowCtrl::DoPropExchange - Persistence support

void CToHvShowCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.
	PX_Long(pPX,_T("RecvSnapImageFlag"),m_RecvSnapImageFlag,1);
	PX_Long(pPX,_T("RecvPlateImageFlag"),m_RecvPlateImageFlag,1);
	PX_Long(pPX,_T("RecvPlateBinImageFlag"),m_RecvPlateBinImageFlag,1);
	PX_Long(pPX,_T("RecvSnapDriverImageFlag"),m_RecvSnapDriverImageFlag,1);
	PX_Short(pPX,_T("AutoSaveFlag"),m_AutoSaveFlag,0);
	PX_String(pPX,_T("ShowPlateImage"),m_ShowPlateImage,_T("ShowPlateImage"));
	PX_String(pPX,_T("ShowFirstImage"),m_ShowFirstImage,_T("ShowFirstImage"));
	PX_String(pPX,_T("ShowSecondImage"),m_ShowSecondImage,_T("ShowSecondImage"));
	PX_String(pPX,_T("ShowPlateResult"),m_ShowPlateResult,_T("ShowPlateResult"));
	//////////////////////////////////////////////自定义属性
	PX_Long(pPX,_T("LogFile"),m_LogFile,1);
}



// CToHvShowCtrl::OnResetState - Reset control to default state

void CToHvShowCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}


CString g_csPlateData;//车牌数据,全局变量的主要原因是怕发送消息客户端不及时处理，而数据丢失。
//消息处理函数来发送消息。
LRESULT CToHvShowCtrl::OnRecieve(WPARAM wParam, LPARAM lParam)
{
	//向控件发送数据，之后再向外发送消息。
	NET_DVR_PLATE_RESULT		stPlateInfo;
	memset(&stPlateInfo, 0, sizeof(stPlateInfo));
#ifndef ONE_DEVICE
	if (GetBufferManagerByUid(m_lCurrentUid) != NULL && GetBufferManagerByUid(m_lCurrentUid)->GetPlateInfo(&stPlateInfo) == true)
	{
		if (m_AutoSaveFlag == 1)
		{
			// 当前自动保存图片
			WriteLogFile((CA2T)__FUNCTION__, FALSE, _T("当前自动保存图片！！！"));
			SaveSnapImage(GetImageFileName(0));
			SavePlateImage(GetImageFileName(1));
			SavePlateBinImage(GetImageFileName(2));
			SaveSnapImage2(GetImageFileName(3));
			SavePlateBin2BMP(GetImageFileName(4));
		}
		////////////////////////////////////////////////////////////////////////////
		if(m_pClientSite)
		{
			EnumAllControlNames();//获取所有控件名称
		}
		resultMSG msg = {0};
		/////////////////////////////////////////////////////////////////车牌信息
		USES_CONVERSION;
		//csPlateData.Format(_T("%s%s"),stPlateInfo.struPlateInfo.byColor==0?_T("蓝"):
		//												stPlateInfo.struPlateInfo.byColor==1?_T("黄"):
		//												stPlateInfo.struPlateInfo.byColor==2?_T("白"):
		//												stPlateInfo.struPlateInfo.byColor==3?_T("黑"):_T("")
		//												,A2W(stPlateInfo.struPlateInfo.sLicense));
		g_csPlateData.Format(_T("%s"),A2W(stPlateInfo.struPlateInfo.sLicense));
		if(g_csPlateData == _T("无车牌") || g_csPlateData == _T(""))
		{
			g_csPlateData = _T("未检测");
		}
		char* strPlate = W2A(g_csPlateData.GetBuffer(0));
		msg.pdata = strPlate;
		msg.pDataLen = strlen (strPlate);
		msg.dataType = 1; // 1是字符串
		SendData(m_ShowPlateResult, msg.pdata, msg.pDataLen, 1, &msg);
		g_csPlateData.ReleaseBuffer();
		///////////////////////////////////////////////////////////////////车牌图片
		msg.pdata = GetBufferManagerByUid(m_lCurrentUid)->GetPicBuffer(1);
		msg.pDataLen = stPlateInfo.dwPicPlateLen;
		msg.dataType = 0;  // 0 是图片
		SendData(m_ShowPlateImage, msg.pdata, msg.pDataLen, 0, &msg);
		///////////////////////////////////////////////////////////////////车辆图片
		msg.pdata = GetBufferManagerByUid(m_lCurrentUid)->GetPicBuffer(0);
		msg.pDataLen = stPlateInfo.dwPicLen;
		msg.dataType = 0;  // 0 是图片
		SendData(m_ShowFirstImage, msg.pdata, msg.pDataLen, 0, &msg);
		///////////////////////////////////////////////////////////////////车脸图片
		msg.pdata = GetBufferManagerByUid(m_lCurrentUid)->GetPicBuffer(3);
		msg.pDataLen = stPlateInfo.dwCarPicLen;
		msg.dataType = 0;  // 0 是图片
		SendData(m_ShowSecondImage, msg.pdata, msg.pDataLen, 0, &msg);
	}
#else
	if (m_cBufferManager.GetPlateInfo(&stPlateInfo) == true)
	{
		if (m_AutoSaveFlag == 1)
		{
			// 当前自动保存图片
			WriteLogFile((CA2T)__FUNCTION__, FALSE, _T("当前自动保存图片！！！"));
			SaveSnapImage(GetImageFileName(0));
			SavePlateImage(GetImageFileName(1));
			SavePlateBinImage(GetImageFileName(2));
			SaveSnapImage2(GetImageFileName(3));
			SavePlateBin2BMP(GetImageFileName(4));
		}
		////////////////////////////////////////////////////////////////////////////
		if(m_pClientSite)
		{
			EnumAllControlNames();//获取所有控件名称
		}
		resultMSG msg = {0};
		/////////////////////////////////////////////////////////////////车牌信息
		USES_CONVERSION;
		//csPlateData.Format(_T("%s%s"),stPlateInfo.struPlateInfo.byColor==0?_T("蓝"):
		//												stPlateInfo.struPlateInfo.byColor==1?_T("黄"):
		//												stPlateInfo.struPlateInfo.byColor==2?_T("白"):
		//												stPlateInfo.struPlateInfo.byColor==3?_T("黑"):_T("")
		//												,A2W(stPlateInfo.struPlateInfo.sLicense));
		g_csPlateData.Format(_T("%s"),A2W(stPlateInfo.struPlateInfo.sLicense));
		if(g_csPlateData == _T("无车牌") || g_csPlateData == _T(""))
		{
			g_csPlateData = _T("未检测");
		}
		char* strPlate = W2A(g_csPlateData.GetBuffer(0));
		msg.pdata = strPlate;
		msg.pDataLen = strlen (strPlate);
		msg.dataType = 1; // 1是字符串
		SendData(m_ShowPlateResult, msg.pdata, msg.pDataLen, 1, &msg);
		g_csPlateData.ReleaseBuffer();
		///////////////////////////////////////////////////////////////////车牌图片
		msg.pdata = m_cBufferManager.GetPicBuffer(1);
		msg.pDataLen = stPlateInfo.dwPicPlateLen;
		msg.dataType = 0;  // 0 是图片
		SendData(m_ShowPlateImage, msg.pdata, msg.pDataLen, 0, &msg);
		///////////////////////////////////////////////////////////////////车辆图片
		msg.pdata = m_cBufferManager.GetPicBuffer(0);
		msg.pDataLen = stPlateInfo.dwPicLen;
		msg.dataType = 0;  // 0 是图片
		SendData(m_ShowFirstImage, msg.pdata, msg.pDataLen, 0, &msg);
		///////////////////////////////////////////////////////////////////车脸图片
		msg.pdata = m_cBufferManager.GetPicBuffer(3);
		msg.pDataLen = stPlateInfo.dwCarPicLen;
		msg.dataType = 0;  // 0 是图片
		SendData(m_ShowSecondImage, msg.pdata, msg.pDataLen, 0, &msg);
	}
#endif
	OnReceivePlate();
	return 0;
}

// CToHvShowCtrl message handlers

void CToHvShowCtrl::OnRecvSnapImageFlagChanged(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your property handler code here
	CString csLog;
	csLog.Format(_T("修改RecvSnapImageFlag属性： %d"), m_RecvSnapImageFlag);
	WriteLogFile((CA2T)__FUNCTION__, FALSE, csLog);

	BoundPropertyChanged(0x1);
	SetModifiedFlag();
}

void CToHvShowCtrl::OnRecvPlateImageFlagChanged(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your property handler code here
	CString csLog;
	csLog.Format(_T("修改RecvPlateImage属性： %d"), m_RecvPlateImageFlag);
	WriteLogFile((CA2T)__FUNCTION__, FALSE, csLog);

	BoundPropertyChanged(0x2);
	SetModifiedFlag();
}

void CToHvShowCtrl::OnRecvPlateBinImageFlagChanged(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your property handler code here
	CString csLog;
	csLog.Format(_T("修改RecvPlateBinImageFlag属性： %d"), m_RecvPlateBinImageFlag);
	WriteLogFile((CA2T)__FUNCTION__, FALSE, csLog);

	BoundPropertyChanged(0x3);
	SetModifiedFlag();
}

void CToHvShowCtrl::OnRecvSnapDriverImageFlagChanged(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your property handler code here
	CString csLog;
	csLog.Format(_T("修改RecvSnapDriverImageFlag属性： %d"), m_RecvSnapDriverImageFlag);
	WriteLogFile((CA2T)__FUNCTION__, FALSE, csLog);

	BoundPropertyChanged(0x4);
	SetModifiedFlag();
}

void CToHvShowCtrl::OnAutoSaveFlagChanged(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your property handler code here
	CString csLog;
	csLog.Format(_T("修改AutoSaveFlag属性： %d"), m_AutoSaveFlag);
	WriteLogFile((CA2T)__FUNCTION__, FALSE, csLog);

	BoundPropertyChanged(0x5);
	SetModifiedFlag();
}

void CToHvShowCtrl::OnShowPlateImageChanged(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your property handler code here
	CString csLog;
	csLog.Format(_T("修改ShowPlateImage属性： %s"), m_ShowPlateImage);
	WriteLogFile((CA2T)__FUNCTION__, FALSE, csLog);

	BoundPropertyChanged(0x6);
	SetModifiedFlag();
}

void CToHvShowCtrl::OnShowFirstImageChanged(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your property handler code here
	CString csLog;
	csLog.Format(_T("修改ShowFirstImage属性： %s"), m_ShowFirstImage);
	WriteLogFile((CA2T)__FUNCTION__, FALSE, csLog);

	BoundPropertyChanged(0x7);
	SetModifiedFlag();
}

void CToHvShowCtrl::OnShowSecondImageChanged(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your property handler code here
	CString csLog;
	csLog.Format(_T("修改ShowSecondImage属性： %s"), m_ShowSecondImage);
	WriteLogFile((CA2T)__FUNCTION__, FALSE, csLog);

	BoundPropertyChanged(0x8);
	SetModifiedFlag();
}

void CToHvShowCtrl::OnShowPlateResultChanged(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your property handler code here
	CString csLog;
	csLog.Format(_T("修改ShowPlateResult属性： %s"), m_ShowPlateResult);
	WriteLogFile((CA2T)__FUNCTION__, FALSE, csLog);

	BoundPropertyChanged(0x9);
	SetModifiedFlag();
}

void CToHvShowCtrl::OnLogFileChanged(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your property handler code here
	CString csLog;
	csLog.Format(_T("修改LogFile属性： %d"), m_LogFile);
	WriteLogFile((CA2T)__FUNCTION__, FALSE, csLog);

	BoundPropertyChanged(0x19);
	SetModifiedFlag();
}


///////////////////////////////////////////////////////////////////////////////////////
//判断是否为正确的IP格式
bool isIPAddress(const char *s)  
{  
	const char *pChar;  
	bool rv = true;  
	int tmp1, tmp2, tmp3, tmp4, i;  

	while( 1 )  
	{  
		i = sscanf_s(s, "%d.%d.%d.%d", &tmp1, &tmp2, &tmp3, &tmp4);  

		if( i != 4 )  
		{  
			rv = false;  
			break;  
		}  

		if( (tmp1 > 255) || (tmp2 > 255) || (tmp3 > 255) || (tmp4 > 255) )  
		{  
			rv = false;  
			break;  
		}  

		for( pChar = s; *pChar != 0; pChar++ )  
		{  
			if( (*pChar != '.')  
				&& ((*pChar < '0') || (*pChar > '9')) )  
			{  
				rv = false;  
				break;  
			}  
		}  
		break;  
	}  

	return rv;  
}  



void CToHvShowCtrl::ConnectTo(LPCTSTR strAddr)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString tmp;
	// TODO: Add your dispatch handler code here
#ifndef ONE_DEVICE
	pDeviceInfo pDeviceinfoAdd = new DeviceInfo();
	//if(m_lUid>=0)
	//{
	//	Disconnect();
	//}
	if(strAddr == NULL)
	{
		return;
	}
	CString csAddr(strAddr);
	int pos = csAddr.Find(_T(":"),0);
	USES_CONVERSION;
	if(pos == -1)//只输入IP将默认端口为8000
	{
		pDeviceinfoAdd->m_csIPAddr = csAddr;

		if(isIPAddress(W2A(pDeviceinfoAdd->m_csIPAddr.GetBuffer(0))))
		{
			pDeviceinfoAdd->m_csIPAddr.ReleaseBuffer();
			pDeviceinfoAdd->m_csPort = _T("8000");
		}
		else
		{
			pDeviceinfoAdd->m_csIPAddr.ReleaseBuffer();
			pDeviceinfoAdd->m_csIPAddr = _T("");
			pDeviceinfoAdd->m_csPort = _T("");
			pDeviceinfoAdd->m_lUid = -1;
			return;
		}
	}
	else//输入IP和端口号 IP:Port 的形式
	{
		pDeviceinfoAdd->m_csIPAddr = csAddr.Left(pos);
		pDeviceinfoAdd->m_csPort = csAddr.Right(csAddr.GetLength() - pos - 1);
		if(isIPAddress(W2A(pDeviceinfoAdd->m_csIPAddr.GetBuffer(0))))
		{
			int iPort = atoi(W2A(pDeviceinfoAdd->m_csPort.GetBuffer(0)));
			if(iPort >= 1 && iPort <= 65535)
			{
				pDeviceinfoAdd->m_csIPAddr.ReleaseBuffer();
				pDeviceinfoAdd->m_csPort.ReleaseBuffer();
			}
			else
			{
				pDeviceinfoAdd->m_csIPAddr.ReleaseBuffer();
				pDeviceinfoAdd->m_csPort.ReleaseBuffer();
				pDeviceinfoAdd->m_csIPAddr = _T("");
				pDeviceinfoAdd->m_csPort = _T("");
				pDeviceinfoAdd->m_lUid = -1;
				return;
			}
		}
		else
		{
			pDeviceinfoAdd->m_csIPAddr.ReleaseBuffer();
			pDeviceinfoAdd->m_csIPAddr = _T("");
			pDeviceinfoAdd->m_csPort = _T("");
			pDeviceinfoAdd->m_lUid = -1;
			return;
		}
	}
	//若运行到此处，说明输入的参数为正确的参数。之后即开始连接设备
	memset(&m_stDeviceInfo, 0, sizeof(m_stDeviceInfo));
	pDeviceinfoAdd->m_lUid = NET_DVR_Login_V30(W2A(pDeviceinfoAdd->m_csIPAddr.GetBuffer(0)), atoi(W2A(pDeviceinfoAdd->m_csPort.GetBuffer(0))), "admin", "12345", &m_stDeviceInfo);
	m_lLoginUid = pDeviceinfoAdd->m_lUid;
	pDeviceinfoAdd->m_csIPAddr.ReleaseBuffer();
	pDeviceinfoAdd->m_csPort.ReleaseBuffer();

	if (pDeviceinfoAdd->m_lUid == -1)
	{
		tmp.Format(_T("登录设备: %s 失败!!"),pDeviceinfoAdd->m_csIPAddr);
		WriteLogFile((CA2T)__FUNCTION__, TRUE, tmp);
		return /*RETVAL_LOGIN_ERROR*/;
	}
	////////////////////////////////////////////////////////////////////
	EnterCriticalSection(&m_csVecLock);
	std::vector<pDeviceInfo>::iterator iter = m_vecDeviceInfoTable.begin();
	while(iter != m_vecDeviceInfoTable.end())
	{
		if((*iter)->m_csIPAddr == pDeviceinfoAdd->m_csIPAddr)
		{
			LeaveCriticalSection(&m_csVecLock);
			NET_DVR_Logout(m_lLoginUid);
			tmp.Format(_T("该设备 %s uid:%d 已经登录！！！"),pDeviceinfoAdd->m_csIPAddr,(*iter)->m_lUid);
			WriteLogFile((CA2T)__FUNCTION__, TRUE, tmp);
			//WriteLogFile((CA2T)__FUNCTION__, FALSE, _T("该设备已经登录！！！"));
			return;
		}
		iter ++;
	}
	m_vecDeviceInfoTable.push_back(pDeviceinfoAdd);
	LeaveCriticalSection(&m_csVecLock);
	///////////////////////////////////////////////////////
	tmp.Format(_T("该设备 %s uid:%d 登录成功！！！"),pDeviceinfoAdd->m_csIPAddr,pDeviceinfoAdd->m_lUid);
	WriteLogFile((CA2T)__FUNCTION__, FALSE, tmp);

	if(SetUpAlarm() > 0)
	{
		tmp.Format(_T("该设备 %s uid:%d 布防失败！！！"),pDeviceinfoAdd->m_csIPAddr,pDeviceinfoAdd->m_lUid);
		WriteLogFile((CA2T)__FUNCTION__, FALSE, tmp);
	}
	else
	{
		tmp.Format(_T("该设备 %s uid:%d 布防成功！！！"),pDeviceinfoAdd->m_csIPAddr,pDeviceinfoAdd->m_lUid);
		WriteLogFile((CA2T)__FUNCTION__, FALSE, tmp);
	}
	return;
#else
	if(m_lUid>=0)
	{
		Disconnect();
	}
	if(strAddr == NULL)
	{
		return;
	}
	CString csAddr(strAddr);
	int pos = csAddr.Find(_T(":"),0);
	USES_CONVERSION;
	if(pos == -1)//只输入IP将默认端口为8000
	{
		m_csIPAddr = csAddr;

		if(isIPAddress(W2A(m_csIPAddr.GetBuffer(0))))
		{
			m_csIPAddr.ReleaseBuffer();
			m_csPort = _T("8000");
		}
		else
		{
			m_csIPAddr.ReleaseBuffer();
			m_csIPAddr = _T("");
			m_csPort = _T("");
			m_lUid = -1;
			return;
		}
	}
	else//输入IP和端口号 IP:Port 的形式
	{
		m_csIPAddr = csAddr.Left(pos);
		m_csPort = csAddr.Right(csAddr.GetLength() - pos - 1);
		if(isIPAddress(W2A(m_csIPAddr.GetBuffer(0))))
		{
			int iPort = atoi(W2A(m_csPort.GetBuffer(0)));
			if(iPort >= 1 && iPort <= 65535)
			{
				m_csIPAddr.ReleaseBuffer();
				m_csPort.ReleaseBuffer();
			}
			else
			{
				m_csIPAddr.ReleaseBuffer();
				m_csPort.ReleaseBuffer();
				m_csIPAddr = _T("");
				m_csPort = _T("");
				m_lUid = -1;
				return;
			}
		}
		else
		{
			m_csIPAddr.ReleaseBuffer();
			m_csIPAddr = _T("");
			m_csPort = _T("");
			m_lUid = -1;
			return;
		}
	}
	//若运行到此处，说明输入的参数为正确的参数。之后即开始连接设备
	memset(&m_stDeviceInfo, 0, sizeof(m_stDeviceInfo));
	m_lUid = NET_DVR_Login_V30(W2A(m_csIPAddr.GetBuffer(0)), atoi(W2A(m_csPort.GetBuffer(0))), "admin", "12345", &m_stDeviceInfo);
	m_csIPAddr.ReleaseBuffer();
	m_csPort.ReleaseBuffer();

	if (m_lUid == -1)
	{
		WriteLogFile((CA2T)__FUNCTION__, TRUE, _T("登录设备失败"));
		return /*RETVAL_LOGIN_ERROR*/;
	}
	WriteLogFile((CA2T)__FUNCTION__, FALSE, _T("登录设备"));

	if(SetUpAlarm() > 0)
	{
		WriteLogFile((CA2T)__FUNCTION__, FALSE, _T("布防失败"));
	}
	else
	{
		WriteLogFile((CA2T)__FUNCTION__, FALSE, _T("布防成功"));
	}
	return;
#endif
}

LONG CToHvShowCtrl::GetStatus(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your dispatch handler code here
	BOOL	bRet = TRUE;
	LONG lRet = -2;
	NET_DVR_DEVICEINFO_V30 stDeviceInfo;
	LONG		lUserID;
#ifndef ONE_DEVICE
	EnterCriticalSection(&m_csVecLock);
	std::vector<pDeviceInfo>::iterator iter = m_vecDeviceInfoTable.begin();
	while (iter != m_vecDeviceInfoTable.end())
	{
		if(lRet == -2)
		{
			lRet = 0;//第一次进入while循环，为了防止第一次循环不进来返回正确的结果，其实是无设备，说明登录失败。
		}
		if((*iter)->m_lUid >= 0)
		{
			memset(&stDeviceInfo, 0, sizeof(NET_DVR_DEVICEINFO_V30));
			USES_CONVERSION;
			lUserID = NET_DVR_Login_V30(W2A((*iter)->m_csIPAddr.GetBuffer(0)), atoi(W2A((*iter)->m_csPort.GetBuffer(0))), "admin", "12345", &stDeviceInfo);
			(*iter)->m_csIPAddr.ReleaseBuffer();
			(*iter)->m_csPort.ReleaseBuffer();
			if (lUserID == -1)
			{
				CString tmp;
				tmp.Format(_T("设备状态异常,相机IP为 %s"),(*iter)->m_csIPAddr);
				WriteLogFile((CA2T)__FUNCTION__, TRUE, tmp);
				lRet = -1;
			}
			bRet = NET_DVR_Logout(lUserID);
		}
		else
		{
			CString tmp;
			tmp.Format(_T("设备状态异常,相机IP为 %s"),(*iter)->m_csIPAddr);
			WriteLogFile((CA2T)__FUNCTION__, TRUE, tmp);
			lRet = -1;
		}
		iter++;
	}
	Invalidate(TRUE);
	LeaveCriticalSection(&m_csVecLock);
	return lRet;
#else
	if (m_lUid < 0)
	{
		return RETVAL_ERROR;
	}
	memset(&stDeviceInfo, 0, sizeof(NET_DVR_DEVICEINFO_V30));
	USES_CONVERSION;
	lUserID = NET_DVR_Login_V30(W2A(m_csIPAddr.GetBuffer(0)), atoi(W2A(m_csPort.GetBuffer(0))), "admin", "12345", &stDeviceInfo);
	m_csIPAddr.ReleaseBuffer();
	m_csPort.ReleaseBuffer();
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
#endif
}



void CToHvShowCtrl::Disconnect(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// TODO: Add your dispatch handler code here
	CleanAlarm();	
#ifndef ONE_DEVICE
	EnterCriticalSection(&m_csVecLock);
	std::vector<pDeviceInfo>::iterator iter = m_vecDeviceInfoTable.begin();
	while (iter != m_vecDeviceInfoTable.end())
	{
		if((*iter)->m_lUid >= 0)
		{
			if(!NET_DVR_Logout((*iter)->m_lUid))
			{
				CString tmp;
				tmp.Format(_T("登出设备失败,相机IP为 %s"),(*iter)->m_csIPAddr);
				WriteLogFile((CA2T)__FUNCTION__, TRUE, tmp);
			}
			else
			{
				(*iter)->m_lUid = -1;
				delete (*iter);
			}
		}
		iter++;
	}
	Invalidate(TRUE);
	m_vecDeviceInfoTable.clear();
	LeaveCriticalSection(&m_csVecLock);
#else
	if (m_lUid < 0)
	{
		Invalidate(TRUE);
		return /*RETVAL_OK*/;		
	}
	BOOL  bRet = NET_DVR_Logout(m_lUid);
	if (bRet == FALSE)
	{
		WriteLogFile((CA2T)__FUNCTION__, TRUE, _T("关闭设备失败"));
		return /*RETVAL_LOGOUT_ERROR*/;
	}
	m_lUid = -1;
	Invalidate(TRUE);
#endif
	WriteLogFile((CA2T)__FUNCTION__, FALSE, _T("关闭设备"));
	return ;
}

void CToHvShowCtrl::SetImgSavePath(LPCTSTR strPath)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your dispatch handler code here
	if (strPath == NULL)
	{
		WriteLogFile((CA2T)__FUNCTION__, TRUE, _T("无效参数"));
		return ;
	}

	m_csImagePath.Format(_T("%s"),strPath);
	while(m_csImagePath.GetAt(m_csImagePath.GetLength() - 1) == _T('\\'))
	{
		m_csImagePath.Delete(m_csImagePath.GetLength() - 1);
	}
	CString csLog;
	csLog.Format(_T("设置保存路径为： %s"), m_csImagePath);
	WriteLogFile((CA2T)__FUNCTION__, FALSE, csLog);
}

BSTR CToHvShowCtrl::GetPlate(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	NET_DVR_PLATE_RESULT		stPlateInfo;
	memset(&stPlateInfo, 0, sizeof(stPlateInfo));
#ifndef ONE_DEVICE
	if (GetBufferManagerByUid(m_lCurrentUid) != NULL && GetBufferManagerByUid(m_lCurrentUid)->GetPlateInfo(&stPlateInfo) == true)
	{
		strResult.Format(_T("%s"),(CA2T)(stPlateInfo.struPlateInfo.sLicense));
		if (strResult != _T("无车牌") && strResult != _T(""))
		{
			strResult.Delete(0);
		}
		else
		{
			strResult = _T("未检测");
		}
	}
#else
	if (m_cBufferManager.GetPlateInfo(&stPlateInfo) == true)
	{
		strResult.Format(_T("%s"),(CA2T)(stPlateInfo.struPlateInfo.sLicense));
		if (strResult != _T("无车牌") && strResult != _T(""))
		{
			strResult.Delete(0);
		}
		else
		{
			strResult = _T("未检测");
		}
	}
#endif

	CString csLog = _T("获取车牌号码:	");
	csLog += strResult;
	WriteLogFile((CA2T)__FUNCTION__, FALSE, csLog);
	
	return strResult.AllocSysString();
}

BSTR CToHvShowCtrl::GetPlateColor(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	NET_DVR_PLATE_RESULT		stPlateInfo;
	memset(&stPlateInfo, 0, sizeof(stPlateInfo));
#ifndef ONE_DEVICE
	if (GetBufferManagerByUid(m_lCurrentUid) != NULL && GetBufferManagerByUid(m_lCurrentUid)->GetPlateInfo(&stPlateInfo) == true)
	{
		//strResult.Format(_T("%s"),(CA2T)(stPlateInfo.struPlateInfo.byColor));
		switch(stPlateInfo.struPlateInfo.byColor)
		{
		case VCA_BLUE_PLATE:
			{
				strResult = _T("蓝");
			}
			break;
		case VCA_YELLOW_PLATE:
			{
				strResult = _T("黄");
			}
			break;
		case VCA_WHITE_PLATE:
			{
				strResult = _T("白");
			}
			break;
		case VCA_BLACK_PLATE:
			{
				strResult = _T("黑");
			}
			break;
		default:
			{
				strResult = _T("无");
			}
		}
	}
#else
	if (m_cBufferManager.GetPlateInfo(&stPlateInfo) == true)
	{
		//strResult.Format(_T("%s"),(CA2T)(stPlateInfo.struPlateInfo.byColor));
		switch(stPlateInfo.struPlateInfo.byColor)
		{
		case VCA_BLUE_PLATE:
			{
				strResult = _T("蓝");
			}
			break;
		case VCA_YELLOW_PLATE:
			{
				strResult = _T("黄");
			}
			break;
		case VCA_WHITE_PLATE:
			{
				strResult = _T("白");
			}
			break;
		case VCA_BLACK_PLATE:
			{
				strResult = _T("黑");
			}
			break;
		default:
			{
				strResult = _T("无");
			}
		}
	}
#endif

	CString csLog = _T("获取车牌颜色为:	");
	csLog += strResult;
	WriteLogFile((CA2T)__FUNCTION__, FALSE, csLog);

	return strResult.AllocSysString();
}

BSTR CToHvShowCtrl::GetPlateInfo(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	NET_DVR_PLATE_RESULT		stPlateInfo;
	memset(&stPlateInfo, 0, sizeof(stPlateInfo));
#ifndef ONE_DEVICE
	if (GetBufferManagerByUid(m_lCurrentUid) != NULL && GetBufferManagerByUid(m_lCurrentUid)->GetPlateInfo(&stPlateInfo) == true)
	{
		switch(stPlateInfo.struVehicleInfo.byIllegalType)
		{
		case 3:
			{
				strResult.Format(_T("%s"),_T("车辆逆向行驶:是\n"));
			}
			break;
		default:
			{
				strResult.Format(_T("%s"),_T("车辆逆向行驶:否\n"));
			}
			break;
		}
	}
#else
	if (m_cBufferManager.GetPlateInfo(&stPlateInfo) == true)
	{
		switch(stPlateInfo.struVehicleInfo.byIllegalType)
		{
		case 3:
			{
				strResult.Format(_T("%s"),_T("车辆逆向行驶:是\n"));
			}
			break;
		default:
			{
				strResult.Format(_T("%s"),_T("车辆逆向行驶:否\n"));
			}
			break;
		}
	}
#endif
	return strResult.AllocSysString();
}

LONG CToHvShowCtrl::SaveSnapImage(LPCTSTR strFileName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your dispatch handler code here
	LONG		retVal = RETVAL_OK;
	NET_DVR_PLATE_RESULT stPlateInfo;

	if (strFileName == NULL)
	{
		WriteLogFile((CA2T)__FUNCTION__, TRUE, _T("无效参数"));
		return RETVAL_INVALID_PARAM;
	}
	if (m_RecvSnapImageFlag == 0)
	{
		// 当期不接受图片
		WriteLogFile((CA2T)__FUNCTION__, FALSE, _T("当前不接受图片"));
		return RETVAL_NOT_RECV_PIC;
	}
	memset(&stPlateInfo, 0, sizeof(NET_DVR_PLATE_RESULT));
#ifndef ONE_DEVICE
	if (GetBufferManagerByUid(m_lCurrentUid) != NULL && GetBufferManagerByUid(m_lCurrentUid)->GetPlateInfo(&stPlateInfo) == false)
	{
		WriteLogFile((CA2T)__FUNCTION__, FALSE, _T("获取图片失败"));;
		return RETVAL_GET_PLATE_INFO_ERROR;
	}
#else
	if (m_cBufferManager.GetPlateInfo(&stPlateInfo) == false)
	{
		WriteLogFile((CA2T)__FUNCTION__, FALSE, _T("获取图片失败"));;
		return RETVAL_GET_PLATE_INFO_ERROR;
	}
#endif
	///////////////////////////////////////////////////////////////////////////////////////
	//完整路径
	CString csfullPath;
	csfullPath.Format(_T("%s"),strFileName);
	if (csfullPath.GetLength() >= MAX_PATH)
	{ 
		WriteLogFile((CA2T)__FUNCTION__, FALSE, _T("图片路径过长，抓图失败"));;
		return -1;
	}
	if (CreateFileDir(csfullPath) == FALSE)
	{
		return -1;
	}
#ifndef ONE_DEVICE
	char * pchpic = GetBufferManagerByUid(m_lCurrentUid)->GetPicBuffer(0);
#else
	char * pchpic = m_cBufferManager.GetPicBuffer(0);
#endif
	if (pchpic != NULL && stPlateInfo.dwPicLen != 0)
	{
		if (SavePic(csfullPath, (BYTE *)pchpic, stPlateInfo.dwPicLen) == FALSE)
		{
			WriteLogFile((CA2T)__FUNCTION__, TRUE, _T("保存车辆图片失败"));
			retVal = RETVAL_SAVE_PIC_ERROR;
		}
	}
	else
	{
		WriteLogFile((CA2T)__FUNCTION__, FALSE, _T("没有车辆图片"));
		retVal = RETVAL_HAVE_NO_IMAGE;
	}
	if (retVal == RETVAL_OK)
	{
		CString		csLog;
		csLog.Format(_T("保存车辆图片： 地址：%s"), csfullPath);
		WriteLogFile((CA2T)__FUNCTION__, FALSE, csLog);
	}
	
	return retVal;
}

LONG CToHvShowCtrl::SaveSnapImage2(LPCTSTR strFileName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your dispatch handler code here
	LONG		retVal = RETVAL_OK;
	NET_DVR_PLATE_RESULT	stPlateInfo;

	if (strFileName == NULL)
	{
		WriteLogFile((CA2T)__FUNCTION__, TRUE, _T("无效参数"));
		return RETVAL_INVALID_PARAM;
	}
	if (m_RecvSnapDriverImageFlag == 0)
	{
		// 当期不接受图片
		WriteLogFile((CA2T)__FUNCTION__, FALSE, _T("当前不接受图片"));
		return RETVAL_NOT_RECV_PIC;
	}
	memset(&stPlateInfo, 0, sizeof(NET_DVR_PLATE_RESULT));
#ifndef ONE_DEVICE
	if (GetBufferManagerByUid(m_lCurrentUid) != NULL && GetBufferManagerByUid(m_lCurrentUid)->GetPlateInfo(&stPlateInfo) == false)
	{
		WriteLogFile((CA2T)__FUNCTION__, FALSE, _T("获取图片失败"));;
		return RETVAL_GET_PLATE_INFO_ERROR;
	}
#else
	if (m_cBufferManager.GetPlateInfo(&stPlateInfo) == false)
	{
		WriteLogFile((CA2T)__FUNCTION__, FALSE, _T("获取图片失败"));;
		return RETVAL_GET_PLATE_INFO_ERROR;
	}
#endif
	///////////////////////////////////////////////////////////////////////////////////////
	CString csfullPath;
	csfullPath.Format(_T("%s"),strFileName);
	if (csfullPath.GetLength() >= MAX_PATH)
	{ 
		WriteLogFile((CA2T)__FUNCTION__, FALSE, _T("图片路径过长，抓图失败"));;
		return -1;
	}
	if (CreateFileDir(csfullPath) == FALSE)
	{
		return -1;
	}
#ifndef ONE_DEVICE
	char * pchpic = GetBufferManagerByUid(m_lCurrentUid)->GetPicBuffer(3);
#else
	char * pchpic = m_cBufferManager.GetPicBuffer(3);
#endif
	if (pchpic != NULL && stPlateInfo.dwCarPicLen != 0)
	{
		if (SavePic(csfullPath, (BYTE *)pchpic, stPlateInfo.dwCarPicLen) == FALSE)
		{
			WriteLogFile((CA2T)__FUNCTION__, TRUE, _T("保存车脸图片失败"));
			retVal = RETVAL_SAVE_PIC_ERROR;
		}
	}
	else
	{
		WriteLogFile((CA2T)__FUNCTION__, FALSE, _T("没有车脸图片"));
		retVal = RETVAL_HAVE_NO_IMAGE;
	}
	if (retVal == RETVAL_OK)
	{
		CString		csLog;
		csLog.Format(_T("保存车脸图片： 地址：%s"), csfullPath);
		WriteLogFile((CA2T)__FUNCTION__, FALSE, csLog);
	}

	return retVal;
}

LONG CToHvShowCtrl::SavePlateImage(LPCTSTR strFileName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your dispatch handler code here
	LONG		retVal = RETVAL_OK;
	NET_DVR_PLATE_RESULT	stPlateInfo;

	if (strFileName == NULL)
	{
		WriteLogFile((CA2T)__FUNCTION__, TRUE, _T("无效参数"));
		return RETVAL_INVALID_PARAM;
	}
	if (m_RecvPlateImageFlag == 0)
	{
		// 当期不接受图片
		WriteLogFile((CA2T)__FUNCTION__, FALSE, _T("当前不接受图片"));
		return RETVAL_NOT_RECV_PIC;
	}
	memset(&stPlateInfo, 0, sizeof(NET_DVR_PLATE_RESULT));
#ifndef ONE_DEVICE
	if (GetBufferManagerByUid(m_lCurrentUid) != NULL && GetBufferManagerByUid(m_lCurrentUid)->GetPlateInfo(&stPlateInfo) == false)
	{
		WriteLogFile((CA2T)__FUNCTION__, FALSE, _T("获取图片失败"));;
		return RETVAL_GET_PLATE_INFO_ERROR;
	}
#else
	if (m_cBufferManager.GetPlateInfo(&stPlateInfo) == false)
	{
		WriteLogFile((CA2T)__FUNCTION__, FALSE, _T("获取图片失败"));;
		return RETVAL_GET_PLATE_INFO_ERROR;
	}
#endif
	///////////////////////////////////////////////////////////////////////////////////////
	CString csfullPath;
	csfullPath.Format(_T("%s"),strFileName);
	if (csfullPath.GetLength() >= MAX_PATH)
	{ 
		WriteLogFile((CA2T)__FUNCTION__, FALSE, _T("图片路径过长，抓图失败"));;
		return -1;
	}
	if (CreateFileDir(csfullPath) == FALSE)
	{
		return -1;
	}
#ifndef ONE_DEVICE
	char * pchpic = GetBufferManagerByUid(m_lCurrentUid)->GetPicBuffer(1);
#else
	char * pchpic = m_cBufferManager.GetPicBuffer(1);
#endif
	if (pchpic != NULL && stPlateInfo.dwPicPlateLen != 0)
	{
		if (SavePic(csfullPath, (BYTE *)pchpic, stPlateInfo.dwPicPlateLen) == FALSE)
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
		csLog.Format(_T("保存车牌图片： 地址：%s"), csfullPath);
		WriteLogFile((CA2T)__FUNCTION__, FALSE, csLog);
	}

	return retVal;
}

LONG CToHvShowCtrl::SavePlateBinImage(LPCTSTR strFileName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your dispatch handler code here
	LONG		retVal = RETVAL_OK;
	NET_DVR_PLATE_RESULT	stPlateInfo;

	if (strFileName == NULL)
	{
		WriteLogFile((CA2T)__FUNCTION__, TRUE, _T("无效参数"));
		return RETVAL_INVALID_PARAM;
	}
	if (m_RecvPlateBinImageFlag == 0)
	{
		// 当期不接受图片
		WriteLogFile((CA2T)__FUNCTION__, FALSE, _T("当前不接受图片"));
		return RETVAL_NOT_RECV_PIC;
	}
	memset(&stPlateInfo, 0, sizeof(NET_DVR_PLATE_RESULT));
#ifndef ONE_DEVICE
	if (GetBufferManagerByUid(m_lCurrentUid) != NULL && GetBufferManagerByUid(m_lCurrentUid)->GetPlateInfo(&stPlateInfo) == false)
	{
		WriteLogFile((CA2T)__FUNCTION__, FALSE, _T("获取图片失败"));;
		return RETVAL_GET_PLATE_INFO_ERROR;
	}
#else
	if (m_cBufferManager.GetPlateInfo(&stPlateInfo) == false)
	{
		WriteLogFile((CA2T)__FUNCTION__, FALSE, _T("获取图片失败"));;
		return RETVAL_GET_PLATE_INFO_ERROR;
	}
#endif
	///////////////////////////////////////////////////////////////////////////////////////
	CString csfullPath;
	csfullPath.Format(_T("%s"),strFileName);
	if (csfullPath.GetLength() >= MAX_PATH)
	{ 
		WriteLogFile((CA2T)__FUNCTION__, FALSE, _T("图片路径过长，抓图失败"));;
		return -1;
	}
	if (CreateFileDir(csfullPath) == FALSE)
	{
		return -1;
	}
#ifndef ONE_DEVICE
	char * pchpic = GetBufferManagerByUid(m_lCurrentUid)->GetPicBuffer(2);
#else
	char * pchpic = m_cBufferManager.GetPicBuffer(2);
#endif
	if (pchpic != NULL && stPlateInfo.dwBinPicLen != 0)
	{
		if (SavePic(csfullPath, (BYTE *)pchpic, stPlateInfo.dwBinPicLen) == FALSE)
		{
			WriteLogFile((CA2T)__FUNCTION__, TRUE, _T("保存车牌二值图片失败"));
			retVal = RETVAL_SAVE_PIC_ERROR;
		}
	}
	else
	{
		WriteLogFile((CA2T)__FUNCTION__, FALSE, _T("没有车牌二值图片"));
		retVal = RETVAL_HAVE_NO_IMAGE;
	}
	if (retVal == RETVAL_OK)
	{
		CString		csLog;
		csLog.Format(_T("保存车牌二值图片： 地址：%s"), csfullPath);
		WriteLogFile((CA2T)__FUNCTION__, FALSE, csLog);
	}

	return retVal;
}

LONG CToHvShowCtrl::SavePlateBin2BMP(LPCTSTR strFileName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your dispatch handler code here
	LONG		retVal = RETVAL_OK;
	NET_DVR_PLATE_RESULT	stPlateInfo;

	if (strFileName == NULL)
	{
		WriteLogFile((CA2T)__FUNCTION__, TRUE, _T("无效参数"));
		return RETVAL_INVALID_PARAM;
	}
	if (m_RecvPlateBinImageFlag == 0)
	{
		// 当期不接受图片
		WriteLogFile((CA2T)__FUNCTION__, FALSE, _T("当前不接受图片"));
		return RETVAL_NOT_RECV_PIC;
	}
	memset(&stPlateInfo, 0, sizeof(NET_DVR_PLATE_RESULT));
#ifndef ONE_DEVICE
	if (GetBufferManagerByUid(m_lCurrentUid) != NULL && GetBufferManagerByUid(m_lCurrentUid)->GetPlateInfo(&stPlateInfo) == false)
	{
		WriteLogFile((CA2T)__FUNCTION__, FALSE, _T("获取图片失败"));;
		return RETVAL_GET_PLATE_INFO_ERROR;
	}
#else
	if (GetBufferManagerByUid(m_lCurrentUid) != NULL && m_cBufferManager.GetPlateInfo(&stPlateInfo) == false)
	{
		WriteLogFile((CA2T)__FUNCTION__, FALSE, _T("获取图片失败"));;
		return RETVAL_GET_PLATE_INFO_ERROR;
	}
#endif
	///////////////////////////////////////////////////////////////////////////////////////
	CString csfullPath;
	csfullPath.Format(_T("%s"),strFileName);
	if (csfullPath.GetLength() >= MAX_PATH)
	{ 
		WriteLogFile((CA2T)__FUNCTION__, FALSE, _T("图片路径过长，抓图失败"));;
		return -1;
	}
	if (CreateFileDir(csfullPath) == FALSE)
	{
		return -1;
	}

#ifndef ONE_DEVICE
	char * pchpic = GetBufferManagerByUid(m_lCurrentUid)->GetPicBuffer(2);
#else
	char * pchpic = m_cBufferManager.GetPicBuffer(2);
#endif

	if (pchpic != NULL && stPlateInfo.dwBinPicLen != 0)
	{
		memset(m_pchPlateBinBMP,0,m_iBinBMPSize);
		if(0 == ConvertBin2BmpData((BYTE *)pchpic,(BYTE *)m_pchPlateBinBMP,&m_iBinBMPSize))
		{
			if (SavePic(csfullPath, (BYTE *)m_pchPlateBinBMP, m_iBinBMPSize) == FALSE)
			{
				WriteLogFile((CA2T)__FUNCTION__, TRUE, _T("保存车牌二值图BMP失败"));
				retVal = RETVAL_SAVE_PIC_ERROR;
			}
		}
		else
		{
			WriteLogFile((CA2T)__FUNCTION__, TRUE, _T("转换车牌二值图BMP失败"));
			retVal = RETVAL_SAVE_PIC_ERROR;
		}
	}
	else
	{
		WriteLogFile((CA2T)__FUNCTION__, FALSE, _T("没有车牌二值图BMP"));
		retVal = RETVAL_HAVE_NO_IMAGE;
	}
	if (retVal == RETVAL_OK)
	{
		CString		csLog;
		csLog.Format(_T("保存车牌二值BMP图片： 地址：%s"), csfullPath);
		WriteLogFile((CA2T)__FUNCTION__, FALSE, csLog);
	}
	return retVal;
}

BSTR CToHvShowCtrl::GetImageRecTime(LONG lImgType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	NET_DVR_PLATE_RESULT		stPlateInfo;
	memset(&stPlateInfo, 0, sizeof(stPlateInfo));
#ifndef ONE_DEVICE
	if (GetBufferManagerByUid(m_lCurrentUid) != NULL && GetBufferManagerByUid(m_lCurrentUid)->GetPlateInfo(&stPlateInfo) == true)
	{
		USES_CONVERSION;
		strResult.Format(_T("%s"),A2W((char *)(stPlateInfo.byAbsTime)));//yyyy4mm6dd8hh10mm12ss14xxx
		strResult.Insert(14,_T('-'));
		strResult.Insert(12,_T(':'));
		strResult.Insert(10,_T(':'));
		strResult.Insert(8,_T(' '));
		strResult.Insert(6,_T('-'));
		strResult.Insert(4,_T('-'));
	}
	else
	{
		strResult = _T("");
	}
#else
	if (m_cBufferManager.GetPlateInfo(&stPlateInfo) == true)
	{
		USES_CONVERSION;
		strResult.Format(_T("%s"),A2W((char *)(stPlateInfo.byAbsTime)));//yyyy4mm6dd8hh10mm12ss14xxx
		strResult.Insert(14,_T('-'));
		strResult.Insert(12,_T(':'));
		strResult.Insert(10,_T(':'));
		strResult.Insert(8,_T(' '));
		strResult.Insert(6,_T('-'));
		strResult.Insert(4,_T('-'));
	}
	else
	{
		strResult = _T("");
	}
#endif

	return strResult.AllocSysString();
}

BSTR CToHvShowCtrl::GetImageFileName(LONG lImgType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	NET_DVR_PLATE_RESULT		stPlateInfo;
	memset(&stPlateInfo, 0, sizeof(stPlateInfo));
#ifndef ONE_DEVICE
	if (GetBufferManagerByUid(m_lCurrentUid) != NULL && GetBufferManagerByUid(m_lCurrentUid)->GetPlateInfo(&stPlateInfo) == true)
	{
		//完整路径
		CString csGetTimeDir;
		CString csFileName;
		USES_CONVERSION;
		csGetTimeDir.Format(_T("%s"),A2W((char *)(stPlateInfo.byAbsTime)));
		csFileName.Format(_T("%s"),csGetTimeDir.Right(9));
		csFileName.Insert(9,_T('-'));
		csFileName.Insert(6,_T('-'));
		///////////////////////////////////////////////////////////////////////
		switch(lImgType)
		{
		case 0:
			{
				csFileName += _T("fullimg.jpg");
			}
			break;
		case 1:
			{
				csFileName += _T("plateimg.jpg");
			}
			break;
		case 2:
			{
				csFileName += _T("binimg.bin");
			}
			break;
		case 3:
			{
				csFileName += _T("driver.jpg");
			}
			break;
		case 4:
			{
				csFileName += _T("binimg.bmp");
			}
			break;
		default:
			{
				csFileName += _T("fullimg.jpg");
			}
		}
		strResult.Format(_T("%s\\%s\\%s\\%s"), m_csImagePath, GetIPAddrByUid(m_lCurrentUid),csGetTimeDir.Left(8),csFileName);
	}
#else
	if (m_cBufferManager.GetPlateInfo(&stPlateInfo) == true)
	{
		//完整路径
		CString csGetTimeDir;
		CString csFileName;
		USES_CONVERSION;
		csGetTimeDir.Format(_T("%s"),A2W((char *)(stPlateInfo.byAbsTime)));
		csFileName.Format(_T("%s"),csGetTimeDir.Right(9));
		csFileName.Insert(9,_T('-'));
		csFileName.Insert(6,_T('-'));
		///////////////////////////////////////////////////////////////////////
		switch(lImgType)
		{
		case 0:
			{
				csFileName += _T("fullimg.jpg");
			}
			break;
		case 1:
			{
				csFileName += _T("plateimg.jpg");
			}
			break;
		case 2:
			{
				csFileName += _T("binimg.bin");
			}
			break;
		case 3:
			{
				csFileName += _T("driver.jpg");
			}
			break;
		case 4:
			{
				csFileName += _T("binimg.bmp");
			}
			break;
		default:
			{
				csFileName += _T("fullimg.jpg");
			}
		}
		strResult.Format(_T("%s\\%s\\%s\\%s"), m_csImagePath, m_csIPAddr,csGetTimeDir.Left(8),csFileName);
	}
#endif

	if (strResult.GetLength() >= MAX_PATH)
	{ 
		WriteLogFile((CA2T)__FUNCTION__, FALSE, _T("图片路径过长，抓图失败"));
		strResult = _T("");
		return strResult.AllocSysString();
	}
	return strResult.AllocSysString();
}

BSTR CToHvShowCtrl::GetImageDataFromMem(LONG lImgType, VARIANT* pVarImg, LONG* lImgSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here

	return strResult.AllocSysString();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CToHvShowCtrl::SendData( CString CtrlName, char *pdata, long datalen, long iType, resultMSG *pmsg )
{
	bool freturn(false);
	bool find(false);
	if( m_fChangingCtrlName || CtrlName.IsEmpty() )
		return freturn;
	int icount = (int)m_ArrayDispatch.GetCount(); // 注：这个数组含有所有OLE控件, 后面会有说明如何获取
	for( int i = 0;i < icount; i++ )
	{
		DispatchArray arraydis;
		memset(&arraydis,0,sizeof(DispatchArray));
		arraydis = m_ArrayDispatch.GetAt(i);
		CComDispatchDriver drivertmp(arraydis.lpunknow);
		if( CtrlName.Compare(arraydis.Name) == 0 )
		{
			VARIANT v1,v2;
			CComVariant var(datalen);
			CComVariant vartype(iType);
			VariantInit(&v1);
			VariantInit(&v2);
			v1.vt= VT_BYREF|VT_I1;
			v1.pcVal = pdata;
			v2 = var;
			if( pdata == NULL )
			{
				drivertmp.Invoke0(L"Clean");
			}
			else
			{
				drivertmp.Invoke1(L"SetType",&vartype,0);	//设置本次数据属性
				HRESULT hr = drivertmp.Invoke2(L"SetData",&v1,&v2,0);	//发送数据
				if( SUCCEEDED(hr) )
				{
					freturn = true;
				}
			}
			find = true;
			break;
		}
	}
	if(!find)// 如果找不到相关的组件就发送用消息方式
	{
		HWND   hwndafter =   ::FindWindowEx(m_hWndParent,NULL,NULL,CtrlName);
		if( hwndafter )
		{
			if( pdata == NULL||datalen == 0 )
				::SendMessage(hwndafter,MSG_REPAINT_RESULT,0,NULL);
			else
				::SendMessage(hwndafter,MSG_CLI_RESULT,0,(LPARAM)pmsg);
		}
	}
	return freturn;
}

/*获取所有OLE控件*/
void CToHvShowCtrl::EnumAllControlNames()
{
	//如果是取窗口句柄
	IOleInPlaceSite *pOleInPlaceSite = NULL;
	if (NOERROR==m_pClientSite->QueryInterface(IID_IOleInPlaceSite, (LPVOID *)(&pOleInPlaceSite)))
	{
		HRESULT hresult = pOleInPlaceSite->GetWindow((HWND *)&m_hWndParent); 
		if(FAILED(hresult))
		{
			return;
		}
		pOleInPlaceSite->Release();
	}

	//取容器
	LPOLECONTAINER lpContainer = NULL;
	LPENUMUNKNOWN lpEnumUnk = NULL;
	HRESULT hr = m_pClientSite->GetContainer(&lpContainer);
	if(FAILED(hr)) 
	{
		return;
	}
	m_fChangingCtrlName = true;
	if (lpContainer == NULL)
		return;
	hr = lpContainer->EnumObjects(OLECONTF_EMBEDDINGS | OLECONTF_OTHERS,&lpEnumUnk);
	if(FAILED(hr)) 
	{
		lpContainer->Release();
		return;
	}
	m_ArrayDispatch.RemoveAll();
	LPUNKNOWN lpUnk = NULL;
	while (lpEnumUnk->Next(1, &lpUnk, NULL) == S_OK)
	{
		CComDispatchDriver driver(lpUnk);
		CComVariant var;
		hr = driver.GetPropertyByName(L"ControlName",&var);
		if(SUCCEEDED(hr)) 
		{
			CString szTmp((LPCWSTR)var.bstrVal);
			if(szTmp.IsEmpty())
			{
				continue;
			}
			DispatchArray arraydis;
			memset(&arraydis,0,sizeof(DispatchArray));
			arraydis.lpunknow = lpUnk;
			szTmp.Append(_T("\0"));
			wcscpy_s(arraydis.Name,MAX_PATH,szTmp.GetBuffer());
			szTmp.ReleaseBuffer();
			m_ArrayDispatch.Add(arraydis);
		}
	}
	lpEnumUnk->Release();
	lpContainer->Release();
	m_fChangingCtrlName = false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////日志模块

/** @fn		VOID CToHvShowCtrl::WriteLogFile(LPCTSTR szLog) 
 *	@brief	写日志
 *	@param	[IN]	szFun	函数名
 *	@param	[IN]	bError	是否为错误
 *	@param	[IN]	szLog	日志内容
 *	@return	void
 */
VOID CToHvShowCtrl::WriteLogFile(LPCTSTR szFun, BOOL bError, LPCTSTR szLog) 
{
	if (m_LogFile == 0 || m_hLogFile == NULL || m_hMutexLogFile == NULL)
	{
		return ;
	}
	SetFilePointer(m_hLogFile, 0, NULL, FILE_END);
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
	sprintf_s(szPrint,512,"%s\r\n", (CT2A)csLog);

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

/** @fn		BOOL CToHvShowCtrl::CreateLogFile()
 *	@brief	创建日志文件
 *	@return	BOOL	true表示正确，否则失败
 */
BOOL CToHvShowCtrl::CreateLogFile()
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
								GENERIC_WRITE|GENERIC_READ,
								FILE_SHARE_READ|FILE_SHARE_WRITE,
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


/** @fn		BOOL CToHvShowCtrl::CreateFileDir(LPCTSTR lpPath)
 *	@brief	创建指定的文件夹
 *	@param	[IN]	lpPath	文件路径
 *	@return	BOOL	TRUE表示成功，否则失败
 */
BOOL CToHvShowCtrl::CreateFileDir(LPCTSTR lpPath)
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



int CToHvShowCtrl::GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
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



BOOL CToHvShowCtrl::ConvertImgBytes2HBITMAP(BYTE* pImageBuffer,DWORD dwImageDataLen, int iWidth, int iHeight, WCHAR *  szFilePath)
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


/** @fn		void CToHvShowCtrl::InsertNewPlate(LPNET_DVR_PLATE_RESULT pPlateInfo)
 *	@brief	插入新车牌信息
 *	@param	[IN]	pPlateInfo		车牌信息
 *	@return	void
 */
void CToHvShowCtrl::InsertNewPlate(LPNET_DVR_PLATE_RESULT pPlateInfo)
{
	if (pPlateInfo == NULL)
	{
		WriteLogFile((CA2T)__FUNCTION__, FALSE, _T("收到新的车牌信息为空,回调处"));
		return ;
	}
#ifndef ONE_DEVICE
	if (GetBufferManagerByUid(m_lCurrentUid) != NULL && GetBufferManagerByUid(m_lCurrentUid)->InsertPlateInfo(pPlateInfo,this) == true)
	{
		WriteLogFile((CA2T)__FUNCTION__, FALSE, _T("收到新的车牌信息并存入缓存,回调处"));
		::PostMessage(this->m_hWnd,WM_USER+100,NULL,NULL);
		return;
	}
#else
	if (m_cBufferManager.InsertPlateInfo(pPlateInfo,this) == true)
	{
		WriteLogFile((CA2T)__FUNCTION__, FALSE, _T("收到新的车牌信息并存入缓存,回调处"));
		::PostMessage(this->m_hWnd,WM_USER+100,NULL,NULL);
		return;
	}
#endif
	WriteLogFile((CA2T)__FUNCTION__, TRUE, _T("收到新的车牌信息但是存入缓存失败,回调处"));	
	return;
}


void CALLBACK MessageCallback (LONG lCommand, NET_DVR_ALARMER *pAlarmer, char *pAlarmInfo, DWORD dwBufLen, void* pUser)
{
	CToHvShowCtrl	*pMian	= (CToHvShowCtrl*)pUser;
	if (pMian == NULL)
	{
		return ;
	}
	if (lCommand != COMM_UPLOAD_PLATE_RESULT)
	{
		//不是车牌直接返回
		return ;
	}
	if (pAlarmInfo == NULL)
	{
		return;
	}
#ifndef ONE_DEVICE
	USES_CONVERSION;
	pMian->m_lCurrentUid = pAlarmer->lUserID;
	CString strTmp;
	strTmp.Format(_T("New：回调Uid值为：%d，有效性为：%d , IP值为 %s, 有效性为： %d"),pAlarmer->lUserID,pAlarmer->byUserIDValid,A2W((char*)(pAlarmer->sDeviceIP)),pAlarmer->byDeviceIPValid);
	pMian->WriteLogFile((CA2T)__FUNCTION__, FALSE, strTmp);
#endif
	LPNET_DVR_PLATE_RESULT	pPlate = (LPNET_DVR_PLATE_RESULT)pAlarmInfo;

	pMian->InsertNewPlate(pPlate);

}

/** @fn		VOID CToHvShowCtrl::SetUpAlarm()
 *	@brief	设置布防
 *	@return	LONG	0表示成功，否则失败
 */
LONG CToHvShowCtrl::SetUpAlarm()
{
	BOOL	bRet = FALSE;
	//开启布防
#ifndef ONE_DEVICE
	if (m_lLoginUid >= 0)
	{
		GetDevicePtrByUid(m_lLoginUid)->m_lAlarmHandle = NET_DVR_SetupAlarmChan_V30(m_lLoginUid);
		if (GetDevicePtrByUid(m_lLoginUid)->m_lAlarmHandle < 0)
		{
			WriteLogFile((CA2T)__FUNCTION__, TRUE, _T("开启布防失败"));
			return RETVAL_DEPLOYMEMT_FAIL;
		}
	}
	//设置布防回调函数
	bRet = NET_DVR_SetGuardMessageCallBack(GetDevicePtrByUid(m_lLoginUid)->m_lAlarmHandle,MessageCallback, this);
	if (bRet == FALSE)
	{
		WriteLogFile((CA2T)__FUNCTION__, TRUE, _T("设置布防回调函数失败"));
		return RETVAL_SET_ALARM_CALL_BACK_FAIL;
	}
#else
	if (m_lUid >= 0)
	{
		m_lAlarmHandle = NET_DVR_SetupAlarmChan_V30(m_lUid);
		if (m_lAlarmHandle < 0)
		{
			WriteLogFile((CA2T)__FUNCTION__, TRUE, _T("开启布防失败"));
			return RETVAL_DEPLOYMEMT_FAIL;
		}
	}
#endif

	WriteLogFile((CA2T)__FUNCTION__, FALSE, _T("开启布防"));
	return RETVAL_OK;
}


/** @fn		LONG CToHvShowCtrl::CleanAlarm()
 *	@brief	撤销布防
 *	@return	LONG	0表示成功， 否则失败
 */
LONG CToHvShowCtrl::CleanAlarm()
{
	BOOL	bRet = TRUE;
#ifndef ONE_DEVICE
	EnterCriticalSection(&m_csVecLock);
	std::vector<pDeviceInfo>::iterator iter = m_vecDeviceInfoTable.begin();
	while (iter != m_vecDeviceInfoTable.end())
	{
		if((*iter)->m_lAlarmHandle >= 0)
		{
			if(!NET_DVR_CloseAlarmChan_V30((*iter)->m_lAlarmHandle))
			{
				CString tmp;
				tmp.Format(_T("撤销布防失败,相机IP为 %s"),(*iter)->m_csIPAddr);
				WriteLogFile((CA2T)__FUNCTION__, TRUE, tmp);
				bRet = FALSE;
			}
			else
			{
				(*iter)->m_lAlarmHandle = -1;
			}
		}
		iter++;
	}
	LeaveCriticalSection(&m_csVecLock);
	return bRet;
#else
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
#endif
}

/** @fn		BOOL CSCCPCtrl::SavePic(LPCTSTR lpPath, LPCTSTR lpPicName, BYTE * pImage, DWORD dwPicDataLen)
 *	@brief	保存图片
 *	@param	[IN]	lpPath		图片路径
 *	@param	[IN]	lpPicName	图片名字
 *	@param	[IN]	pImage		图片内容
 *	@param	[IN]	dwPicDataLen图片长度
 *	@return	BOOL	true表示成功，否则失败
 */
BOOL CToHvShowCtrl::SavePic(LPCTSTR strFileName, BYTE * pImage, DWORD dwPicDataLen)
{
	if (pImage == NULL || dwPicDataLen <= 0 
		|| strFileName == NULL)
	{
		return FALSE;
	}
	
	//CString csPath(lpPath);
	//如果以"\"结尾,删除"\"
	//if (csPath.GetAt(csPath.GetLength() - 1) == _T('\\'))
	//{
	//	csPath.Delete(csPath.GetLength() - 1);
	//}
	//完整路径
	CString csFullPath;
	csFullPath.Format(_T("%s"),strFileName);
	if (csFullPath.GetLength() >= MAX_PATH)
	{ 
		WriteLogFile((CA2T)__FUNCTION__, TRUE, _T("图片路径过长，抓图失败"));
		return FALSE;
	}
	//将路径名中的？改成#号
	//strFileName.Replace('?', '#');
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
			WriteLogFile((CA2T)__FUNCTION__, TRUE, _T("创建图片文件失败！"));
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
		WriteLogFile((CA2T)__FUNCTION__, TRUE, _T("保存图片文件出错"));
		return false;
	}
	return true;
}


/***************************************************
函数名称：BinImageFormatConvert(BYTE *pByteBinPacked,int iWidth = 112,int iHeight=20)
函数功能：二值图象格式转换，转换为适合bmp存储格式
参数说明：pBytePinPacked(BYTE*):指向图象缓冲区的指针
	iWidth(int):
	iHeigth(int):二值图象的宽度和高度
返回值：1：表示转换成功
备注： 无
***************************************************/
int CToHvShowCtrl::BinImageFormatConvert(BYTE *pByteBinPacked,int iWidth, int iHeight)
{
	int i;
    int iWidthBytes = iWidth>>3;
	BYTE *pByteTemp;
	BYTE *pByteTemp2;
	BYTE *pByteTemp3;
	BYTE  sImageByte,sImageByte2;
	pByteTemp = new BYTE[iWidthBytes*iHeight];
    ASSERT(pByteTemp!=NULL);
	
	pByteTemp2 = pByteTemp + iWidthBytes*(iHeight-1);
	pByteTemp3 = pByteBinPacked;
	//上下颠倒转换
	for(i=0;i<iHeight;i++)
	{
		memcpy(pByteTemp2,pByteTemp3,iWidthBytes);
		pByteTemp3+=iWidthBytes;
		pByteTemp2-=iWidthBytes;
	}
	memcpy(pByteBinPacked,pByteTemp,iWidthBytes*iHeight);
	
	//字节中前后bit位互换
	for(i=0;i<iWidthBytes*iHeight;i++)
	{
		sImageByte = pByteBinPacked[i];
		sImageByte2 = (BYTE)((sImageByte&0x80)>0);  
		sImageByte2 +=(BYTE)(((sImageByte&0x40)>0)<<1);
        sImageByte2 +=(BYTE)(((sImageByte&0x20)>0)<<2);
		sImageByte2 +=(BYTE)(((sImageByte&0x10)>0)<<3);
		sImageByte2 +=(BYTE)(((sImageByte&0x08)>0)<<4);
		sImageByte2 +=(BYTE)(((sImageByte&0x04)>0)<<5);
		sImageByte2 +=(BYTE)(((sImageByte&0x02)>0)<<6);
		sImageByte2 +=(BYTE)(((sImageByte&0x01)>0)<<7);
		pByteBinPacked[i] = sImageByte2;
	}
	delete [] pByteTemp;
	pByteTemp = NULL;
	return 1;
}

/*******************************************************************************
函数名：
	ConvertBin2BmpData
描述：
	将二值数据转成BMP数据。
参数：
	BYTE *pByteBinPacked,			//二值图数据
	BYTE *pByteBmpData				//要生成的BMP数据
	unsigned int *pnBmpDataLength	//生成的BMP数据的大小
返回值：
	int
		0	操作成功
		!=0	操作失败
备注：
*******************************************************************************/
int CToHvShowCtrl::ConvertBin2BmpData(BYTE *pByteBinPacked, BYTE *pByteBmpData, unsigned int *pnBmpDataLength)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//
	if( pByteBmpData == NULL || pByteBinPacked == NULL)
	{
		return -1;
	}
	UINT dwBmpDataSize =  0;
	BYTE *pByteBmpDataTmp = pByteBmpData;
	//转换BIN数据格式
	BinImageFormatConvert(
		pByteBinPacked,
		112, 20
		);
	//
	int iIndex;
	BITMAPFILEHEADER bmpfh;
	BITMAPINFOHEADER bmpih;
	RGBQUAD *bmpcolormap;
	
	bmpfh.bfType = 0x4D42; //"BM";	
	bmpfh.bfReserved1 = 0;
	bmpfh.bfReserved2 = 0;
	bmpfh.bfOffBits = sizeof(BITMAPFILEHEADER) + 
		sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*2;
	
	bmpih.biSize = sizeof(BITMAPINFOHEADER);
	bmpih.biWidth = 112;
	bmpih.biHeight = 20;
	bmpih.biPlanes = 1;
	bmpih.biBitCount = 1;
	bmpih.biCompression = 0;
	bmpih.biXPelsPerMeter = 0;
	bmpih.biYPelsPerMeter = 0;
	bmpih.biClrUsed = 0;
	bmpih.biClrImportant = 0;
	
	int ibyteWidth= (bmpih.biWidth*bmpih.biBitCount+31)/32*4;
    bmpih.biSizeImage = ibyteWidth * bmpih.biHeight;
	
	bmpfh.bfSize = bmpfh.bfOffBits + bmpih.biSizeImage;
    bmpcolormap =(RGBQUAD*) new BYTE[sizeof(RGBQUAD)*(1<<bmpih.biBitCount)];
	//ASSERT(bmpcolormap!=NULL);
	//调色板
	for (iIndex=0; iIndex<2; iIndex++)
	{
		bmpcolormap[iIndex].rgbBlue =
			bmpcolormap[iIndex].rgbGreen =
			bmpcolormap[iIndex].rgbRed = iIndex*255;
		bmpcolormap[iIndex].rgbReserved = 0;
	}
	
	//图象数据校正
	BYTE *pImageTemp;//[16*20];
	pImageTemp = (BYTE*) new BYTE[ibyteWidth * bmpih.biHeight];
    //ASSERT(pImageTemp!=NULL);
	
	int iWidthPacked = bmpih.biWidth>>3;
	memset(pImageTemp, 0, ibyteWidth * bmpih.biHeight);
	BYTE *pImageTemp2 = pImageTemp;
	BYTE *pImageTemp3 = pByteBinPacked;
	for(iIndex=0; iIndex<bmpih.biHeight; iIndex++)
	{
		memcpy(pImageTemp2, pImageTemp3, iWidthPacked);
		pImageTemp2 += ibyteWidth;
		pImageTemp3 += iWidthPacked;
	}	
	//
	memcpy( pByteBmpDataTmp, &bmpfh, sizeof(BITMAPFILEHEADER) );
	pByteBmpDataTmp += sizeof(BITMAPFILEHEADER);
	dwBmpDataSize += sizeof(BITMAPFILEHEADER);
	memcpy( pByteBmpDataTmp, &bmpih, sizeof(BITMAPINFOHEADER) );
	pByteBmpDataTmp += sizeof(BITMAPINFOHEADER);
	dwBmpDataSize += sizeof(BITMAPINFOHEADER);
	memcpy( pByteBmpDataTmp, bmpcolormap, sizeof(RGBQUAD)*2 );
	pByteBmpDataTmp += sizeof(RGBQUAD)*2;
	dwBmpDataSize += sizeof(RGBQUAD)*2;
	memcpy( pByteBmpDataTmp, pImageTemp, bmpih.biSizeImage );
	dwBmpDataSize += bmpih.biSizeImage;
	//
	*pnBmpDataLength = dwBmpDataSize;
	//
	delete[] pImageTemp;
	pImageTemp = NULL;
	delete[] bmpcolormap;
	bmpcolormap = NULL;
	
	return 0;
}
/** @fn		void CToHvShowCtrl::OnDestroy()
 *	@brief	销毁消息
 *	@return	void
 */
void CToHvShowCtrl::OnDestroy()
{
#ifndef ONE_DEVICE
	Disconnect();
#else
	if(m_lUid>=0)
	{
		Disconnect();
	}
#endif
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


#ifndef ONE_DEVICE
CString CToHvShowCtrl::GetIPAddrByUid(LONG uid)//获取设备IP
{
	EnterCriticalSection(&m_csVecLock);
	CString csRet = _T("");
	std::vector<pDeviceInfo>::iterator iter = m_vecDeviceInfoTable.begin();
	while(iter != m_vecDeviceInfoTable.end())
	{
		if((*iter)->m_lUid == uid)
		{
			csRet = (*iter)->m_csIPAddr;
			LeaveCriticalSection(&m_csVecLock);
			return csRet;
		}
		iter++;
	}
	LeaveCriticalSection(&m_csVecLock);
	return csRet;
}

CString CToHvShowCtrl::GetPortByUid(LONG uid)//获取设备端口
{
	EnterCriticalSection(&m_csVecLock);
	CString csRet = _T("");
	std::vector<pDeviceInfo>::iterator iter = m_vecDeviceInfoTable.begin();
	while(iter != m_vecDeviceInfoTable.end())
	{
		if((*iter)->m_lUid == uid)
		{
			csRet = (*iter)->m_csPort;
			LeaveCriticalSection(&m_csVecLock);
			return csRet;
		}
		iter++;
	}
	LeaveCriticalSection(&m_csVecLock);
	return csRet;
}

LONG CToHvShowCtrl::GetAlarmHandleByUid(LONG uid)//获取布防句柄
{
	EnterCriticalSection(&m_csVecLock);
	LONG lRet = -1;
	std::vector<pDeviceInfo>::iterator iter = m_vecDeviceInfoTable.begin();
	while(iter != m_vecDeviceInfoTable.end())
	{
		if((*iter)->m_lUid == uid)
		{
			lRet = (*iter)->m_lAlarmHandle;
			LeaveCriticalSection(&m_csVecLock);
			return lRet;
		}
		iter++;
	}
	LeaveCriticalSection(&m_csVecLock);
	return lRet;
}

CBufferManager * CToHvShowCtrl::GetBufferManagerByUid(LONG uid)//获取缓存管理类 
{
	CString strTmp;
	strTmp.Format(_T("New：查找Uid值为：%d"),uid);
	WriteLogFile((CA2T)__FUNCTION__, FALSE, strTmp);
	EnterCriticalSection(&m_csVecLock);
	CBufferManager * pCBuff = NULL;
	std::vector<pDeviceInfo>::iterator iter = m_vecDeviceInfoTable.begin();
	while(iter != m_vecDeviceInfoTable.end())
	{
		strTmp.Format(_T("New：容器迭代器最新Uid值为：%d"),(*iter)->m_lUid);
		WriteLogFile((CA2T)__FUNCTION__, FALSE, strTmp);
		if((*iter)->m_lUid == uid)
		{
			pCBuff = ((*iter)->m_cBufferManager);
			LeaveCriticalSection(&m_csVecLock);
			return pCBuff;
		}
		iter++;
	}
	LeaveCriticalSection(&m_csVecLock);
	if(pCBuff == NULL )
	{
		WriteLogFile((CA2T)__FUNCTION__, TRUE, _T("New：获取缓存管理类对象失败！！！"));
	}
	return pCBuff;
}

pDeviceInfo CToHvShowCtrl::GetDevicePtrByUid(LONG uid)//获取设备信息结构体指针
{
	EnterCriticalSection(&m_csVecLock);
	pDeviceInfo pDevice = NULL;
	std::vector<pDeviceInfo>::iterator iter = m_vecDeviceInfoTable.begin();
	while(iter != m_vecDeviceInfoTable.end())
	{
		if((*iter)->m_lUid == uid)
		{
			pDevice = (pDeviceInfo)(*iter);
			LeaveCriticalSection(&m_csVecLock);
			return pDevice;
		}
		iter++;
	}
	LeaveCriticalSection(&m_csVecLock);
	return pDevice;
}
#endif