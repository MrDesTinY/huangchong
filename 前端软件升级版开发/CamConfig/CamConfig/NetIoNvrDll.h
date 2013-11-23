#pragma once
//#include "NetIoNvr.h"
#include "incCn/HCNetSDK.h"
#define NET_IO_PREV_CHAN_VALID 0x040001	//询问端口是否有效
#define NET_IO_PREV_CHAN_START 0x040002	//通道开始取流
#define NET_IO_PREV_CHAN_STOP 0x040003	//通道停止取流
#define NET_IO_PREV_CHAN_HEAD 0x040004	//通道取头

#define MAX_CHANNEL_NUM 32

typedef enum
{
	NET_IO_Type_None,  //无效
	NET_IO_Type_Normal,//非高清通道
	NET_IO_Type_HD     //高清通道
}NET_IO_ChanType;

typedef struct zzNetRealPlayCallbackParam{
	LPVOID p;
	//int devid;
	int chan;
	bool bMaster;
	zzNetRealPlayCallbackParam()
	{
		p = NULL;
		chan = -1;
		//devid = -1;
		bMaster = true;
	}
}NET_IO_NET_REALPLAY_PARAM;

#ifdef NET_IO_NVR_DLL_DEFINE
#define NET_IO_NVR_API _declspec(dllexport)
#else
#define NET_IO_NVR_API _declspec(dllimport)
#endif


#ifndef NET_IO_NVR_DLL_DEFINE

#ifndef _COMMU_DEFINE_H__XX
#define _COMMU_DEFINE_H__XX
#endif

#ifndef MAX_VERTEX_NUM
#define MAX_VERTEX_NUM 10
#endif

#ifndef MAX_MASK_NUM 
#define MAX_MASK_NUM 4
#endif

#ifndef MAX_INTERVAL_NUM
#define MAX_INTERVAL_NUM 4
#endif

#ifndef NAME_LEN
#define NAME_LEN 32
#endif

#ifndef SERIALNO_LEN 
#define SERIALNO_LEN 48
#endif

#ifndef MACADDR_LEN
#define MACADDR_LEN 6
#endif

#ifndef MAX_CHJC_NUM
#define MAX_CHJC_NUM 3
#endif

#ifndef MAX_LANERECT_NUM 
#define MAX_LANERECT_NUM 5
#endif

#define NET_DVR_SYSHEAD			1	//系统头数据
#define NET_DVR_STREAMDATA		2	//视频流数据（包括复合流和音视频分开的视频流数据）
#define NET_DVR_AUDIOSTREAMDATA	3	//音频流数据
#define NET_DVR_STD_VIDEODATA	4	//标准视频流数据
#define NET_DVR_STD_AUDIODATA	5	//标准音频流数据
#define NET_DVR_PRIVATE_DATA    112 //私有数据,包括智能信息


