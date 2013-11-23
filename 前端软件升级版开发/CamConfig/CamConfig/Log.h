#pragma once

class CLog
{
public:
	CLog(void);
	~CLog(void);
public:
	CString GetDirectoryPath();
	CString CLog::GetUpDir(CString lpStrPath);
	BOOL SaveLogInfo(const char *pchMsg);
	BOOL ClearLogInfo();
};