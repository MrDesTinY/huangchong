#pragma once

#include "clsNetIONvr.h"

class clsCfgParam
{
public:
	clsCfgParam(void);
	~clsCfgParam(void);
public:
	static void funcNET_IO_PREV_CHAN_VALID_Cam(DEVICE *d,char *sendbuf);
	static bool funcNET_IO_PREV_CHAN_START_Cam(DEVICE *d,char *recvbuf,int iChannel);
	static bool funcNET_IO_PREV_CHAN_STOP_Cam(DEVICE *d);
	static bool funcNET_IO_PREV_CHAN_HEAD_Cam(DEVICE *d,int iChannel,char *recvbuf,char *sendbuf,int *iLen,void *pNetIoNvr);

	static CAM_BASIC_CFG funcGET_CAM_BASICCFG_Cam(DEVICE *d,char *sendbuf,int iChannel,int *iLen);
	static bool funcSET_CAM_BASICCFG_Cam(DEVICE *d,char *recvbuf,int iChannel);
	static CAM_COMPRESS_CFG funcGET_CAM_COMPRESSCFG_Cam(DEVICE *d,char *sendbuf,int iChannel,int *iLen);
	static bool funcSET_CAM_COMPRESSCFG_Cam(DEVICE *d,char *recvbuf,int iChannel);
	static CAM_OSD_CFG funcGET_CAM_OSDCFG_Cam(DEVICE *d,char *sendbuf,int iChannel,int *iLen);
	static bool funcSET_CAM_OSDCFG_Cam(DEVICE *d,char *recvbuf,int iChannel);
	static CAM_NET_CFG funcGET_CAM_NETCFG_Cam(DEVICE *d,char *sendbuf,int iChannel,int *iLen);
	static bool funcSET_CAM_NETCFG_Cam(DEVICE *d,char *recvbuf,int iChannel);
	static CAM_PLATE_CFG funcGET_CAM_PLATECFG_Cam(DEVICE *d,char *sendbuf,int iChannel,int *iLen);
	static bool funcSET_CAM_PLATECFG_Cam(DEVICE *d,char *recvbuf,int iChannel);
	static CAM_FACE_CFG funcGET_CAM_FACECFG_Cam(DEVICE *d,char *sendbuf,int iChannel,int *iLen);
	static bool funcSET_CAM_FACECFG_Cam(DEVICE *d,char *recvbuf,int iChannel);
	static CAM_VEDIO_CFG funcGET_CAM_VEDIO_FRONTCFG_Cam(DEVICE *d,char *sendbuf,int iChannel,int *iLen);
	static bool funcSET_CAM_VEDIO_FRONTCFG_Cam(DEVICE *d,char *recvbuf,int iChannel);
	static CAM_PLATE_BRT_CFG funcGET_CAM_PLATE_BRTCFG_Cam(DEVICE *d,char *sendbuf,int iChannel,int *iLen);
	static bool funcSET_CAM_PLATE_BRTCFG_Cam(DEVICE *d,char *recvbuf,int iChannel);
	static CAM_ABILITY_CFG funcGET_CAM_ABILITYCFG_Cam(DEVICE *d,char *sendbuf,int iChannel,int *iLen);
	static bool funcSET_CAM_ABILITYCFG_Cam(DEVICE *d,char *recvbuf,int iChannel);
	static CAM_IMAGEOVERLAY_CFG funcGET_CAM_IMAGEOVERLAY_Cam(DEVICE *d,char *sendbuf,int iChannel,int *iLen);
	static bool funcSET_CAM_IMAGEOVERLAY_Cam(DEVICE *d,char *recvbuf,int iChannel);
	static CAM_FLASH_CFG funcGET_CAM_FLASHCFG_Cam(DEVICE *d,char *sendbuf,int iChannel,int param1,int *iLen);
	static bool funcSET_CAM_FLASHCFG_Cam(DEVICE *d,char *recvbuf,int iChannel,int param1);
	static CAM_IO_IN_CFG funcGET_CAM_IO_INCFG_Cam(DEVICE *d,char *sendbuf,int iChannel,int param1,int *iLen);
	static bool funcSET_CAM_IO_INCFG_Cam(DEVICE *d,char *recvbuf,int iChannel,int param1);
	static CAM_IO_OUT_CFG funcGET_CAM_IO_OUTCFG_Cam(DEVICE *d,char *sendbuf,int iChannel,int param1,int *iLen);
	static bool funcSET_CAM_IO_OUTCFG_Cam(DEVICE *d,char *recvbuf,int iChannel,int param1);
	static CAM_IO_SNAP_CFG funcGET_CAM_IO_SNAPCFG_Cam(DEVICE *d,char *sendbuf,int iChannel,int param1,int *iLen);
	static bool funcSET_CAM_IO_SNAPCFG_Cam(DEVICE *d,char *recvbuf,int iChannel,int param1);

	static bool funcSET_CAM_REBOOT_Cam(DEVICE *d,int iChannel);
	static bool funcSET_CAM_UPDATE_Cam(DEVICE *d,char *recvbuf,int iChannel);
	static bool funcSET_CAM_DEFAULT_Cam(DEVICE *d,int iChannel);
};