#define GET_CAM_BASICCFG				0x030c01		//获取相机基本配置参数 NET_DVR_DEVICECFG_V40 NET_DVR_DEVICECFG
#define SET_CAM_BASICCFG                0x030c02    //设置相机基本参数
#define GET_CAM_COMPRESSCFG				0x030c03		//获取相机压缩参数 NET_DVR_COMPRESSIONCFG
#define SET_CAM_COMPRESSCFG             0x030c04    //设置相机压缩参数
#define GET_CAM_OSDCFG				    0x030c05		//获取相机OSD参数 NET_DVR_PICCFG
#define SET_CAM_OSDCFG                  0x030c06   //设置相机OSD参数
#define GET_CAM_NETCFG				    0x030c07	//获取相机网络配置参数 NET_DVR_NETCFG
#define SET_CAM_NETCFG                  0x030c08 //设置相机网络参数
#define GET_CAM_PLATECFG				0x030c09	//获取相机牌识参数 NET_DVR_SPRCFG
#define SET_CAM_PLATECFG                0x030c0a       //设置相机牌识参数
#define GET_CAM_FACECFG			        0x030c0b//获取相机人脸参数
#define SET_CAM_FACECFG                 0x030c0c       //置相机人脸参数
#define GET_CAM_VEDIO_FRONTCFG			0x030c0d//获取相机视频配置参数 NET_DVR_CAMERAPARAMCFG
#define SET_CAM_VEDIO_FRONTCFG          0x030c0e//设置相机视频配置参数
#define GET_CAM_PLATE_BRTCFG            0x030c0f//获取车牌亮度补偿参数 NET_DVR_PLCCFG
#define SET_CAM_PLATE_BRTCFG            0x030c10 //设置车牌亮度补偿参数
#define GET_CAM_ABILITYCFG              0x030c11 //获取相机能力集参数 NET_DVR_SNAPENABLECFG 
#define SET_CAM_ABILITYCFG              0x030c12 //设置相机能力集参数
#define GET_CAM_IMAGEOVERLAY              0x030c13 //获取相机字符叠加参数 NET_DVR_IMAGEOVERLAYCFG
#define SET_CAM_IMAGEOVERLAY              0x030c14 //设置相机字符叠加参数
#define GET_CAM_FLASHCFG                0x030c15 //获取相机闪光灯参数 NET_DVR_FLASH_OUTCFG
#define SET_CAM_FLASHCFG                0x030c16 //设置相机闪光灯参数
#define GET_CAM_IO_INCFG                0x030c17 //获取IO输入参数 NET_DVR_IO_INCFG
#define SET_CAM_IO_INCFG                0x030c18 //设置IO输入参数
#define GET_CAM_IO_OUTCFG               0x030c19 //获取IO输出参数 NET_DVR_IO_OUTCFG
#define SET_CAM_IO_OUTCFG               0x030c1a //设置IO输出参数
#define GET_CAM_IO_SNAPCFG              0x030c1b //获取IO单线圈触发参数 NET_DVR_SNAPCFG
#define SET_CAM_IO_SNAPCFG              0x030c1c //设置IO单线圈触发参数
#define SET_CAM_REBOOT                  0x030c1d //相机远程重启 NET_DVR_RebootDVR
#define SET_CAM_UPDATE                  0x030c1e //相机远程升级 NET_DVR_Upgrade
#define SET_CAM_DEFAULT                 0x030c1f //相机恢复默认参数

// 相机结构体
typedef struct _tagCAM_INFO
{
	BYTE byChannel;  // 相机的通道号
	BYTE byIoChannel; // 相机IO 控制通道号 0xff 为未使用
	BYTE byRes[2]; //预留置0

	_tagCAM_INFO()
	{
		byChannel = 0;
		byIoChannel = 0xff;
		memset(byRes,0,sizeof(byRes));
	}

}CAM_INFO,*LPCAM_INFO;

//相机基本配置信息
typedef struct _tagCAM_BASIC_CFG
{
	DWORD dwSize;
	BYTE  byCamName[NAME_LEN]; // 通道名称

	//----------以下字段均为只读字段
	BYTE  byDeviceType; // 相机类型
	BYTE  byChannelNum; //相机通道数目
	BYTE  byIoNum;  // IO通道数目 
	BYTE  byRes1;  //预留，置0

	BYTE  sSerialNumber[SERIALNO_LEN]; // 序列号
	DWORD dwSoftwareVersion;  //软件版本号,高16位是主版本,低16位是次版本
	DWORD dwSoftwareBuildDate; //软件生成日期,0xYYYYMMDD  
	DWORD dwDSPSoftwareVersion;//DSP软件版本,高16位是主版本,低16位是次版本
	DWORD dwDSPSoftwareBuildDate; //DSP 软件生成日期 OxYYYYMMDD
	DWORD dwHardwareVersion; // //硬件版本,高16位是主版本,低16位是次版本
	DWORD dwRes; //预留置 0

}CAM_BASIC_CFG,*LPCAM_BASIC_CFG;

typedef struct _tagSET_CAM_BASIC_CFG
{
	CAM_INFO stCamInfo;
	CAM_BASIC_CFG stCamBasicCfg;
}SET_CAM_BASIC_CFG,*LPSET_CAM_BASIC_CFG;


