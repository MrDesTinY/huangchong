#pragma once
#include "stdafx.h"
#include <vector>


class CBufferManager
{
public:
	CBufferManager(void);
	~CBufferManager(void);
	// 初始化buffer
	bool InitBufferManage();
	// 插入车牌信息
	bool InsertPlateInfo(LPNET_DVR_PLATE_RESULT pPlateInfo);
	// 获取车牌信息
	bool GetPlateInfo(LPNET_DVR_PLATE_RESULT pPlateInfo);
	// 清空buffer
	void ClearBufferManage();
private:
	std::vector<LPNET_DVR_PLATE_RESULT> m_vPlateInfo;			///< 车牌信息
	std::vector<char *>					m_vPicBuf;				///< 图片buf
	std::vector<bool>					m_vFlag;				///< 标记该车牌信息是否获取过
	ULONG								m_nPlateIndex;			///< 车牌索引
	ULONG								m_nPicIndex;			///< 图片索引
	BOOL								m_bEmpty;				///< 车牌信息是否为空

	HANDLE								m_hMutexPlate;			///< 车牌信息锁
};
