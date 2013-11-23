#ifndef __VPRCTRLINTERFACE__
#define __VPRCTRLINTERFACE__

//数据结构定义
//typedef unsigned char UINT8;
//typedef unsigned int UINT32;
//typedef unsigned int UINT;
//#define WINAPI      __stdcall
//typedef void *HANDLE;
//#define EXPORT
//typedef unsigned char BYTE;
//typedef long BOOL;
//typedef unsigned long ULONG;
//typedef long LONG;
//typedef unsigned short USHORT;
//typedef void *HINSTANCE;
typedef struct tagVedioPara
{
	int Brightness;//亮度(0-255)
	int Contrast;//对比度(0-127)
	int Saturation; //饱和度(0-127)
	int Hue;//色调(0-255)
}VEDIO_PARA,*PVEDIO_PARA;

typedef struct _GDW_IMAGE_INFO_
{
	UINT8	IsSendPic; 				//图片是否发送    0-不发送 1-发送
	UINT8	JpegQuality;	     	//Jpeg压缩质量   1/2/3  1为压缩效果最差
	UINT8	IsAddPlate;
	UINT8	res;
}GDW_IMAGE_INFO,*pGDW_IMAGE_INFO;

typedef struct tagVehiclePara
{
	UINT8 CaptureCmd;   			//抓拍     0-线圈触发抓拍 1-视频触发抓拍 2-手动触发抓拍 3-过图数据测试
	UINT8 byteUseLED;        		//使用LED控制  0-不使用1-使用LED, 2-使用闪光灯
	UINT8 IsSendBinImage;  	//车牌二值图   0-不发送 1-发送
	UINT8 NearAddLiscence;  //近景叠加车牌 号 0不叠加 1叠加
	UINT8 NearAddTime;	    //近景图中叠加时间
	UINT8 res[3];
	UINT32 CameraType;	  			//相机类型
	UINT8 def_name[4];   				//默认省名
	GDW_IMAGE_INFO	plateJpegInfo; 		//彩色车牌图信息
	GDW_IMAGE_INFO	nearJpegInfo;  		//近景JPEG图信息	
	UINT  	platePosX;		//车牌叠加X坐标
	UINT  	platePosY;		//车牌叠加Y坐标
	UINT  liscencePosX;		//车牌叠加X坐标（OSD)
	UINT  liscencePosY;		//车牌叠加Y坐标（OSD)
	UINT  timePosX;			//字符叠加X坐标（OSD)
	UINT  timePosY;			//车牌叠加Y坐标（OSD)
} Veicle_Para,*pVeicle_Para;

enum EquType
{
	HD976=0,
	DVS=1,
	NVR=2
};
typedef enum {
   StandardNone                   = 0x80000000,
   StandardNTSC                   = 0x00000001,
   StandardPAL                    = 0x00000002,
   StandardSECAM                  = 0x00000004,
} VideoStandard_t;  
typedef void (*TypeSadpStart)(char*,char*,	
										  char* ,	
										  char* ,	
										  char* ,	
										  unsigned int ,
										  unsigned int ,
										  unsigned int ,
										  unsigned int ,
										  char*,
										  char*, 
										  char*,
										  int);
/*****************************************************************
函数名: VPR_Init
功能:   系统初始化
参数:   EquType i_equType;设备类型
返回值:
TRUE      成功
FALSE      失败
备注:
******************************************************************/
extern "C" bool WINAPI  EXPORT VPR_Init(EquType i_equType);
/*****************************************************************
函数名: VPR_CameraInit
功能:   连接相机
参数:
EquType i_equType;设备类型
io_uID        <0:系统给相机分配的编号。2:通道号>
o_handle  <2:返回通道句柄>
i_pchPort   <0:输入格式如下:127.0.0.1:8000<admin,12345>>
i_hWndHandle  接收消息的窗体句柄<0,2>
i_uMsg        用户自定义消息，当hWndHandle不为NULL时，
检测到有新的车牌识别结果并准备好当前车牌缓冲区信息后，
用::PostMessage 给窗口hWndHandle发送uMsg消息，其中WPARAM参数为uID，
LPARAM参数为0.<0,2>
返回值:
TRUE        连接相机成功，并保存uID编号
FALSE       连接相机失败
备注:
检测有新车牌信息后，须先准备好车牌缓冲区，然后发消息；
******************************************************************/
extern "C" bool WINAPI  EXPORT VPR_CameraInit(EquType i_equType,
											  int& io_uID,
											  HANDLE &o_handle,
											  char * i_pchPort,
											  HWND i_hWndHandle,
											  UINT i_uMsg
											  );