//压缩参数
typedef struct _tagCAM_COMPRESS_INFO
{
	BYTE byStreamType;  // 码流类型 0-视频流，1-复合流
	BYTE byResolution;  // 分辨率 0-DCIF，1-CIF，2-QCIF，3-4CIF，4-2CIF，16-VGA，17-UXGA，18-SVGA，19-HD720p，20-XVGA，21-HD900p，22-SXGAp，27-1920*1080，28-2560*1920，29-1600*304，30-2048*1536，31-2448*2048，32-2448*1200，33-2448*800，34-XGA（1024*768），35-SXGA（1280*1024）,36-WD1(960*576/960*480),37-1080i 

	BYTE byBitrateType; // 码率类型 0-变码率 1-定码率
	BYTE byPicQuality;  //图片质量  0最好，1次好，2 较好，3 一般，4较差，5 差
	DWORD dwVideoBitrate; //码率0-保留，1-16K(保留)，2-32K，3-48k，4-64K，5-80K，6-96K，7-128K，8-160k，9-192K，10-224K，11-256K，12-320K，13-384K，14-448K，15-512K，16-640K，17-768K，18-896K，19-1024K，20-1280K，21-1536K，22-1792K，23-2048K，24-2560K，25-3072K，26-4096K，27-5120K，28-6144K，29-7168K，30-8192K。
	//最高位(31位)置成1表示是自定义码流，0～30位表示码流值，最小值16k

	DWORD dwVideoFrameRate; //帧率 0-全部，1-1/16，2-1/8，3-1/4，4-1/2，5-1，6-2，7-4，8-6，9-8，10-10，11-12，12-16，13-20，14-15，15-18，16－22 
	WORD  wIntervalFrameI; //I帧间隔 0xffff无效
	BYTE byIntervalBPFrame; // 帧格式 0 -BBP帧 1 BP帧 2-单P帧,0xff无效
	BYTE byRes1;            //预留 置0
	BYTE byVideoEncType; //视频编码类型 0-私有264，1-标准h264，2-标准mpeg4，7-M-JPEG，0xff-无效
	BYTE byAudioEncType; // //音频编码类型 0-OggVorbis；1-G711_U；2-G711_A；0xff-无效
	BYTE byRes[10];
}CAM_COMPRESS_INFO,*LPCAM_COMPRESS_INFO;

typedef struct _tagCAM_COMPRESS_CFG
{
	DWORD dwSize;
	CAM_COMPRESS_INFO stMainRecordParam; //主码流
	CAM_COMPRESS_INFO stSubRecordParam;  // 子码流
	CAM_COMPRESS_INFO stRes[2]; //预留置 0
}CAM_COMPRESS_CFG,*LPCAM_CAMPRESS_CFG;

typedef struct _tagSET_CAM_COMPRESS_CFG
{
	CAM_INFO stCamInfo;
	CAM_COMPRESS_CFG stCamCompressCfg;
}SET_CAM_COMPRESS_CFG,*LPSET_CAM_COMPRESS_CFG;

typedef struct _tagCAM_OSD_CFG
{
	DWORD dwSize; // 结构体大小
	DWORD dwShowOSD; //预览图像上是否显示OSD 0- 不显示，1显示
	WORD  wOSDTopLeftX; //OSD 的 x坐标
	WORD  wOSDTopLeftY; //OSD 的 y坐标

	BYTE  byOSDType; // OSD的类型 0－XXXX-XX-XX 年月日 1－XX-XX-XXXX 月日年 
	// 2－XXXX年XX月XX日  3－XX月XX日XXXX年  4－XX-XX-XXXX 日月年 5－XX日XX月XXXX年
	BYTE  byDispWeek; // 是否显示 星期 0-不显示，1显示
	BYTE  byOSDAttrib;// OSD 属性 1- 透明，闪烁 2 - 透明、不闪烁 3- 闪烁，不透明 4 不透明，不闪烁
	BYTE  byHourOSDType; //小时制 0 - 24 小时制 1- 表示12 小时 am/pm制
	BYTE  byRes[12]; //预留 置0                        

}CAM_OSD_CFG,*LPCAM_OSD_CFG;

