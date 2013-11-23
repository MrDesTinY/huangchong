#include "StdAfx.h"
#include "Mutex.h"

CMutex::CMutex(void)
{
}

CMutex::~CMutex(void)
{
}
INT  CMutex::MutexCreate(CRITICAL_SECTION* pMutex)
{
	InitializeCriticalSection(pMutex);
	return 0;
}
INT  CMutex::MutexDestroy(CRITICAL_SECTION* pMutex)
{
	DeleteCriticalSection(pMutex);
	return 0;
}
INT  CMutex::Lock(CRITICAL_SECTION* pMutex)
{
	EnterCriticalSection(pMutex);
	return 0;
}
INT  CMutex::Unlock(CRITICAL_SECTION* pMutex)
{
	LeaveCriticalSection(pMutex);
	return 0;
}