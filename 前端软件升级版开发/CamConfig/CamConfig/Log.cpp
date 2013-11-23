#include "StdAfx.h"
#include "Log.h"
#include <Shlwapi.h> 
#include <sys/stat.h>
#include "Mutex.h"

CRITICAL_SECTION g_CriticalSectLog;//声明日志临界区
CMutex g_mutex;

CLog::CLog(void)
{
	g_mutex.MutexCreate(&g_CriticalSectLog);
}

CLog::~CLog(void)
{
	g_mutex.MutexDestroy(&g_CriticalSectLog);
}
//获取当前目录路径
CString CLog::GetDirectoryPath()
{
	CString lpStrPath;
	const int nBufSize=512;
	TCHAR chBuf[nBufSize];
	ZeroMemory(chBuf,nBufSize);
	if(GetModuleFileName(NULL,chBuf,nBufSize))
	{		
		PathRemoveFileSpec(chBuf);
		lpStrPath=chBuf;
	}
	return lpStrPath;
}
CString CLog::GetUpDir(CString lpStrPath)
{
   int aa= lpStrPath.ReverseFind('\\');
   lpStrPath.Delete(aa,lpStrPath.GetLength()- aa);
   return lpStrPath;
}
CString strFileName;//文件名	
FILE* debugInfoFile;//文件
//写日志文件
BOOL CLog::SaveLogInfo(const char *pLogMsg)
{	
	CTime tmCurr;//当前时间
	CString strTimeCurr;//当前时间
	CString strDebug;//调试信息	
	tmCurr = CTime::GetCurrentTime();
	
	strTimeCurr = tmCurr.Format("%Y-%m-%d %H:%M:%S ");
	strDebug.Format("%s%s\r\n", strTimeCurr, pLogMsg);
	strFileName.Format("%s\\CamConfig_Log.txt", GetDirectoryPath());
	//为日志增加临界区
	g_mutex.Lock(&g_CriticalSectLog);
	fopen_s(&debugInfoFile,(char *)(LPCTSTR)strFileName,"a");
	if(debugInfoFile != NULL)
	{
		fprintf(
			debugInfoFile,
			"%s",
			strDebug
			);
		fclose(debugInfoFile);	
		//为日志增加临界区
		g_mutex.Unlock(&g_CriticalSectLog);
		return TRUE;
	}
	else
	{
		return FALSE;
	}	
}

//清理日志文件
BOOL CLog::ClearLogInfo()
{	
	strFileName.Format("%s\\CamConfig.txt", GetDirectoryPath());
	CFileStatus status;
	CFile::GetStatus(strFileName,status);
	ULONGLONG sizeOfFile=status.m_size;
	if (sizeOfFile > 1024 * 1024) //如果日志文件大小超过10M,清空
	{		
		fopen_s(&debugInfoFile,strFileName, "w");
		if(debugInfoFile == NULL)
		{
			return TRUE;
		}
	}
	return FALSE;	
}