typedef struct _tagSET_CAM_OSD_CFG
{
	CAM_INFO stCamInfo;
	CAM_OSD_CFG stCamOSDCfg;
}SET_CAM_OSD_CFG,*LPSET_CAM_OSD_CFG;


// IP 地址
typedef struct 
{		
	char	sIpV4[16];						/* IPv4地址 */
	BYTE	byRes[128];						/* 保留 */
}NET_IPADDR, *LPNET_IPADDR;

typedef struct _tagCAM_NET_CFG
{
	DWORD dwSize; // 结构体大小
	NET_IPADDR stCamIP; // 相机IP地址
	NET_IPADDR stMaskIP; // 子网掩码地址
	NET_IPADDR stGateIP;  // 网关地址
	NET_IPADDR stDNSIP;   //DNS 地址

	BYTE byMacAddr[MACADDR_LEN]; // 网卡
	WORD wDVRPort;               // 相机监听端口
	WORD wMTU;                   //MTU 大小
	DWORD dwNetInterface;        //1-10MBase-T；2-10MBase-T全双工；3-100MBase-TX
	//4-100M全双工；5-10M/100M/1000M自适应
	NET_IPADDR stDnsServer1IPAddr; //私有域名地址
	WORD wHttpPort; //http 监听端口
	BYTE byRes[32]; //预留
}CAM_NET_CFG,*LPCAM_NET_CFG;

typedef struct _tagSET_CAM_NET_CFG
{
	CAM_INFO stCamInfo;
	CAM_NET_CFG stCamNetCfg;
}SET_CAM_NET_CFG,*LPSET_CAM_NET_CFG; 

typedef struct _tagNET_VCA_RECT
{
	float fX;               //边界框左上角点的X轴坐标, 0.001~1
	float fY;               //边界框左上角点的Y轴坐标, 0.001~1
	float fWidth;           //边界框的宽度, 0.001~1
	float fHeight;          //边界框的高度, 0.001~1
}NET_VCA_RECT1, *LPNET_VCA_RECT1;

// 车牌参数设置
typedef struct _tagCAM_PLATE_CFG
{
	DWORD dwSize;
	BYTE  byDefaultCHN[MAX_CHJC_NUM]; // 默认省名
	BYTE  byTotalLaneNum; //识别的车道数
	BYTE  byPlateOSD; //0 - 不叠加车牌 1 叠加车牌彩色图片
	WORD  wDesignedPlateWidth; // 车牌设计宽度
	NET_VCA_RECT1 struLaneRect[MAX_LANERECT_NUM]; //识别区域
	BYTE  byRes[32]; //预留
}CAM_PLATE_CFG,*LPCAM_PLATE_CFG;

typedef struct _tagSET_CAM_PLATE_CFG
{
	CAM_INFO stCamInfo;
	CAM_PLATE_CFG stCamPlateCfg;
}SET_CAM_PLATE_CFG,*LPSET_CAM_PLATE_CFG;

typedef struct _tagFACE_RECT
{
	float fLeft;        //边界框左上角点的X轴坐标, 0.001~1        
	float fTop;         //边界框左上角点的Y轴坐标, 0.001~1
	float fWidth;       //边界框的宽度, 0.001~1
	float fHeight;     //边界框的高度, 0.001~1
}FACE_RECT,*LPFACE_RECT;

typedef struct _tagFACE_POINT
{
	float fX;  // x坐标
	float fY;  // y坐标
}FACE_POINT,*LPFACE_POINT;

typedef struct FACE_MASK_AREA
{
	UINT32 nVertexNum;
	FACE_POINT struFacePoint[MAX_VERTEX_NUM];
}FACE_MASK_AREA,*LP_FACE_MASK_AREA;

