#pragma once
#define STATIC_LOAD_IO_NVR
#include "NetIoNvrDll.h"
#include "OperMysql.h"
#include "Config.h"
#include "Log.h"

//设备结构体
typedef struct _tagDevice{
	int CamID;//相机对应数据库id
	int IsUsing;
	long LoginHandle;
	char Head[300];
	char SubHead[300];
	long HeadLength;
	long SubHeadLength;
	long PlayHandle;
	long SubHandle;
	long HeadHandle;
	NET_IO_NET_REALPLAY_PARAM MainParam;
	NET_IO_NET_REALPLAY_PARAM SubParam;
	NET_DVR_DEVICEINFO_V30 DeviceInfo;
	_tagDevice()
	{
		CamID = -1;
		IsUsing = 0;
		LoginHandle = -1;
		ZeroMemory(Head,sizeof(Head));
		HeadLength = -1;
		ZeroMemory(SubHead,sizeof(SubHead));
		SubHeadLength = -1;
		PlayHandle = -1;
		SubHandle = -1;
		HeadHandle = -1;
		ZeroMemory(&DeviceInfo,sizeof(DeviceInfo));
	}
	~_tagDevice()
	{
		if (HeadHandle >= 0)
		{
			NET_DVR_StopRealPlay(HeadHandle);
			HeadHandle = -1;
		}
		if (PlayHandle >= 0)
		{
			NET_DVR_StopRealPlay(PlayHandle);
			PlayHandle = -1;
		}
		if (SubHandle >= 0)
		{
			NET_DVR_StopRealPlay(SubHandle);
			SubHandle = -1;
		}
		if (LoginHandle >=0)
		{
			NET_DVR_Logout_V30(LoginHandle);
			LoginHandle = -1;
		}
	}
}DEVICE,*pDEVICE;

//海康取流函数指针类型
typedef void(__stdcall *fRealDataCallBack_V30)(LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, void *pUser);
//远程相机参数配置回调函数指针类型
typedef int(__stdcall *pCfgDataCallBack)(int iChannel,int iCammandCode,char *recvbuf,char *sendbuf,int *iLen,int userdata,int param1,int param2);

//海康取流回调函数，其中应该调用NET_IO_PutStream去向中心送流。
void __stdcall RealDataCallBack_V30(LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, void *pUser);

// 配置相机参数的回调
/*回调函数
1.取流部分，回调函数会依次给四种命令：
	NET_IO_PREV_CHAN_VALID
	询问某个通道是否可用。
	此时把sendbuf看作一个bool指针，根据情况，*取值后赋入true或者false即可
	NET_IO_PREV_CHAN_HEAD
	要求码流的系统头
	此时把recvbuf看作一个BOOL类型（非指针），强制转换类型后，得到的值为是否为主码流。
	再根据iChannel参数，将对应的头写入sendbuf所指的内存区域中，再于*iLen中赋入系统头的长度。
	NET_IO_PREV_CHAN_START
	此时把recvbuf看作一个BOOL类型（非指针），强制转换类型后，得到的值为是否为主码流。
	再根据iChannel参数，开始取该通道的主码流或子码流。
	收到此命令后，应开始使用NET_IO_PutStream来提供码流。
	NET_IO_PREV_CHAN_STOP
	某通道所有连接都关闭后，停止播放。iChannel为需要停止的通道。
	收到此命令后可以停止取流。主码流和子码流都停止。
2.配置部分，根据回调函数给的命令，将recvbuf看作不同的结构体指针来处理。
	如果是SET系列配置命令，操作成功时，回调函数返回值为0，反之，操作失败，则回调函数应返回负数。
	GET指令则需要恰当地填写sendbuf与iLen，操作成功时回调函数返回0，否则返回负数。
*/
/****
回调函数参数说明
int iChannel:通道号
int iCammandCode:命令码
char *recvbuf:接收的数据buffer，为远程重启命令时，为升级的二进制文件的完全路径
char *sendbuf:发送的数据buffer
ilen         :发送数据长度指针
int userdata 用户自定义数据类型
int param1   相机I0配置时为IO号
int param2   未使用

发送接收的buffer 最大的长度为MAX_CLIENT_DATA (2K bytes)
*****/
int __stdcall CfgDataCallBack(int iChannel,int iCammandCode,char *recvbuf,char *sendbuf,int *iLen,int userdata,int param1,int param2);

