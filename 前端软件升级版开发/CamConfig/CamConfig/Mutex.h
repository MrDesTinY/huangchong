#pragma once

class CMutex
{
public:
	CMutex(void);
	~CMutex(void);
public:
	INT  MutexCreate(CRITICAL_SECTION* pMutex); 
	INT  MutexDestroy(CRITICAL_SECTION* pMutex); 
	INT  Lock(CRITICAL_SECTION* pMutex); 
	INT  Unlock(CRITICAL_SECTION* pMutex); 
};
