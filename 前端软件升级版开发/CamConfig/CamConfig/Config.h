/**
@FileName:  Config.h
@ClassName: CConfig
@Brief: 配置文件读写类

*/


#pragma once

class CConfig
{
    public:
        CConfig(CString strIniFile);
		CConfig(){};
        virtual ~CConfig(void);
           
        CString ReadConfig(LPCTSTR lpAppName,LPCTSTR  lpKey,LPCTSTR lpDefaultKey);
        int ReadConfig(LPCTSTR lpAppName, LPCTSTR lpKeyName, int nDefault); 
        BOOL WriteConfig(LPCTSTR lpAppName,LPCTSTR lpKeyName,LPCTSTR lpString);
        BOOL SetFilePath(CString strIniFile); //设置配置文件路径
        CString GetFilePath();
     private:
        CString m_strIniFile; // 配置文件路径
        CRITICAL_SECTION m_cs; // 读配置文件锁
 };
