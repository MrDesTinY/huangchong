// PowerInfo.cpp: implementation of the CPowerInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "PowerInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//DWORD CPowerInfo::m_dwRefresh = 0;			//位号值读取刷新次数
//DWORD CPowerInfo::m_dwWrite = 0;			//写位号值次数
//DWORD CPowerInfo::m_dwWriteErr = 0;			//写位号值错误次数
//DWORD CPowerInfo::m_dwTcpReceiveCount = 0;
//DWORD CPowerInfo::m_dwTcpSendCount = 0;

CString CPowerInfo::m_strFileName = "PMSLog.txt";	//日志文件名, 默认PowerToolLog.txt
DWORD CPowerInfo::m_dwMsgTotal = 1000;		//日志总数, 默认1000
BOOL CPowerInfo::m_bSaveLastLog = FALSE;	//开机时是否保存上次的日志
BOOL CPowerInfo::m_bSaveFullLog = FALSE;	//LOG文件满时是否保存
CRITICAL_SECTION CPowerInfo::m_csFile;		//保护记录文件
BOOL CPowerInfo::m_bPrintCmdFlag = FALSE;		//是否打印命令于Log文件中

CPowerInfo::CPowerInfo()
{

}

//CPowerInfo::CPowerInfo(CConfig config)
//{
//   // Init(config.m_strIniFile);
//}

void CPowerInfo::SaveLastLogFile(BOOL bSave)
{
	if(bSave)
	{
		COleDateTime tm = COleDateTime::GetCurrentTime();
		CString strFileName;
		CFileFind ff;
		TRY
		{
			int iPos = m_strFileName.Find('.');
			if(iPos == -1)
			{
				strFileName.Format(".\\%s_last%02d%02d%02d%02d%02d%02d.txt", 
					m_strFileName, tm.GetYear(), tm.GetMonth(), tm.GetDay(), tm.GetHour(), tm.GetMinute(), tm.GetSecond());
			}
			else
			{
				strFileName.Format(".\\%s_last%02d%02d%02d%02d%02d%02d.txt", 
					m_strFileName.Left(iPos), tm.GetYear(), tm.GetMonth(), tm.GetDay(), tm.GetHour(), tm.GetMinute(), tm.GetSecond());
			}
			if(ff.FindFile(".\\"+m_strFileName))
			{
				CFile::Rename(".\\"+m_strFileName, strFileName);
			}
		}
		CATCH( CFileException, e )
		{
		}
		END_CATCH
	}
}

void CPowerInfo::Log(LPCTSTR lpType, LPCTSTR lpTitle, LPCTSTR lpEvent)
{
	if(!m_bPrintCmdFlag)
	{
		return;
	}
	CString m_strEvent;
	m_strEvent.Format("%s, %s, %s.", lpType, lpTitle, lpEvent);
	AddLog(m_strEvent);
}

void CPowerInfo::PrintHexData(LPCTSTR lpType, BYTE* pbyData, DWORD dwLen)
{
	COleDateTime odtNow = COleDateTime::GetCurrentTime();
	CString strTime = odtNow.Format("%Y-%m-%d %H:%M:%S");
	CString strCommand = "";
	CString strTempCmd = "";
	for(DWORD dwLp=0; dwLp<dwLen; dwLp++)
	{
		strTempCmd.Format("%02X ", *(pbyData+dwLp));
		strCommand += strTempCmd;
	}
	CString m_strEvent;
	m_strEvent.Format("%s, TIME: %s, The Hex Data:%s.", lpType, strTime, strCommand);
	AddLog(m_strEvent);
}

void CPowerInfo::PrintASCIIData(LPCTSTR lpType, BYTE* pbyData, DWORD dwLen)
{
	COleDateTime odtNow = COleDateTime::GetCurrentTime();
	CString strTime = odtNow.Format("%Y-%m-%d %H:%M:%S");
	CString strCommand = "";
	CString strTempCmd = "";
	for(DWORD dwLp=0; dwLp<dwLen; dwLp++)
	{
		strTempCmd.Format("%c ", *(pbyData+dwLp));
		strCommand += strTempCmd;
	}
	CString strEvent;
	strEvent.Format("%s, TIME: %s, The ASCII Data:%s.", lpType, strTime, strCommand);
	AddLog(strEvent);
	(CString) pbyData;
	strEvent.Format("%s, TIME: %s, The String Data:%s.", lpType, strTime, (CString)pbyData);
	AddLog(strEvent);
}