#ifndef STATIC_LOAD_IO_NVR
//函数指针类型定义集
typedef int(__cdecl  *pfuncNET_IO_Initialize)(void);
typedef int(__cdecl  *pfuncNET_IO_Finalize )(void);
typedef int(__cdecl  *pfuncNET_IO_StartService)(UINT Port);
typedef int(__cdecl  *pfuncNET_IO_StopService)(void);
typedef int(__cdecl  *pfuncNET_IO_PutStream)(int Channel, bool bIsMasterStream, char* Stream, int StreamLength,int FrameType);
typedef int(__cdecl  *pfuncNET_IO_SetCallBackCfgData)(pCfgDataCallBack pDataFunc,int userdata);
typedef int(__cdecl  *pfuncNET_IO_SetDeviceBuffer)(int* DeviceType);
#endif
//通讯库类
class clsNetIONvr
{
private:
	clsNetIONvr(void);
	~clsNetIONvr(void);
public:
	void ServerStart();
	void ServerStop();
	static int m_iServerFlag;
#ifndef STATIC_LOAD_IO_NVR
private:
	pfuncNET_IO_Initialize NET_IO_Initialize;
	pfuncNET_IO_Finalize NET_IO_Finalize;
	HMODULE m_hModule;
public:
	pfuncNET_IO_StartService NET_IO_StartService;
	pfuncNET_IO_StopService NET_IO_StopService;
	pfuncNET_IO_PutStream NET_IO_PutStream;
	pfuncNET_IO_SetCallBackCfgData NET_IO_SetCallBackCfgData;
	pfuncNET_IO_SetDeviceBuffer NET_IO_SetDeviceBuffer;
#endif
	static clsNetIONvr* GetInstance();
	static void LeaveInstance();
private:
	static clsNetIONvr *m_pInstance;//实例指针
	static CRITICAL_SECTION m_csLock;//实例锁，解决多线程问题
	//内嵌类，用于释放m_pInstance指针
	//class CGarbo{
	//public:
	//	~CGarbo()
	//	{
	//		if(clsNetIONvr::m_pInstance)
	//		{
	//			delete clsNetIONvr::m_pInstance;
	//			clsNetIONvr::m_pInstance = NULL;
	//		}
	//	}
	//	static CGarbo m_Carbo;
	//};
	//
public:
	static COpMySQL m_OperMysql;//数据库操作类

	static CConfig m_Config;//配置文件类
	CString	m_strDBServer;
	CString	m_strDBName; 
	CString	m_strDBUser;   
	CString	m_strDBPwd;   
	CString	m_strDBPort;
	CString m_strNetIoPort;

	static CLog m_CLog;//日志文件类
public:
	int m_iDevicesTable[MAX_CHANNEL_NUM];//用于记录设备的位置，为NET_IO_SetDeviceBuffer的参数，0～15为高清相机，16～31为51xx设备.
	DEVICE m_Devices[MAX_CHANNEL_NUM];//设备类型数组
	//获取所有设备
	void GetAllAccessDevices();
private:
	BOOL m_InitFlag;
public:
	BOOL GetDLLStatus(void);
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//参数配置类定义部分
class clsCfgParam
{
public:
	clsCfgParam(void);
	~clsCfgParam(void);
public:
	static bool funcNET_IO_PREV_CHAN_VALID_Cam(DEVICE *d,char *sendbuf);
	static bool funcNET_IO_PREV_CHAN_START_Cam(DEVICE *d,char *recvbuf,int iChannel);
	static bool funcNET_IO_PREV_CHAN_STOP_Cam(DEVICE *d);
	static bool funcNET_IO_PREV_CHAN_HEAD_Cam(DEVICE *d,int iChannel,char *recvbuf,char *sendbuf,int *iLen,void *pNetIoNvr);

