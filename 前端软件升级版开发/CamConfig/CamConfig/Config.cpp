#include "StdAfx.h"
#include "Config.h"

CConfig::CConfig(CString strIniFile):m_strIniFile(strIniFile)
{
    //InitializeCriticalSection(&m_cs);
}

CConfig::~CConfig(void)
{
    //DeleteCriticalSection(&m_cs);
   
}

int CConfig::ReadConfig(LPCTSTR lpAppName, LPCTSTR lpKeyName, int nDefaultKey)
{
   // EnterCriticalSection(&m_cs);
    char chTmp[256] ={0};
    char ch[16] ={0};
    _itoa_s(nDefaultKey,ch,10);
    GetPrivateProfileString(lpAppName,lpKeyName,ch,chTmp,sizeof(chTmp),m_strIniFile);
    //LeaveCriticalSection(&m_cs);
    return atoi(chTmp);
}

CString CConfig::ReadConfig(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpDefaultKey)
{
   // EnterCriticalSection(&m_cs);
    CString strMsg;
    CString strTmp;
    char chTmp[256] ={0};
    GetPrivateProfileString(lpAppName,lpKeyName,lpDefaultKey,chTmp,sizeof(chTmp),m_strIniFile);
    
    
   
    strTmp.Format("%s",chTmp);
   // LeaveCriticalSection(&m_cs); 
    return strTmp;
}

BOOL CConfig::WriteConfig(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpString)
{
    return WritePrivateProfileString(lpAppName,lpKeyName,lpString,m_strIniFile);
}

BOOL CConfig::SetFilePath(CString strIniFile)
{
  //  EnterCriticalSection(&m_cs);
    m_strIniFile = strIniFile;
   // LeaveCriticalSection(&m_cs);
    return TRUE;
}

CString CConfig::GetFilePath()
{
    return m_strIniFile;
}