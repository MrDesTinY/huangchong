#include "StdAfx.h"
#include "BufferManager.h"
#include "ToHvShowCtrl.h"
#ifdef LOCK_MUTEX
CBufferManager::CBufferManager(void):
m_nPicIndex(0)
,m_nPlateIndex(0)
,m_hMutexPlate(NULL)
,m_bEmpty(TRUE)
{
	m_vPlateInfo.clear();
	m_vPicBuf.clear();
	m_vFlag.clear();
}
#else
CBufferManager::CBufferManager(void):
m_nPicIndex(0)
,m_nPlateIndex(0)
,m_bEmpty(TRUE)
{
	m_vPlateInfo.clear();
	m_vPicBuf.clear();
	m_vFlag.clear();
}
#endif

CBufferManager::~CBufferManager(void)
{
	ClearBufferManage();	
}

/** @fn		void CBufferManager::InitBufferManage()
 *	@brief	初始Buffer
 *	@return	bool	true表示成功，否则失败
 */
bool CBufferManager::InitBufferManage()
{
	if (m_vPlateInfo.empty() == TRUE)
	{
		//车牌列表为空的时候创建
		for (int i = 0; i < SCCP_MAX_PLATEINFO_NUM; i ++)
		{
			LPNET_DVR_PLATE_RESULT pPlateInfo = NULL;
			try
			{
				pPlateInfo = new NET_DVR_PLATE_RESULT;
			}
			catch (...)
			{
				return false;
			}
			if (pPlateInfo != NULL)
			{
				m_vPlateInfo.push_back(pPlateInfo);
				m_vFlag.push_back(true);
			}
		}
		m_nPlateIndex = 0;
		m_bEmpty = TRUE;
	}
	if (m_vPicBuf.empty() == TRUE)
	{
		// 图片列表为空时创建
		for (int i = 0; i < SCCP_MAX_PIC_NUM; i ++)
		{
			char	*pBuf = NULL;
			try
			{
				pBuf = new char[SCCP_MAX_PIC_SIZE];
			}
			catch (...)
			{
				return false;
			}
			if (pBuf != NULL)
			{
				m_vPicBuf.push_back(pBuf);
			}
		}
		m_nPicIndex = 0;
	}
#ifdef LOCK_MUTEX
	if (m_hMutexPlate == NULL)
	{
		m_hMutexPlate = CreateMutex(NULL, FALSE, NULL);
		if (m_hMutexPlate == NULL)
		{
			return false;
		}
	}
#else
	InitializeCriticalSectionAndSpinCount(&m_csLockPlate,4000);
#endif

	return true;
}

/** @fn		bool CBufferManager::InsertPlateInfo(LPNET_DVR_PLATE_RESULT pPlateInfo)
 *	@brief	插入车牌信息
 *	@param	[IN]	pPlateInfo	车牌信息
 *	@return	true表示成功，否则失败
 */
bool CBufferManager::InsertPlateInfo(LPNET_DVR_PLATE_RESULT pPlateInfo,CToHvShowCtrl *pHandle)
{
	if (pPlateInfo == NULL)
	{
		pHandle->WriteLogFile((CA2T)__FUNCTION__, TRUE, _T("New：回调车牌信息为空！！！"));
		return false;
	}

#ifdef LOCK_MUTEX
	if (WaitForSingleObject(m_hMutexPlate, INFINITE) != WAIT_OBJECT_0)
	{
		return false;
	}
#else
	EnterCriticalSection(&m_csLockPlate);
#endif

	int nIndex = m_nPlateIndex;

	if (m_vPlateInfo[m_nPlateIndex] != NULL)
	{
		// 插入车牌信息
		memcpy(m_vPlateInfo[m_nPlateIndex], pPlateInfo, sizeof(NET_DVR_PLATE_RESULT));
		m_vFlag[m_nPlateIndex] = false;
		m_nPlateIndex = (m_nPlateIndex + 1) % SCCP_MAX_PLATEINFO_NUM;
		m_bEmpty = FALSE;
	}

	if (pPlateInfo->pBuffer1 != NULL && pPlateInfo->dwPicLen != 0)
	{
		// 设置近景图
		if (m_vPicBuf[m_nPicIndex] != NULL)
		{
			memset(m_vPicBuf[m_nPicIndex],0,SCCP_MAX_PIC_SIZE);
			memcpy(m_vPicBuf[m_nPicIndex], pPlateInfo->pBuffer1, pPlateInfo->dwPicLen);
			m_vPlateInfo[nIndex]->pBuffer1 = (BYTE*)m_vPicBuf[m_nPicIndex];
		}
	}
	m_nPicIndex = (m_nPicIndex + 1) % SCCP_MAX_PIC_NUM;
	if (pPlateInfo->pBuffer2 != NULL && pPlateInfo->dwPicPlateLen != 0)
	{
		//设置车牌彩图
		if (m_vPicBuf[m_nPicIndex] != NULL)
		{
			memset(m_vPicBuf[m_nPicIndex],0,SCCP_MAX_PIC_SIZE);
			memcpy(m_vPicBuf[m_nPicIndex], pPlateInfo->pBuffer2, pPlateInfo->dwPicPlateLen);
			m_vPlateInfo[nIndex]->pBuffer2 = (BYTE*)m_vPicBuf[m_nPicIndex];
		}
	}
	m_nPicIndex = (m_nPicIndex + 1) % SCCP_MAX_PIC_NUM;
	if (pPlateInfo->pBuffer3 != NULL && pPlateInfo->dwBinPicLen != 0)
	{
		//设置车牌二值图
		if (m_vPicBuf[m_nPicIndex] != NULL)
		{
			memset(m_vPicBuf[m_nPicIndex],0,SCCP_MAX_PIC_SIZE);
			memcpy(m_vPicBuf[m_nPicIndex], pPlateInfo->pBuffer3, pPlateInfo->dwBinPicLen);
			m_vPlateInfo[nIndex]->pBuffer3 = (BYTE*)m_vPicBuf[m_nPicIndex];
		}
	}
	m_nPicIndex = (m_nPicIndex + 1) % SCCP_MAX_PIC_NUM;
	if (pPlateInfo->pBuffer4 != NULL && pPlateInfo->dwCarPicLen != 0)
	{
		//设置车辆原图
		if (m_vPicBuf[m_nPicIndex] != NULL)
		{
			memset(m_vPicBuf[m_nPicIndex],0,SCCP_MAX_PIC_SIZE);
			memcpy(m_vPicBuf[m_nPicIndex], pPlateInfo->pBuffer4, pPlateInfo->dwCarPicLen);
			m_vPlateInfo[nIndex]->pBuffer4 = (BYTE*)m_vPicBuf[m_nPicIndex];
		}
	}
	m_nPicIndex = (m_nPicIndex + 1) % SCCP_MAX_PIC_NUM;
	if (pPlateInfo->pBuffer5 != NULL && pPlateInfo->dwFarCarPicLen != 0)
	{
		//设置远景图
		if (m_vPicBuf[m_nPicIndex] != NULL)
		{
			memset(m_vPicBuf[m_nPicIndex],0,SCCP_MAX_PIC_SIZE);
			memcpy(m_vPicBuf[m_nPicIndex], pPlateInfo->pBuffer5, pPlateInfo->dwFarCarPicLen);
			m_vPlateInfo[nIndex]->pBuffer5 = (BYTE*)m_vPicBuf[m_nPicIndex];
		}
	}
	m_nPicIndex = (m_nPicIndex + 1) % SCCP_MAX_PIC_NUM;
#ifdef LOCK_MUTEX
	ReleaseMutex(m_hMutexPlate);
#else
	LeaveCriticalSection(&m_csLockPlate);
#endif
	pHandle->WriteLogFile((CA2T)__FUNCTION__, FALSE, _T("New：插入到缓存成功！！！"));
	return true;
}