	static bool funcGET_CAM_BASICCFG_Cam(DEVICE *d,char *sendbuf,int iChannel,int *iLen);
	static bool funcSET_CAM_BASICCFG_Cam(DEVICE *d,char *recvbuf,int iChannel);
	static bool funcGET_CAM_COMPRESSCFG_Cam(DEVICE *d,char *sendbuf,int iChannel,int *iLen);
	static bool funcSET_CAM_COMPRESSCFG_Cam(DEVICE *d,char *recvbuf,int iChannel);
	static bool funcGET_CAM_OSDCFG_Cam(DEVICE *d,char *sendbuf,int iChannel,int *iLen);
	static bool funcSET_CAM_OSDCFG_Cam(DEVICE *d,char *recvbuf,int iChannel);
	static bool funcGET_CAM_NETCFG_Cam(DEVICE *d,char *sendbuf,int iChannel,int *iLen);
	static bool funcSET_CAM_NETCFG_Cam(DEVICE *d,char *recvbuf,int iChannel);
	static bool funcGET_CAM_PLATECFG_Cam(DEVICE *d,char *sendbuf,int iChannel,int *iLen);
	static bool funcSET_CAM_PLATECFG_Cam(DEVICE *d,char *recvbuf,int iChannel);
	static bool funcGET_CAM_FACECFG_Cam(DEVICE *d,char *sendbuf,int iChannel,int *iLen);
	static bool funcSET_CAM_FACECFG_Cam(DEVICE *d,char *recvbuf,int iChannel);
	static bool funcGET_CAM_VEDIO_FRONTCFG_Cam(DEVICE *d,char *sendbuf,int iChannel,int *iLen);
	static bool funcSET_CAM_VEDIO_FRONTCFG_Cam(DEVICE *d,char *recvbuf,int iChannel);
	static bool funcGET_CAM_PLATE_BRTCFG_Cam(DEVICE *d,char *sendbuf,int iChannel,int *iLen);
	static bool funcSET_CAM_PLATE_BRTCFG_Cam(DEVICE *d,char *recvbuf,int iChannel);
	static bool funcGET_CAM_ABILITYCFG_Cam(DEVICE *d,char *sendbuf,int iChannel,int *iLen);
	static bool funcSET_CAM_ABILITYCFG_Cam(DEVICE *d,char *recvbuf,int iChannel);
	static bool funcGET_CAM_IMAGEOVERLAY_Cam(DEVICE *d,char *sendbuf,int iChannel,int *iLen);
	static bool funcSET_CAM_IMAGEOVERLAY_Cam(DEVICE *d,char *recvbuf,int iChannel);
	static bool funcGET_CAM_FLASHCFG_Cam(DEVICE *d,char *sendbuf,int iChannel,int param1,int *iLen);
	static bool funcSET_CAM_FLASHCFG_Cam(DEVICE *d,char *recvbuf,int iChannel,int param1);
	static bool funcGET_CAM_IO_INCFG_Cam(DEVICE *d,char *sendbuf,int iChannel,int param1,int *iLen);
	static bool funcSET_CAM_IO_INCFG_Cam(DEVICE *d,char *recvbuf,int iChannel,int param1);
	static bool funcGET_CAM_IO_OUTCFG_Cam(DEVICE *d,char *sendbuf,int iChannel,int param1,int *iLen);
	static bool funcSET_CAM_IO_OUTCFG_Cam(DEVICE *d,char *recvbuf,int iChannel,int param1);
	static bool funcGET_CAM_IO_SNAPCFG_Cam(DEVICE *d,char *sendbuf,int iChannel,int param1,int *iLen);
	static bool funcSET_CAM_IO_SNAPCFG_Cam(DEVICE *d,char *recvbuf,int iChannel,int param1);

	static bool funcSET_CAM_REBOOT_Cam(DEVICE *d,int iChannel);
	static bool funcSET_CAM_UPDATE_Cam(DEVICE *d,char *recvbuf,int iChannel);
	static bool funcSET_CAM_DEFAULT_Cam(DEVICE *d,int iChannel);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static bool funcNET_IO_PREV_CHAN_VALID_MCam(DEVICE *d,char *sendbuf);
	static bool funcNET_IO_PREV_CHAN_START_MCam(DEVICE *d,char *recvbuf,int iChannel);
	static bool funcNET_IO_PREV_CHAN_STOP_MCam(DEVICE *d);
	static bool funcNET_IO_PREV_CHAN_HEAD_MCam(DEVICE *d,int iChannel,char *recvbuf,char *sendbuf,int *iLen,void *pNetIoNvr);