typedef struct _tagCAM_FACE_CFG
{
	DWORD dwSize;
	float fScoreLevel; // 评分阈值
	float fLikeLevel;   // 相似度阈值
	UINT32 nDetectAreaSence;  //检测区域灵敏度
	float fTargetGenSpeed;    // 目标生产速度

	UINT32    nDetectAreaNum;//检测区域个数
	FACE_POINT struDectAreaPt[MAX_VERTEX_NUM];//检测区域顶点数
	WORD       wFilter;  // 0 不启用尺寸过滤器，1 启用尺寸过滤器
	WORD       wFilterType;// 过滤器类型
	FACE_RECT  stMinTargetFrame; // 最小目标框
	FACE_RECT  stMaxTargetFrame;  //最大目标框
	UINT32     nMaskAreaNum; //屏蔽区域个数
	FACE_MASK_AREA struMaskArea[MAX_MASK_NUM]; // 屏蔽区域
	BYTE       byRes[32]; //预留 置0

}CAM_FACE_CFG,*LPCAM_FACE_CFG;

typedef struct _tagSET_CAM_FACE_CFG
{
	CAM_INFO stCamInfo;
	CAM_FACE_CFG stCamFaceCfg;
}SET_CAM_FACE_CFG,*LPSET_CAM_FACE_CFG;

#define MAXRANGE 30

// typedef struct WhiteBalance
// {
// 	int range[MAXRANGE];
// 	CString strRangeName[MAXRANGE];
// 	int iRGainMin;
// 	int iRGainMax;
// 	int iBGainMin;
// 	int iBGainMax;
// 	bool bWhiteBalance;
// 	WhiteBalance()
// 	{
// 		bWhiteBalance = false;
// 		memset(range, -1, sizeof(int) * MAXRANGE);
// 
// 		iRGainMin = 0;
// 		iRGainMax = 255;
// 		iBGainMin = 0;
// 		iBGainMax = 255;
// 
// 		strRangeName[0] = "手动白平衡";
// 		strRangeName[1] = "自动白平衡1";
// 		strRangeName[2] = "自动白平衡2";
// 		strRangeName[3] = "自动控制";
// 		//			strRangeName[0] = g_HikStrSrc.fGetString(IDS_CCDMANUALWHITEBALANCE);
// 		//			strRangeName[1] = g_HikStrSrc.fGetString(IDS_CCDAUTOBALANCE1);
// 		//			strRangeName[2] = g_HikStrSrc.fGetString(IDS_CCDAUTOBALANCE2);
// 		//			strRangeName[3] = g_HikStrSrc.fGetString(IDS_CCDAUTOCTRL);
// 	}
// 
// 	void InitWhiteBalance()
// 	{
// 		bWhiteBalance = false;
// 		memset(range, -1, sizeof(int) * MAXRANGE);
// 
// 		iRGainMin = 0;
// 		iRGainMax = 255;
// 		iBGainMin = 0;
// 		iBGainMax = 255;
// 
// 		strRangeName[0] = "手动白平衡";
// 		strRangeName[1] = "自动白平衡1";
// 		strRangeName[2] = "自动白平衡2";
// 		strRangeName[3] = "自动控制";
// 		//			strRangeName[0] = g_HikStrSrc.fGetString(IDS_CCDMANUALWHITEBALANCE);
// 		//			strRangeName[1] = g_HikStrSrc.fGetString(IDS_CCDAUTOBALANCE1);
// 		//			strRangeName[2] = g_HikStrSrc.fGetString(IDS_CCDAUTOBALANCE2);
// 		//			strRangeName[3] = g_HikStrSrc.fGetString(IDS_CCDAUTOCTRL);
// 	}
// }NET_IO_WHITEBALANCE;

typedef struct{  
	BYTE       byWhiteBalanceMode;
	BYTE       byWhiteBalanceModeRGain;
	BYTE       byWhiteBalanceModeBGain;
	BYTE       byRes[5];
}NET_IO_WHITEBALANCE, *LPNET_IO_WHITEBALANCE;

