#pragma once

#include "BufferManager.h"
#include <gdiplus.h>
#include <GdiplusImaging.h>
#include <GdiplusImageCodec.h>
#include <vector>
//#define ONE_DEVICE
//#pragma comment(lib,"gdiplus.lib")
using namespace Gdiplus;

//自定义消息结构体和控件信息结构体
typedef struct _MESSAGWE
{
	char* pdata;
	int pDataLen;
	int dataType;		//0是图片1为车牌号码

	/*车牌附加信息定义：
		车牌附加信息是与车牌相关的一组信息，以字符串形式存在。
		它包括车牌类型、车辆逆行标志、车辆检测时间、车速、车身颜色等，目前在二义性路径识别系统中，使用到的车牌附加信息只有车辆逆向标志，以后还会在这基础上增加其他相关信息。
	车牌附加信息格式规范
		为了使从车牌附加信息字符串中获取所关心的值更简便，该字符串的格式统一如下：
		“信息描述:(半角冒号)信息值\n”，如”车辆逆向行驶:是\n”，注意信息描述和信息值之间一定是半角冒号，并且不能有空格，该条信息结束后用回车符分开，以便区分下一条信息。*/
	char* pAppinfo;		//附加信息

	int appinfolen;		///附加信息数据长度

	char* pdata_se;		//预留备用
	int pdata_se_len;	//预留备用数据长度
} resultMSG;

typedef struct _CArray//控件信息结构体
{
	LPUNKNOWN lpunknow;
	TCHAR Name[MAX_PATH];
} DispatchArray;

#ifndef ONE_DEVICE
typedef struct _tagDeviceInfo//设备信息结构体，用于管理多设备。
{
	CString m_csIPAddr;//设备IP
	CString m_csPort;//设备端口号
	LONG m_lUid;//设备索引号
	LONG m_lAlarmHandle;//< 布防句柄
	CBufferManager * m_cBufferManager;//< 车牌存储
	_tagDeviceInfo()
	{
		m_csPort = _T("");
		m_csIPAddr = _T("");
		m_lUid = -1;
		m_lAlarmHandle = -1;
		m_cBufferManager = new CBufferManager();
		m_cBufferManager->InitBufferManage();
	}
	~_tagDeviceInfo()
	{
		m_cBufferManager->ClearBufferManage();
		delete m_cBufferManager;
		m_cBufferManager = NULL;
	}
}DeviceInfo,*pDeviceInfo;
#endif

#define MSG_REPAINT_RESULT (WM_USER + 113)
#define MSG_CLI_RESULT (WM_USER + 112)

// HVActiveX2Ctrl.h : Declaration of the CHVActiveX2Ctrl ActiveX Control class.


// CHVActiveX2Ctrl : See HVActiveX2Ctrl.cpp for implementation.

class CHVActiveX2Ctrl : public COleControl
{
	DECLARE_DYNCREATE(CHVActiveX2Ctrl)

// Constructor
public:
	CHVActiveX2Ctrl();

// Overrides
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

// Implementation
protected:
	~CHVActiveX2Ctrl();

