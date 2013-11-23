#ifndef _COMMUPARKLIB_H
#define _COMMUPARKLIB_H

#ifdef COMMUPARKLIB_EXPORTS
#define COMMUPARKLIB_API __declspec(dllexport)
#else
#define COMMUPARKLIB_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {

COMMUPARKLIB_API int __stdcall InitIniFile(char chFileName[256]);
COMMUPARKLIB_API int __stdcall InitLib();
COMMUPARKLIB_API int __stdcall UnInitLib();
COMMUPARKLIB_API int __stdcall InitLog(char chFileName[256]);
COMMUPARKLIB_API int __stdcall UnInitLog();
COMMUPARKLIB_API int __stdcall AddLog(CString strLog);
COMMUPARKLIB_API int __stdcall StartServer();
COMMUPARKLIB_API int __stdcall StopServer();
COMMUPARKLIB_API int __stdcall CheckTime();

#ifdef __cplusplus
}
#endif
#endif