typedef struct _tagCAM_VEDIO_CFG
{
	DWORD dwSize;
	DWORD dwBright; //亮度
	DWORD dwContrast; // 对比度
	DWORD dwAcute;     // 锐度
	DWORD dwSaturation; // 饱和度

	//以下字段仅IPC有效
	BYTE byVedioGain; // 视频增益
	BYTE byVedioShutterSpeed; //视频快门速度
	BYTE byBlackPwl; // 黑电平补偿
	BYTE byJPEGQuality; // JPEG 图片质量
	BYTE byDynamicContrastEn; //动态对比度增强 0- 不增强 1 -增强
	BYTE byDynamicContrast; //动态对比度 0-100
	BYTE byCaptureGain; //前端抓拍增益
	BYTE byCaptureShutterSpeed; //前端抓拍速度

	NET_IO_WHITEBALANCE struWhiteBalance; //白平衡配置
	BYTE byRes[32];//预留
}CAM_VEDIO_CFG,*LPCAM_VEDIO_CFG;

typedef struct _tagSET_CAM_VEDIO_CFG
{
	CAM_INFO stCamInfo;
	CAM_VEDIO_CFG stCamVedioCfg;
}SET_CAM_VEDIO_CFG,*LPSET_CAM_VEDIO_CFG;

typedef struct _tagCAM_PLATE_BRT_CFG
{
	DWORD dwSize;
	BYTE  byPicEnable;  //是否启用车牌亮度补偿（默认启用）0-关闭，1-启用
	BYTE  byPlateExpectedBright; // 车牌预期亮度默认为50范围为0～100
	BYTE  byTradeOffFlash; ////是否考虑闪光灯影响 0关闭，1启用
	//使用闪光灯补光时，如果考虑减弱闪光灯的亮度增强效应，则需要//设为1，否则为0
	BYTE  byCorrectFactor;//纠正系数 默认为50
	BYTE  byRes[32]; //预留 置0 
}CAM_PLATE_BRT_CFG,*LPCAM_PLATE_BRT_CFG;

typedef struct _tagSET_CAM_PLATE_BRT_CFG
{
	CAM_INFO stCamInfo;
	CAM_PLATE_BRT_CFG stCamPlateBrtCfg;
}SET_CAM_PLATE_BRT_CFG,*LPSET_CAM_PLATE_BRT_CFG;

typedef struct _tagCAM_ABILITY_CFG
{
	DWORD dwSize;
	BYTE  byPlateEnable; //是否支持车牌识别 0-不支持，1- 支持
	BYTE  byFrameFlip;  //图像是否翻转 0-不翻转 1-翻转
	WORD  wFlipAngle;   // 图片翻转角度 0，90，180，270
	WORD  wLightPhase;    //相位 取值范围(0,360)
	BYTE  byLightSyncPower; // 是否信号灯电源同步 0-不同步 1 同步
	BYTE  byFrequency;  //信号频率
	BYTE  byUploadSDEnable;  //是否自动上传SD图片 0否 1是
	//--------以上字段仅IPC有效，以下字段仅模拟相机有效

	BYTE  byPlateMode;       //识别模式参数(仅Ids-65xx支持)0-视频触发 1外部触发
	BYTE  byBlackLightMode;  // 背光补偿模式 0-关闭背光补偿 1-开启背光补偿 2-开启强光抑制
	BYTE  byColorBlackMode;  // 彩转黑模式 0-自动彩转黑 1-黑白模式，2彩色模式
	BYTE  byApertureMode;    // 光圈模式，0 电子光圈 1自动光圈
	BYTE  byShutterValue;    //快门值 取值范围[0,12]
	BYTE byRes[30]; //预留 置0
}CAM_ABILITY_CFG,*LPCAM_ABILITY_CFG;

typedef struct _tagSET_CAM_ABILITY_CFG
{
	CAM_INFO stCamInfo;
	CAM_ABILITY_CFG stCamAbilityCfg;
}SET_CAM_ABILITY_CFG,*LPSET_CAM_ABILITY_CFG;

