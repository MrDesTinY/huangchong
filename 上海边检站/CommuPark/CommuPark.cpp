// CommuPark.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "CommuPark.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

// CCommuParkApp

BEGIN_MESSAGE_MAP(CCommuParkApp, CWinApp)
END_MESSAGE_MAP()


// CCommuParkApp construction

CCommuParkApp::CCommuParkApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CCommuParkApp object

CCommuParkApp theApp;


// CCommuParkApp initialization

BOOL CCommuParkApp::InitInstance()
{
	CWinApp::InitInstance();
	//_crtBreakAlloc = 237;

	return TRUE;
}

/**
全局变量

*/
CConfig g_config("PMS.ini");
Termin  g_Termin(g_config);
extern UCI_TrafficData g_UCIData;
extern CRITICAL_SECTION g_csUCI;



/*以下是封装的通讯库调用接口
*/


/** 
指定配置文件的路径，该函数需要首先被调用，确保配置文件指定正确
配置文件具体的配置项 
如下:
UploadConfig     // 上传配置项
    Port          数据处理服务器监听端口8682
    IP            数据处理服务器IP地址
    SendInterval  过车数据发送间隔，以毫秒为单位
    UploadMode    上传模式，1为实时数据优先，0 为历史数据优先
DBConfig        // MYSQL 数据配置项
    DBServer    // MYSQL 数据库主机IP 地址
    DBName      //       数据库名称
    DBUser               数据库用户名
    DBPassword           数据库登录密码
    DBPort         MYSQL 服务监听端口
SYSTEM             // 日志的配置
    PRINTCMDFLAG       // 是否打印日志 0不打印 1 打印
    SAVELASTLOGFILEFLAG  // 是否保存上次的日志记录 0不保存 1 保存
    SAVEFULLLOGFILEFLAG  // 是否保存全部日志       0不保存 1 保存
    LOGFILENAME          // 默认的日志名称
    MESSAGESTOTAL_THOUSAND // 日志文件消息条数

*/
extern "C" BOOL PASCAL EXPORT InitIniFile(char chFileName[256])
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    
    CString strFileName = chFileName;
    g_config.SetFilePath(strFileName);
    return TRUE;
    
}
// 初始化通讯库，该函数只有在InitIniFile() 后调用，否则无效
extern "C" BOOL PASCAL EXPORT InitLib()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return  g_Termin.Init(g_config);
    
}
// 反初始化通讯库，最后调用
extern "C" BOOL PASCAL EXPORT UnInitLib()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return TRUE;

}
// 初始化写日志的功能类 建议 在停车场终端软件中使用时，配置文件路径与传入的InitIniFile参数一致  
extern "C" BOOL PASCAL EXPORT InitLog(char chFileName[256])
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    CString strFileName = chFileName;
    CPowerInfo::Init(strFileName);
    return TRUE;
}
// 反初始化写日志功能类
extern "C" BOOL PASCAL EXPORT UnInitLog()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    CPowerInfo::Stop();
    return TRUE;
}
// 写日志函数
extern "C" BOOL PASCAL EXPORT AddLog(CString strLog)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    CPowerInfo::WLog(strLog);
    return TRUE;
}

/*
启动上传过车数据服务及监听命令通道
*/
extern "C" BOOL PASCAL EXPORT StartServer()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	return g_Termin.Start();
				
}

/**
停止上传数据服务，关闭命令通道的监听
*/

extern "C" BOOL PASCAL EXPORT StopServer()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    
    return g_Termin.Stop();
}


/**
校时函数，该函数只有在开启上传数据服务后可用，否则不可用。
*/
extern "C" BOOL PASCAL EXPORT CheckTime()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    return g_Termin.CheckTime();
   
}

/**
终端服务器联检车辆信息上传函数，实现终端服务器联检车辆信息从终端向中心的主动上传
*/
extern "C" BOOL PASCAL EXPORT UCIUpdate(char *PlateNo, char *CarType, char *CarColor, char *OwnerName, char *CardNum, char *Reserved)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString strTmp;
	EnterCriticalSection(&g_csUCI);
	memset(&g_UCIData,0,sizeof(g_UCIData));
	memcpy(g_UCIData.chPlateNo,PlateNo,strlen(PlateNo)); 
	memcpy(g_UCIData.chCarType,CarType,strlen(CarType));
	memcpy(g_UCIData.chCarColor,CarColor,strlen(CarColor));
	memcpy(g_UCIData.chOwnerName,OwnerName,strlen(OwnerName));
	memcpy(g_UCIData.chCardNum,CardNum,strlen(CardNum));
	memcpy(g_UCIData.chReserved,Reserved,strlen(Reserved));
	if(g_Termin.SendUCIData())
	{
 		LeaveCriticalSection(&g_csUCI);
		return true;
	}
	else
	{
		if(!g_Termin.SaveTmpUCI())
		{
		    strTmp.Format("SaveTmpUCI ERROR!");
                    OutputDebugString(strTmp);
                    CPowerInfo::WLog(strTmp);	
		}
		else
		{
		    strTmp.Format("SaveTmpUCI SUCCESS!");
                    OutputDebugString(strTmp);
                    CPowerInfo::WLog(strTmp);
		}
		LeaveCriticalSection(&g_csUCI);
		return false;
	}
}