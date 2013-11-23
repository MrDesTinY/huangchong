#include "StdAfx.h"
#include "clsNetIONvr.h"
clsNetIONvr *clsNetIONvr::m_pInstance = NULL;//单实例指针
COpMySQL clsNetIONvr::m_OperMysql;//数据库操作类
CConfig clsNetIONvr::m_Config;//配置文件类
CLog clsNetIONvr::m_CLog;//日志文件类
CRITICAL_SECTION clsNetIONvr::m_csLock;//类锁
int clsNetIONvr::m_iServerFlag = 0;//是否启动服务标志

clsNetIONvr::clsNetIONvr(void)
{
	
}

clsNetIONvr::~clsNetIONvr(void)
{
	
}
//初始化类
void clsNetIONvr::ServerStart()
{
	if(m_iServerFlag == 1)
	{
		return;
	}
	NET_DVR_Init();
	m_InitFlag = false;
#ifndef STATIC_LOAD_IO_NVR
	m_hModule = NULL;
	CString csModuleName = _T("NET_IO_NVR.dll");
	m_hModule = LoadLibrary(csModuleName);
	if(NULL == m_hModule)
	{
		AfxMessageBox(_T("无法找到NET_IO_NVR.dll！"));
		return;
	}
	NET_IO_Initialize = (pfuncNET_IO_Initialize)GetProcAddress(m_hModule,"NET_IO_Initialize");
	NET_IO_Finalize = (pfuncNET_IO_Finalize)GetProcAddress(m_hModule,"NET_IO_Finalize");
	NET_IO_StartService = (pfuncNET_IO_StartService)GetProcAddress(m_hModule,"NET_IO_StartService");
	NET_IO_StopService = (pfuncNET_IO_StopService)GetProcAddress(m_hModule,"NET_IO_StopService");
	NET_IO_PutStream = (pfuncNET_IO_PutStream)GetProcAddress(m_hModule,"NET_IO_PutStream");
	NET_IO_SetCallBackCfgData = (pfuncNET_IO_SetCallBackCfgData)GetProcAddress(m_hModule,"NET_IO_SetCallBackCfgData");
	NET_IO_SetDeviceBuffer = (pfuncNET_IO_SetDeviceBuffer)GetProcAddress(m_hModule,"NET_IO_SetDeviceBuffer");
	
	if(NET_IO_Initialize == NULL || 
		NET_IO_Finalize == NULL || 
		NET_IO_StartService  == NULL || 
		NET_IO_StopService == NULL || 
		NET_IO_PutStream == NULL ||
		NET_IO_SetCallBackCfgData == NULL ||
		NET_IO_SetDeviceBuffer == NULL)
	{
		AfxMessageBox(_T("NET_IO_NVR 函数导出失败！"));
		return;
	}
#endif
	//配置文件读取
	m_Config.SetFilePath("./CamConfig.ini");
	m_strDBServer  = m_Config.ReadConfig("Main","DBServer","localhost");
	m_strDBName  =  m_Config.ReadConfig("Main","DBName","parking_db");
	m_strDBUser    =  m_Config.ReadConfig("Main","DBUser","root");
	m_strDBPwd    =  m_Config.ReadConfig("Main","DBPassword","sa");
	m_strDBPort    =  m_Config.ReadConfig("Main","DBPort","3306");   
	m_strNetIoPort	  = m_Config.ReadConfig("Main","NetIoPort","8000");   
	//连接数据库过程
	if(!m_OperMysql.OpenConn(m_strDBServer,m_strDBUser,m_strDBPwd,m_strDBName,m_strDBPort))
	{
		AfxMessageBox(_T("连接数据库失败！"));
		return;
	}
	m_InitFlag = true;
	memset(m_iDevicesTable,0,sizeof(m_iDevicesTable));
	GetAllAccessDevices();
	NET_IO_Initialize();
	NET_IO_StartService((UINT)atoi(m_strNetIoPort));
	NET_IO_SetDeviceBuffer(m_iDevicesTable);
	NET_IO_SetCallBackCfgData(CfgDataCallBack,(int)this);

	m_CLog.SaveLogInfo("clsNetIONvr初始化成功！");

	m_iServerFlag = 1;
	return;
}
//销毁类
void clsNetIONvr::ServerStop()
{
	if(m_iServerFlag == 0)
	{
		return;
	}
	DeleteCriticalSection(&m_csLock);
	NET_IO_StopService();
	NET_IO_Finalize();
	NET_DVR_Cleanup();
	m_OperMysql.CloseConn();
	for(int i=0;i<MAX_CHANNEL_NUM;i++)
	{
		m_Devices[i].HeadHandle = -1;
		m_Devices[i].PlayHandle = -1;
		m_Devices[i].SubHandle = -1;
		m_Devices[i].LoginHandle = -1;
		m_Devices[i].IsUsing = 0;
		ZeroMemory(m_Devices[i].Head,sizeof(m_Devices[i].Head));
		m_Devices[i].HeadLength = -1;
		ZeroMemory(m_Devices[i].SubHead,sizeof(m_Devices[i].SubHead));
		m_Devices[i].SubHeadLength = -1;
		ZeroMemory(&(m_Devices[i].DeviceInfo),sizeof(m_Devices[i].DeviceInfo));
	}
#ifndef STATIC_LOAD_IO_NVR
	if(m_hModule)
	{
		FreeLibrary(m_hModule);
		m_hModule = NULL;
	}
#endif
	m_iServerFlag = 0;
}
//获取单实例类指针
clsNetIONvr* clsNetIONvr::GetInstance()
{
	static clsNetIONvr OneclsNetIONvr;
	if(m_pInstance == NULL)
	{
		m_pInstance = &OneclsNetIONvr;//new clsNetIONvr;
		InitializeCriticalSection(&m_csLock);
	}
	else
	{
		InitializeCriticalSection(&m_csLock);
		EnterCriticalSection(&m_csLock);
	}
	return m_pInstance;
}

//离开实例
void clsNetIONvr::LeaveInstance()
{
	LeaveCriticalSection(&m_csLock);
}
//获取所有的设备
void clsNetIONvr::GetAllAccessDevices()
{
	if(!GetDLLStatus())
	{
		return;
	}
	CString strSQL;
	MYSQL_RES* pRes;
	strSQL.Format("select IP,Port,UserName,PassWord,CamNo from camerainfo");

	pRes = m_OperMysql.OpenRecordset(strSQL);

	if(pRes == NULL)
	{
		m_CLog.SaveLogInfo("clsNetIONvr::GetAllAccessDevices:OpenRecordset fails！");
		return;
	}

	MYSQL_ROW row;
	my_ulonglong nFieldNum;
	int iRowNum = 0;

	while(row = mysql_fetch_row(pRes))
	{
		CString strData[5] = {_T(""),_T(""),_T(""),_T(""),_T("")};
		nFieldNum = mysql_num_fields(pRes);
		for(int i=0;i<nFieldNum;i++)
		{
			strData[i].Format("%s",row[i]);
			strData[i].Trim();
		}
		if(strData[0].GetLength() > 5)//高清相机的IP长度为16，标清相机2~3位
		{
			m_Devices[iRowNum].CamID = atoi(strData[4]);
			m_Devices[iRowNum].LoginHandle = NET_DVR_Login_V30( strData[0].GetBuffer(0),atoi(strData[1]), strData[2].GetBuffer(0),strData[3].GetBuffer(0),&(m_Devices[iRowNum].DeviceInfo));
			strData[0].ReleaseBuffer();
			strData[2].ReleaseBuffer();
			strData[3].ReleaseBuffer();
			if(m_Devices[iRowNum].LoginHandle<0)
			{
				CString strTemp;
				strTemp.Format("注册设备%d失败！！",iRowNum);
				AfxMessageBox(strTemp);
			}
			m_iDevicesTable[iRowNum] = 2;
			iRowNum ++;
		}
		else
		{
			m_iDevicesTable[16] = 1;
			m_iDevicesTable[17] = 1;
			m_iDevicesTable[18] = 1;
			m_iDevicesTable[19] = 1;
			return;
		}
	}

	m_OperMysql.CloseRecordset(pRes);
	pRes = NULL;

}
//DLL的初始化成功与否的状态获取
BOOL clsNetIONvr::GetDLLStatus(void)
{
	return m_InitFlag;
}