	DECLARE_OLECREATE_EX(CHVActiveX2Ctrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CHVActiveX2Ctrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CHVActiveX2Ctrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CHVActiveX2Ctrl)		// Type name and misc status

// Message maps
	afx_msg LRESULT  OnRecieve(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	DECLARE_DISPATCH_MAP()

// Event maps
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
		dispidLogFile = 25,
		eventidOnReceivePlate = 1L,
		dispidGetImageDataFromMem = 24L,
		dispidGetImageFileName = 23L,
		dispidGetImageRecTime = 22L,
		dispidSavePlateBin2BMP = 21L,
		dispidSavePlateBinImage = 20L,
		dispidSavePlateImage = 19L,
		dispidSaveSnapImage2 = 18L,
		dispidSaveSnapImage = 17L,
		dispidGetPlateInfo = 16L,
		dispidGetPlateColor = 15L,
		dispidGetPlate = 14L,
		dispidSetImgSavePath = 13L,
		dispidDisconnect = 12L,
		dispidGetStatus = 11L,
		dispidConnectTo = 10L,
		dispidShowPlateResult = 9,
		dispidShowSecondImage = 8,
		dispidShowFirstImage = 7,
		dispidShowPlateImage = 6,
		dispidAutoSaveFlag = 5,
		dispidRecvSnapDriverImageFlag = 4,
		dispidRecvPlateBinImageFlag = 3,
		dispidRecvPlateImageFlag = 2,
		dispidRecvSnapImageFlag = 1
	};
protected:
	CArray< DispatchArray > m_ArrayDispatch; //获取控件信息存放序列
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////属性
	void OnRecvSnapImageFlagChanged(void);
	LONG m_RecvSnapImageFlag;//是否接收车辆大图开关，0表示不接收车辆图片，非0表示接收车辆图片。
	void OnRecvPlateImageFlagChanged(void);
	LONG m_RecvPlateImageFlag;//是否接收车牌小图开关，0表示不接收车牌图片，非0表示接收车牌图片。
	void OnRecvPlateBinImageFlagChanged(void);
	LONG m_RecvPlateBinImageFlag;//是否接收车牌二值化图开关，0表示不接收车牌二值化图片，非0表示接收车牌二值化图片。
	void OnRecvSnapDriverImageFlagChanged(void);
	LONG m_RecvSnapDriverImageFlag;//是否接收车脸图开关，0表示不接收车脸图片，非0表示接收车脸图片。
	void OnAutoSaveFlagChanged(void);
	VARIANT_BOOL m_AutoSaveFlag;//是否自动保存识别结果开关，true为自动保存，false为不自动保存。
	void OnShowPlateImageChanged(void);
	CString m_ShowPlateImage;//显示车牌图HVSHOW控件名 
	void OnShowFirstImageChanged(void);
	CString m_ShowFirstImage;//显示车辆大图HVSHOW控件名 
	void OnShowSecondImageChanged(void);
	CString m_ShowSecondImage;//显示车脸图HVSHOW控件名
	void OnShowPlateResultChanged(void);
	CString m_ShowPlateResult;//显示车牌号码HVSHOW控件名
	//////////////////////////////////////////////////////////////////////////////////自定义属性
	void OnLogFileChanged(void);
	LONG m_LogFile;//是否打印日志到文件中。
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////控制接口函数

	/*连接到车牌识别器，strAddr为车牌识别器的IP地址加端口号或者IP地址，
	例如“192.168.0.18:9999”或者“192.168.0.18”。是否连接成功可以
	通过GetStatus（）函数调用取得。*/
	void ConnectTo(LPCTSTR strAddr);

	/*取得当前的连接状态。
	可能的返回值包括如下：
	0：已连接
	-1：已断开*/
	LONG GetStatus(void);

	/*断开当前连接。*/
	void Disconnect(void);

	/*在自动存储有效时，设置图像要存储的目录，文件名及子目录由控件自动生成。
	子目录及文件名命名规则“存储目录\\设备IP\\日期\\文件名”，例如：
	设置目录为“d:\\image”
	设备IP为“192.16.1.12”
	自动建立目录为“d:\\image \\192.16.1.12\\20111025\\”
	全景图文件名“165949-031-fullimg.jpg”
	车脸图文件名“165949-031-driver.jpg”
	二值化图文件名“165949-031-binimg.bin”
	车牌图文件名“165949-031-plateimg.jpg”*/
	void SetImgSavePath(LPCTSTR strPath);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////获取信息和保存结果接口函数

	/*取得当前检测到的车牌号码。
	当没有识别结果时，输出是“未检测”*/
	BSTR GetPlate(void);

	/*取得当前检测到的车牌颜色（“黄”、“蓝”、“白”、“黑”）。
	当没有检测到车牌颜色时，输出是空字符串*/
	BSTR GetPlateColor(void);

	/*取得当前车牌的附加信息
	（车速、车牌亮度、车身颜色等，
	目前车辆逆向行驶必须保存在车牌附加信息中，
	具体格式见6）。*/
	BSTR GetPlateInfo(void);

	/*保存车辆全景图到指定的文件。返回0表示成功。*/
	LONG SaveSnapImage(LPCTSTR strFileName);

	/*保存车脸大图到指定的文件。返回0表示成功。*/
	LONG SaveSnapImage2(LPCTSTR strFileName);

	/*保存车牌图片到指定文件。返回0表示成功。*/
	LONG SavePlateImage(LPCTSTR strFileName);

	/*保存车牌二值化图片到指定文件。返回0表示成功。*/
	LONG SavePlateBinImage(LPCTSTR strFileName);

	/*保存二值图为BMP图。返回0表示成功。*/
	LONG SavePlateBin2BMP(LPCTSTR strFileName);

	/*返回接收图像的时间，0全景图、1车牌图、2二值化图、3车脸图
	时间格式，举例说明“2011-10-25 12:33:32-345”，最后三位是毫秒*/
	BSTR GetImageRecTime(LONG lImgType);

	/*自动存储时，获取文件绝对路径及文件名
	如：“D:\image\127.0.0.1\20111026\ 100021-718-fullimg.jpg”*/
	BSTR GetImageFileName(LONG lImgType);