/** @fn		bool CBufferManager::GetPlateInfo(LPNET_DVR_PLATE_RESULT pPlateInfo)
 *	@biref	获取车牌信息
 *	@param	[OUT]	pPlateInfo	车牌信息
 *	@return	bool	true表示成功，否则失败
 */
bool CBufferManager::GetPlateInfo(LPNET_DVR_PLATE_RESULT pPlateInfo)
{
	if (pPlateInfo == NULL)
	{
		return false;
	}
#ifdef LOCK_MUTEX
	if (WaitForSingleObject(m_hMutexPlate, INFINITE) != WAIT_OBJECT_0)
	{
		return false;
	}
#else
	EnterCriticalSection(&m_csLockPlate);
#endif


	if (m_bEmpty == TRUE)
	{
#ifdef LOCK_MUTEX
		ReleaseMutex(m_hMutexPlate);
#else
		LeaveCriticalSection(&m_csLockPlate);
#endif
		return false;
	}
	int			iIndex = m_nPlateIndex -1;
	int			iCount = SCCP_MAX_PLATEINFO_NUM;
	if (iIndex == -1)
	{
		iIndex = SCCP_MAX_PLATEINFO_NUM - 1;
	}


	if (m_vPlateInfo[iIndex] != NULL)
	{
		memcpy(pPlateInfo, m_vPlateInfo[iIndex], sizeof(NET_DVR_PLATE_RESULT));
		m_vFlag[iIndex] = true;
	}
#ifdef LOCK_MUTEX
	ReleaseMutex(m_hMutexPlate);
#else
	LeaveCriticalSection(&m_csLockPlate);
#endif

	return true;
}

char* CBufferManager::GetPicBuffer(LONG lPicType)
{
	return m_vPicBuf[lPicType];//0 车辆图片 1 车牌图片 2 二值图片 3 近景图片 4 远景图片
}

/** @fn		void CBufferManager::ClearBufferManage()
 *	@brief	清空缓冲
 *	@return	void
 */
void CBufferManager::ClearBufferManage()
{
	std::vector<LPNET_DVR_PLATE_RESULT>::iterator	iterPlate;
	std::vector<char*>::iterator					iterPic;

	for (iterPlate = m_vPlateInfo.begin(); iterPlate != m_vPlateInfo.end(); iterPlate ++)
	{
		if (*iterPlate!= NULL)
		{
			delete *iterPlate;
		}
	}
	for (iterPic = m_vPicBuf.begin(); iterPic != m_vPicBuf.end(); iterPic ++)
	{
		if (*iterPic != NULL)
		{
			delete [](*iterPic);
		}
	}
	
	m_vPicBuf.clear();
	m_vPlateInfo.clear();
	m_vFlag.clear();
#ifdef LOCK_MUTEX
	if (m_hMutexPlate != NULL)
	{
		CloseHandle(m_hMutexPlate);
		m_hMutexPlate = NULL;
	}
#else
	DeleteCriticalSection(&m_csLockPlate);
#endif

}