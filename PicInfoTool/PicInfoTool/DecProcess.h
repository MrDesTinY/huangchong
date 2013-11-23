#ifndef DEC_PROCESS_
#define DEC_PROCESS_



#include "stdafx.h"

#include "dec_windows/jpegdec.h"
#include "enc_windows/ejpeglib.h"

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <malloc.h>

#define  FIRST_VERSION		1
//#define	SECOND_VERSION	2
//#define	THIRD_VERSION    3

//时间结构体
typedef struct _tagJD_TIME
{
	short wYear;
	short wMonth;
	short wDayOfWeek;//星期几
	short wDay;
	short wHour;
	short wMinute;
	short wSecond;
	short wMillisecond;
	_tagJD_TIME()
	{
		wYear = 0;
		wMonth = 0;
		wDayOfWeek = 0;
		wDay = 0;
		wHour = 0;
		wMinute = 0;
		wSecond = 0;
		wMillisecond = 0;
	}
}JD_TIME,*pJD_TIME;
//点结构体
typedef struct _tagJD_POINT
{
	short x;
	short y;
	_tagJD_POINT()
	{
		x = 0;
		y = 0;
	}
}JD_POINT,*pJD_POINT;
//线结构体
typedef struct _tagJD_LINE
{
	JD_POINT Point1;
	JD_POINT Point2;
}JD_LINE,pJD_LINE;
//矩形结构体
typedef struct _tagJD_RECT
{
	short left;
	short top;
	short right;
	short bottom;
	_tagJD_RECT()
	{
		left = 0;
		top = 0;
		right = 0;
		bottom = 0;
	}
}JD_RECT,*pJD_RECT;
//信息头结构体
typedef struct _tagJD_INFO_HEADER
{
	char    m_Flag[8];
	UINT    m_nVersion;
	UINT    m_nSize;
	int    m_LPTOffset;
	int    m_LPTSize;
	int    m_MprOffset;
	int    m_MprSize;
	int    m_PROffset;
	int    m_PRSize;
	int    m_PSOffset;
	int    m_PSSize;
	int    m_BvtOffset;
	int    m_BvtSize;
	int    m_TVDOffset;
	int    m_TVDSize;
	int    m_TLDOffset;
	int    m_TLDSize;
	BYTE m_Res[1000];
	_tagJD_INFO_HEADER()
	{
		memset(m_Flag,0,8);
		m_nVersion = 0;
		m_nSize = 0;
		m_LPTOffset = 0;
		m_LPTSize = 0;
		m_MprOffset = 0;
		m_MprSize = 0;
		m_PROffset = 0;
		m_PRSize = 0;
		m_PSOffset = 0;
		m_PSSize = 0;
		m_BvtOffset = 0;
		m_BvtSize = 0;
		m_TVDOffset = 0;
		m_TVDSize = 0;
		m_TLDOffset = 0;
		m_TLDSize = 0;
		memset(m_Res,0,1000);
	}
}JD_INFO_HEADER,*pJD_INFO_HEADER;
//抓拍结构体
typedef struct _tagJD_SNAP_INFO
{
	UINT    m_MergMode;
	JD_TIME  m_SnapTime;
	char   m_szSnapAddr[40];
	UINT    m_nSnapType;
	UINT     m_nCarSpeed;
	UINT     m_nSpeedLimited;
	UINT     m_nLaneID;
	char    m_cPlateNum[32];
	UINT     m_nCarColor;
	UINT     m_nCarID;
	UINT     m_nLPTState[5];//
	JD_RECT m_PlateRect[5];//
	UINT     m_nFrameNum[5];//
	BYTE  m_Res[1000];
	_tagJD_SNAP_INFO()
	{
		m_MergMode = 0;
		memset(m_szSnapAddr,0,40);
		m_nSnapType = 0;
		m_nCarSpeed = 0;
		m_nSpeedLimited = 0;
		m_nLaneID = 0;
		memset(m_cPlateNum,0,32);
		m_nCarColor = 0;
		m_nCarID = 0;
		m_nLPTState[0] = 0;
		m_nLPTState[1] = 0;
		m_nLPTState[2] = 0;
		m_nLPTState[3] = 0;
		m_nLPTState[4] = 0;
		m_nFrameNum[0] = 0;
		m_nFrameNum[1] = 0;
		m_nFrameNum[2] = 0;
		m_nFrameNum[3] = 0;
		m_nFrameNum[4] = 0;
		memset(m_Res,0,1000);
	}

}JD_SNAP_INFO,*pJD_SNAP_INFO;
//初始化结构体
typedef struct _tagJD_PARAM_INFO
{
	BYTE		m_Version[20][4];
	UINT		m_LaneNum;
	UINT		m_LaneCls[6];//车道属性
	UINT		m_LightNum;
	JD_RECT		m_LightRect[16];//信号灯区域
	BYTE		m_LightCls[16];//信号灯方向属性
	BYTE		m_LightColor[16];//灯颜色
	WORD		m_SubLight[16];//每个信号灯的子灯数目
	JD_LINE		m_LaneLine[7];
	JD_LINE		m_StopLine[6];
	JD_LINE		m_TrigleLine[6];
	JD_LINE		m_LeftTurnLine;
	JD_LINE		m_RightTurnLine;
	JD_LINE		m_WaitTrigLine[6];//待行区触发线
	JD_LINE		m_WaitStopLine[6];//待行区停止线
	UINT		m_LaneLineCls[7];//车道线属性
	BYTE		m_Res[1000];
	_tagJD_PARAM_INFO()
	{
		memset(m_Version,0,80);
		memset(m_LaneCls,0,6);
		m_LaneNum = 0;
		m_LightNum = 0;
		memset(m_LightCls,0,sizeof(m_LightCls));
		memset(m_Res,0,1000);
	}
}JD_PARAM_INFO,*pJD_PARAM_INFO;
//实时结构体
typedef struct _tagJD_RUNTIME_INFO
{
	UINT			m_nBright;//视频亮度
	UINT			m_nContrast;//视频对比度
	BYTE			m_nLightStatus[16];//信号灯状态
	BYTE			m_nLaneStatus[6];//车道状态
	UINT			m_nShutter;//相机快门值
	UINT			m_nAfe;//相机增益值
	//JD_RECT  m_PlateRect[3];
	BYTE      m_Res[1002];//预留位
	_tagJD_RUNTIME_INFO()
	{
		m_nBright = 0;
		m_nContrast = 0;
		memset(m_nLightStatus,0,16);
		memset(m_nLaneStatus,0,6);
		m_nShutter = 0;
		m_nAfe = 0;
		memset(m_Res,0,1002);
	}
}JD_RUNTIME_INFO,*pJD_RUNTIME_INFO;