typedef struct _tagCAM_IMAGEOVERLAY_CFG
{
	DWORD dwSize;
	BYTE  byOverlayInfo; //叠加使能开关 0-不叠加，1-叠加
	BYTE  byOverlayMonitorInfo; //是否叠加监控点信息 0-不叠加，1-叠加
	BYTE  byOverlayTime;   // 是否叠加时间 0-不叠加，1-叠加
	BYTE  byOverSpeed;  ////是否叠加速度 0-不叠加，1-叠加
	BYTE  byOverlaySpeeding; // 是否叠加超速比例 0-不叠加，1-叠加
	BYTE  byOverlayLimitFlag; //是否叠加限速标记 0-不叠加，1-叠加
	BYTE  byOverlayPlate;     //是否叠加车牌 0-不叠加，1-叠加
	BYTE  byOverlayColor;      //是否叠加车身颜色 0-不叠加，1-叠加
	BYTE  byOverlayLength;     // 是否叠加车长 0-不叠加，1-叠加
	BYTE  byOverlayType;       // 是否叠加车型 0- 不叠加，1-叠加
	BYTE  byOverlayColorDepth; //是否叠加车身颜色深浅 0-不叠加，1-叠加
	BYTE  byOverlayDriveChan;   //是否叠加车道 0-不叠加 1-叠加
	BYTE  byOverlayMillSec;     //是否叠加毫秒信息 0-不叠加，1-叠加
	BYTE  byOverlayIllegalInfo;  //是否叠加违章信息 0-不叠加 1-叠加
	BYTE  byOverlayRedOnTime;    // 是否叠加红灯已亮时间 0-不叠加，1-叠加
	BYTE  byRes1;                //预留 置 0
	BYTE  byMonitor1;            //监控点1
	BYTE  byMonitor2;            //监控点2
	BYTE  byRes2[34]; //预留置 0

}CAM_IMAGEOVERLAY_CFG,*LPCAM_IMAGEOVERLAY_CFG;

typedef struct _tagSET_CAM_IMAGEOVERLAY_CFG
{
	CAM_INFO stCamInfo;
	CAM_IMAGEOVERLAY_CFG stCamImageOverlayCfg;
}SET_CAM_IMAGEOVERLAY_CFG,*LPSET_CAM_IMAGEOVERLAY_CFG;


typedef struct _tagCAM_FLASH_CFG
{
	DWORD dwSize;
	BYTE  byCamAddr; //摄像机485地址,0xFF表示无效
	BYTE  byFlashAddr; //闪光灯控制小板 485地址，0xFF表示无效
	BYTE  byRes[2]; //预留 置0
	float fPhase; // 闪光灯控制小板 相位取值范围
	float fDutyRate;//
	UINT32 nFreqValue;  //闪光灯控制小板 频率参数 取值 0-4
	BYTE   byMode; //闪光灯闪烁模式 0- 不闪，1--闪，2-- 关联闪，3-轮闪
	BYTE   byRelatedIoIn; //闪光灯关联输入IO号，关联闪时，此参数有效，保留
	BYTE   byRecognizedLane; // 关联的IO号 按位表示,bit0 表示io1 是否关联 0-不关联，1关联
	BYTE   byDetectBrightness; // 自动检测亮度使能闪光灯 0- 不检测 1- 检测
	BYTE   byBrightnessThreld;// 使能闪光灯亮度阈值，范围 [0-100],高于阈值闪
	BYTE   byStartHour; //开始时间 单位小时
	BYTE   byStartMinute; //开始时间单位分 取值范围[0,59]
	BYTE   byEndHour;    // 结束时间 单位小时，取值范围[0，23]
	BYTE   byEndMinute; // 结束时间 单位分，取值范围[0,59]
	BYTE   byFlashLightEnable; //闪光灯是否开启 0 -关，1 - 开
	BYTE   byRes2[6];//预留置 0

}CAM_FLASH_CFG,*LPCAM_FLASH_CFG;