/*****************************************************************
函数名: VPR_CameraQuit
功能:   断开与指定相机的连接或退出指定的通道
参数:	
EquType i_equType;设备类型
i_uID         <0:相机编号.2:通道句柄>
返回值:
TRUE        关闭相机成功
FALSE       关闭相机失败
备注:必须先停止预览才能执行此函数
******************************************************************/
extern "C" bool WINAPI  EXPORT VPR_CameraQuit(EquType i_equType,int *i_uID);

/*****************************************************************
函数名: VPR_Quit
功能:  系统关闭退出
参数:	EquType i_equType;设备类型
返回值:
TRUE        关闭成功
FALSE       关闭失败
备注:
******************************************************************/
extern "C" bool WINAPI  EXPORT VPR_Quit(EquType i_equType);

/************************************************************************************
函数名： VPR_SetCameraVideoPara
功能：设置车牌识别参数
参数：
EquType i_equType;设备类型
int i_uID；<0:相机编号.2.通道号>
i_VehiclePara 车牌识别参数配置<2>
返回值：成功TRUE；失败FALSE
备注：
**************************************************************************************/
//extern "C" bool WINAPI  VPR_SetCameraPRPara(EquType i_equType,int i_uID,Vehicle_Para i_VehiclePara)
extern "C" bool WINAPI  VPR_SetCameraPRPara(EquType i_equType,int i_uID);
/*****************************************************************
函数名: VPR_CameraCapture
功能:   以非阻塞方式向指定的相机发送抓拍图像并识别车牌指令
参数:
EquType i_equType;设备类型
i_uID         <0:相机编号.2:通道号>
返回值:
TRUE        已接收命令
FALSE       其它故障(如通讯故障)
备注:   该函数为非阻塞式函数，需立即返回
******************************************************************/
extern "C" bool WINAPI  EXPORT VPR_CameraCapture(EquType i_equType,int i_uID);
/*****************************************************************
函数名:  VPR_GetVehicleInfo
功能:    获取当前车牌缓冲区中的车牌号、车牌二值化图片以及车辆抓拍图像
参数:	
EquType i_equType;设备类型
i_uID               <0:相机编号2:通道号（用来判断该设备是否已经启用）>
o_pchPlate          返回的车牌号(缓冲区长度≥32个字节)<0,2>
o_piByteBinImagLen  返回的车牌二值化图片的字节长度<0,2>
o_pbByteBinImage    返回的车牌二值化图片数据(缓冲区≥280个字节)<0,2>
o_piJpegImageLen    返回的车辆抓拍图片大小<0,2>
o_pbByteJpegImage   返回的车辆图片数据，JPEG/24bit格式(缓冲区≥200K个字节)<0,2>
o_pVehicleWayNo     返回车道号<0,2>
o_pVehicleColor     返回的车牌颜色<0,2>
返回值:
TRUE              成功
FALSE             其它故障(如通讯故障)
备注:    无论成功与否均不清除车牌识别仪当前缓冲区中的信息
******************************************************************/
extern "C" bool WINAPI  EXPORT VPR_GetVehicleInfo(EquType i_equType,
												  int i_uID,
												  char * o_pchPlate,
												  int * o_piByteBinImagLen,
												  BYTE * o_pbByteBinImage,
												  int * o_piJpegImageLen,
												  BYTE *o_pbByteJpegImage,
												  char *o_pVehicleWayNo,
												  char *o_pPlateColor);