void CPowerInfo::AddLog(LPCTSTR lpszMsg)
{
	// Modified by wtl 2002.1.16
	/*
	HANDLE  hEventSource;

	// Use event logging to log the error.
	//
	hEventSource = RegisterEventSource(NULL, "AdvPowerTool");

	if (hEventSource != NULL) 
	{
		ReportEvent(hEventSource, // handle of event source
			EVENTLOG_INFORMATION_TYPE,  // event type
			0X01,                    // event category
			0X01,                    // event ID
			NULL,                 // current user's SID
			1,                    // strings in lpszStrings
			0,                    // no bytes of raw data
			&lpszMsg,			  // array of error strings
			NULL);                // no raw data
	}

	DeregisterEventSource(hEventSource);
	*/
	// Modified by wtl 2002.1.16
    if(!m_bPrintCmdFlag)
    {
        return;
    }
	
	static BOOL bFirst = TRUE;
	static DWORD dwMsgCount = 0;
	UINT nOpenFlags;
	//CString strMsg = lpszMsg;
	CString strMsg;
	SYSTEMTIME st;
	GetLocalTime(&st);
	CString strTmp = lpszMsg;
	int len = strTmp.GetLength();
	if(strTmp[len-1] != '\n')
	{
	    strMsg.Format("[TID:%d]%04d-%02d-%02d %02d:%02d.%03d：%s\n",GetCurrentThreadId(),st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,st.wMilliseconds,lpszMsg);
	}
	else
	{
	    strMsg.Format("[TID:%d]%04d-%02d-%02d %02d:%02d.%03d：%s",GetCurrentThreadId(),st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,st.wMilliseconds,lpszMsg);
	}
	
	EnterCriticalSection(&m_csFile);
	try
	{
		nOpenFlags = CFile::modeCreate | CFile::shareDenyNone
			| CFile::modeWrite | CFile::modeNoTruncate;
		if(bFirst)
		{
			SaveLastLogFile(m_bSaveLastLog);
			nOpenFlags ^= CFile::modeNoTruncate;
			bFirst = FALSE;
			dwMsgCount = 0;
		}
		else if(dwMsgCount > m_dwMsgTotal)
		{
			SaveLastLogFile(m_bSaveFullLog);
			nOpenFlags ^= CFile::modeNoTruncate;
			dwMsgCount = 0;
		}
		CStdioFile fLog(m_strFileName,  nOpenFlags);
		fLog.SeekToEnd();
		fLog.WriteString(strMsg);
		dwMsgCount++;
		fLog.Close();
	}
	catch (CMemoryException* e)
	{
		// We can't recover from this memory exception, so we'll
		// just terminate the app without any cleanup. Normally, an
		// an application should do everything it possibly can to
		// clean up properly and _not_ call AfxAbort().
		e->Delete();
		AfxAbort();
	}
	catch (CFileException* e)
	{
		//e->ReportError();
		e->Delete();
	}
	catch (CException* e)
	{
		e->Delete();
	}
	LeaveCriticalSection(&m_csFile);
}

void CPowerInfo::Init(CString strIniFileName)
{
	strIniFileName.Trim();
	char szVal[128] = {0};
	UINT uiData;
	uiData = GetPrivateProfileInt("SYSTEM", "PRINTCMDFLAG", 0, strIniFileName);
	m_bPrintCmdFlag = (BOOL)uiData;
	uiData = GetPrivateProfileInt("SYSTEM", "SAVELASTLOGFILEFLAG", 0, strIniFileName);
	m_bSaveLastLog = (BOOL)uiData;
	uiData = GetPrivateProfileInt("SYSTEM", "SAVEFULLLOGFILEFLAG", 0, strIniFileName);
	m_bSaveFullLog = (BOOL)uiData;
	GetPrivateProfileString("SYSTEM", "LOGFILENAME", "PowerToolLog.txt", szVal, 128, strIniFileName);
	CString strVal = szVal;
	strVal.Trim();
	m_strFileName = strVal;	
	uiData = GetPrivateProfileInt("SYSTEM", "MESSAGESTOTAL_THOUSAND", 10, strIniFileName);
	m_dwMsgTotal = 1000*uiData;

	InitializeCriticalSection(&m_csFile);
}

void CPowerInfo::Stop()
{
	DeleteCriticalSection(&m_csFile);
}
