#include "StdAfx.h"
#include "clsCfgParam.h"

clsCfgParam::clsCfgParam(void)
{
}

clsCfgParam::~clsCfgParam(void)
{
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void clsCfgParam::funcNET_IO_PREV_CHAN_VALID_Cam(DEVICE *d,char *sendbuf)
{
	*(bool*)sendbuf = (d->LoginHandle >= 0)?true:false;
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
		clsNetIONvr::m_CLog.SaveLogInfo("²¥·ÅÊ§°Ü£¡");
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
		clsNetIONvr::m_CLog.SaveLogInfo("ÖØÖÃIÖ¡Ê§°Ü");
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
				clsNetIONvr::m_CLog.SaveLogInfo("²¥·ÅÊ§°Ü£¡");
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
				clsNetIONvr::m_CLog.SaveLogInfo("²¥·ÅÊ§°Ü£¡");
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

CAM_BASIC_CFG clsCfgParam::funcGET_CAM_BASICCFG_Cam(DEVICE *d,char *sendbuf,int iChannel,int *iLen)
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
	return cfg;
}
bool clsCfgParam::funcSET_CAM_BASICCFG_Cam(DEVICE *d,char *recvbuf,int iChannel)
{
	return true;
}
CAM_COMPRESS_CFG clsCfgParam::funcGET_CAM_COMPRESSCFG_Cam(DEVICE *d,char *sendbuf,int iChannel,int *iLen)
{
	CAM_COMPRESS_CFG cfg;
	return cfg;
}
bool clsCfgParam::funcSET_CAM_COMPRESSCFG_Cam(DEVICE *d,char *recvbuf,int iChannel)
{
	return true;
}
CAM_OSD_CFG clsCfgParam::funcGET_CAM_OSDCFG_Cam(DEVICE *d,char *sendbuf,int iChannel,int *iLen)
{
	CAM_OSD_CFG cfg;
	return cfg;
}
bool clsCfgParam::funcSET_CAM_OSDCFG_Cam(DEVICE *d,char *recvbuf,int iChannel)
{
	return true;
}
CAM_NET_CFG clsCfgParam::funcGET_CAM_NETCFG_Cam(DEVICE *d,char *sendbuf,int iChannel,int *iLen)
{
	CAM_NET_CFG cfg;
	return cfg;
}
bool clsCfgParam::funcSET_CAM_NETCFG_Cam(DEVICE *d,char *recvbuf,int iChannel)
{
	return true;
}
CAM_PLATE_CFG clsCfgParam::funcGET_CAM_PLATECFG_Cam(DEVICE *d,char *sendbuf,int iChannel,int *iLen)
{
	CAM_PLATE_CFG cfg;
	return cfg;
}
bool clsCfgParam::funcSET_CAM_PLATECFG_Cam(DEVICE *d,char *recvbuf,int iChannel)
{
	return true;
}
CAM_FACE_CFG clsCfgParam::funcGET_CAM_FACECFG_Cam(DEVICE *d,char *sendbuf,int iChannel,int *iLen)
{
	CAM_FACE_CFG cfg;
	return cfg;
}
bool clsCfgParam::funcSET_CAM_FACECFG_Cam(DEVICE *d,char *recvbuf,int iChannel)
{
	return true;
}
CAM_VEDIO_CFG clsCfgParam::funcGET_CAM_VEDIO_FRONTCFG_Cam(DEVICE *d,char *sendbuf,int iChannel,int *iLen)
{
	CAM_VEDIO_CFG cfg;
	return cfg;
}
bool clsCfgParam::funcSET_CAM_VEDIO_FRONTCFG_Cam(DEVICE *d,char *recvbuf,int iChannel)
{
	return true;
}
CAM_PLATE_BRT_CFG clsCfgParam::funcGET_CAM_PLATE_BRTCFG_Cam(DEVICE *d,char *sendbuf,int iChannel,int *iLen)
{
	CAM_PLATE_BRT_CFG cfg;
	return cfg;
}
bool clsCfgParam::funcSET_CAM_PLATE_BRTCFG_Cam(DEVICE *d,char *recvbuf,int iChannel)
{
	return true;
}
CAM_ABILITY_CFG clsCfgParam::funcGET_CAM_ABILITYCFG_Cam(DEVICE *d,char *sendbuf,int iChannel,int *iLen)
{
	CAM_ABILITY_CFG cfg;
	return cfg;
}
bool clsCfgParam::funcSET_CAM_ABILITYCFG_Cam(DEVICE *d,char *recvbuf,int iChannel)
{
	return true;
}
CAM_IMAGEOVERLAY_CFG clsCfgParam::funcGET_CAM_IMAGEOVERLAY_Cam(DEVICE *d,char *sendbuf,int iChannel,int *iLen)
{
	CAM_IMAGEOVERLAY_CFG cfg;
	return cfg;
}
bool clsCfgParam::funcSET_CAM_IMAGEOVERLAY_Cam(DEVICE *d,char *recvbuf,int iChannel)
{
	return true;
}
CAM_FLASH_CFG clsCfgParam::funcGET_CAM_FLASHCFG_Cam(DEVICE *d,char *sendbuf,int iChannel,int param1,int *iLen)
{
	CAM_FLASH_CFG cfg;
	return cfg;
}
bool clsCfgParam::funcSET_CAM_FLASHCFG_Cam(DEVICE *d,char *recvbuf,int iChannel,int param1)
{
	return true;
}
CAM_IO_IN_CFG clsCfgParam::funcGET_CAM_IO_INCFG_Cam(DEVICE *d,char *sendbuf,int iChannel,int param1,int *iLen)
{
	CAM_IO_IN_CFG cfg;
	return cfg;
}
bool clsCfgParam::funcSET_CAM_IO_INCFG_Cam(DEVICE *d,char *recvbuf,int iChannel,int param1)
{
	return true;
}
CAM_IO_OUT_CFG clsCfgParam::funcGET_CAM_IO_OUTCFG_Cam(DEVICE *d,char *sendbuf,int iChannel,int param1,int *iLen)
{
	CAM_IO_OUT_CFG cfg;
	return cfg;
}
bool clsCfgParam::funcSET_CAM_IO_OUTCFG_Cam(DEVICE *d,char *recvbuf,int iChannel,int param1)
{
	return true;
}
CAM_IO_SNAP_CFG clsCfgParam::funcGET_CAM_IO_SNAPCFG_Cam(DEVICE *d,char *sendbuf,int iChannel,int param1,int *iLen)
{
	CAM_IO_SNAP_CFG cfg;
	return cfg;
}
bool clsCfgParam::funcSET_CAM_IO_SNAPCFG_Cam(DEVICE *d,char *recvbuf,int iChannel,int param1)
{
	return true;
}

bool clsCfgParam::funcSET_CAM_REBOOT_Cam(DEVICE *d,int iChannel)
{
	return true;
}
bool clsCfgParam::funcSET_CAM_UPDATE_Cam(DEVICE *d,char *recvbuf,int iChannel)
{
	return true;
}
bool clsCfgParam::funcSET_CAM_DEFAULT_Cam(DEVICE *d,int iChannel)
{
	return true;
}