//调试信息结构体
#ifdef 	FIRST_VERSION
typedef struct _tagJD_INFO
{
	JD_INFO_HEADER	m_Header;
	JD_SNAP_INFO	m_SnapInfo;
	JD_PARAM_INFO	m_ParamInit;
	JD_RUNTIME_INFO	m_RunTime;
	BYTE   m_Res[1000];
	_tagJD_INFO()
	{
		memset(m_Res,0,1000);
	}
}JD_INFO,*pJD_INFO;
#endif
#ifdef SECOND_VERSION
typedef  _tag JD_INFO
{

} JD_INFO;
#endif
#ifdef THIRD_VERSION
typedef  _tag JD_INFO
{

} JD_INFO;
#endif

#define FILE_BUFF 1024*1024*100
#define COM_BUFF 1024*1024*2
#define RGB_BUFF 1024*1024*100
#define QUALITY 80
#define LANELINENO 6

class DecProcess
{
public:
	DecProcess();
	~DecProcess();
public:
	BOOL OpenProcessFile();//打开图片文件并提取信息
	BOOL SaveProcessFile();//写入图片文件
	BOOL DrawLine(JD_LINE line,unsigned char r,unsigned char g,unsigned char b,UINT SynType,int imgWidth, int imgHeigth);//画线
	BOOL DrawRect(JD_RECT rect,unsigned char r,unsigned char g,unsigned char b,UINT SynType,int imgWidth, int imgHeigth);//画矩形
private:
	BOOL GetFileCommentInfo();//获取图像yuv流和指定调试信息
	FILE *m_PicFile;//图像文件
	int m_JPGFileSize;//图像文件大小
	unsigned char *m_FileStreamBuf;//图片文件缓存
	HKGDW_JPEG_COM_INFO m_ComInfo;//调试信息结构体
	unsigned char *m_YUVStream;//YUV数据流
	//unsigned char *m_YUVStream422;//YUV数据流
public:
	unsigned char *m_RGBStream;//RGB数据流,用于显示。
	CString m_FilePathName;//文件路径
	HKGDW_JPEG_IMAGE_INOF m_JPGInfo;//图像文件信息
	JD_INFO m_JD_INFO;//获取的指定调试信息
};
#endif