	/*取到车车辆图片内存数据，包括全景图、车牌图、
	二值化图、车脸图，
	lImgType为0表示全景图、
	lImgType为1表示车牌图、
	lImgType为2表示二值化图、
	lImgType为3表示车脸图
	pVarImg为数据区、
	lImgSize返回数据长度。*/
	BSTR GetImageDataFromMem(LONG lImgType, VARIANT* pVarImg, LONG* lImgSize);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////事件函数

	/*识别结果到达信息。
	在该事件下可调用以下接口函数：
	调用GetPlate()取得车牌结果。
	调用GetPlateColor()取得车牌颜色。
	调用GetPlateInfo()取得车牌附加信息。
	调用SaveSnapImage ()保存车辆全景到指定的文件。
	调用SaveSnapImage2 ()保存车辆车脸图到指定的文件。
	调用SavePlateImage ()保存车牌小图到指定的文件。
	调用SavePlateBinImage ()保存车牌二值化图到指定的文件。
	调用SavePlateBin2BMP()保存二值图为BMP图。
	调用GetImageRecTime获取图像接收时间
	调用GetImageFileName获取自动存储时图像绝对路径
	调用GetImageDataFromMem ()取到车车辆图片内存数据，包括全景图、车牌图、二值化图、车脸图。*/
	void OnReceivePlate(void)
	{
		FireEvent(eventidOnReceivePlate, EVENT_PARAM(VTS_NONE));
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////接口文档中的自定义函数
	bool m_fChangingCtrlName;
	HWND m_hWndParent;
	/*向控件显示数据
	const char* strControlName = “showVideoImage”;
	extern char* pbData;
	extern long iLen;
	resultMsg msg;
	msg.pdata = pbData;
	msg.pdataLen = iLen;
	msg.dataType = 0;
	SendData (strControlName, pbData, iLen, 0, &msg);*/
	bool SendData( CString CtrlName, char *pdata, long datalen, long iType, resultMSG *pmsg );

	/*获取所有OLE控件*/
	void EnumAllControlNames();
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////设备及设备获取交互信息节点
private:
	NET_DVR_DEVICEINFO_V30 m_stDeviceInfo;				///< 设备信息
	//NET_INTELCFG_DEVLOGININFO m_stDevLogInfo;				///< 设备信息
#ifndef ONE_DEVICE
	CRITICAL_SECTION m_csVecLock;
	static std::vector<pDeviceInfo> m_vecDeviceInfoTable;//多设备管理容器
	CString GetIPAddrByUid(LONG uid);//获取设备IP
	CString GetPortByUid(LONG uid);//获取设备端口
	LONG GetAlarmHandleByUid(LONG uid);//获取布防句柄
	CBufferManager *GetBufferManagerByUid(LONG uid);//获取缓存管理类 
	pDeviceInfo GetDevicePtrByUid(LONG uid);//获取设备信息结构体指针
public:
	LONG m_lCurrentUid;//标识当前触发设备Uid
	LONG m_lLoginUid;//标识当前登录设备Uid，用于布防。
#else
	CString m_csIPAddr;//设备IP
	CString m_csPort;//设备端口号
	LONG m_lUid;//设备索引号
	LONG m_lAlarmHandle;//< 布防句柄
	CBufferManager m_cBufferManager;//< 车牌存储
#endif

	ULONG_PTR m_gdiplusToken;		//GDI口令
	CString m_csImagePath;//图片保存路径
	char * m_pchPlateBinBMP;//车牌二值化转BMP图片
	unsigned int m_iBinBMPSize;//BMP图片大小
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////日志功能节点
public:
	HANDLE							m_hLogFile;					///< 日志文件句柄
	HANDLE							m_hMutexLogFile;			///< 日志文件锁
	//写日志文件
	VOID WriteLogFile(LPCTSTR szFun,BOOL bError, LPCTSTR szLog);
	//创建日志文件
	BOOL CreateLogFile();
	//创建文件路径
	BOOL CreateFileDir(LPCTSTR lpPath);
public:
	int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
	BOOL ConvertImgBytes2HBITMAP(BYTE* pImageBuffer,DWORD dwImageDataLen, int iWidth, int iHeight, WCHAR * szFileNae);

	void InsertNewPlate(LPNET_DVR_PLATE_RESULT pPlateInfo);
	LONG SetUpAlarm();
	LONG CleanAlarm();
	BOOL SavePic(LPCTSTR strFileName, BYTE * pImage, DWORD dwPicDataLen);

	int BinImageFormatConvert(BYTE *pByteBinPacked,int iWidth, int iHeight);
	int ConvertBin2BmpData(BYTE *pByteBinPacked,BYTE *pByteBmpData,unsigned int *pnBmpDataLength);

public:
	afx_msg void OnDestroy();

};

