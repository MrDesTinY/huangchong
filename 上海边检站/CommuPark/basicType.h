#pragma  once

#include <winsock2.h>
#include <stdio.h>
#include <TCHAR.H>
#include <vector>
#include <map>

using namespace std;

#include <windows.h>

typedef unsigned __int64 UINT64;
typedef unsigned int     UINT32;
typedef unsigned short   UINT16;
typedef unsigned char    UINT8;
typedef short            INT16;
typedef signed char      INT8;

#define MAXNAMELEN     260
#define MAXPATHLEN     260

#define RECVTIMEOUT    1*1000*60 
#define SENDTIMEOUT    2*1000*60

#define MINPACKETLEN       120
#define UCIPACKETLEN       112

#define MAX_PLATE_JPEG_SIZE  10*1024  //车牌图片大小上限
#define MAX_CAR_JPEG_SIZE    600 *1024  // 车辆图片大小上限

#define PLATE_JPEG_SIZE_ADDR 120  //车牌JPEG在通讯协议的起始位置

#define MAX_PLATE_LEN        16
#define MAX_CARD_LEN         30 
#define DATA_LISTEN_PORT    8682  //数据监听端口
#define CMD_LISTEN_PORT     8680  //指令监听端口

// 停车场通讯协议上传数据类型
typedef struct _tagCMR_TrafficData
{
    UINT16  nVehicleNo;                        // 过车序号   
    UINT16  nLaneId;                           // 车道号
    char    chPlateNo[MAX_PLATE_LEN];     // 车牌
    UINT16  nPlateConfidence;                  // 整牌置信度
    UINT8   nRecognitionCharacterNo;           // 识别字符个数
    UINT8   nReserved_1;                       // 预留
    INT16   nRecognitionCharacterConfiendce[10];  //识别字符置信度
    UINT8   nVehicleColor;                        // 车身颜色
    UINT8   nVehicleColorDeep;                    // 车身颜色深浅
    UINT16  nReserved_2;                          // 预留
    char    nPassTime[8];                         // 过车时间
    UINT16  nTypeIO;                              // 出入类别 
    UINT16  nTypeVehicle;                         // 车辆类别
    UINT32  nTypePark;                            // 停车类别
    char    chCardNo[MAX_CARD_LEN];           // 卡号
    UINT16  nReserved_3;                          // 预留
    UINT32  nDoorIndex;                           // 出入口编号
    UINT32  nOutMode;                             // 放行类别
    UINT32  nOpPersonIndex;                       // 操作人员编号
    char    nStopTime[8];                         // 停车时间
    UINT8   nReserved_4;                          // 预留
    UINT8   nTypeAlarm;                           // 报警类别
    UINT8   nRecChanNo;                           // 报警录像通道
    UINT8   nReserved_5;                          // 预留

    UINT32  nPlateJpegSize;                       // 车牌图片大小
    UINT8   *pnPlateJpegData;                     // 车牌图片数据
    UINT32  nCarJpegSize;                         // 车辆图片大小
    UINT8   *pnCarJpegData;                       // 车辆图片数据
    UINT32  nCarJpeg1Size;                       //  车辆图片1大小
    UINT8   *pnCarJpeg1Data;                     // 车辆图片1数据

    _tagCMR_TrafficData()
    {
        nOpPersonIndex = 0;
        chPlateNo[0] = '\0';
        chCardNo[0] = '\0';
        pnPlateJpegData = NULL;
        pnCarJpegData = NULL;
        pnCarJpeg1Data = NULL;

    }

}CMR_TrafficData,*pCMR_TrafficData;


//终端服务器联检车辆信息上传
typedef struct _tagUCI_TrafficData{
	char chPlateNo[MAX_PLATE_LEN];     // 车牌号码
	char chCarType[20];						//车辆类型
	char chCarColor[12];						//车身颜色
	char chOwnerName[32];					//车主姓名
	char chCardNum[30];						//卡号
	char chReserved[2];						//预留
	_tagUCI_TrafficData()
	{
		chPlateNo[0]='\0';
		chCarType[0]='\0';
		chCarColor[0]='\0';
		chOwnerName[0]='\0';
		chCardNum[0]='\0';
		chReserved[0]='\0';
	}
}UCI_TrafficData, *pUCI_TrafficData;


#define COMU_REQUEST	1
#define COMU_RESPONSE	2

const char SynInfo[] = {'G','D','W','S','I','C'};
#define SYN_HEADER_SIZE     (sizeof(SynInfo))

#define MAX_PLATE_NUM_SIZE 16
#define BINARY_IMG_SIZE    280
#define MAX_PLATE_JPEG_SIZE 10*1024
#define MAX_VEHICLE_JPEG_SIZE 600*1024
#define HR_IMG_WIDTH        1600
#define HR_IMG_HEIGHT       1200
#define HF_IMG_WIDTH        1600
#define HF_IMG_HEIGHT       300



#define MAX_BUFFER_SIZE 600*1024
#define LISTENPORT 8682

#define TYPE_IN    0
#define TYPE_OUT   1

//通讯头
typedef struct _tagGDW_PacketHead
    {
    unsigned int dwDataLen;			//数据体长度
    unsigned short wDataType;		//数据类型
    unsigned short wCheckSum;		//校验和
    unsigned short wCommandType;	//命令类型
    unsigned short wCommand;		//命令
    unsigned short wError;			//错误码
    unsigned short wReserved;		//保留位
    //默认构造函数
    _tagGDW_PacketHead(void)
        :dwDataLen(0)
        ,wDataType(0)
        ,wCheckSum(0)
        ,wCommandType(0)
        ,wCommand(0)
        ,wError(0)
        ,wReserved(0)
        {
        //空
        }
    }GDW_PacketHead,*pGDW_PacketHead;


unsigned short CheckSum(unsigned short *pBuffer, UINT32 nSize);
