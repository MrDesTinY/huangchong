#pragma once
#ifdef SDBDLL
#define SDBDLLAPI __declspec(dllexport)
#else
#define SDBDLLAPI __declspec(dllimport)
#endif
#ifdef __cplusplus
extern "C"
{
#endif
unsigned SDBDLLAPI WINAPI dsbdll(unsigned ComPort,char * OutString);
#ifdef __cplusplus
};
#endif