typedef struct _tagSET_CAM_FLASH_CFG
{
	CAM_INFO stCamInfo;
	CAM_FLASH_CFG stCamFlashCfg;
}SET_CAM_FLASH_CFG,*LPSET_CAM_FLASH_CFG;

typedef struct _tagCAM_IO_IN_CFG
{
	DWORD dwSize; 
	BYTE  byIoInStatus; // 输入的IO口状态 0-下降沿，1上升沿，2- 上升沿和下降沿，3-高电平，4-低电平
	BYTE  byRes[7];//预留置 0
}CAM_IO_IN_CFG,*LPCAM_IO_IN_CFG;

typedef struct _tagSET_CAM_IO_IN_CFG
{
	CAM_INFO stCamInfo;
	CAM_IO_IN_CFG stCamIoInCfg;
}SET_CAM_IO_IN_CFG,*LPSET_CAM_IO_IN_CFG;

typedef struct _tagCAM_IO_OUT_CFG
{
	DWORD dwSize;
	BYTE  byDefaultStatus; // IO 默认状态 0-低电平 1- 高电平
	BYTE  byIoOutStatus;  // IO起效时状态 0-低电平 1- 高电平，2- 脉冲
	WORD  wAheadTime;     // 输出IO提前时间 单位us,取值范围 [0,300]
	DWORD dwTimePluse;    // 脉冲间隔时间 单位 us
	DWORD dwTimeDelay;    // IO有效持续时间 单位us
	BYTE  byFreqMulti;    // 倍频，取值范围[1,15] 
	BYTE  byDutyRate;     // 占空比 单位%[0,40]
	BYTE  byRes[6]; //预留 置0
}CAM_IO_OUT_CFG,LPCAM_IO_OUT_CFG;

typedef struct _tagSET_IO_OUT_CFG
{
	CAM_INFO stCamInfo;
	CAM_IO_OUT_CFG stCamIoOutCfg;  
}SET_IO_OUT_CFG,*LPSET_IO_OUT_CFG;

typedef struct _tagCAM_IO_SNAP_CFG
{
	DWORD dwSize;
	BYTE  byRelatedDriveWay; // 触发IO关联的车道号 取值范围 [0-9]
	BYTE  bySnapTimes; // 线圈抓拍次数 0-不抓拍 非0 为抓拍次数 最大为5次
	WORD wSnapWaitTime;   //抓拍等待时间 单位ms,取值范围 [0,6000]
	WORD wIntervalTime[MAX_INTERVAL_NUM]; // 抓拍间隔时间 单位ms,取值范围[67,60000]
	BYTE byRes[24]; //预留置0
}CAM_IO_SNAP_CFG,*LPCAM_IO_SNAP_CFG;

typedef struct _tagSET_CAM_IO_SNAP_CFG
{
	CAM_INFO stCamInfo;
	CAM_IO_SNAP_CFG stCamIoSnapCfg;

}SET_CAM_IO_SNAP_CFG,*LPSET_CAM_IO_SNAP_CFG;

#endif

// 配置相机参数的回调
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
#if defined(STATIC_LOAD_IO_NVR)
typedef int (CALLBACK *pCfgDataCallBack)(int iChannel, int iCammandCode,char *recvbuf,char *sendbuf,int *iLen,int userdata,int param1,int param2);

#ifdef __cplusplus
extern "C"
{
#endif
	int NET_IO_NVR_API NET_IO_Initialize();
	int NET_IO_NVR_API NET_IO_Finalize();

	int NET_IO_NVR_API NET_IO_StartService(UINT Port);
	int NET_IO_NVR_API NET_IO_StopService(void);
	
	int NET_IO_NVR_API NET_IO_PutStream(int Channel, bool bIsMasterStream, char* Stream, int StreamLength, int FrameType);
	
    int NET_IO_NVR_API NET_IO_SetCallBackCfgData(pCfgDataCallBack pDataFunc,int userdata);

	int NET_IO_NVR_API NET_IO_SetDeviceBuffer(int* DeviceType);

#ifdef __cplusplus
}
#endif

#endif




