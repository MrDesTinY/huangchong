// PowerInfo.h: interface for the CPowerInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "Config.h"

#if !defined(AFX_POWERINFO_H__0A93100A_0600_4DC8_AEA0_4AEA6EF05887__INCLUDED_)
#define AFX_POWERINFO_H__0A93100A_0600_4DC8_AEA0_4AEA6EF05887__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPowerInfo  
{
public:
	CPowerInfo();
//	CPowerInfo::CPowerInfo(CConfig config);

	static void Log(LPCTSTR lpType, LPCTSTR lpTitle, LPCTSTR lpEvent);
	static void WLog(LPCTSTR lpszMsg){AddLog(lpszMsg);}
	static void SaveLastLogFile(BOOL bSave);
	static void PrintHexData(LPCTSTR lpType, BYTE* pbyData, DWORD dwLen);
	static void PrintASCIIData(LPCTSTR lpType, BYTE* pbyData, DWORD dwLen);

private:
	static void AddLog(LPCTSTR lpszMsg);
	static CRITICAL_SECTION m_csFile;		//保护记录文件

public:
	static void Stop();
	static void Init(CString strIniFileName);
	//static DWORD m_dwRefresh;			//位号值读取刷新次数
	//static DWORD m_dwWrite;				//写位号值次数
	//static DWORD m_dwWriteErr;			//写位号值错误次数
	//static DWORD m_dwTcpReceiveCount;	//TCP通信收到的包数
	//static DWORD m_dwTcpSendCount;		//TCP通信发送的包数

	static CString m_strFileName;		//日志文件名
	static DWORD m_dwMsgTotal;			//日志总数
	static BOOL m_bSaveLastLog;			//开机时是否保存上次的日志
	static BOOL m_bSaveFullLog;			//LOG文件满时是否保存
	static BOOL m_bPrintCmdFlag;		//是否打印命令于Log文件中
};

#endif // !defined(AFX_POWERINFO_H__0A93100A_0600_4DC8_AEA0_4AEA6EF05887__INCLUDED_)