//取流回调函数
void __stdcall RealDataCallBack_V30(LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, void *pUser)
{
	NET_IO_NET_REALPLAY_PARAM* pParam = (NET_IO_NET_REALPLAY_PARAM*) pUser;
	clsNetIONvr* pNetIoNvr =(clsNetIONvr*) pParam->p;
	DEVICE* pDevice = &pNetIoNvr->m_Devices[pParam->chan];

	switch (dwDataType)
	{
	case NET_DVR_SYSHEAD: //系统头
		if (pParam->bMaster)
		{
			memcpy_s(pDevice->Head, 300, pBuffer, dwBufSize);
			InterlockedExchange(&pDevice->HeadLength ,dwBufSize);
			CString strTmp;
			strTmp.Format("RealDataCallBack_V30:Get chan %d Head,Main\n",pParam->chan);
			pNetIoNvr->m_CLog.SaveLogInfo(strTmp.GetBuffer(0));
			strTmp.ReleaseBuffer();
		}
		else
		{
			memcpy_s(pDevice->SubHead, 300, pBuffer, dwBufSize);
			InterlockedExchange(&pDevice->SubHeadLength ,dwBufSize);
			CString strTmp;
			strTmp.Format("RealDataCallBack_V30:Get chan %d Head,Sub\n",pParam->chan);
			pNetIoNvr->m_CLog.SaveLogInfo(strTmp.GetBuffer(0));
			strTmp.ReleaseBuffer();
		}

	case NET_DVR_STREAMDATA:   //码流数据
		if (dwBufSize > 0)
		{
#ifndef STATIC_LOAD_IO_NVR
			pNetIoNvr->NET_IO_PutStream(pParam->chan,pParam->bMaster,(char*)pBuffer,dwBufSize,dwDataType);
#else
			NET_IO_PutStream(pParam->chan,pParam->bMaster,(char*)pBuffer,dwBufSize,dwDataType);
#endif
		}
	}
}
//远程参数配置回调函数
int __stdcall CfgDataCallBack(int iChannel,int iCammandCode,char *recvbuf,char *sendbuf,int *iLen,int userdata,int param1,int param2)
{
	clsNetIONvr *pNetIoNvr = clsNetIONvr::GetInstance();
	if(!pNetIoNvr->GetDLLStatus())
	{
		clsNetIONvr::LeaveInstance();
		return -1;
	}
	int iDeviceStyle = iChannel<=15?0:1;//设备类型，0为高清相机，1为51xx。
	DEVICE* d = &(pNetIoNvr->m_Devices[iChannel]);
	BOOL bResult = FALSE;
	switch(iCammandCode)
	{
	case NET_IO_PREV_CHAN_VALID:
		{
			switch(iDeviceStyle)
			{
			case 0:
				{
					bResult = clsCfgParam::funcNET_IO_PREV_CHAN_VALID_Cam(d,sendbuf);
				}
				break;
			case 1:
				{
					bResult = clsCfgParam::funcNET_IO_PREV_CHAN_VALID_MCam(d,sendbuf);
				}
				break;
			default:
				{
					clsNetIONvr::LeaveInstance();
					return -2;
				}
			}
		}
		break;
	case NET_IO_PREV_CHAN_START:
		{
			switch(iDeviceStyle)
			{
			case 0:
				{
					bResult = clsCfgParam::funcNET_IO_PREV_CHAN_START_Cam(d,recvbuf,iChannel);
				}
				break;
			case 1:
				{
					bResult = clsCfgParam::funcNET_IO_PREV_CHAN_START_MCam(d,recvbuf,iChannel);
				}
				break;
			default:
				{
					clsNetIONvr::LeaveInstance();
					return -2;
				}
			}
		}
		break;
	case NET_IO_PREV_CHAN_STOP:
		{
			switch(iDeviceStyle)
			{
			case 0:
				{
					bResult = clsCfgParam::funcNET_IO_PREV_CHAN_STOP_Cam(d);
				}
				break;
			case 1:
				{
					bResult = clsCfgParam::funcNET_IO_PREV_CHAN_STOP_MCam(d);
				}
				break;
			default:
				{
					clsNetIONvr::LeaveInstance();
					return -2;
				}
			}
		}
		break;
	case NET_IO_PREV_CHAN_HEAD:
		{
			switch(iDeviceStyle)
			{
			case 0:
				{
					bResult = clsCfgParam::funcNET_IO_PREV_CHAN_HEAD_Cam(d,iChannel,recvbuf,sendbuf,iLen,pNetIoNvr);
				}
				break;
			case 1:
				{
					bResult = clsCfgParam::funcNET_IO_PREV_CHAN_HEAD_MCam(d,iChannel,recvbuf,sendbuf,iLen,pNetIoNvr);
				}
				break;
			default:
				{
					clsNetIONvr::LeaveInstance();
					return -2;
				}
			}
		}
		break;
	case GET_CAM_BASICCFG:
		{
			switch(iDeviceStyle)
			{
			case 0:
				{
					bResult = clsCfgParam::funcGET_CAM_BASICCFG_Cam(d,sendbuf,iChannel,iLen);
				}
				break;
			case 1:
				{
					bResult = clsCfgParam::funcGET_CAM_BASICCFG_MCam(d,sendbuf,iChannel,iLen);
				}
				break;
			default:
				{
					clsNetIONvr::LeaveInstance();
					return -2;
				}
			}
		}
		break;
	case SET_CAM_BASICCFG:
		{
			switch(iDeviceStyle)
			{
			case 0:
				{
					bResult = clsCfgParam::funcSET_CAM_BASICCFG_Cam(d,recvbuf,iChannel);
				}
				break;
			case 1:
				{
					bResult = clsCfgParam::funcSET_CAM_BASICCFG_MCam(d,recvbuf,iChannel);
				}
				break;
			default:
				{
					clsNetIONvr::LeaveInstance();
					return -2;
				}
			}
		}
		break;
	case GET_CAM_COMPRESSCFG:
		{
			switch(iDeviceStyle)
			{
			case 0:
				{
					bResult = clsCfgParam::funcGET_CAM_COMPRESSCFG_Cam(d,sendbuf,iChannel,iLen);
				}
				break;
			case 1:
				{
					bResult = clsCfgParam::funcGET_CAM_COMPRESSCFG_MCam(d,sendbuf,iChannel,iLen);
				}
				break;
			default:
				{
					clsNetIONvr::LeaveInstance();
					return -2;
				}
			}
		}
		break;
	case SET_CAM_COMPRESSCFG:
		{
			switch(iDeviceStyle)
			{
			case 0:
				{
					bResult = clsCfgParam::funcSET_CAM_COMPRESSCFG_Cam(d,recvbuf,iChannel);
				}
				break;
			case 1:
				{
					bResult = clsCfgParam::funcSET_CAM_COMPRESSCFG_MCam(d,recvbuf,iChannel);
				}
				break;
			default:
				{
					clsNetIONvr::LeaveInstance();
					return -2;
				}
			}
		}
		break;
	case GET_CAM_OSDCFG:
		{
			switch(iDeviceStyle)
			{
			case 0:
				{
					bResult = clsCfgParam::funcGET_CAM_OSDCFG_Cam(d,sendbuf,iChannel,iLen);
				}
				break;
			case 1:
				{
					bResult = clsCfgParam::funcGET_CAM_OSDCFG_MCam(d,sendbuf,iChannel,iLen);
				}
				break;
			default:
				{
					clsNetIONvr::LeaveInstance();
					return -2;
				}
			}
		}
		break;
	case SET_CAM_OSDCFG:
		{
			switch(iDeviceStyle)
			{
			case 0:
				{
					bResult = clsCfgParam::funcSET_CAM_OSDCFG_Cam(d,recvbuf,iChannel);
				}
				break;
			case 1:
				{
					bResult = clsCfgParam::funcSET_CAM_OSDCFG_MCam(d,recvbuf,iChannel);
				}
				break;
			default:
				{
					clsNetIONvr::LeaveInstance();
					return -2;
				}
			}
		}
		break;
	case GET_CAM_NETCFG:
		{
			switch(iDeviceStyle)
			{
			case 0:
				{
					bResult = clsCfgParam::funcGET_CAM_NETCFG_Cam(d,sendbuf,iChannel,iLen);
				}
				break;
			case 1:
				{
					bResult = clsCfgParam::funcGET_CAM_NETCFG_MCam(d,sendbuf,iChannel,iLen);
				}
				break;
			default:
				{
					clsNetIONvr::LeaveInstance();
					return -2;
				}
			}
		}
		break;
	case SET_CAM_NETCFG:
		{
			switch(iDeviceStyle)
			{
			case 0:
				{
					bResult = clsCfgParam::funcSET_CAM_NETCFG_Cam(d,recvbuf,iChannel);
				}
				break;
			case 1:
				{
					bResult = clsCfgParam::funcSET_CAM_NETCFG_MCam(d,recvbuf,iChannel);
				}
				break;
			default:
				{
					clsNetIONvr::LeaveInstance();
					return -2;
				}
			}
		}
		break;
	case GET_CAM_PLATECFG:
		{
			switch(iDeviceStyle)
			{
			case 0:
				{
					bResult = clsCfgParam::funcGET_CAM_PLATECFG_Cam(d,sendbuf,iChannel,iLen);
				}
				break;
			case 1:
				{
					bResult = clsCfgParam::funcGET_CAM_PLATECFG_MCam(d,sendbuf,iChannel,iLen);
				}
				break;
			default:
				{
					clsNetIONvr::LeaveInstance();
					return -2;
				}
			}
		}
		break;
	case SET_CAM_PLATECFG:
		{
			switch(iDeviceStyle)
			{
			case 0:
				{
					bResult = clsCfgParam::funcSET_CAM_PLATECFG_Cam(d,recvbuf,iChannel);
				}
				break;
			case 1:
				{
					bResult = clsCfgParam::funcSET_CAM_PLATECFG_MCam(d,recvbuf,iChannel);
				}
				break;
			default:
				{
					clsNetIONvr::LeaveInstance();
					return -2;
				}
			}
		}
		break;
	case GET_CAM_FACECFG:
		{
			switch(iDeviceStyle)
			{
			case 0:
				{
					bResult = clsCfgParam::funcGET_CAM_FACECFG_Cam(d,sendbuf,iChannel,iLen);
				}
				break;
			case 1:
				{
					bResult = clsCfgParam::funcGET_CAM_FACECFG_MCam(d,sendbuf,iChannel,iLen);
				}
				break;
			default:
				{
					clsNetIONvr::LeaveInstance();
					return -2;
				}
			}
		}
		break;
	case SET_CAM_FACECFG:
		{
			switch(iDeviceStyle)
			{
			case 0:
				{
					bResult = clsCfgParam::funcSET_CAM_FACECFG_Cam(d,recvbuf,iChannel);
				}
				break;
			case 1:
				{
					bResult = clsCfgParam::funcSET_CAM_FACECFG_MCam(d,recvbuf,iChannel);
				}
				break;
			default:
				{
					clsNetIONvr::LeaveInstance();
					return -2;
				}
			}
		}
		break;
	case GET_CAM_VEDIO_FRONTCFG:
		{
			switch(iDeviceStyle)
			{
			case 0:
				{
					bResult = clsCfgParam::funcGET_CAM_VEDIO_FRONTCFG_Cam(d,sendbuf,iChannel,iLen);
				}
				break;
			case 1:
				{
					bResult = clsCfgParam::funcGET_CAM_VEDIO_FRONTCFG_MCam(d,sendbuf,iChannel,iLen);
				}
				break;
			default:
				{
					clsNetIONvr::LeaveInstance();
					return -2;
				}
			}
		}
		break;
	case SET_CAM_VEDIO_FRONTCFG:
		{
			switch(iDeviceStyle)
			{
			case 0:
				{
					bResult = clsCfgParam::funcSET_CAM_VEDIO_FRONTCFG_Cam(d,recvbuf,iChannel);
				}
				break;
			case 1:
				{
					bResult = clsCfgParam::funcSET_CAM_VEDIO_FRONTCFG_MCam(d,recvbuf,iChannel);
				}
				break;
			default:
				{
					clsNetIONvr::LeaveInstance();
					return -2;
				}
			}
		}
		break;
	case GET_CAM_PLATE_BRTCFG:
		{
			switch(iDeviceStyle)
			{
			case 0:
				{
					bResult = clsCfgParam::funcGET_CAM_PLATE_BRTCFG_Cam(d,sendbuf,iChannel,iLen);
				}
				break;
			case 1:
				{
					bResult = clsCfgParam::funcGET_CAM_PLATE_BRTCFG_MCam(d,sendbuf,iChannel,iLen);
				}
				break;
			default:
				{
					clsNetIONvr::LeaveInstance();
					return -2;
				}
			}
		}
		break;
	case SET_CAM_PLATE_BRTCFG:
		{
			switch(iDeviceStyle)
			{
			case 0:
				{
					bResult = clsCfgParam::funcSET_CAM_PLATE_BRTCFG_Cam(d,recvbuf,iChannel);
				}
				break;
			case 1:
				{
					bResult = clsCfgParam::funcSET_CAM_PLATE_BRTCFG_MCam(d,recvbuf,iChannel);
				}
				break;
			default:
				{
					clsNetIONvr::LeaveInstance();
					return -2;
				}
			}
		}
		break;
	case GET_CAM_ABILITYCFG:
		{
			switch(iDeviceStyle)
			{
			case 0:
				{
					bResult = clsCfgParam::funcGET_CAM_ABILITYCFG_Cam(d,sendbuf,iChannel,iLen);
				}
				break;
			case 1:
				{
					bResult = clsCfgParam::funcGET_CAM_ABILITYCFG_MCam(d,sendbuf,iChannel,iLen);
				}
				break;
			default:
				{
					clsNetIONvr::LeaveInstance();
					return -2;
				}
			}
		}
		break;
	case SET_CAM_ABILITYCFG:
		{
			switch(iDeviceStyle)
			{
			case 0:
				{
					bResult = clsCfgParam::funcSET_CAM_ABILITYCFG_Cam(d,recvbuf,iChannel);
				}
				break;
			case 1:
				{
					bResult = clsCfgParam::funcSET_CAM_ABILITYCFG_MCam(d,recvbuf,iChannel);
				}
				break;
			default:
				{
					clsNetIONvr::LeaveInstance();
					return -2;
				}
			}
		}
		break;
	case GET_CAM_IMAGEOVERLAY:
		{
			switch(iDeviceStyle)
			{
			case 0:
				{
					bResult = clsCfgParam::funcGET_CAM_IMAGEOVERLAY_Cam(d,sendbuf,iChannel,iLen);
				}
				break;
			case 1:
				{
					bResult = clsCfgParam::funcGET_CAM_IMAGEOVERLAY_MCam(d,sendbuf,iChannel,iLen);
				}
				break;
			default:
				{
					clsNetIONvr::LeaveInstance();
					return -2;
				}
			}
		}
		break;
	case SET_CAM_IMAGEOVERLAY:
		{
			switch(iDeviceStyle)
			{
			case 0:
				{
					bResult = clsCfgParam::funcSET_CAM_IMAGEOVERLAY_Cam(d,recvbuf,iChannel);
				}
				break;
			case 1:
				{
					bResult = clsCfgParam::funcSET_CAM_IMAGEOVERLAY_MCam(d,recvbuf,iChannel);
				}
				break;
			default:
				{
					clsNetIONvr::LeaveInstance();
					return -2;
				}
			}
		}
		break;
	case GET_CAM_FLASHCFG:
		{
			switch(iDeviceStyle)
			{
			case 0:
				{
					bResult = clsCfgParam::funcGET_CAM_FLASHCFG_Cam(d,sendbuf,iChannel,param1,iLen);
				}
				break;
			case 1:
				{
					bResult = clsCfgParam::funcGET_CAM_FLASHCFG_MCam(d,sendbuf,iChannel,param1,iLen);
				}
				break;
			default:
				{
					clsNetIONvr::LeaveInstance();
					return -2;
				}
			}
		}
		break;
	case SET_CAM_FLASHCFG:
		{
			switch(iDeviceStyle)
			{
			case 0:
				{
					bResult = clsCfgParam::funcSET_CAM_FLASHCFG_Cam(d,recvbuf,iChannel,param1);
				}
				break;
			case 1:
				{
					bResult = clsCfgParam::funcSET_CAM_FLASHCFG_MCam(d,recvbuf,iChannel,param1);
				}
				break;
			default:
				{
					clsNetIONvr::LeaveInstance();
					return -2;
				}
			}	
		}
		break;
	case GET_CAM_IO_INCFG:
		{
			switch(iDeviceStyle)
			{
			case 0:
				{
					bResult = clsCfgParam::funcGET_CAM_IO_INCFG_Cam(d,sendbuf,iChannel,param1,iLen);
				}
				break;
			case 1:
				{
					bResult = clsCfgParam::funcGET_CAM_IO_INCFG_MCam(d,sendbuf,iChannel,param1,iLen);
				}
				break;
			default:
				{
					clsNetIONvr::LeaveInstance();
					return -2;
				}
			}
		}
		break;
	case SET_CAM_IO_INCFG:
		{
			switch(iDeviceStyle)
			{
			case 0:
				{
					bResult = clsCfgParam::funcSET_CAM_IO_INCFG_Cam(d,recvbuf,iChannel,param1);
				}
				break;
			case 1:
				{
					bResult = clsCfgParam::funcSET_CAM_IO_INCFG_MCam(d,recvbuf,iChannel,param1);
				}
				break;
			default:
				{
					clsNetIONvr::LeaveInstance();
					return -2;
				}
			}
		}
		break;
	case GET_CAM_IO_OUTCFG:
		{
			switch(iDeviceStyle)
			{
			case 0:
				{
					bResult = clsCfgParam::funcGET_CAM_IO_OUTCFG_Cam(d,sendbuf,iChannel,param1,iLen);
				}
				break;
			case 1:
				{
					bResult = clsCfgParam::funcGET_CAM_IO_OUTCFG_MCam(d,sendbuf,iChannel,param1,iLen);
				}
				break;
			default:
				{
					clsNetIONvr::LeaveInstance();
					return -2;
				}
			}
		}
		break;
	case SET_CAM_IO_OUTCFG:
		{
			switch(iDeviceStyle)
			{
			case 0:
				{
					bResult = clsCfgParam::funcSET_CAM_IO_OUTCFG_Cam(d,recvbuf,iChannel,param1);
				}
				break;
			case 1:
				{
					bResult = clsCfgParam::funcSET_CAM_IO_OUTCFG_MCam(d,recvbuf,iChannel,param1);
				}
				break;
			default:
				{
					clsNetIONvr::LeaveInstance();
					return -2;
				}
			}
		}
		break;
	case GET_CAM_IO_SNAPCFG:
		{
			switch(iDeviceStyle)
			{
			case 0:
				{
					bResult = clsCfgParam::funcGET_CAM_IO_SNAPCFG_Cam(d,sendbuf,iChannel,param1,iLen);
				}
				break;
			case 1:
				{
					bResult = clsCfgParam::funcGET_CAM_IO_SNAPCFG_MCam(d,sendbuf,iChannel,param1,iLen);
				}
				break;
			default:
				{
					clsNetIONvr::LeaveInstance();
					return -2;
				}
			}
		}
		break;
	case SET_CAM_IO_SNAPCFG:
		{
			switch(iDeviceStyle)
			{
			case 0:
				{
					bResult = clsCfgParam::funcSET_CAM_IO_SNAPCFG_Cam(d,recvbuf,iChannel,param1);
				}
				break;
			case 1:
				{
					bResult = clsCfgParam::funcSET_CAM_IO_SNAPCFG_MCam(d,recvbuf,iChannel,param1);
				}
				break;
			default:
				{
					clsNetIONvr::LeaveInstance();
					return -2;
				}
			}
		}
		break;
	case SET_CAM_UPDATE:
		{
			switch(iDeviceStyle)
			{
			case 0:
				{
					bResult = clsCfgParam::funcSET_CAM_UPDATE_Cam(d,recvbuf,iChannel);
				}
				break;
			case 1:
				{
					bResult = clsCfgParam::funcSET_CAM_UPDATE_MCam(d,recvbuf,iChannel);
				}
				break;
			default:
				{
					clsNetIONvr::LeaveInstance();
					return -2;
				}
			}
		}
		break;
	case SET_CAM_REBOOT:
		{
			switch(iDeviceStyle)
			{
			case 0:
				{
					bResult = clsCfgParam::funcSET_CAM_REBOOT_Cam(d,iChannel);
				}
				break;
			case 1:
				{
					bResult = clsCfgParam::funcSET_CAM_REBOOT_MCam(d,iChannel);
				}
				break;
			default:
				{
					clsNetIONvr::LeaveInstance();
					return -2;
				}
			}
		}
		break;
	case SET_CAM_DEFAULT:
		{
			switch(iDeviceStyle)
			{
			case 0:
				{
					bResult = clsCfgParam::funcSET_CAM_DEFAULT_Cam(d,iChannel);
				}
				break;
			case 1:
				{
					bResult = clsCfgParam::funcSET_CAM_DEFAULT_MCam(d,iChannel);
				}
				break;
			default:
				{
					clsNetIONvr::LeaveInstance();
					return -2;
				}
			}
		}
		break;
	default:
		{
			clsNetIONvr::LeaveInstance();
			return -3;
		}
		
		break;
	}
	clsNetIONvr::LeaveInstance();
	if(bResult)
	{
		return 0;
	}
	else
	{
		return -1;
	}
	
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//配置参数类实现部分
clsCfgParam::clsCfgParam(void)
{
}

clsCfgParam::~clsCfgParam(void)
{
}

bool clsCfgParam::funcNET_IO_PREV_CHAN_VALID_Cam(DEVICE *d,char *sendbuf)
{
	*(bool*)sendbuf = (d->LoginHandle >= 0)?true:false;
	return true;
}
bool clsCfgParam::funcNET_IO_PREV_CHAN_START_Cam(DEVICE *d,char *recvbuf,int iChannel)
{
	BOOL bMaster = (BOOL)recvbuf;
	NET_DVR_CLIENTINFO ndci;
	ZeroMemory(&ndci,sizeof(ndci));
	ndci.hPlayWnd = NULL;
	ndci.lChannel = 1;
	ndci.lLinkMode = 0;
	if (!bMaster)
	{
		ndci.lLinkMode &= 0x80000000;
	}

	NET_IO_NET_REALPLAY_PARAM* pParam;
	if (bMaster)
	{
		pParam = &d->MainParam;
	}
	else
	{
		pParam = &d->SubParam;
	}

	if ((d->PlayHandle < 0 && bMaster)||(d->SubHandle < 0 && !bMaster))
	{
		if (bMaster)
		{
			d->PlayHandle = NET_DVR_RealPlay_V30(d->LoginHandle,&ndci,RealDataCallBack_V30,pParam);
			TRACE("Play chan %d,Main\n",iChannel);
		}
		else
		{
			d->SubHandle = NET_DVR_RealPlay_V30(d->LoginHandle,&ndci,RealDataCallBack_V30,pParam);
			TRACE("Play chan %d,Sub\n",iChannel);
		}

	}

	if ((d->PlayHandle < 0 && bMaster)||(d->SubHandle < 0 && !bMaster))
	{
		clsNetIONvr::m_CLog.SaveLogInfo("播放失败！");
		return false;
	}
	BOOL ret;
	if (bMaster)
	{
		ret = NET_DVR_MakeKeyFrame(d->LoginHandle,d->PlayHandle);
	}
	else
	{
		ret = NET_DVR_MakeKeyFrameSub(d->LoginHandle,d->SubHandle);
	}
	if (ret < 0)
	{
		clsNetIONvr::m_CLog.SaveLogInfo("重置I帧失败");
		return false;
	}
	return true;
}
bool clsCfgParam::funcNET_IO_PREV_CHAN_STOP_Cam(DEVICE *d)
{
	NET_DVR_StopRealPlay(d->PlayHandle);
	d->PlayHandle = -1;
	d->HeadLength = 0;
	NET_DVR_StopRealPlay(d->SubHandle);
	d-> SubHandle = -1;
	d->SubHeadLength = 0;
	return true;
}
bool clsCfgParam::funcNET_IO_PREV_CHAN_HEAD_Cam(DEVICE *d,int iChannel,char *recvbuf,char *sendbuf,int *iLen,void *pNetIoNvr)
{
	BOOL bMaster = (BOOL)recvbuf;
	if (bMaster)
	{
		printf_s("Begin to get chan %d Head,Main\n",iChannel);
		if (d->HeadLength <= 0)
		{
			NET_DVR_CLIENTINFO ndci;
			ZeroMemory(&ndci,sizeof(ndci));
			ndci.hPlayWnd = NULL;
			ndci.lChannel = 1;
			ndci.lLinkMode = 0;
			if (!bMaster)
			{
				ndci.lLinkMode &= 0x80000000;
			}

			NET_IO_NET_REALPLAY_PARAM* pParam;
			if (bMaster)
			{
				pParam = &d->MainParam;
			}
			else
			{
				pParam = &d->SubParam;
			}
			pParam->p = pNetIoNvr;
			pParam->chan = iChannel;
			pParam->bMaster = (BOOL)recvbuf;

			if (d->PlayHandle < 0)
			{
				d->PlayHandle = NET_DVR_RealPlay_V30(d->LoginHandle,&ndci,RealDataCallBack_V30,pParam);
			}
			if (d->PlayHandle < 0)
			{
				clsNetIONvr::m_CLog.SaveLogInfo("播放失败！");
			}
			else
			{
				printf_s("Already begin to get chan %d Head,Main\n",iChannel);
				int i = 1000;
				while(d->HeadLength <= 0)
				{
					i--;
					Sleep(1);
					if (i < 0)
					{
						*iLen = d->HeadLength;
						printf_s("Failed to get chan %d Head,Main\n",iChannel);
						return 0;
					}
				}
				memcpy_s(sendbuf, 300, d->Head, d->HeadLength);
				*iLen = d->HeadLength;
				printf_s("Get chan %d Head,Main, len is %d\n",iChannel, *iLen);
			}
		}
		else
		{
			memcpy_s(sendbuf, 300, d->Head, d->HeadLength);
			*iLen = d->HeadLength;
		}
	} 
	else
	{
		printf_s("Begin to get chan %d Head,Sub\n",iChannel);
		if (d->SubHeadLength <= 0)
		{
			NET_DVR_CLIENTINFO ndci;
			ZeroMemory(&ndci,sizeof(ndci));
			ndci.hPlayWnd = NULL;
			ndci.lChannel = 1;
			ndci.lLinkMode = 0;
			if (!bMaster)
			{
				ndci.lLinkMode &= 0x80000000;
			}

			NET_IO_NET_REALPLAY_PARAM* pParam;
			if (bMaster)
			{
				pParam = &d->MainParam;
			}
			else
			{
				pParam = &d->SubParam;
			}
			pParam->p = pNetIoNvr;
			pParam->chan = iChannel;
			pParam->bMaster = (BOOL)recvbuf;

			if (d->SubHandle < 0)
			{
				d->SubHandle = NET_DVR_RealPlay_V30(d->LoginHandle,&ndci,RealDataCallBack_V30,pParam);
			}
			if (d->SubHandle < 0)
			{
				clsNetIONvr::m_CLog.SaveLogInfo("播放失败！");
			}
			else
			{
				int i = 800;
				while(d->SubHeadLength <= 0)
				{
					i--;
					Sleep(1);
					if (i < 0)
					{
						*iLen = d->SubHeadLength;
						return 0;
					}
				}
				memcpy_s(sendbuf, 300, d->SubHead, d->SubHeadLength);
				*iLen = d->SubHeadLength;
			}
		}
		else
		{
			memcpy_s(sendbuf, 300, d->SubHead, d->SubHeadLength);
			*iLen = d->SubHeadLength;
		}
	}
	return true;
}

bool clsCfgParam::funcGET_CAM_BASICCFG_Cam(DEVICE *d,char *sendbuf,int iChannel,int *iLen)
{
	printf_s("Channel %d\n",iChannel);
	NET_DVR_DEVICECFG dvrcfg;
	DWORD dwLength;
	if(!NET_DVR_GetDVRConfig(d->LoginHandle,NET_DVR_GET_DEVICECFG,0xFFFFFFFF,&dvrcfg,sizeof(dvrcfg),&dwLength))
	{
		clsNetIONvr::m_CLog.SaveLogInfo(_T("funcGET_CAM_BASICCFG_Cam:GetDVRConfig failure!!!"));
		return false;
	}
	CAM_BASIC_CFG cfg;
	memset(&cfg,0,sizeof(cfg));
	strcpy_s((char *)cfg.byCamName,32,(char *)dvrcfg.sDVRName);
	cfg.dwSize = sizeof(cfg);
	cfg.byDeviceType = dvrcfg.byDVRType;
	cfg.byChannelNum = dvrcfg.byChanNum;
	cfg.byIoNum = 0;//??
	strcpy_s((char *)cfg.sSerialNumber,SERIALNO_LEN,(char *)dvrcfg.sSerialNumber);
	cfg.dwSoftwareVersion = dvrcfg.dwSoftwareVersion;
	cfg.dwSoftwareBuildDate = dvrcfg.dwSoftwareBuildDate;
	cfg.dwDSPSoftwareVersion = dvrcfg.dwDSPSoftwareVersion;
	cfg.dwDSPSoftwareBuildDate = dvrcfg.dwDSPSoftwareBuildDate;
	cfg.dwHardwareVersion = dvrcfg.dwHardwareVersion;

	memcpy_s(sendbuf, 300, &cfg, sizeof(cfg));
	*iLen = sizeof(cfg);
	return true;
}
bool clsCfgParam::funcSET_CAM_BASICCFG_Cam(DEVICE *d,char *recvbuf,int iChannel)
{
	CAM_BASIC_CFG * cfg = (CAM_BASIC_CFG *)recvbuf;
	NET_DVR_DEVICECFG dvrcfg;
	DWORD dwLength;
	if(!NET_DVR_GetDVRConfig(d->LoginHandle,NET_DVR_GET_DEVICECFG,0xFFFFFFFF,&dvrcfg,sizeof(dvrcfg),&dwLength))
	{
		clsNetIONvr::m_CLog.SaveLogInfo(_T("funcSET_CAM_BASICCFG_Cam:GetDVRConfig failure!!!"));
		return false;
	}
	memset(dvrcfg.sDVRName,0,NAME_LEN);
	memset(dvrcfg.sSerialNumber,0,SERIALNO_LEN);
	memcpy_s((char *)dvrcfg.sDVRName,NAME_LEN,(char *)cfg->byCamName,NAME_LEN);
	memcpy_s((char *)dvrcfg.sSerialNumber,SERIALNO_LEN,(char *)cfg->sSerialNumber,SERIALNO_LEN);
	dvrcfg.dwSoftwareVersion = cfg->dwSoftwareVersion;
	dvrcfg.dwSoftwareBuildDate = cfg->dwSoftwareBuildDate;
	dvrcfg.dwDSPSoftwareVersion = cfg->dwDSPSoftwareVersion;
	dvrcfg.dwDSPSoftwareBuildDate = cfg->dwDSPSoftwareBuildDate;
	dvrcfg.dwHardwareVersion = cfg->dwHardwareVersion;
	dvrcfg.dwSize = sizeof(NET_DVR_DEVICECFG);
	if(!NET_DVR_SetDVRConfig(d->LoginHandle,NET_DVR_SET_DEVICECFG,0xFFFFFFFF,&dvrcfg,sizeof(dvrcfg)))
	{
		clsNetIONvr::m_CLog.SaveLogInfo(_T("funcSET_CAM_BASICCFG_Cam:SetDVRConfig failure!!!"));
		return false;
	}
	printf_s("Channel %d\n",iChannel);
	printf_s(((char *)cfg->byCamName));
	printf_s("\n");
	return true;
}
bool clsCfgParam::funcGET_CAM_COMPRESSCFG_Cam(DEVICE *d,char *sendbuf,int iChannel,int *iLen)
{
	printf_s("COMPRESS\n");
	printf_s("Channel %d\n",iChannel);
	NET_DVR_COMPRESSIONCFG dvrcfg;
	DWORD dwLength;
	if(!NET_DVR_GetDVRConfig(d->LoginHandle,NET_DVR_GET_COMPRESSCFG,1,&dvrcfg,sizeof(dvrcfg),&dwLength))
	{
		clsNetIONvr::m_CLog.SaveLogInfo(_T("funcGET_CAM_COMPRESSCFG_Cam:GetDVRConfig failure!!!"));
		return false;
	}
	CAM_COMPRESS_CFG cfg;
	memset(&cfg,0,sizeof(cfg));
	cfg.dwSize = sizeof(cfg);
	cfg.stMainRecordParam.byStreamType = dvrcfg.struRecordPara.byStreamType;
	cfg.stMainRecordParam.byResolution = dvrcfg.struRecordPara.byResolution;
	cfg.stMainRecordParam.byBitrateType = dvrcfg.struRecordPara.byBitrateType;
	cfg.stMainRecordParam.byPicQuality = dvrcfg.struRecordPara.byPicQuality;
	cfg.stMainRecordParam.dwVideoBitrate = dvrcfg.struRecordPara.dwVideoBitrate;
	cfg.stMainRecordParam.dwVideoFrameRate = dvrcfg.struRecordPara.dwVideoFrameRate;
	//cfg.stMainRecordParam.wIntervalFrameI = 0xffff;
	//cfg.stMainRecordParam.byVideoEncType = 0;
	//cfg.stMainRecordParam.byAudioEncType = 0;
	cfg.stSubRecordParam.byStreamType = dvrcfg.struNetPara.byStreamType;
	cfg.stSubRecordParam.byResolution = dvrcfg.struNetPara.byResolution;
	cfg.stSubRecordParam.byBitrateType = dvrcfg.struNetPara.byBitrateType;
	cfg.stSubRecordParam.byPicQuality = dvrcfg.struNetPara.byPicQuality;
	cfg.stSubRecordParam.dwVideoBitrate = dvrcfg.struNetPara.dwVideoBitrate;
	cfg.stSubRecordParam.dwVideoFrameRate = dvrcfg.struNetPara.dwVideoFrameRate;
	//cfg.stSubRecordParam.wIntervalFrameI = 0xffff;
	//cfg.stSubRecordParam.byVideoEncType = 0;
	//cfg.stSubRecordParam.byAudioEncType = 0;
	memcpy_s(sendbuf, 300, &cfg, sizeof(cfg));
	*iLen = sizeof(cfg);
	return true;
}
bool clsCfgParam::funcSET_CAM_COMPRESSCFG_Cam(DEVICE *d,char *recvbuf,int iChannel)
{
	CAM_COMPRESS_CFG * cfg = (CAM_COMPRESS_CFG *)recvbuf;
	NET_DVR_COMPRESSIONCFG dvrcfg;
	DWORD dwLength;
	if(!NET_DVR_GetDVRConfig(d->LoginHandle,NET_DVR_GET_COMPRESSCFG,1,&dvrcfg,sizeof(dvrcfg),&dwLength))
	{
		clsNetIONvr::m_CLog.SaveLogInfo(_T("funcSET_CAM_COMPRESSCFG_Cam:GetDVRConfig failure!!!"));
		return false;
	}
	dvrcfg.dwSize = sizeof(CAM_COMPRESS_CFG);
	dvrcfg.struRecordPara.byBitrateType = cfg->stMainRecordParam.byBitrateType;
	dvrcfg.struRecordPara.byPicQuality = cfg->stMainRecordParam.byPicQuality;
	dvrcfg.struRecordPara.byResolution = cfg->stMainRecordParam.byResolution;
	dvrcfg.struRecordPara.byStreamType = cfg->stMainRecordParam.byStreamType;
	dvrcfg.struRecordPara.dwVideoBitrate = cfg->stMainRecordParam.dwVideoBitrate;
	dvrcfg.struRecordPara.dwVideoFrameRate = cfg->stMainRecordParam.dwVideoFrameRate;

	dvrcfg.struNetPara.byBitrateType = cfg->stSubRecordParam.byBitrateType;
	dvrcfg.struNetPara.byPicQuality = cfg->stSubRecordParam.byPicQuality;
	dvrcfg.struNetPara.byResolution = cfg->stSubRecordParam.byResolution;
	dvrcfg.struNetPara.byStreamType = cfg->stSubRecordParam.byStreamType;
	dvrcfg.struNetPara.dwVideoBitrate = cfg->stSubRecordParam.dwVideoBitrate;
	dvrcfg.struNetPara.dwVideoFrameRate = cfg->stSubRecordParam.dwVideoFrameRate;
	if(!NET_DVR_SetDVRConfig(d->LoginHandle,NET_DVR_SET_COMPRESSCFG,1,&dvrcfg,sizeof(dvrcfg)))
	{
		clsNetIONvr::m_CLog.SaveLogInfo(_T("funcSET_CAM_COMPRESSCFG_Cam:SetDVRConfig failure!!!"));
		return false;
	}
	printf_s("COMPRESS\n");
	printf_s("Channel %d\n",iChannel);
	printf_s("\n");
	return true;
}
bool clsCfgParam::funcGET_CAM_OSDCFG_Cam(DEVICE *d,char *sendbuf,int iChannel,int *iLen)
{
	printf_s("OSD\n");
	printf_s("Channel %d\n",iChannel);
	NET_DVR_PICCFG_EX dvrcfg;
	DWORD dwLength;
	if(!NET_DVR_GetDVRConfig(d->LoginHandle,NET_DVR_GET_PICCFG_EX,1,&dvrcfg,sizeof(dvrcfg),&dwLength))
	{
		clsNetIONvr::m_CLog.SaveLogInfo(_T("funcGET_CAM_OSDCFG_Cam:GetDVRConfig failure!!!"));
		return false;
	}
	CAM_OSD_CFG cfg;
	memset(&cfg,0,sizeof(cfg));
	cfg.dwSize = sizeof(cfg);
	cfg.dwShowOSD = dvrcfg.dwShowOsd;
	cfg.wOSDTopLeftX = dvrcfg.wOSDTopLeftX;
	cfg.wOSDTopLeftY = dvrcfg.wOSDTopLeftY;
	cfg.byOSDType = dvrcfg.byOSDType;
	cfg.byOSDAttrib = dvrcfg.byOSDAttrib;
	cfg.byDispWeek =dvrcfg.byDispWeek;
	cfg.byHourOSDType = dvrcfg.byHourOsdType;

	memcpy_s(sendbuf, 300, &cfg, sizeof(cfg));
	*iLen = sizeof(cfg);

	return true;
}
bool clsCfgParam::funcSET_CAM_OSDCFG_Cam(DEVICE *d,char *recvbuf,int iChannel)
{
	CAM_OSD_CFG * cfg = (CAM_OSD_CFG *)recvbuf;
	NET_DVR_PICCFG_EX dvrcfg;
	DWORD dwLength;
	if(!NET_DVR_GetDVRConfig(d->LoginHandle,NET_DVR_GET_PICCFG_EX,1,&dvrcfg,sizeof(dvrcfg),&dwLength))
	{
		clsNetIONvr::m_CLog.SaveLogInfo(_T("funcSET_CAM_OSDCFG_Cam:GetDVRConfig failure!!!"));
		return false;
	}
	dvrcfg.dwSize = sizeof(CAM_OSD_CFG);
	dvrcfg.dwShowOsd = cfg->dwShowOSD;
	dvrcfg.wOSDTopLeftX = cfg->wOSDTopLeftX;
	dvrcfg.wOSDTopLeftY = cfg->wOSDTopLeftY;
	dvrcfg.byOSDType = cfg->byOSDType;
	dvrcfg.byOSDAttrib = cfg->byOSDAttrib;
	dvrcfg.byDispWeek = cfg->byDispWeek;
	dvrcfg.byHourOsdType = cfg->byHourOSDType;
	if(!NET_DVR_SetDVRConfig(d->LoginHandle,NET_DVR_SET_PICCFG_EX,1,&dvrcfg,sizeof(dvrcfg)))
	{
		clsNetIONvr::m_CLog.SaveLogInfo(_T("funcSET_CAM_OSDCFG_Cam:SetDVRConfig failure!!!"));
		return false;
	}
	printf_s("OSD\n");
	printf_s("Channel %d\n",iChannel);
	printf_s("OSD\nstr:%s\nstrlen: %d, size: %d \n",(char*) cfg, strlen((char*)cfg), sizeof(*cfg));
	return true;
}
bool clsCfgParam::funcGET_CAM_NETCFG_Cam(DEVICE *d,char *sendbuf,int iChannel,int *iLen)
{
	printf_s("NET\n");
	printf_s("Channel %d\n",iChannel);
	NET_DVR_NETCFG_V30 dvrcfg;
	DWORD dwLength;
	if(!NET_DVR_GetDVRConfig(d->LoginHandle,NET_DVR_GET_NETCFG_V30,0xFFFFFFFF,&dvrcfg,sizeof(dvrcfg),&dwLength))
	{
		clsNetIONvr::m_CLog.SaveLogInfo(_T("funcGET_CAM_NETCFG_Cam:GetDVRConfig failure!!!"));
		return false;
	}
	CAM_NET_CFG cfg;
	memset(&cfg,0,sizeof(cfg));
	cfg.dwSize = sizeof(cfg);
	strcpy_s(cfg.stCamIP.sIpV4,16,dvrcfg.struEtherNet->struDVRIP.sIpV4);
	strcpy_s(cfg.stMaskIP.sIpV4,16,dvrcfg.struEtherNet->struDVRIPMask.sIpV4);
	strcpy_s(cfg.stGateIP.sIpV4,16,dvrcfg.struGatewayIpAddr.sIpV4);
	strcpy_s(cfg.stDNSIP.sIpV4,16,dvrcfg.struDnsServer1IpAddr.sIpV4);
	strcpy_s((char*)(cfg.byMacAddr),6,(char*)(dvrcfg.struEtherNet->byMACAddr));
	cfg.wDVRPort = dvrcfg.struEtherNet->wDVRPort;
	cfg.wMTU = dvrcfg.struEtherNet->wMTU;
	cfg.dwNetInterface = dvrcfg.struEtherNet->dwNetInterface;
	strcpy_s(cfg.stDnsServer1IPAddr.sIpV4,16,dvrcfg.struDnsServer2IpAddr.sIpV4);
	cfg.wHttpPort = dvrcfg.wHttpPortNo;

	memcpy_s(sendbuf, 2048, &cfg, sizeof(cfg));
	*iLen = sizeof(cfg);
	return true;
}
bool clsCfgParam::funcSET_CAM_NETCFG_Cam(DEVICE *d,char *recvbuf,int iChannel)
{
	CAM_NET_CFG * cfg = (CAM_NET_CFG *)recvbuf;
	NET_DVR_NETCFG_V30 dvrcfg;
	//NET_DVR_NETCFG dvrcfg1;
	DWORD dwLength;
	if(!NET_DVR_GetDVRConfig(d->LoginHandle,NET_DVR_GET_NETCFG_V30,0xFFFFFFFF,&dvrcfg,sizeof(dvrcfg),&dwLength))
	{
		clsNetIONvr::m_CLog.SaveLogInfo(_T("funcSET_CAM_NETCFG_Cam:GetDVRConfig failure!!!"));
		return false;
	}
	//if(!NET_DVR_GetDVRConfig(d->LoginHandle,NET_DVR_GET_NETCFG,0xFFFFFFFF,&dvrcfg1,sizeof(dvrcfg1),&dwLength))
	//{
	//	clsNetIONvr::m_CLog.SaveLogInfo(_T("funcSET_CAM_NETCFG_Cam:GetDVRConfig failure!!!"));
	//	return false;
	//}
	dvrcfg.dwSize = sizeof(NET_DVR_NETCFG_V30);
	memcpy_s(dvrcfg.struEtherNet->struDVRIP.sIpV4,16,cfg->stCamIP.sIpV4,16);
	memcpy_s(dvrcfg.struEtherNet->struDVRIPMask.sIpV4,16,cfg->stMaskIP.sIpV4,16);
	memcpy_s(dvrcfg.struGatewayIpAddr.sIpV4,16,cfg->stGateIP.sIpV4,16);
	memcpy_s(dvrcfg.struDnsServer1IpAddr.sIpV4,16,cfg->stDnsServer1IPAddr.sIpV4,16);
	memcpy_s((char*)(dvrcfg.struEtherNet->byMACAddr),6,(char*)(cfg->byMacAddr),6);
	dvrcfg.struEtherNet->wDVRPort = cfg->wDVRPort;
	dvrcfg.struEtherNet->wMTU = cfg->wMTU;
	dvrcfg.struEtherNet->dwNetInterface = cfg->dwNetInterface;
	memcpy_s(dvrcfg.struDnsServer2IpAddr.sIpV4,16,cfg->stDnsServer1IPAddr.sIpV4,16);
	dvrcfg.wHttpPortNo = cfg->wHttpPort;
	if(!NET_DVR_SetDVRConfig(d->LoginHandle,NET_DVR_SET_NETCFG_V30,0xFFFFFFFF,&dvrcfg,sizeof(dvrcfg)))
	{
		clsNetIONvr::m_CLog.SaveLogInfo(_T("funcSET_CAM_NETCFG_Cam:SetDVRConfig failure!!!"));
		return false;
	}
	CString strSQL;
	strSQL.Format("update camerainfo set IP=\"%s\",Port=%d where CamNo=%d ;",cfg->stCamIP.sIpV4,cfg->wDVRPort,d->CamID);
	clsNetIONvr::m_OperMysql.ExecuteSQL(strSQL);
	printf_s("NET\n");
	printf_s("Channel %d\n",iChannel);
	printf_s("str:%s\nstrlen: %d, size: %d \n",(char*) cfg, strlen((char*)cfg), sizeof(*cfg));
	return true;
}
bool clsCfgParam::funcGET_CAM_PLATECFG_Cam(DEVICE *d,char *sendbuf,int iChannel,int *iLen)
{
	printf_s("PLATE\n");
	printf_s("Channel %d\n",iChannel);
	NET_DVR_SPRCFG dvrcfg;
	DWORD dwLength;
	if(!NET_DVR_GetDVRConfig(d->LoginHandle,NET_DVR_GET_SPRCFG,0xFFFFFFFF,&dvrcfg,sizeof(NET_DVR_SPRCFG),&dwLength))
	{
		clsNetIONvr::m_CLog.SaveLogInfo(_T("funcGET_CAM_PLATECFG_Cam:GetDVRConfig failure!!!"));
		return false;
	}
	CAM_PLATE_CFG cfg;
	memset(&cfg,0,sizeof(cfg));
	cfg.dwSize = sizeof(cfg);
	strcpy_s((char*)cfg.byDefaultCHN,3,(char*)dvrcfg.byDefaultCHN);
	cfg.byTotalLaneNum = dvrcfg.byTotalLaneNum;
	cfg.byPlateOSD = dvrcfg.byPlateOSD;
	cfg.wDesignedPlateWidth = dvrcfg.wDesignedPlateWidth;
	cfg.struLaneRect[0].fHeight = dvrcfg.struLaneRect[0].fHeight;
	cfg.struLaneRect[0].fWidth = dvrcfg.struLaneRect[0].fWidth;
	cfg.struLaneRect[0].fX = dvrcfg.struLaneRect[0].fX;
	cfg.struLaneRect[0].fY = dvrcfg.struLaneRect[0].fY;

	cfg.struLaneRect[1].fHeight = dvrcfg.struLaneRect[1].fHeight;
	cfg.struLaneRect[1].fWidth = dvrcfg.struLaneRect[1].fWidth;
	cfg.struLaneRect[1].fX = dvrcfg.struLaneRect[1].fX;
	cfg.struLaneRect[1].fY = dvrcfg.struLaneRect[1].fY;

	cfg.struLaneRect[2].fHeight = dvrcfg.struLaneRect[2].fHeight;
	cfg.struLaneRect[2].fWidth = dvrcfg.struLaneRect[2].fWidth;
	cfg.struLaneRect[2].fX = dvrcfg.struLaneRect[2].fX;
	cfg.struLaneRect[2].fY = dvrcfg.struLaneRect[2].fY;

	cfg.struLaneRect[3].fHeight = dvrcfg.struLaneRect[3].fHeight;
	cfg.struLaneRect[3].fWidth = dvrcfg.struLaneRect[3].fWidth;
	cfg.struLaneRect[3].fX = dvrcfg.struLaneRect[3].fX;
	cfg.struLaneRect[3].fY = dvrcfg.struLaneRect[3].fY;

	cfg.struLaneRect[4].fHeight = dvrcfg.struLaneRect[4].fHeight;
	cfg.struLaneRect[4].fWidth = dvrcfg.struLaneRect[4].fWidth;
	cfg.struLaneRect[4].fX = dvrcfg.struLaneRect[4].fX;
	cfg.struLaneRect[4].fY = dvrcfg.struLaneRect[4].fY;

	memcpy_s(sendbuf, 2048, &cfg, sizeof(cfg));
	*iLen = sizeof(cfg);
	return true;
}
bool clsCfgParam::funcSET_CAM_PLATECFG_Cam(DEVICE *d,char *recvbuf,int iChannel)
{
	CAM_PLATE_CFG * cfg = (CAM_PLATE_CFG *)recvbuf;
	NET_DVR_SPRCFG dvrcfg;
	DWORD dwLength;
	if(!NET_DVR_GetDVRConfig(d->LoginHandle,NET_DVR_GET_SPRCFG,0xFFFFFFFF,&dvrcfg,sizeof(NET_DVR_SPRCFG),&dwLength))
	{
		clsNetIONvr::m_CLog.SaveLogInfo(_T("funcGET_CAM_PLATECFG_Cam:GetDVRConfig failure!!!"));
		return false;
	}
	dvrcfg.dwSize = sizeof(NET_DVR_SPRCFG);
	memcpy_s((char *)dvrcfg.byDefaultCHN,3,(char*)cfg->byDefaultCHN,3);
	dvrcfg.byTotalLaneNum = cfg->byTotalLaneNum;
	dvrcfg.byPlateOSD = cfg->byPlateOSD;
	dvrcfg.wDesignedPlateWidth = cfg->wDesignedPlateWidth;
	dvrcfg.struLaneRect[0].fHeight = cfg->struLaneRect[0].fHeight;
	dvrcfg.struLaneRect[0].fWidth = cfg->struLaneRect[0].fWidth;
	dvrcfg.struLaneRect[0].fX = cfg->struLaneRect[0].fX;
	dvrcfg.struLaneRect[0].fY = cfg->struLaneRect[0].fY;

	dvrcfg.struLaneRect[1].fHeight = cfg->struLaneRect[1].fHeight;
	dvrcfg.struLaneRect[1].fWidth = cfg->struLaneRect[1].fWidth;
	dvrcfg.struLaneRect[1].fX = cfg->struLaneRect[1].fX;
	dvrcfg.struLaneRect[1].fY = cfg->struLaneRect[1].fY;

	dvrcfg.struLaneRect[2].fHeight = cfg->struLaneRect[2].fHeight;
	dvrcfg.struLaneRect[2].fWidth = cfg->struLaneRect[2].fWidth;
	dvrcfg.struLaneRect[2].fX = cfg->struLaneRect[2].fX;
	dvrcfg.struLaneRect[2].fY = cfg->struLaneRect[2].fY;

	dvrcfg.struLaneRect[3].fHeight = cfg->struLaneRect[3].fHeight;
	dvrcfg.struLaneRect[3].fWidth = cfg->struLaneRect[3].fWidth;
	dvrcfg.struLaneRect[3].fX = cfg->struLaneRect[3].fX;
	dvrcfg.struLaneRect[3].fY = cfg->struLaneRect[3].fY;

	dvrcfg.struLaneRect[4].fHeight = cfg->struLaneRect[4].fHeight;
	dvrcfg.struLaneRect[4].fWidth = cfg->struLaneRect[4].fWidth;
	dvrcfg.struLaneRect[4].fX = cfg->struLaneRect[4].fX;
	dvrcfg.struLaneRect[4].fY = cfg->struLaneRect[4].fY;
	
	if(!NET_DVR_SetDVRConfig(d->LoginHandle,NET_DVR_SET_SPRCFG,0xFFFFFFFF,&dvrcfg,sizeof(NET_DVR_SPRCFG)))
	{
		clsNetIONvr::m_CLog.SaveLogInfo(_T("funcGET_CAM_PLATECFG_Cam:SetDVRConfig failure!!!"));
		return false;
	}
	printf_s("PLATE\n");
	printf_s("Channel %d\n",iChannel);
	printf_s("str:%s\nstrlen: %d, size: %d \n",(char*) cfg, strlen((char*)cfg), sizeof(*cfg));
	return true;
}
bool clsCfgParam::funcGET_CAM_FACECFG_Cam(DEVICE *d,char *sendbuf,int iChannel,int *iLen)
{
	printf_s("FACE\n");
	printf_s("Channel %d\n",iChannel);
	NET_VCA_FACESNAPCFG dvrcfg1;
	NET_VCA_MASK_REGION_LIST dvrcfg2; 
	DWORD dwLength;
	if(!NET_DVR_GetDVRConfig(d->LoginHandle,NET_DVR_GET_FACESNAPCFG,1,&dvrcfg1,sizeof(NET_VCA_FACESNAPCFG),&dwLength))
	{
		clsNetIONvr::m_CLog.SaveLogInfo(_T("funcGET_CAM_FACECFG_Cam:GetDVRConfig-FaceSnapCfg failure!!!"));
		return false;
	}
	if(!NET_DVR_GetDVRConfig(d->LoginHandle,NET_DVR_GET_VCA_MASK_REGION,1,&dvrcfg2,sizeof(NET_VCA_MASK_REGION_LIST),&dwLength))
	{
		clsNetIONvr::m_CLog.SaveLogInfo(_T("funcGET_CAM_FACECFG_Cam:GetDVRConfig-MaskRegion failure!!!"));
		return false;
	}
	CAM_FACE_CFG cfg;
	memset(&cfg,0,sizeof(cfg));
	cfg.dwSize = sizeof(cfg);
	cfg.fScoreLevel = 0;
	cfg.fLikeLevel = 0;
	cfg.nDetectAreaSence = dvrcfg1.bySensitive;
	cfg.fTargetGenSpeed = dvrcfg1.byGenerateRate;
	cfg.wFilterType = dvrcfg1.struRule[0].struSizeFilter.byMode;
	cfg.nDetectAreaNum = 0;
	for(int i=0;i<MAX_RULE_NUM;i++)
	{
		if(dvrcfg1.struRule[i].byActive == 1)
		{
			cfg.nDetectAreaNum ++;
		}
	}
	for(int i=0; i <dvrcfg1.struRule[0].struVcaPolygon.dwPointNum; i ++)
	{
		cfg.struDectAreaPt[i].fX = dvrcfg1.struRule[0].struVcaPolygon.struPos[i].fX;
		cfg.struDectAreaPt[i].fY = dvrcfg1.struRule[0].struVcaPolygon.struPos[i].fY;
	}
	cfg.wFilter = dvrcfg1.struRule[0].struSizeFilter.byActive;
	cfg.stMinTargetFrame.fHeight = dvrcfg1.struRule[0].struSizeFilter.struMiniRect.fHeight;
	cfg.stMinTargetFrame.fLeft = dvrcfg1.struRule[0].struSizeFilter.struMiniRect.fX;
	cfg.stMinTargetFrame.fTop = dvrcfg1.struRule[0].struSizeFilter.struMiniRect.fY;
	cfg.stMinTargetFrame.fWidth = dvrcfg1.struRule[0].struSizeFilter.struMiniRect.fWidth;
	cfg.stMaxTargetFrame.fLeft = dvrcfg1.struRule[0].struSizeFilter.struMaxRect.fX;
	cfg.stMaxTargetFrame.fTop = dvrcfg1.struRule[0].struSizeFilter.struMaxRect.fY;
	cfg.stMaxTargetFrame.fWidth = dvrcfg1.struRule[0].struSizeFilter.struMaxRect.fWidth;
	cfg.stMaxTargetFrame.fHeight = dvrcfg1.struRule[0].struSizeFilter.struMaxRect.fHeight;
	
	cfg.nMaskAreaNum = 0;
	for(int i = 0;i<MAX_MASK_REGION_NUM;i++)
	{
		if(dvrcfg2.struMask[i].byEnable)
		{
			cfg.nMaskAreaNum ++;
		}
	}

	for (int i=0;i<cfg.nMaskAreaNum;i++)
	{
		cfg.struMaskArea[i].nVertexNum = dvrcfg2.struMask[i].struPolygon.dwPointNum;
		for(int j = 0;j<cfg.struMaskArea[i].nVertexNum;j++)
		{
			cfg.struMaskArea[i].struFacePoint[j].fX = cfg.struMaskArea[i].struFacePoint[j].fX;
			cfg.struMaskArea[i].struFacePoint[j].fY = cfg.struMaskArea[i].struFacePoint[j].fY;
		}
	}

	memcpy_s(sendbuf, 2048, &cfg, sizeof(cfg));
	*iLen = sizeof(cfg);
	return true;
}
bool clsCfgParam::funcSET_CAM_FACECFG_Cam(DEVICE *d,char *recvbuf,int iChannel)
{
	CAM_FACE_CFG * cfg = (CAM_FACE_CFG *)recvbuf;
	NET_VCA_FACESNAPCFG dvrcfg1;
	NET_VCA_MASK_REGION_LIST dvrcfg2; 
	DWORD dwLength;
	if(!NET_DVR_GetDVRConfig(d->LoginHandle,NET_DVR_GET_FACESNAPCFG,1,&dvrcfg1,sizeof(NET_VCA_FACESNAPCFG),&dwLength))
	{
		clsNetIONvr::m_CLog.SaveLogInfo(_T("funcSET_CAM_FACECFG_Cam:GetDVRConfig-FaceSnapCfg failure!!!"));
		return false;
	}
	if(!NET_DVR_GetDVRConfig(d->LoginHandle,NET_DVR_GET_VCA_MASK_REGION,1,&dvrcfg2,sizeof(NET_VCA_MASK_REGION_LIST),&dwLength))
	{
		clsNetIONvr::m_CLog.SaveLogInfo(_T("funcSET_CAM_FACECFG_Cam:GetDVRConfig-MaskRegion failure!!!"));
		return false;
	}
	dvrcfg1.dwSize = sizeof(NET_VCA_FACESNAPCFG);
	dvrcfg1.byGenerateRate = cfg->fTargetGenSpeed;
	dvrcfg1.bySensitive = cfg->nDetectAreaSence;
	dvrcfg1.struRule[0].struSizeFilter.byActive = cfg->wFilter;
	dvrcfg1.struRule[0].struSizeFilter.byMode = cfg->wFilterType;
	
	for(int i=0;i<cfg->nDetectAreaNum;i++)
	{
		for(int j=0;j<MAX_VERTEX_NUM;j++)
		{
			dvrcfg1.struRule[i].struVcaPolygon.struPos[j].fX = cfg->struDectAreaPt[j].fX;
			dvrcfg1.struRule[i].struVcaPolygon.struPos[j].fY = cfg->struDectAreaPt[j].fY;
		}
	}
	dvrcfg1.struRule[0].struSizeFilter.struMiniRect.fHeight = cfg->stMinTargetFrame.fHeight;
	dvrcfg1.struRule[0].struSizeFilter.struMiniRect.fWidth = cfg->stMinTargetFrame.fWidth;
	dvrcfg1.struRule[0].struSizeFilter.struMiniRect.fX = cfg->stMinTargetFrame.fLeft;
	dvrcfg1.struRule[0].struSizeFilter.struMiniRect.fY = cfg->stMinTargetFrame.fTop;
	dvrcfg1.struRule[0].struSizeFilter.struMaxRect.fHeight = cfg->stMaxTargetFrame.fHeight;
	dvrcfg1.struRule[0].struSizeFilter.struMaxRect.fWidth = cfg->stMaxTargetFrame.fWidth;
	dvrcfg1.struRule[0].struSizeFilter.struMaxRect.fX = cfg->stMaxTargetFrame.fLeft;
	dvrcfg1.struRule[0].struSizeFilter.struMaxRect.fY = cfg->stMaxTargetFrame.fTop;

	dvrcfg2.dwSize = sizeof(NET_VCA_MASK_REGION_LIST);
	for(int i=0;i<MAX_MASK_REGION_NUM;i++)
	{
		if(i<cfg->nMaskAreaNum)
		{
			dvrcfg2.struMask[i].byEnable = 1;
			dvrcfg2.struMask[i].struPolygon.dwPointNum = cfg->struMaskArea[i].nVertexNum;
			for(int j=0;j<cfg->struMaskArea[i].nVertexNum;j++)
			{
				dvrcfg2.struMask[i].struPolygon.struPos[j].fX = cfg->struMaskArea[i].struFacePoint[j].fX;
				dvrcfg2.struMask[i].struPolygon.struPos[j].fY = cfg->struMaskArea[i].struFacePoint[j].fY;
			}	
		}
		else
		{
			dvrcfg2.struMask[i].byEnable = 0;
		}
	}
	if(!NET_DVR_SetDVRConfig(d->LoginHandle,NET_DVR_SET_FACESNAPCFG,1,&dvrcfg1,sizeof(NET_VCA_FACESNAPCFG)))
	{
		clsNetIONvr::m_CLog.SaveLogInfo(_T("funcSET_CAM_FACECFG_Cam:SetDVRConfig-FaceSnapCfg failure!!!"));
		return false;
	}
	if(!NET_DVR_SetDVRConfig(d->LoginHandle,NET_DVR_SET_VCA_MASK_REGION,1,&dvrcfg2,sizeof(NET_VCA_MASK_REGION_LIST)))
	{
		clsNetIONvr::m_CLog.SaveLogInfo(_T("funcSET_CAM_FACECFG_Cam:SetDVRConfig-MaskRegion failure!!!"));
		return false;
	}
	printf_s("FACE\n");
	printf_s("Channel %d\n",iChannel);
	printf_s("str:%s\nstrlen: %d, size: %d \n",(char*) cfg, strlen((char*)cfg), sizeof(*cfg));
	return true;
}
bool clsCfgParam::funcGET_CAM_VEDIO_FRONTCFG_Cam(DEVICE *d,char *sendbuf,int iChannel,int *iLen)
{
	printf_s("Video\n");
	printf_s("Channel %d\n",iChannel);
	NET_DVR_CAMERAPARAMCFG dvrcfg;
	DWORD dwLength;
	if(!NET_DVR_GetDVRConfig(d->LoginHandle,NET_DVR_GET_CCDPARAMCFG,0xFFFFFFFF,&dvrcfg,sizeof(NET_DVR_CAMERAPARAMCFG),&dwLength))
	{
		clsNetIONvr::m_CLog.SaveLogInfo(_T("funcGET_CAM_VEDIO_FRONTCFG_Cam:GetDVRConfig failure!!!"));
		return false;
	}
	CAM_VEDIO_CFG cfg;
	memset(&cfg,0,sizeof(cfg));
	cfg.dwSize = sizeof(cfg);
	cfg.dwBright = 0;
	cfg.dwContrast = 0;
	cfg.dwAcute = 0;
	cfg.dwSaturation = 0;
	cfg.byVedioGain = dvrcfg.struGain.byGainLevel;
	cfg.byVedioShutterSpeed = dvrcfg.struExposure.dwVideoExposureSet;
	cfg.byBlackPwl = dvrcfg.byBlackPwl;
	cfg.byJPEGQuality = dvrcfg.byJPEGQuality;
	cfg.byDynamicContrast = dvrcfg.byDynamicContrast;
	cfg.byDynamicContrastEn = dvrcfg.byDynamicContrastEN;
	cfg.byCaptureGain = dvrcfg.struGain.byGainUserSet;
	cfg.byCaptureShutterSpeed = dvrcfg.struExposure.dwExposureUserSet;
	cfg.struWhiteBalance.byWhiteBalanceMode = dvrcfg.struWhiteBalance.byWhiteBalanceMode;
	cfg.struWhiteBalance.byWhiteBalanceModeBGain = dvrcfg.struWhiteBalance.byWhiteBalanceModeBGain;
	cfg.struWhiteBalance.byWhiteBalanceModeRGain = dvrcfg.struWhiteBalance.byWhiteBalanceModeRGain;

	memcpy_s(sendbuf, 2048, &cfg, sizeof(cfg));
	*iLen = sizeof(cfg);
	return true;
}
bool clsCfgParam::funcSET_CAM_VEDIO_FRONTCFG_Cam(DEVICE *d,char *recvbuf,int iChannel)
{
	CAM_VEDIO_CFG * cfg = (CAM_VEDIO_CFG *)recvbuf;
	NET_DVR_CAMERAPARAMCFG dvrcfg;
	DWORD dwLength;
	if(!NET_DVR_GetDVRConfig(d->LoginHandle,NET_DVR_GET_CCDPARAMCFG,0xFFFFFFFF,&dvrcfg,sizeof(NET_DVR_CAMERAPARAMCFG),&dwLength))
	{
		clsNetIONvr::m_CLog.SaveLogInfo(_T("funcSET_CAM_VEDIO_FRONTCFG_Cam:GetDVRConfig failure!!!"));
		return false;
	}
	dvrcfg.dwSize = sizeof(NET_DVR_CAMERAPARAMCFG);
	dvrcfg.struGain.byGainLevel = cfg->byVedioGain;
	dvrcfg.struExposure.dwVideoExposureSet = cfg->byVedioShutterSpeed;
	dvrcfg.byBlackPwl = cfg->byBlackPwl;
	dvrcfg.byJPEGQuality = cfg->byJPEGQuality;
	dvrcfg.byDynamicContrast = cfg->byDynamicContrast;
	dvrcfg.byDynamicContrastEN = cfg->byDynamicContrastEn;
	dvrcfg.struGain.byGainUserSet = cfg->byCaptureGain;
	dvrcfg.struExposure.dwExposureUserSet = cfg->byCaptureShutterSpeed;
	dvrcfg.struWhiteBalance.byWhiteBalanceMode = cfg->struWhiteBalance.byWhiteBalanceMode;
	dvrcfg.struWhiteBalance.byWhiteBalanceModeBGain = cfg->struWhiteBalance.byWhiteBalanceModeBGain;
	dvrcfg.struWhiteBalance.byWhiteBalanceModeRGain = cfg->struWhiteBalance.byWhiteBalanceModeRGain;
	if(!NET_DVR_SetDVRConfig(d->LoginHandle,NET_DVR_SET_CCDPARAMCFG,0xFFFFFFFF,&dvrcfg,sizeof(NET_DVR_CAMERAPARAMCFG)))
	{
		clsNetIONvr::m_CLog.SaveLogInfo(_T("funcSET_CAM_VEDIO_FRONTCFG_Cam:SetDVRConfig failure!!!"));
		return false;
	}
	printf_s("Video\n");
	printf_s("Channel %d\n",iChannel);
	printf_s("str:%s\nstrlen: %d, size: %d \n",(char*) cfg, strlen((char*)cfg), sizeof(*cfg));
	return true;
}
bool clsCfgParam::funcGET_CAM_PLATE_BRTCFG_Cam(DEVICE *d,char *sendbuf,int iChannel,int *iLen)
{
	printf_s("BRT\n");
	printf_s("Channel %d\n",iChannel);
	NET_DVR_PLCCFG dvrcfg;
	DWORD dwLength;
	if(!NET_DVR_GetDVRConfig(d->LoginHandle,NET_DVR_GET_PLCCFG,0xFFFFFFFF,&dvrcfg,sizeof(NET_DVR_PLCCFG),&dwLength))
	{
		clsNetIONvr::m_CLog.SaveLogInfo(_T("funcSET_CAM_VEDIO_FRONTCFG_Cam:GetDVRConfig failure!!!"));
		return false;
	}
	CAM_PLATE_BRT_CFG cfg;
	memset(&cfg,0,sizeof(cfg));
	cfg.dwSize = sizeof(cfg);
	cfg.byPicEnable = dvrcfg.byPlcEnable;
	cfg.byPlateExpectedBright = dvrcfg.byPlateExpectedBright;
	cfg.byTradeOffFlash = dvrcfg.byTradeoffFlash;
	cfg.byCorrectFactor = dvrcfg.byCorrectFactor;

	memcpy_s(sendbuf, 2048, &cfg, sizeof(cfg));
	*iLen = sizeof(cfg);;
	return true;
}
bool clsCfgParam::funcSET_CAM_PLATE_BRTCFG_Cam(DEVICE *d,char *recvbuf,int iChannel)
{
	CAM_PLATE_BRT_CFG * cfg = (CAM_PLATE_BRT_CFG *)recvbuf;
	NET_DVR_PLCCFG dvrcfg;
	DWORD dwLength;
	if(!NET_DVR_GetDVRConfig(d->LoginHandle,NET_DVR_GET_PLCCFG,0xFFFFFFFF,&dvrcfg,sizeof(NET_DVR_PLCCFG),&dwLength))
	{
		clsNetIONvr::m_CLog.SaveLogInfo(_T("funcSET_CAM_PLATE_BRTCFG_Cam:GetDVRConfig failure!!!"));
		return false;
	}
	dvrcfg.byCorrectFactor = cfg->byCorrectFactor;
	dvrcfg.byTradeoffFlash = cfg->byTradeOffFlash;
	dvrcfg.byPlateExpectedBright = cfg->byPlateExpectedBright;
	dvrcfg.byPlcEnable = cfg->byPicEnable;
	dvrcfg.dwSize = sizeof(NET_DVR_PLCCFG);
	if(!NET_DVR_SetDVRConfig(d->LoginHandle,NET_DVR_GET_PLCCFG,0xFFFFFFFF,&dvrcfg,sizeof(NET_DVR_PLCCFG)))
	{
		clsNetIONvr::m_CLog.SaveLogInfo(_T("funcSET_CAM_PLATE_BRTCFG_Cam:SetDVRConfig failure!!!"));
		return false;
	}
	printf_s("BRT\n");
	printf_s("Channel %d\n",iChannel);
	printf_s("str:%s\nstrlen: %d, size: %d \n",(char*) cfg, strlen((char*)cfg), sizeof(*cfg));
	return true;
}
bool clsCfgParam::funcGET_CAM_ABILITYCFG_Cam(DEVICE *d,char *sendbuf,int iChannel,int *iLen)
{
	printf_s("Ability\n");
	printf_s("Channel %d\n",iChannel);
	NET_DVR_SNAPENABLECFG dvrcfg;
	DWORD dwLength;
	if(!NET_DVR_GetDVRConfig(d->LoginHandle,NET_DVR_GET_SNAPENABLECFG,0xFFFFFFFF,&dvrcfg,sizeof(NET_DVR_SNAPENABLECFG),&dwLength))
	{
		clsNetIONvr::m_CLog.SaveLogInfo(_T("funcSET_CAM_PLATE_BRTCFG_Cam:GetDVRConfig failure!!!"));
		return false;
	}
	CAM_ABILITY_CFG cfg;
	memset(&cfg,0,sizeof(cfg));
	cfg.dwSize = sizeof(cfg);
	cfg.byPlateEnable = dvrcfg.byPlateEnable; //是否支持车牌识别 0-不支持，1- 支持
	cfg.byFrameFlip = dvrcfg.byFrameFlip;  //图像是否翻转 0-不翻转 1-翻转
	cfg.wFlipAngle = dvrcfg.wFlipAngle;   // 图片翻转角度 0，90，180，270
	cfg.wLightPhase = dvrcfg.wLightPhase;    //相位 取值范围(0,360)
	cfg.byLightSyncPower = dvrcfg.byLightSyncPower; // 是否信号灯电源同步 0-不同步 1 同步
	cfg.byFrequency = dvrcfg.byFrequency;  //信号频率
	cfg.byUploadSDEnable = dvrcfg.byUploadSDEnable;  //是否自动上传SD图片 0否 1是
	//--------以上字段仅IPC有效，以下字段仅模拟相机有效

	cfg.byPlateMode = 0;       //识别模式参数(仅Ids-65xx支持)0-视频触发 1外部触发
	cfg.byBlackLightMode = 0;  // 背光补偿模式 0-关闭背光补偿 1-开启背光补偿 2-开启强光抑制
	cfg.byColorBlackMode = 0;  // 彩转黑模式 0-自动彩转黑 1-黑白模式，2彩色模式
	cfg.byApertureMode = 0;    // 光圈模式，0 电子光圈 1自动光圈
	cfg.byShutterValue = 0;    //快门值 取值范围[0,12]

	memcpy_s(sendbuf, 2048, &cfg, sizeof(cfg));
	*iLen = sizeof(cfg);
	return true;
}
bool clsCfgParam::funcSET_CAM_ABILITYCFG_Cam(DEVICE *d,char *recvbuf,int iChannel)
{
	CAM_ABILITY_CFG * cfg = (CAM_ABILITY_CFG *)recvbuf;
	NET_DVR_SNAPENABLECFG dvrcfg;
	DWORD dwLength;
	if(!NET_DVR_GetDVRConfig(d->LoginHandle,NET_DVR_GET_SNAPENABLECFG,0xFFFFFFFF,&dvrcfg,sizeof(NET_DVR_SNAPENABLECFG),&dwLength))
	{
		clsNetIONvr::m_CLog.SaveLogInfo(_T("funcSET_CAM_ABILITYCFG_Cam:GetDVRConfig failure!!!"));
		return false;
	}
	dvrcfg.byPlateEnable = cfg->byPlateEnable;
	dvrcfg.byFrameFlip = cfg->byFrameFlip;
	dvrcfg.wFlipAngle = cfg->wFlipAngle;
	dvrcfg.wLightPhase = cfg->wLightPhase;
	dvrcfg.byLightSyncPower = cfg->byLightSyncPower;
	dvrcfg.byFrequency = cfg->byFrequency;
	dvrcfg.byUploadSDEnable = cfg->byUploadSDEnable;
	dvrcfg.dwSize = sizeof(NET_DVR_SNAPENABLECFG);
	if(!NET_DVR_SetDVRConfig(d->LoginHandle,NET_DVR_SET_SNAPENABLECFG,0xFFFFFFFF,&dvrcfg,sizeof(NET_DVR_SNAPENABLECFG)))
	{
		clsNetIONvr::m_CLog.SaveLogInfo(_T("funcSET_CAM_ABILITYCFG_Cam:SetDVRConfig failure!!!"));
		return false;
	}
	printf_s("Ability\n");
	printf_s("Channel %d\n",iChannel);
	printf_s("str:%s\nstrlen: %d, size: %d \n",(char*) cfg, strlen((char*)cfg), sizeof(*cfg));
	return true;
}
bool clsCfgParam::funcGET_CAM_IMAGEOVERLAY_Cam(DEVICE *d,char *sendbuf,int iChannel,int *iLen)
{
	printf_s("IMAGEOVERLAY\n");
	printf_s("Channel %d\n",iChannel);
	NET_DVR_IMAGEOVERLAYCFG dvrcfg;
	DWORD dwLength;
	if(!NET_DVR_GetDVRConfig(d->LoginHandle,NET_DVR_GET_IMAGEOVERLAYCFG,0xFFFFFFFF,&dvrcfg,sizeof(NET_DVR_IMAGEOVERLAYCFG),&dwLength))
	{
		clsNetIONvr::m_CLog.SaveLogInfo(_T("funcGET_CAM_IMAGEOVERLAY_Cam:GetDVRConfig failure!!!"));
		return false;
	}
	CAM_IMAGEOVERLAY_CFG cfg;
	memset(&cfg,0,sizeof(cfg));
	cfg.dwSize = sizeof(cfg);
	cfg.byOverlayInfo = dvrcfg.byOverlayInfo; //叠加使能开关 0-不叠加，1-叠加
	cfg.byOverlayMonitorInfo = dvrcfg.byOverlayMonitorInfo; //是否叠加监控点信息 0-不叠加，1-叠加
	cfg.byOverlayTime = dvrcfg.byOverlayTime;   // 是否叠加时间 0-不叠加，1-叠加
	cfg.byOverSpeed = dvrcfg.byOverlaySpeed;  ////是否叠加速度 0-不叠加，1-叠加
	cfg.byOverlaySpeeding = dvrcfg.byOverlaySpeeding; // 是否叠加超速比例 0-不叠加，1-叠加
	cfg.byOverlayLimitFlag = dvrcfg.byOverlayLimitFlag; //是否叠加限速标记 0-不叠加，1-叠加
	cfg.byOverlayPlate = dvrcfg.byOverlayPlate;     //是否叠加车牌 0-不叠加，1-叠加
	cfg.byOverlayColor = dvrcfg.byOverlayColor;      //是否叠加车身颜色 0-不叠加，1-叠加
	cfg.byOverlayLength = dvrcfg.byOverlayLength;     // 是否叠加车长 0-不叠加，1-叠加
	cfg.byOverlayType = dvrcfg.byOverlayType;       // 是否叠加车型 0- 不叠加，1-叠加
	cfg.byOverlayColorDepth = dvrcfg.byOverlayColor; //是否叠加车身颜色深浅 0-不叠加，1-叠加
	cfg.byOverlayDriveChan = dvrcfg.byOverlayDriveChan;   //是否叠加车道 0-不叠加 1-叠加
	cfg.byOverlayMillSec = dvrcfg.byOverlayMilliSec;     //是否叠加毫秒信息 0-不叠加，1-叠加
	cfg.byOverlayIllegalInfo = dvrcfg.byOverlayIllegalInfo;  //是否叠加违章信息 0-不叠加 1-叠加
	cfg.byOverlayRedOnTime = dvrcfg.byOverlayRedOnTime;    // 是否叠加红灯已亮时间 0-不叠加，1-叠加
	cfg.byMonitor1 = 0;//strcpy_s((char*)(cfg.byMonitor1),32,(char*)(dvrcfg.byMonitorInfo1));            //监控点1
	cfg.byMonitor2 = 0;//strcpy_s((char*)(cfg.byMonitor2),44,(char*)(dvrcfg.byMonitorInfo2));            //监控点2

	memcpy_s(sendbuf, 2048, &cfg, sizeof(cfg));
	*iLen = sizeof(cfg);
	return true;
}
bool clsCfgParam::funcSET_CAM_IMAGEOVERLAY_Cam(DEVICE *d,char *recvbuf,int iChannel)
{
	CAM_IMAGEOVERLAY_CFG * cfg = (CAM_IMAGEOVERLAY_CFG *)recvbuf;
	NET_DVR_IMAGEOVERLAYCFG dvrcfg;
	DWORD dwLength;
	if(!NET_DVR_GetDVRConfig(d->LoginHandle,NET_DVR_GET_IMAGEOVERLAYCFG,0xFFFFFFFF,&dvrcfg,sizeof(NET_DVR_IMAGEOVERLAYCFG),&dwLength))
	{
		clsNetIONvr::m_CLog.SaveLogInfo(_T("funcSET_CAM_IMAGEOVERLAY_Cam:GetDVRConfig failure!!!"));
		return false;
	}
	dvrcfg.dwSize = sizeof(NET_DVR_IMAGEOVERLAYCFG);
	dvrcfg.byOverlayInfo = cfg->byOverlayInfo;
	dvrcfg.byOverlayMonitorInfo = cfg->byOverlayMonitorInfo;
	dvrcfg.byOverlayTime = cfg->byOverlayTime;
	dvrcfg.byOverlaySpeed = cfg->byOverSpeed;
	dvrcfg.byOverlaySpeeding = cfg->byOverlaySpeeding;
	dvrcfg.byOverlayLimitFlag = cfg->byOverlayLimitFlag;
	dvrcfg.byOverlayPlate = cfg->byOverlayPlate;
	dvrcfg.byOverlayColor = cfg->byOverlayColor;
	dvrcfg.byOverlayLength = cfg->byOverlayLength;
	dvrcfg.byOverlayType  = cfg->byOverlayType;
	dvrcfg.byOverlayColorDepth = cfg->byOverlayColorDepth;
	dvrcfg.byOverlayDriveChan = cfg->byOverlayDriveChan;
	dvrcfg.byOverlayMilliSec = cfg->byOverlayMillSec;
	dvrcfg.byOverlayIllegalInfo = cfg->byOverlayIllegalInfo;
	dvrcfg.byOverlayRedOnTime = cfg->byOverlayRedOnTime;
	//strcpy_s((char*)(dvrcfg.byMonitorInfo1),32,(char*)(cfg->byMonitor1));
	//strcpy_s((char*)(dvrcfg.byMonitorInfo2),44,(char*)(cfg->byMonitor2));

	if(!NET_DVR_SetDVRConfig(d->LoginHandle,NET_DVR_SET_IMAGEOVERLAYCFG,0xFFFFFFFF,&dvrcfg,sizeof(NET_DVR_IMAGEOVERLAYCFG)))
	{
		clsNetIONvr::m_CLog.SaveLogInfo(_T("funcSET_CAM_IMAGEOVERLAY_Cam:SetDVRConfig failure!!!"));
		return false;
	}
	printf_s("IMAGEOVERLAY\n");
	printf_s("Channel %d\n",iChannel);
	printf_s("str:%s\nstrlen: %d, size: %d \n",(char*) cfg, strlen((char*)cfg), sizeof(*cfg));
	return true;
}
bool clsCfgParam::funcGET_CAM_FLASHCFG_Cam(DEVICE *d,char *sendbuf,int iChannel,int param1,int *iLen)
{
	printf_s("FLASH\n");
	printf_s("Channel %d\n",iChannel);
	printf_s("IO Channel %d\n",param1);
	NET_DVR_FLASH_OUTCFG dvrcfg;
	DWORD dwLength;
	if(!NET_DVR_GetDVRConfig(d->LoginHandle,NET_DVR_GET_FLASHCFG,param1,&dvrcfg,sizeof(NET_DVR_FLASH_OUTCFG),&dwLength))
	{
		clsNetIONvr::m_CLog.SaveLogInfo(_T("funcGET_CAM_FLASHCFG_Cam:GetDVRConfig failure!!!"));
		return false;
	}
	CAM_FLASH_CFG cfg;
	memset(&cfg,0,sizeof(cfg));
	cfg.dwSize = sizeof(cfg);
	cfg.byCamAddr = 0; //摄像机485地址,0xFF表示无效
	cfg.byFlashAddr = 0; //闪光灯控制小板 485地址，0xFF表示无效
	cfg.fPhase = 0; // 闪光灯控制小板 相位取值范围
	cfg.fDutyRate = 0;//
	cfg.nFreqValue = 0;  //闪光灯控制小板 频率参数 取值 0-4

	cfg.byMode = dvrcfg.byMode; //闪光灯闪烁模式 0- 不闪，1--闪，2-- 关联闪，3-轮闪
	cfg.byRelatedIoIn = dvrcfg.byRelatedIoIn; //闪光灯关联输入IO号，关联闪时，此参数有效，保留
	cfg.byRecognizedLane = dvrcfg.byRecognizedLane; // 关联的IO号 按位表示,bit0 表示io1 是否关联 0-不关联，1关联
	cfg.byDetectBrightness = dvrcfg.byDetectBrightness; // 自动检测亮度使能闪光灯 0- 不检测 1- 检测
	cfg.byBrightnessThreld =  dvrcfg.byBrightnessThreld;// 使能闪光灯亮度阈值，范围 [0-100],高于阈值闪
	cfg.byStartHour = dvrcfg.byStartHour; //开始时间 单位小时
	cfg.byStartMinute = dvrcfg.byStartMinute; //开始时间单位分 取值范围[0,59]
	cfg.byEndHour = dvrcfg.byEndHour;    // 结束时间 单位小时，取值范围[0，23]
	cfg.byEndMinute = dvrcfg.byEndMinute; // 结束时间 单位分，取值范围[0,59]
	cfg.byFlashLightEnable = dvrcfg.byFlashLightEnable; //闪光灯是否开启 0 -关，1 - 开

	memcpy_s(sendbuf, 2048, &cfg, sizeof(cfg));
	*iLen = sizeof(cfg);
	return true;
}
bool clsCfgParam::funcSET_CAM_FLASHCFG_Cam(DEVICE *d,char *recvbuf,int iChannel,int param1)
{
	CAM_FLASH_CFG * cfg = (CAM_FLASH_CFG *)recvbuf;
	NET_DVR_FLASH_OUTCFG dvrcfg;
	DWORD dwLength;
	if(!NET_DVR_GetDVRConfig(d->LoginHandle,NET_DVR_GET_FLASHCFG,param1,&dvrcfg,sizeof(NET_DVR_FLASH_OUTCFG),&dwLength))
	{
		clsNetIONvr::m_CLog.SaveLogInfo(_T("funcSET_CAM_FLASHCFG_Cam:GetDVRConfig failure!!!"));
		return false;
	}
	dvrcfg.dwSize = sizeof(NET_DVR_FLASH_OUTCFG);
	dvrcfg.byMode = cfg->byMode;
	dvrcfg.byRelatedIoIn = cfg->byRelatedIoIn;
	dvrcfg.byRecognizedLane = cfg->byRecognizedLane;
	dvrcfg.byDetectBrightness = cfg->byDetectBrightness;
	dvrcfg.byBrightnessThreld = cfg->byBrightnessThreld;
	dvrcfg.byStartHour = cfg->byStartHour;
	dvrcfg.byStartMinute = cfg->byStartMinute;
	dvrcfg.byEndHour = cfg->byEndHour;
	dvrcfg.byEndMinute = cfg->byEndMinute;
	if(!NET_DVR_SetDVRConfig(d->LoginHandle,NET_DVR_SET_FLASHCFG,param1,&dvrcfg,sizeof(NET_DVR_FLASH_OUTCFG)))
	{
		clsNetIONvr::m_CLog.SaveLogInfo(_T("funcSET_CAM_FLASHCFG_Cam:SetDVRConfig failure!!!"));
		return false;
	}
	printf_s("FLASH\n");
	printf_s("Channel %d\n",iChannel);
	printf_s("IO Channel %d\n",param1);
	printf_s("str:%s\nstrlen: %d, size: %d \n",(char*) cfg, strlen((char*)cfg), sizeof(*cfg));
	return true;
}
bool clsCfgParam::funcGET_CAM_IO_INCFG_Cam(DEVICE *d,char *sendbuf,int iChannel,int param1,int *iLen)
{
	printf_s("IOIN\n");
	printf_s("Channel %d\n",iChannel);
	printf_s("IO Channel %d\n",param1);
	NET_DVR_IO_INCFG dvrcfg;
	DWORD dwLength;
	if(!NET_DVR_GetDVRConfig(d->LoginHandle,NET_DVR_GET_IOINCFG,param1,&dvrcfg,sizeof(NET_DVR_IO_INCFG),&dwLength))
	{
		clsNetIONvr::m_CLog.SaveLogInfo(_T("funcGET_CAM_IO_INCFG_Cam:GetDVRConfig failure!!!"));
		return false;
	}
	CAM_IO_IN_CFG cfg;
	memset(&cfg,0,sizeof(cfg));
	cfg.dwSize = sizeof(cfg);
	cfg.byIoInStatus = dvrcfg.byIoInStatus;

	memcpy_s(sendbuf, 2048, &cfg, sizeof(cfg));
	*iLen = sizeof(cfg);
	return true;
}
bool clsCfgParam::funcSET_CAM_IO_INCFG_Cam(DEVICE *d,char *recvbuf,int iChannel,int param1)
{
	CAM_IO_IN_CFG * cfg = (CAM_IO_IN_CFG *)recvbuf;
	NET_DVR_IO_INCFG dvrcfg;
	DWORD dwLength;
	if(!NET_DVR_GetDVRConfig(d->LoginHandle,NET_DVR_GET_IOINCFG,param1,&dvrcfg,sizeof(NET_DVR_IO_INCFG),&dwLength))
	{
		clsNetIONvr::m_CLog.SaveLogInfo(_T("funcSET_CAM_IO_INCFG_Cam:GetDVRConfig failure!!!"));
		return false;
	}
	dvrcfg.dwSize = sizeof(NET_DVR_IO_INCFG);
	dvrcfg.byIoInStatus = cfg->byIoInStatus;
	if(!NET_DVR_SetDVRConfig(d->LoginHandle,NET_DVR_SET_IOINCFG,param1,&dvrcfg,sizeof(NET_DVR_IO_INCFG)))
	{
		clsNetIONvr::m_CLog.SaveLogInfo(_T("funcSET_CAM_IO_INCFG_Cam:SetDVRConfig failure!!!"));
		return false;
	}
	printf_s("IOIN\n");
	printf_s("Channel %d\n",iChannel);
	printf_s("IO Channel %d\n",param1);
	printf_s("str:%s\nstrlen: %d, size: %d \n",(char*) cfg, strlen((char*)cfg), sizeof(*cfg));
	return true;
}
bool clsCfgParam::funcGET_CAM_IO_OUTCFG_Cam(DEVICE *d,char *sendbuf,int iChannel,int param1,int *iLen)
{
	printf_s("IOOUT\n");
	printf_s("Channel %d\n",iChannel);
	printf_s("IO Channel %d\n",param1);
	NET_DVR_IO_OUTCFG dvrcfg;
	DWORD dwLength;
	if(!NET_DVR_GetDVRConfig(d->LoginHandle,NET_DVR_GET_IOOUTCFG,param1,&dvrcfg,sizeof(NET_DVR_IO_OUTCFG),&dwLength))
	{
		clsNetIONvr::m_CLog.SaveLogInfo(_T("funcGET_CAM_IO_OUTCFG_Cam:GetDVRConfig failure!!!"));
		return false;
	}
	CAM_IO_OUT_CFG cfg;
	memset(&cfg,0,sizeof(cfg));
	cfg.dwSize = sizeof(cfg);
	cfg.byDefaultStatus = dvrcfg.byDefaultStatus; // IO 默认状态 0-低电平 1- 高电平
	cfg.byIoOutStatus = dvrcfg.byIoOutStatus;  // IO起效时状态 0-低电平 1- 高电平，2- 脉冲
	cfg.wAheadTime = dvrcfg.wAheadTime;     // 输出IO提前时间 单位us,取值范围 [0,300]
	cfg.dwTimePluse = dvrcfg.dwTimePluse;    // 脉冲间隔时间 单位 us
	cfg.dwTimeDelay = dvrcfg.dwTimeDelay;    // IO有效持续时间 单位us
	cfg.byFreqMulti = dvrcfg.byFreqMulti;    // 倍频，取值范围[1,15] 
	cfg.byDutyRate = dvrcfg.byDutyRate;     // 占空比 单位%[0,40]

	memcpy_s(sendbuf, 2048, &cfg, sizeof(cfg));
	*iLen = sizeof(cfg);
	return true;
}
bool clsCfgParam::funcSET_CAM_IO_OUTCFG_Cam(DEVICE *d,char *recvbuf,int iChannel,int param1)
{
	CAM_IO_OUT_CFG * cfg = (CAM_IO_OUT_CFG *)recvbuf;
	NET_DVR_IO_OUTCFG dvrcfg;
	DWORD dwLength;
	if(!NET_DVR_GetDVRConfig(d->LoginHandle,NET_DVR_GET_IOOUTCFG,param1,&dvrcfg,sizeof(NET_DVR_IO_OUTCFG),&dwLength))
	{
		clsNetIONvr::m_CLog.SaveLogInfo(_T("funcSET_CAM_IO_OUTCFG_Cam:GetDVRConfig failure!!!"));
		return false;
	}
	dvrcfg.dwSize = sizeof(NET_DVR_IO_OUTCFG);
	dvrcfg.byDefaultStatus = cfg->byDefaultStatus;
	dvrcfg.byIoOutStatus = cfg->byIoOutStatus;
	dvrcfg.wAheadTime = cfg->wAheadTime;
	dvrcfg.dwTimeDelay = cfg->dwTimeDelay;
	dvrcfg.dwTimePluse = cfg->dwTimePluse;
	dvrcfg.byFreqMulti = cfg->byFreqMulti;
	dvrcfg.byDutyRate = cfg->byDutyRate;
	if(!NET_DVR_SetDVRConfig(d->LoginHandle,NET_DVR_SET_IOOUTCFG,param1,&dvrcfg,sizeof(NET_DVR_IO_OUTCFG)))
	{
		clsNetIONvr::m_CLog.SaveLogInfo(_T("funcSET_CAM_IO_OUTCFG_Cam:SetDVRConfig failure!!!"));
		return false;
	}
	printf_s("IOOUT\n");
	printf_s("Channel %d\n",iChannel);
	printf_s("IO Channel %d\n",param1);
	printf_s("str:%s\nstrlen: %d, size: %d \n",(char*) cfg, strlen((char*)cfg), sizeof(*cfg));
	return true;
}
bool clsCfgParam::funcGET_CAM_IO_SNAPCFG_Cam(DEVICE *d,char *sendbuf,int iChannel,int param1,int *iLen)
{
	printf_s("IOSNAP\n");
	printf_s("Channel %d\n",iChannel);
	printf_s("IO Channel %d\n",param1);
	NET_DVR_SNAPCFG dvrcfg;
	DWORD dwLength;
	if(!NET_DVR_GetDVRConfig(d->LoginHandle,NET_DVR_GET_SNAPCFG,param1,&dvrcfg,sizeof(NET_DVR_SNAPCFG),&dwLength))
	{
		clsNetIONvr::m_CLog.SaveLogInfo(_T("funcGET_CAM_IO_SNAPCFG_Cam:GetDVRConfig failure!!!"));
		return false;
	}
	CAM_IO_SNAP_CFG cfg;
	memset(&cfg,0,sizeof(cfg));
	cfg.dwSize = sizeof(cfg);
	cfg.byRelatedDriveWay = dvrcfg.byRelatedDriveWay; // 触发IO关联的车道号 取值范围 [0-9]
	cfg.bySnapTimes = dvrcfg.bySnapTimes; // 线圈抓拍次数 0-不抓拍 非0 为抓拍次数 最大为5次
	cfg.wSnapWaitTime = dvrcfg.wSnapWaitTime;   //抓拍等待时间 单位ms,取值范围 [0,6000]
	cfg.wIntervalTime[0] = dvrcfg.wIntervalTime[0]; // 抓拍间隔时间 单位ms,取值范围[67,60000]
	cfg.wIntervalTime[1] = dvrcfg.wIntervalTime[1]; // 抓拍间隔时间 单位ms,取值范围[67,60000]
	cfg.wIntervalTime[2] = dvrcfg.wIntervalTime[2]; // 抓拍间隔时间 单位ms,取值范围[67,60000]
	cfg.wIntervalTime[3] = dvrcfg.wIntervalTime[3]; // 抓拍间隔时间 单位ms,取值范围[67,60000]

	memcpy_s(sendbuf, 2048, &cfg, sizeof(cfg));
	*iLen = sizeof(cfg);
	return true;
}
bool clsCfgParam::funcSET_CAM_IO_SNAPCFG_Cam(DEVICE *d,char *recvbuf,int iChannel,int param1)
{
	CAM_IO_SNAP_CFG * cfg = (CAM_IO_SNAP_CFG *)recvbuf;
	NET_DVR_SNAPCFG dvrcfg;
	DWORD dwLength;
	if(!NET_DVR_GetDVRConfig(d->LoginHandle,NET_DVR_GET_SNAPCFG,param1,&dvrcfg,sizeof(NET_DVR_SNAPCFG),&dwLength))
	{
		clsNetIONvr::m_CLog.SaveLogInfo(_T("funcSET_CAM_IO_SNAPCFG_Cam:GetDVRConfig failure!!!"));
		return false;
	}
	dvrcfg.dwSize = sizeof(NET_DVR_SNAPCFG);
	dvrcfg.byRelatedDriveWay = cfg->byRelatedDriveWay;
	dvrcfg.bySnapTimes = cfg->bySnapTimes;
	dvrcfg.wSnapWaitTime = cfg->wSnapWaitTime;
	dvrcfg.wIntervalTime[0] = cfg->wIntervalTime[0];
	dvrcfg.wIntervalTime[1] = cfg->wIntervalTime[1];
	dvrcfg.wIntervalTime[2] = cfg->wIntervalTime[2];
	dvrcfg.wIntervalTime[3] = cfg->wIntervalTime[3];
	if(!NET_DVR_SetDVRConfig(d->LoginHandle,NET_DVR_SET_SNAPCFG,param1,&dvrcfg,sizeof(NET_DVR_SNAPCFG)))
	{
		clsNetIONvr::m_CLog.SaveLogInfo(_T("funcSET_CAM_IO_SNAPCFG_Cam:SetDVRConfig failure!!!"));
		return false;
	}
	printf_s("IOSNAP\n");
	printf_s("Channel %d\n",iChannel);
	printf_s("IO Channel %d\n",param1);
	printf_s("str:%s\nstrlen: %d, size: %d \n",(char*) cfg, strlen((char*)cfg), sizeof(*cfg));
	return true;
}

bool clsCfgParam::funcSET_CAM_REBOOT_Cam(DEVICE *d,int iChannel)
{
	printf_s("REBOOT\n");
	printf_s("Channel %d\n",iChannel);
	if(!NET_DVR_RebootDVR(d->LoginHandle))
	{
		clsNetIONvr::m_CLog.SaveLogInfo(_T("funcSET_CAM_REBOOT_Cam:Reboot failure!!!"));
		return false;
	}
	return true;
}
bool clsCfgParam::funcSET_CAM_UPDATE_Cam(DEVICE *d,char *recvbuf,int iChannel)
{
	CString updatepath = recvbuf;
	if(-1 == NET_DVR_Upgrade(d->LoginHandle,updatepath.GetBuffer(0)))
	{
		clsNetIONvr::m_CLog.SaveLogInfo(_T("funcSET_CAM_UPDATE_Cam:Upgrade failure!!!"));
		return false;
	}
	printf_s("Channel %d\n",iChannel);
	printf_s(updatepath);
	printf_s("\n");
	return true;
}
bool clsCfgParam::funcSET_CAM_DEFAULT_Cam(DEVICE *d,int iChannel)
{
	printf_s("DEFAULT\n");
	printf_s("Channel %d\n",iChannel);
	if(!NET_DVR_RestoreConfig(d->LoginHandle))
	{
		clsNetIONvr::m_CLog.SaveLogInfo(_T("funcSET_CAM_DEFAULT_Cam:Restore failure!!!"));
		return false;
	}
	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool clsCfgParam::funcNET_IO_PREV_CHAN_VALID_MCam(DEVICE *d,char *sendbuf)
{
	*(bool*)sendbuf = (d->LoginHandle >= 0)?true:false;
	return true;
}
bool clsCfgParam::funcNET_IO_PREV_CHAN_START_MCam(DEVICE *d,char *recvbuf,int iChannel)
{
	BOOL bMaster = (BOOL)recvbuf;
	NET_DVR_CLIENTINFO ndci;
	ZeroMemory(&ndci,sizeof(ndci));
	ndci.hPlayWnd = NULL;
	ndci.lChannel = 1;
	ndci.lLinkMode = 0;
	if (!bMaster)
	{
		ndci.lLinkMode &= 0x80000000;
	}

	NET_IO_NET_REALPLAY_PARAM* pParam;
	if (bMaster)
	{
		pParam = &d->MainParam;
	}
	else
	{
		pParam = &d->SubParam;
	}

	if ((d->PlayHandle < 0 && bMaster)||(d->SubHandle < 0 && !bMaster))
	{
		if (bMaster)
		{
			d->PlayHandle = NET_DVR_RealPlay_V30(d->LoginHandle,&ndci,RealDataCallBack_V30,pParam);
			TRACE("Play chan %d,Main\n",iChannel);
		}
		else
		{
			d->SubHandle = NET_DVR_RealPlay_V30(d->LoginHandle,&ndci,RealDataCallBack_V30,pParam);
			TRACE("Play chan %d,Sub\n",iChannel);
		}

	}

	if ((d->PlayHandle < 0 && bMaster)||(d->SubHandle < 0 && !bMaster))
	{
		clsNetIONvr::m_CLog.SaveLogInfo("播放失败！");
		return false;
	}
	BOOL ret;
	if (bMaster)
	{
		ret = NET_DVR_MakeKeyFrame(d->LoginHandle,d->PlayHandle);
	}
	else
	{
		ret = NET_DVR_MakeKeyFrameSub(d->LoginHandle,d->SubHandle);
	}
	if (ret < 0)
	{
		clsNetIONvr::m_CLog.SaveLogInfo("重置I帧失败");
		return false;
	}
	return true;
}
bool clsCfgParam::funcNET_IO_PREV_CHAN_STOP_MCam(DEVICE *d)
{
	NET_DVR_StopRealPlay(d->PlayHandle);
	d->PlayHandle = -1;
	d->HeadLength = 0;
	NET_DVR_StopRealPlay(d->SubHandle);
	d-> SubHandle = -1;
	d->SubHeadLength = 0;
	return true;
}
bool clsCfgParam::funcNET_IO_PREV_CHAN_HEAD_MCam(DEVICE *d,int iChannel,char *recvbuf,char *sendbuf,int *iLen,void *pNetIoNvr)
{
	BOOL bMaster = (BOOL)recvbuf;
	if (bMaster)
	{
		printf_s("Begin to get chan %d Head,Main\n",iChannel);
		if (d->HeadLength <= 0)
		{
			NET_DVR_CLIENTINFO ndci;
			ZeroMemory(&ndci,sizeof(ndci));
			ndci.hPlayWnd = NULL;
			ndci.lChannel = 1;
			ndci.lLinkMode = 0;
			if (!bMaster)
			{
				ndci.lLinkMode &= 0x80000000;
			}

			NET_IO_NET_REALPLAY_PARAM* pParam;
			if (bMaster)
			{
				pParam = &d->MainParam;
			}
			else
			{
				pParam = &d->SubParam;
			}
			pParam->p = pNetIoNvr;
			pParam->chan = iChannel;
			pParam->bMaster = (BOOL)recvbuf;

			if (d->PlayHandle < 0)
			{
				d->PlayHandle = NET_DVR_RealPlay_V30(d->LoginHandle,&ndci,RealDataCallBack_V30,pParam);
			}
			if (d->PlayHandle < 0)
			{
				clsNetIONvr::m_CLog.SaveLogInfo("播放失败！");
			}
			else
			{
				printf_s("Already begin to get chan %d Head,Main\n",iChannel);
				int i = 1000;
				while(d->HeadLength <= 0)
				{
					i--;
					Sleep(1);
					if (i < 0)
					{
						*iLen = d->HeadLength;
						printf_s("Failed to get chan %d Head,Main\n",iChannel);
						return 0;
					}
				}
				memcpy_s(sendbuf, 300, d->Head, d->HeadLength);
				*iLen = d->HeadLength;
				printf_s("Get chan %d Head,Main, len is %d\n",iChannel, *iLen);
			}
		}
		else
		{
			memcpy_s(sendbuf, 300, d->Head, d->HeadLength);
			*iLen = d->HeadLength;
		}
	} 
	else
	{
		printf_s("Begin to get chan %d Head,Sub\n",iChannel);
		if (d->SubHeadLength <= 0)
		{
			NET_DVR_CLIENTINFO ndci;
			ZeroMemory(&ndci,sizeof(ndci));
			ndci.hPlayWnd = NULL;
			ndci.lChannel = 1;
			ndci.lLinkMode = 0;
			if (!bMaster)
			{
				ndci.lLinkMode &= 0x80000000;
			}

			NET_IO_NET_REALPLAY_PARAM* pParam;
			if (bMaster)
			{
				pParam = &d->MainParam;
			}
			else
			{
				pParam = &d->SubParam;
			}
			pParam->p = pNetIoNvr;
			pParam->chan = iChannel;
			pParam->bMaster = (BOOL)recvbuf;

			if (d->SubHandle < 0)
			{
				d->SubHandle = NET_DVR_RealPlay_V30(d->LoginHandle,&ndci,RealDataCallBack_V30,pParam);
			}
			if (d->SubHandle < 0)
			{
				clsNetIONvr::m_CLog.SaveLogInfo("播放失败！");
			}
			else
			{
				int i = 800;
				while(d->SubHeadLength <= 0)
				{
					i--;
					Sleep(1);
					if (i < 0)
					{
						*iLen = d->SubHeadLength;
						return 0;
					}
				}
				memcpy_s(sendbuf, 300, d->SubHead, d->SubHeadLength);
				*iLen = d->SubHeadLength;
			}
		}
		else
		{
			memcpy_s(sendbuf, 300, d->SubHead, d->SubHeadLength);
			*iLen = d->SubHeadLength;
		}
	}
	return true;
}

bool clsCfgParam::funcGET_CAM_BASICCFG_MCam(DEVICE *d,char *sendbuf,int iChannel,int *iLen)
{
	printf_s("Channel %d\n",iChannel);
	CAM_BASIC_CFG cfg;
	memset(&cfg,0,sizeof(cfg));
	strcpy_s((char *)cfg.byCamName,32,"TestCamName");
	cfg.dwSize = sizeof(cfg);
	cfg.byDeviceType = 1;
	cfg.byChannelNum = 32;
	cfg.byIoNum = 6;
	strcpy_s((char *)cfg.sSerialNumber,32,"iVMS_TestDemo");
	cfg.dwSoftwareVersion = 11;
	cfg.dwSoftwareBuildDate = 0x7DC0101;
	cfg.dwDSPSoftwareVersion = 22;
	cfg.dwDSPSoftwareBuildDate = 0x7DC0102;
	cfg.dwHardwareVersion = 33;

	memcpy_s(sendbuf, 300, &cfg, sizeof(cfg));
	*iLen = sizeof(cfg);
	return true;
}
bool clsCfgParam::funcSET_CAM_BASICCFG_MCam(DEVICE *d,char *recvbuf,int iChannel)
{
	CAM_BASIC_CFG * cfg = (CAM_BASIC_CFG *)recvbuf;
	printf_s("Channel %d\n",iChannel);
	printf_s(((char *)cfg->byCamName));
	printf_s("\n");
	return true;
}
bool clsCfgParam::funcGET_CAM_COMPRESSCFG_MCam(DEVICE *d,char *sendbuf,int iChannel,int *iLen)
{
	printf_s("COMPRESS\n");
	printf_s("Channel %d\n",iChannel);
	CAM_COMPRESS_CFG cfg;
	memset(&cfg,0,sizeof(cfg));
	cfg.dwSize = sizeof(cfg);
	cfg.stMainRecordParam.byStreamType = 1;
	cfg.stMainRecordParam.byResolution = 27;
	cfg.stMainRecordParam.byBitrateType = 1;
	cfg.stMainRecordParam.byPicQuality = 2;
	cfg.stMainRecordParam.dwVideoBitrate = 23;
	cfg.stMainRecordParam.dwVideoFrameRate = 14;
	cfg.stMainRecordParam.wIntervalFrameI = 0xffff;
	cfg.stMainRecordParam.byVideoEncType = 0;
	cfg.stMainRecordParam.byAudioEncType = 0;
	cfg.stSubRecordParam.byStreamType = 1;
	cfg.stSubRecordParam.byResolution = 20;
	cfg.stSubRecordParam.byBitrateType = 1;
	cfg.stSubRecordParam.byPicQuality = 3;
	cfg.stSubRecordParam.dwVideoBitrate = 19;
	cfg.stSubRecordParam.dwVideoFrameRate = 10;
	cfg.stSubRecordParam.wIntervalFrameI = 0xffff;
	cfg.stSubRecordParam.byVideoEncType = 0;
	cfg.stSubRecordParam.byAudioEncType = 0;

	memcpy_s(sendbuf, 300, &cfg, sizeof(cfg));
	*iLen = sizeof(cfg);
	return true;
}
bool clsCfgParam::funcSET_CAM_COMPRESSCFG_MCam(DEVICE *d,char *recvbuf,int iChannel)
{
	CAM_COMPRESS_CFG * cfg = (CAM_COMPRESS_CFG *)recvbuf;
	printf_s("COMPRESS\n");
	printf_s("Channel %d\n",iChannel);
	printf_s("\n");
	return true;
}
bool clsCfgParam::funcGET_CAM_OSDCFG_MCam(DEVICE *d,char *sendbuf,int iChannel,int *iLen)
{
	printf_s("OSD\n");
	printf_s("Channel %d\n",iChannel);
	CAM_OSD_CFG cfg;
	memset(&cfg,'b',sizeof(cfg));
	((char*)&cfg)[sizeof(cfg)-1] = 0;
	memset(&cfg,0,sizeof(cfg));
	cfg.dwSize = sizeof(cfg);
	cfg.dwShowOSD = 1;
	cfg.wOSDTopLeftX = 11;
	cfg.wOSDTopLeftY = 22;
	cfg.byOSDType = 2;
	cfg.byOSDAttrib = 1;
	cfg.byDispWeek =1;
	cfg.byHourOSDType = 0;


	memcpy_s(sendbuf, 300, &cfg, sizeof(cfg));
	*iLen = sizeof(cfg);

	return true;
}
bool clsCfgParam::funcSET_CAM_OSDCFG_MCam(DEVICE *d,char *recvbuf,int iChannel)
{
	CAM_OSD_CFG * cfg = (CAM_OSD_CFG *)recvbuf;
	printf_s("OSD\n");
	printf_s("Channel %d\n",iChannel);
	printf_s("OSD\nstr:%s\nstrlen: %d, size: %d \n",(char*) cfg, strlen((char*)cfg), sizeof(*cfg));
	return true;
}
bool clsCfgParam::funcGET_CAM_NETCFG_MCam(DEVICE *d,char *sendbuf,int iChannel,int *iLen)
{
	printf_s("NET\n");
	printf_s("Channel %d\n",iChannel);
	CAM_NET_CFG cfg;
	memset(&cfg,'b',sizeof(cfg));
	((char*)&cfg)[sizeof(cfg)-1] = 0;
	memset(&cfg,0,sizeof(cfg));
	cfg.dwSize = sizeof(cfg);
	strcpy_s(cfg.stCamIP.sIpV4,16,"111.111.111.111");
	strcpy_s(cfg.stMaskIP.sIpV4,16,"222.222.222.222");
	strcpy_s(cfg.stGateIP.sIpV4,16,"33.33.33.33");
	strcpy_s(cfg.stDNSIP.sIpV4,16,"44.44.44.44");
	memset(cfg.byMacAddr,0x64,6);
	cfg.wDVRPort = 8000;
	cfg.wMTU = 12800;
	cfg.dwNetInterface = 2;
	strcpy_s(cfg.stDnsServer1IPAddr.sIpV4,16,"55.55.55.55");
	cfg.wHttpPort = 9000;

	memcpy_s(sendbuf, 2048, &cfg, sizeof(cfg));
	*iLen = sizeof(cfg);
	return true;
}
bool clsCfgParam::funcSET_CAM_NETCFG_MCam(DEVICE *d,char *recvbuf,int iChannel)
{
	CAM_NET_CFG * cfg = (CAM_NET_CFG *)recvbuf;
	printf_s("NET\n");
	printf_s("Channel %d\n",iChannel);
	printf_s("str:%s\nstrlen: %d, size: %d \n",(char*) cfg, strlen((char*)cfg), sizeof(*cfg));
	return true;
}
bool clsCfgParam::funcGET_CAM_PLATECFG_MCam(DEVICE *d,char *sendbuf,int iChannel,int *iLen)
{
	printf_s("PLATE\n");
	printf_s("Channel %d\n",iChannel);
	CAM_PLATE_CFG cfg;
	memset(&cfg,'b',sizeof(cfg));
	((char*)&cfg)[sizeof(cfg)-1] = 0;
	memset(&cfg,0,sizeof(cfg));
	cfg.dwSize = sizeof(cfg);
	strcpy_s((char*)cfg.byDefaultCHN,3,"浙");
	cfg.byTotalLaneNum = 2;
	cfg.byPlateOSD = 1;
	cfg.wDesignedPlateWidth = 600;
	cfg.struLaneRect[0].fHeight = 11.1;
	cfg.struLaneRect[0].fWidth = 22.2;
	cfg.struLaneRect[0].fX = 33.3;
	cfg.struLaneRect[0].fY = 44.4;

	cfg.struLaneRect[1].fHeight = 11.11;
	cfg.struLaneRect[1].fWidth = 22.21;
	cfg.struLaneRect[1].fX = 33.31;
	cfg.struLaneRect[1].fY = 44.41;

	cfg.struLaneRect[2].fHeight = 11.12;
	cfg.struLaneRect[2].fWidth = 22.22;
	cfg.struLaneRect[2].fX = 33.32;
	cfg.struLaneRect[2].fY = 44.42;

	cfg.struLaneRect[3].fHeight = 11.13;
	cfg.struLaneRect[3].fWidth = 22.23;
	cfg.struLaneRect[3].fX = 33.33;
	cfg.struLaneRect[3].fY = 44.43;

	cfg.struLaneRect[4].fHeight = 11.14;
	cfg.struLaneRect[4].fWidth = 22.24;
	cfg.struLaneRect[4].fX = 33.34;
	cfg.struLaneRect[4].fY = 44.44;


	memcpy_s(sendbuf, 2048, &cfg, sizeof(cfg));
	*iLen = sizeof(cfg);
	return true;
}
bool clsCfgParam::funcSET_CAM_PLATECFG_MCam(DEVICE *d,char *recvbuf,int iChannel)
{
	CAM_PLATE_CFG * cfg = (CAM_PLATE_CFG *)recvbuf;
	printf_s("PLATE\n");
	printf_s("Channel %d\n",iChannel);
	printf_s("str:%s\nstrlen: %d, size: %d \n",(char*) cfg, strlen((char*)cfg), sizeof(*cfg));
	return true;
}
bool clsCfgParam::funcGET_CAM_FACECFG_MCam(DEVICE *d,char *sendbuf,int iChannel,int *iLen)
{
	printf_s("FACE\n");
	printf_s("Channel %d\n",iChannel);
	CAM_FACE_CFG cfg;
	memset(&cfg,'b',sizeof(cfg));
	((char*)&cfg)[sizeof(cfg)-1] = 0;
	memset(&cfg,0,sizeof(cfg));
	cfg.dwSize = sizeof(cfg);
	cfg.fScoreLevel = 0.5;
	cfg.fLikeLevel = 0.6;
	cfg.nDetectAreaSence = 45;
	cfg.fTargetGenSpeed = 5.5;
	cfg.nDetectAreaNum = 10;
	int i = 0;
	for(i = 0; i <MAX_VERTEX_NUM; i ++)
	{
		cfg.struDectAreaPt[i].fX = i * 0.125;
		cfg.struDectAreaPt[i].fY = i * 0.346;
	}
	cfg.wFilter = 1;
	cfg.wFilter = 3;
	cfg.stMinTargetFrame.fHeight = 111;
	cfg.stMinTargetFrame.fLeft = 11;
	cfg.stMinTargetFrame.fTop = 1;
	cfg.stMinTargetFrame.fWidth = 1111;
	cfg.stMaxTargetFrame.fLeft = 1;
	cfg.stMaxTargetFrame.fTop = 11;
	cfg.stMaxTargetFrame.fWidth = 111;
	cfg.stMaxTargetFrame.fHeight = 1111;
	cfg.nMaskAreaNum = 4;
	for (i=0;i<4;i++)
	{
		cfg.struMaskArea[i].nVertexNum = 1;
		cfg.struMaskArea[i].struFacePoint[0].fX = 21;
		cfg.struMaskArea[i].struFacePoint[0].fY = 12;
	}

	memcpy_s(sendbuf, 2048, &cfg, sizeof(cfg));
	*iLen = sizeof(cfg);
	return true;
}
bool clsCfgParam::funcSET_CAM_FACECFG_MCam(DEVICE *d,char *recvbuf,int iChannel)
{
	CAM_FACE_CFG * cfg = (CAM_FACE_CFG *)recvbuf;
	printf_s("FACE\n");
	printf_s("Channel %d\n",iChannel);
	printf_s("str:%s\nstrlen: %d, size: %d \n",(char*) cfg, strlen((char*)cfg), sizeof(*cfg));
	return true;
}
bool clsCfgParam::funcGET_CAM_VEDIO_FRONTCFG_MCam(DEVICE *d,char *sendbuf,int iChannel,int *iLen)
{
	printf_s("Video\n");
	printf_s("Channel %d\n",iChannel);
	CAM_VEDIO_CFG cfg;
	memset(&cfg,'b',sizeof(cfg));
	((char*)&cfg)[sizeof(cfg)-1] = 0;
	memset(&cfg,0,sizeof(cfg));
	cfg.dwSize = sizeof(cfg);
	cfg.dwBright = 60;
	cfg.dwContrast = 55;
	cfg.dwAcute = 50;
	cfg.dwSaturation = 45;
	cfg.byVedioGain = 1;
	cfg.byVedioShutterSpeed = 2;
	cfg.byBlackPwl = 3;
	cfg.byJPEGQuality = 4;
	cfg.byDynamicContrast = 5;
	cfg.byDynamicContrastEn = 6;
	cfg.byCaptureGain = 7;
	cfg.byCaptureShutterSpeed = 8;
	cfg.struWhiteBalance.byWhiteBalanceMode = 9;
	cfg.struWhiteBalance.byWhiteBalanceModeBGain = 10;
	cfg.struWhiteBalance.byWhiteBalanceModeRGain = 11;

	memcpy_s(sendbuf, 2048, &cfg, sizeof(cfg));
	*iLen = sizeof(cfg);
	return true;
}
bool clsCfgParam::funcSET_CAM_VEDIO_FRONTCFG_MCam(DEVICE *d,char *recvbuf,int iChannel)
{
	CAM_VEDIO_CFG * cfg = (CAM_VEDIO_CFG *)recvbuf;
	printf_s("Video\n");
	printf_s("Channel %d\n",iChannel);
	printf_s("str:%s\nstrlen: %d, size: %d \n",(char*) cfg, strlen((char*)cfg), sizeof(*cfg));
	return true;
}
bool clsCfgParam::funcGET_CAM_PLATE_BRTCFG_MCam(DEVICE *d,char *sendbuf,int iChannel,int *iLen)
{
	printf_s("BRT\n");
	printf_s("Channel %d\n",iChannel);
	CAM_PLATE_BRT_CFG cfg;
	memset(&cfg,'b',sizeof(cfg));
	((char*)&cfg)[sizeof(cfg)-1] = 0;
	memset(&cfg,0,sizeof(cfg));
	cfg.dwSize = sizeof(cfg);
	cfg.byPicEnable = 1;
	cfg.byPlateExpectedBright = 2;
	cfg.byTradeOffFlash = 0;
	cfg.byCorrectFactor = 3;

	memcpy_s(sendbuf, 2048, &cfg, sizeof(cfg));
	*iLen = sizeof(cfg);;
	return true;
}
bool clsCfgParam::funcSET_CAM_PLATE_BRTCFG_MCam(DEVICE *d,char *recvbuf,int iChannel)
{
	CAM_PLATE_BRT_CFG * cfg = (CAM_PLATE_BRT_CFG *)recvbuf;
	printf_s("BRT\n");
	printf_s("Channel %d\n",iChannel);
	printf_s("str:%s\nstrlen: %d, size: %d \n",(char*) cfg, strlen((char*)cfg), sizeof(*cfg));
	return true;
}
bool clsCfgParam::funcGET_CAM_ABILITYCFG_MCam(DEVICE *d,char *sendbuf,int iChannel,int *iLen)
{
	printf_s("Ability\n");
	printf_s("Channel %d\n",iChannel);
	CAM_ABILITY_CFG cfg;
	memset(&cfg,'b',sizeof(cfg));
	((char*)&cfg)[sizeof(cfg)-1] = 0;
	memset(&cfg,0,sizeof(cfg));
	cfg.dwSize = sizeof(cfg);
	cfg.byPlateEnable = 1; //是否支持车牌识别 0-不支持，1- 支持
	cfg.byFrameFlip = 0;  //图像是否翻转 0-不翻转 1-翻转
	cfg.wFlipAngle = 180;   // 图片翻转角度 0，90，180，270
	cfg.wLightPhase = 270;    //相位 取值范围(0,360)
	cfg.byLightSyncPower = 1; // 是否信号灯电源同步 0-不同步 1 同步
	cfg.byFrequency = 36;  //信号频率
	cfg.byUploadSDEnable = 1;  //是否自动上传SD图片 0否 1是
	//--------以上字段仅IPC有效，以下字段仅模拟相机有效

	cfg.byPlateMode = 1;       //识别模式参数(仅Ids-65xx支持)0-视频触发 1外部触发
	cfg.byBlackLightMode = 2;  // 背光补偿模式 0-关闭背光补偿 1-开启背光补偿 2-开启强光抑制
	cfg.byColorBlackMode = 2;  // 彩转黑模式 0-自动彩转黑 1-黑白模式，2彩色模式
	cfg.byApertureMode = 1;    // 光圈模式，0 电子光圈 1自动光圈
	cfg.byShutterValue = 8;    //快门值 取值范围[0,12]

	memcpy_s(sendbuf, 2048, &cfg, sizeof(cfg));
	*iLen = sizeof(cfg);
	return true;
}
bool clsCfgParam::funcSET_CAM_ABILITYCFG_MCam(DEVICE *d,char *recvbuf,int iChannel)
{
	CAM_ABILITY_CFG * cfg = (CAM_ABILITY_CFG *)recvbuf;
	printf_s("Ability\n");
	printf_s("Channel %d\n",iChannel);
	printf_s("str:%s\nstrlen: %d, size: %d \n",(char*) cfg, strlen((char*)cfg), sizeof(*cfg));
	return true;
}
bool clsCfgParam::funcGET_CAM_IMAGEOVERLAY_MCam(DEVICE *d,char *sendbuf,int iChannel,int *iLen)
{
	printf_s("IMAGEOVERLAY\n");
	printf_s("Channel %d\n",iChannel);
	CAM_IMAGEOVERLAY_CFG cfg;
	memset(&cfg,'b',sizeof(cfg));
	((char*)&cfg)[sizeof(cfg)-1] = 0;
	memset(&cfg,0,sizeof(cfg));
	cfg.dwSize = sizeof(cfg);
	cfg.byOverlayInfo = 1; //叠加使能开关 0-不叠加，1-叠加
	cfg.byOverlayMonitorInfo = 1; //是否叠加监控点信息 0-不叠加，1-叠加
	cfg.byOverlayTime = 1;   // 是否叠加时间 0-不叠加，1-叠加
	cfg.byOverSpeed = 1;  ////是否叠加速度 0-不叠加，1-叠加
	cfg.byOverlaySpeeding = 1; // 是否叠加超速比例 0-不叠加，1-叠加
	cfg.byOverlayLimitFlag = 1; //是否叠加限速标记 0-不叠加，1-叠加
	cfg.byOverlayPlate = 1;     //是否叠加车牌 0-不叠加，1-叠加
	cfg.byOverlayColor = 1;      //是否叠加车身颜色 0-不叠加，1-叠加
	cfg.byOverlayLength = 1;     // 是否叠加车长 0-不叠加，1-叠加
	cfg.byOverlayType = 1;       // 是否叠加车型 0- 不叠加，1-叠加
	cfg.byOverlayColorDepth = 1; //是否叠加车身颜色深浅 0-不叠加，1-叠加
	cfg.byOverlayDriveChan = 1;   //是否叠加车道 0-不叠加 1-叠加
	cfg.byOverlayMillSec = 1;     //是否叠加毫秒信息 0-不叠加，1-叠加
	cfg.byOverlayIllegalInfo = 1;  //是否叠加违章信息 0-不叠加 1-叠加
	cfg.byOverlayRedOnTime = 1;    // 是否叠加红灯已亮时间 0-不叠加，1-叠加
	cfg.byMonitor1 = 0;//strcpy_s((char*)cfg.byMonitor1,32,"12");            //监控点1
	cfg.byMonitor2 = 0;//strcpy_s((char*)cfg.byMonitor2,44,"34");            //监控点2

	memcpy_s(sendbuf, 2048, &cfg, sizeof(cfg));
	*iLen = sizeof(cfg);
	return true;
}
bool clsCfgParam::funcSET_CAM_IMAGEOVERLAY_MCam(DEVICE *d,char *recvbuf,int iChannel)
{
	CAM_IMAGEOVERLAY_CFG * cfg = (CAM_IMAGEOVERLAY_CFG *)recvbuf;
	printf_s("IMAGEOVERLAY\n");
	printf_s("Channel %d\n",iChannel);
	printf_s("str:%s\nstrlen: %d, size: %d \n",(char*) cfg, strlen((char*)cfg), sizeof(*cfg));
	return true;
}
bool clsCfgParam::funcGET_CAM_FLASHCFG_MCam(DEVICE *d,char *sendbuf,int iChannel,int param1,int *iLen)
{
	printf_s("FLASH\n");
	printf_s("Channel %d\n",iChannel);
	printf_s("IO Channel %d\n",param1);
	CAM_FLASH_CFG cfg;
	memset(&cfg,'b',sizeof(cfg));
	((char*)&cfg)[sizeof(cfg)-1] = 0;
	memset(&cfg,0,sizeof(cfg));
	cfg.dwSize = sizeof(cfg);
	cfg.byCamAddr = 12; //摄像机485地址,0xFF表示无效
	cfg.byFlashAddr = 34; //闪光灯控制小板 485地址，0xFF表示无效
	cfg.fPhase = 1.2; // 闪光灯控制小板 相位取值范围
	cfg.fDutyRate = 3.4;//
	cfg.nFreqValue = 3;  //闪光灯控制小板 频率参数 取值 0-4
	cfg.byMode = 3; //闪光灯闪烁模式 0- 不闪，1--闪，2-- 关联闪，3-轮闪
	cfg.byRelatedIoIn = 1; //闪光灯关联输入IO号，关联闪时，此参数有效，保留
	cfg.byRecognizedLane = 1; // 关联的IO号 按位表示,bit0 表示io1 是否关联 0-不关联，1关联
	cfg.byDetectBrightness = 1; // 自动检测亮度使能闪光灯 0- 不检测 1- 检测
	cfg.byBrightnessThreld = 56;// 使能闪光灯亮度阈值，范围 [0-100],高于阈值闪
	cfg.byStartHour = 12; //开始时间 单位小时
	cfg.byStartMinute = 34; //开始时间单位分 取值范围[0,59]
	cfg.byEndHour = 18;    // 结束时间 单位小时，取值范围[0，23]
	cfg.byEndMinute = 56; // 结束时间 单位分，取值范围[0,59]
	cfg.byFlashLightEnable = 1; //闪光灯是否开启 0 -关，1 - 开

	memcpy_s(sendbuf, 2048, &cfg, sizeof(cfg));
	*iLen = sizeof(cfg);
	return true;
}
bool clsCfgParam::funcSET_CAM_FLASHCFG_MCam(DEVICE *d,char *recvbuf,int iChannel,int param1)
{
	CAM_FLASH_CFG * cfg = (CAM_FLASH_CFG *)recvbuf;
	printf_s("FLASH\n");
	printf_s("Channel %d\n",iChannel);
	printf_s("IO Channel %d\n",param1);
	printf_s("str:%s\nstrlen: %d, size: %d \n",(char*) cfg, strlen((char*)cfg), sizeof(*cfg));
	return true;
}
bool clsCfgParam::funcGET_CAM_IO_INCFG_MCam(DEVICE *d,char *sendbuf,int iChannel,int param1,int *iLen)
{
	printf_s("IOIN\n");
	printf_s("Channel %d\n",iChannel);
	printf_s("IO Channel %d\n",param1);
	CAM_IO_IN_CFG cfg;
	memset(&cfg,'b',sizeof(cfg));
	memset(&cfg,0,sizeof(cfg));
	cfg.dwSize = sizeof(cfg);
	cfg.byIoInStatus = 2;

	((char*)&cfg)[sizeof(cfg)-1] = 0;
	memcpy_s(sendbuf, 2048, &cfg, sizeof(cfg));
	*iLen = sizeof(cfg);
	return true;
}
bool clsCfgParam::funcSET_CAM_IO_INCFG_MCam(DEVICE *d,char *recvbuf,int iChannel,int param1)
{
	CAM_IO_IN_CFG * cfg = (CAM_IO_IN_CFG *)recvbuf;
	printf_s("IOIN\n");
	printf_s("Channel %d\n",iChannel);
	printf_s("IO Channel %d\n",param1);
	printf_s("str:%s\nstrlen: %d, size: %d \n",(char*) cfg, strlen((char*)cfg), sizeof(*cfg));
	return true;
}
bool clsCfgParam::funcGET_CAM_IO_OUTCFG_MCam(DEVICE *d,char *sendbuf,int iChannel,int param1,int *iLen)
{
	printf_s("IOOUT\n");
	printf_s("Channel %d\n",iChannel);
	printf_s("IO Channel %d\n",param1);
	CAM_IO_OUT_CFG cfg;
	memset(&cfg,'b',sizeof(cfg));
	((char*)&cfg)[sizeof(cfg)-1] = 0;
	memset(&cfg,0,sizeof(cfg));
	cfg.dwSize = sizeof(cfg);
	cfg.byDefaultStatus = 1; // IO 默认状态 0-低电平 1- 高电平
	cfg.byIoOutStatus = 2;  // IO起效时状态 0-低电平 1- 高电平，2- 脉冲
	cfg.wAheadTime = 123;     // 输出IO提前时间 单位us,取值范围 [0,300]
	cfg.dwTimePluse = 345;    // 脉冲间隔时间 单位 us
	cfg.dwTimeDelay = 567;    // IO有效持续时间 单位us
	cfg.byFreqMulti = 13;    // 倍频，取值范围[1,15] 
	cfg.byDutyRate = 33;     // 占空比 单位%[0,40]

	memcpy_s(sendbuf, 2048, &cfg, sizeof(cfg));
	*iLen = sizeof(cfg);
	return true;
}
bool clsCfgParam::funcSET_CAM_IO_OUTCFG_MCam(DEVICE *d,char *recvbuf,int iChannel,int param1)
{
	CAM_IO_OUT_CFG * cfg = (CAM_IO_OUT_CFG *)recvbuf;
	printf_s("IOOUT\n");
	printf_s("Channel %d\n",iChannel);
	printf_s("IO Channel %d\n",param1);
	printf_s("str:%s\nstrlen: %d, size: %d \n",(char*) cfg, strlen((char*)cfg), sizeof(*cfg));
	return true;
}
bool clsCfgParam::funcGET_CAM_IO_SNAPCFG_MCam(DEVICE *d,char *sendbuf,int iChannel,int param1,int *iLen)
{
	printf_s("IOSNAP\n");
	printf_s("Channel %d\n",iChannel);
	printf_s("IO Channel %d\n",param1);
	CAM_IO_SNAP_CFG cfg;
	memset(&cfg,'b',sizeof(cfg));
	((char*)&cfg)[sizeof(cfg)-1] = 0;
	memset(&cfg,0,sizeof(cfg));
	cfg.dwSize = sizeof(cfg);
	cfg.byRelatedDriveWay = 8; // 触发IO关联的车道号 取值范围 [0-9]
	cfg.bySnapTimes = 4; // 线圈抓拍次数 0-不抓拍 非0 为抓拍次数 最大为5次
	cfg.wSnapWaitTime = 1234;   //抓拍等待时间 单位ms,取值范围 [0,6000]
	cfg.wIntervalTime[0] = 2345; // 抓拍间隔时间 单位ms,取值范围[67,60000]
	cfg.wIntervalTime[1] = 3456; // 抓拍间隔时间 单位ms,取值范围[67,60000]
	cfg.wIntervalTime[2] = 4567; // 抓拍间隔时间 单位ms,取值范围[67,60000]
	cfg.wIntervalTime[3] = 5678; // 抓拍间隔时间 单位ms,取值范围[67,60000]

	memcpy_s(sendbuf, 2048, &cfg, sizeof(cfg));
	*iLen = sizeof(cfg);
	return true;
}
bool clsCfgParam::funcSET_CAM_IO_SNAPCFG_MCam(DEVICE *d,char *recvbuf,int iChannel,int param1)
{
	CAM_IO_SNAP_CFG * cfg = (CAM_IO_SNAP_CFG *)recvbuf;
	printf_s("IOSNAP\n");
	printf_s("Channel %d\n",iChannel);
	printf_s("IO Channel %d\n",param1);
	printf_s("str:%s\nstrlen: %d, size: %d \n",(char*) cfg, strlen((char*)cfg), sizeof(*cfg));
	return true;
}

bool clsCfgParam::funcSET_CAM_REBOOT_MCam(DEVICE *d,int iChannel)
{
	printf_s("REBOOT\n");
	printf_s("Channel %d\n",iChannel);
	return true;
}
bool clsCfgParam::funcSET_CAM_UPDATE_MCam(DEVICE *d,char *recvbuf,int iChannel)
{
	CString updatepath = recvbuf;
	printf_s("Channel %d\n",iChannel);
	printf_s(updatepath);
	printf_s("\n");
	return true;
}
bool clsCfgParam::funcSET_CAM_DEFAULT_MCam(DEVICE *d,int iChannel)
{
	//CAM_IO_SNAP_CFG * cfg = (CAM_IO_SNAP_CFG *)recvbuf;
	printf_s("DEFAULT\n");
	printf_s("Channel %d\n",iChannel);
	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//远程参数配置服务开启
extern "C" void WINAPI  EXPORT ServerStart()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	clsNetIONvr * pNetIoNvr = clsNetIONvr::GetInstance();
	clsNetIONvr::LeaveInstance();
	pNetIoNvr->ServerStart();
}

//远程参数配置服务停止
extern "C" void WINAPI EXPORT ServerStop()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	clsNetIONvr * pNetIoNvr = clsNetIONvr::GetInstance();
	clsNetIONvr::LeaveInstance();
	pNetIoNvr->ServerStop();
}