/*****************************************************************
函数名:  VPR_CheckCameraStatus
功能:    检测指定相机的状态
参数:	 
EquType i_equType;设备类型
i_uID              相机编号<0>
o_pchVprDevStatus 返回以'\0'结尾的状态说明字符串(缓冲区长度32个字节)<0>
返回值:
0	正常
1	故障
备注:
******************************************************************/
extern "C" bool WINAPI VPR_CheckCameraStatus(EquType i_equType,int i_uID,char *o_pchVprDevStatus);
/*****************************************************************
函数名:  VPR_StartCameraVideoPreview
功能:    开启视频预览
参数:	EquType i_equType;设备类型
int *i_uID；<0:相机编号2:通道号>
HWND i_WndHandle；显示窗口句柄<0,2>
RECT *i_rect；显示窗口内的矩形区域<2>
BOOLEAN i_bOverlay；是否启用Overlay预览模式*<2>
int i_VideoFormat；视频预览格式（目前无效）<2>
int i_FrameRate；视频预览帧率（PAL：1-25，NTSC：1-30）<2>
返回值:
TRUE      成功
FALSE      失败
备注:
******************************************************************/
extern "C" bool WINAPI VPR_StartCameraVideoPreview(EquType i_equType,int *i_uID,HWND i_WndHandle,
												   RECT *i_rect,BOOLEAN i_bOverlay, int i_VideoFormat, int i_FrameRate);

/*****************************************************************
函数名： VPR_StopCameraVideoPreview
功能：停止预览
参数：
EquType i_equType;设备类型
int i_uID；<0:相机编号2 通道号>
返回值：成功返回0；失败返回错误号
备注：
******************************************************************/
extern "C" bool WINAPI  VPR_StopCameraVideoPreview(EquType i_equType,int *i_uID);

/************************************************************************************
函数名： VPR_GetCameraVideoPara
功能：获取视频参数
参数：EquType i_equType;设备类型,
*i_uID；相机编号
i_VideoStandard视频制式,
o_VedioPara视频参数
返回值：成功返回0；失败返回错误号
备注：
**************************************************************************************/
extern "C" int WINAPI VPR_GetCameraVideoPara(EquType i_equType,int *i_uID, VideoStandard_t *i_VideoStandard,PVEDIO_PARA o_VedioPara);

/************************************************************************************
函数名： VPR_SetCameraVideoPara
功能：设置视频参数
参数：
EquType i_equType;设备类型
*i_uID；相机编号
i_VedioPara视频参数
返回值：成功返回0；失败返回错误号
备注：
**************************************************************************************/
extern "C" int WINAPI  VPR_SetCameraVideoPara(EquType i_equType,int *i_uID,VEDIO_PARA i_VedioPara);

/************************************************************************************
函数名： VPR_StartCameraVideoCapture
功能：启动通道编码数据流捕获
参数：EquType i_equType;设备类型
UINT ChannelType;通道类型（1、主通道2、子通道）
int i_uID；相机编号
返回值：成功返回0；失败返回错误号
备注：
**************************************************************************************/
extern "C" int WINAPI VPR_StartCameraVideoCapture(EquType i_equType,UINT i_ChannelType,int i_uID);

