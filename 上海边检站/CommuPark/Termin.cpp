#include "StdAfx.h"
#include "Termin.h"
extern CRITICAL_SECTION g_csUCI;
extern CRITICAL_SECTION g_csTread;
extern CRITICAL_SECTION g_csSend;
extern CRITICAL_SECTION g_csMysql;
Termin::Termin(CConfig &config): m_client(config),m_bUpData(FALSE)
,m_bListen(FALSE)
{
	//Start();
}

Termin::~Termin(void)
{

}

BOOL Termin::Start()
{
    CString strTmp;
    InitializeCriticalSection(&g_csUCI);
	InitializeCriticalSection(&g_csTread);
	InitializeCriticalSection(&g_csSend);
	InitializeCriticalSection(&g_csMysql);
    BOOL bRet =  m_server.StartServer();
    if(!bRet)
    {
        strTmp.Format("启动监听服务失败\n");
        OutputDebugString(strTmp);
        return FALSE;
    }
    
    bRet = m_client.DoStart();
    if(!bRet)
    {
        strTmp.Format("启动数据上传服务失败\n");
        OutputDebugString(strTmp);
        return FALSE;
    }
    m_bListen = TRUE;
    m_bUpData = TRUE;
    
    return TRUE;
   
}

BOOL Termin::Stop()
{
    CString strTmp;
    DeleteCriticalSection(&g_csUCI);
	DeleteCriticalSection(&g_csTread);
	DeleteCriticalSection(&g_csSend);
	DeleteCriticalSection(&g_csMysql);
    if(m_bListen)
    {
        m_server.StopServer();
        m_bListen = FALSE;
    }
    if(m_bUpData)
    {
        m_client.DoStop();
        m_bUpData = FALSE;
    }
    return TRUE;
    
}

BOOL Termin::Init(CConfig &config)
{
   return  m_client.Init(config);
}

BOOL Termin::CheckTime()
{
   return m_client.DoCheckTime();
}

BOOL Termin::SendUCIData()
{
	return m_client.DoSendUCIData();
}

BOOL Termin::SaveTmpUCI()
{
	return m_client.DoSaveTmpUCI();
}