	static bool funcGET_CAM_BASICCFG_MCam(DEVICE *d,char *sendbuf,int iChannel,int *iLen);
	static bool funcSET_CAM_BASICCFG_MCam(DEVICE *d,char *recvbuf,int iChannel);
	static bool funcGET_CAM_COMPRESSCFG_MCam(DEVICE *d,char *sendbuf,int iChannel,int *iLen);
	static bool funcSET_CAM_COMPRESSCFG_MCam(DEVICE *d,char *recvbuf,int iChannel);
	static bool funcGET_CAM_OSDCFG_MCam(DEVICE *d,char *sendbuf,int iChannel,int *iLen);
	static bool funcSET_CAM_OSDCFG_MCam(DEVICE *d,char *recvbuf,int iChannel);
	static bool funcGET_CAM_NETCFG_MCam(DEVICE *d,char *sendbuf,int iChannel,int *iLen);
	static bool funcSET_CAM_NETCFG_MCam(DEVICE *d,char *recvbuf,int iChannel);
	static bool funcGET_CAM_PLATECFG_MCam(DEVICE *d,char *sendbuf,int iChannel,int *iLen);
	static bool funcSET_CAM_PLATECFG_MCam(DEVICE *d,char *recvbuf,int iChannel);
	static bool funcGET_CAM_FACECFG_MCam(DEVICE *d,char *sendbuf,int iChannel,int *iLen);
	static bool funcSET_CAM_FACECFG_MCam(DEVICE *d,char *recvbuf,int iChannel);
	static bool funcGET_CAM_VEDIO_FRONTCFG_MCam(DEVICE *d,char *sendbuf,int iChannel,int *iLen);
	static bool funcSET_CAM_VEDIO_FRONTCFG_MCam(DEVICE *d,char *recvbuf,int iChannel);
	static bool funcGET_CAM_PLATE_BRTCFG_MCam(DEVICE *d,char *sendbuf,int iChannel,int *iLen);
	static bool funcSET_CAM_PLATE_BRTCFG_MCam(DEVICE *d,char *recvbuf,int iChannel);
	static bool funcGET_CAM_ABILITYCFG_MCam(DEVICE *d,char *sendbuf,int iChannel,int *iLen);
	static bool funcSET_CAM_ABILITYCFG_MCam(DEVICE *d,char *recvbuf,int iChannel);
	static bool funcGET_CAM_IMAGEOVERLAY_MCam(DEVICE *d,char *sendbuf,int iChannel,int *iLen);
	static bool funcSET_CAM_IMAGEOVERLAY_MCam(DEVICE *d,char *recvbuf,int iChannel);
	static bool funcGET_CAM_FLASHCFG_MCam(DEVICE *d,char *sendbuf,int iChannel,int param1,int *iLen);
	static bool funcSET_CAM_FLASHCFG_MCam(DEVICE *d,char *recvbuf,int iChannel,int param1);
	static bool funcGET_CAM_IO_INCFG_MCam(DEVICE *d,char *sendbuf,int iChannel,int param1,int *iLen);
	static bool funcSET_CAM_IO_INCFG_MCam(DEVICE *d,char *recvbuf,int iChannel,int param1);
	static bool funcGET_CAM_IO_OUTCFG_MCam(DEVICE *d,char *sendbuf,int iChannel,int param1,int *iLen);
	static bool funcSET_CAM_IO_OUTCFG_MCam(DEVICE *d,char *recvbuf,int iChannel,int param1);
	static bool funcGET_CAM_IO_SNAPCFG_MCam(DEVICE *d,char *sendbuf,int iChannel,int param1,int *iLen);
	static bool funcSET_CAM_IO_SNAPCFG_MCam(DEVICE *d,char *recvbuf,int iChannel,int param1);

	static bool funcSET_CAM_REBOOT_MCam(DEVICE *d,int iChannel);
	static bool funcSET_CAM_UPDATE_MCam(DEVICE *d,char *recvbuf,int iChannel);
	static bool funcSET_CAM_DEFAULT_MCam(DEVICE *d,int iChannel);
};