/************************************************************************************
函数名： VPR_StopCameraVideoCapture
功能：停止通道编码数据流捕获
参数：EquType i_equType;设备类型
UINT i_ChannelType;通道类型（1、主通道2、子通道）
int i_uID；相机编号
返回值：成功返回0；失败返回错误号
备注：
**************************************************************************************/
extern "C" int WINAPI VPR_StopCameraVideoCapture(EquType i_equType,UINT i_ChannelType,int i_uID);
/************************************************************************************
函数名： VPR_SetCameraOsd
功能：设置OSD显示，将当前的系统时间年月日星期时分秒等信息叠加在视频之上，并可作透明处理。
参数：EquType i_equType;设备类型
int i_uID；相机编号
BOOL i_Enable；OSD是否显示
返回值：成功返回0；失败返回错误号
备注：
**************************************************************************************/
extern "C" int WINAPI  VPR_SetCameraOsd(EquType i_equType,int i_uID, BOOL i_Enable);
/************************************************************************************
函数名： VPR_SetCameraOsdDisplayModeEx
功能：设置OSD显示模式
参数：int i_uID；相机编号
int i_Brightness；OSD显示亮度。0最暗，255最亮
BOOL i_Translucent；OSD图像是否做半透明处理
int i_param；Bit0：是否自动进行颜色翻转 Bit16-23垂直放大倍数 Bit24-31水平 放大倍数
int i_nLineCount；OSD显示的行数，最多为8行
USHORT **i_Format；多行字符显示
返回值：成功返回0；失败返回错误号
备注：USHORT **Format；多行字符显示，描述字符叠加的位置和次序的格式串，
其中每一行的第一元素X和第二元素Y 是该字串在标准4CIF图象的起始位置，X必须是16的倍数，Y可以在图象高度内取值即（0-575）PAL 、
（0-479）NTSC；可以是ASCII码也可以是汉字，当想要显示当前时间时，可以指定为固定的时间定义值，其值如下：
_OSD_YEAR4 四位的年显示，如2004
_OSD_YEAR2 两位的年显示，如02
_OSD_MONTH3 英文的月显示，如 Jan
_OSD_MONTH2 两位阿拉伯数字的月显示，如07
_OSD_DAY 两位的阿拉伯数字的日显示，如31
_OSD_WEEK3 英文的星期显示，如Tue
_OSD_CWEEK1 中文的星期显示，如星期二
_OSD_HOUR24 24小时的时钟显示，如18
_OSD_HOUR12 12小时的时钟显示，如AM09或PM09
_OSD_MINUTE 两位分钟的显示
_OSD_SECOND 两位秒的显示
在格式字符串的每一行最后一个元素必须以NULL（0）结尾，否则会显示错误的内容。
SetOsdDisplayModeEx函数支持最多8行OSD字符串的显示。
**************************************************************************************/
extern "C" int WINAPI  VPR_SetCameraOsdDisplayModeEx(EquType i_equType,int i_uID,int i_color,
													 BOOL i_Translucent,int i_param,int i_nLineCount,USHORT **i_Format);
/************************************************************************************
函数名： VPR_SetupCameraBitrateControl
功能：设置编码的最大比特率。
参数：EquType i_equType;设备类型
*i_uID；<0:相机编号2:通道句柄>
ULONG i_Maxbps；最大比特率。取值：10000以上<0,2>
返回值：成功返回0；失败返回错误号
备注：在NVR中：设置为0时码流控制无效，设置为某一最大比特率时，当编码码流超过该值时，DSP会自动调整编码参数来保证不超过最大比特率，当编码码流低于最大比特率时，DSP不进行干涉。调整误差<10%
**************************************************************************************/
extern "C" int WINAPI  VPR_SetupCameraBitrateControl(EquType i_equType,int *i_uID, ULONG i_MaxBps);

/************************************************************************************
函数名： VPR_SADP_Start
功能：SADP启动
参数：
返回值：TRUE   成功
FALSE      失败
备注：
**************************************************************************************/
extern "C" bool WINAPI  VPR_SADP_Start(TypeSadpStart pfun);
/************************************************************************************
函数名： VPR_GetCameraInfo
功能：发送搜索设备命令
参数:
返回值：TRUE   成功
FALSE      失败
备注：
**************************************************************************************/
extern "C" bool WINAPI  VPR_SADP_SendInquiry();
/************************************************************************************
函数名： VPR_SADP_SendUpdateIp
功能：设置设备的IP地址和端口号
参数：i_desDLC 设备物理地址
i_desIP 设备IP地址或要修改的IP地址
i_subnetmask IP地址子网掩码
i_passwd 设备的密码
i_port 设备端口号或要修改的端口号
返回值：TRUE      成功
FALSE      失败
备注：
**************************************************************************************/
extern "C" bool WINAPI  VPR_SADP_SendUpdateIp(char *i_desDLC,
									  char *i_desIP,
									  char *i_subnetmask,
									  char *i_passwd,
									  char *i_port
									  );
/************************************************************************************
函数名：  VPR_SADP_Stop()
功能：停止SADP
参数：
返回值：TRUE      成功
FALSE      失败
备注：
**************************************************************************************/
extern "C" bool WINAPI  VPR_SADP_Stop();



/*****************************************************************
函数名: DEV_SyncTime
功能:   时间同步
参数:   EquType i_equType;设备类型
返回值:
TRUE      成功
FALSE      失败
备注:
******************************************************************/
extern "C" bool WINAPI  EXPORT DEV_SyncTime(EquType i_equType,LONG lUserID);

#endif