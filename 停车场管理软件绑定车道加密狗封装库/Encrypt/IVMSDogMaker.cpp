#include "IVMSDogMaker.h"

CIVMSDogMaker::CIVMSDogMaker()
{
	
}
CIVMSDogMaker::~CIVMSDogMaker()
{
}
/****************************************************************************************************
* @函数名称:	MakePRDog
* @功  能：		制作视频分析记录仪加密狗
* @参  数：		
* @wDays		使用天数
* @nChannels	通道数
* @nLanes		车道数
* @nPlatForm	平台连接
* @nChannelType	通道类型
* @返回值：		S_OK 操作成功 
* @说  明：		
****************************************************************************************************/
HRESULT CIVMSDogMaker::MakePRDog( 
					   WORD wDays,
					   BYTE nChannels,
					   BYTE nLanes,
					   BYTE nPlatForm,
					   BYTE nChannelType  )
{
	HRESULT hr = S_OK;
	memset( m_pMemoryBuf,0,sizeof(m_pMemoryBuf) );
	hr = CIVMSDog::ReadData();
	InitAllParam( );
	if( S_OK != hr  )
	{
		return hr;
	}
	m_InfoBody.m_bBind = FALSE;
	m_InfoBody.m_UseDays = wDays;
	m_CustomInfo.m_nChannels = nChannels;
	m_CustomInfo.m_nLanes = nLanes;
	m_CustomInfo.m_bConnPlatForm = nPlatForm;
	m_CustomInfo.m_nChannelType = nChannelType;
	m_CustomInfo.m_nResverd3[0] = 0xf0;
	hr = WriteData();
	return hr;
}

/****************************************************************************************************
* @函数名称:	MakeFRDog
* @功  能：		制作视频分析记录仪加密狗
* @参  数：		
* @wDays		使用天数
* @nChannels	通道数
* @nLanes		车道数
* @nPlatForm	平台连接
* @nChannelType	通道类型
* @返回值：		S_OK 操作成功 
* @说  明：		
****************************************************************************************************/
HRESULT CIVMSDogMaker::MakeFRDog( 
					   WORD wDays,
					   BYTE nChannelsFace,
					   BYTE nLanesFace,
					   BYTE nPlatFormFace,
					   BYTE nChannelTypeFace  )
{
	HRESULT hr = S_OK;
	memset( m_pMemoryBuf,0,sizeof(m_pMemoryBuf) );
	hr = CIVMSDog::ReadData();
	InitAllParam( );
	if( S_OK != hr  )
	{
		return hr;
	}
	m_InfoBody.m_bBind = FALSE;
	m_InfoBody.m_UseDays = wDays;
	m_CustomInfo.m_nChannelsFace= nChannelsFace;
	m_CustomInfo.m_nLanesFace= nLanesFace;
	m_CustomInfo.m_bConnPlatFormFace= nPlatFormFace;
	m_CustomInfo.m_nChannelTypeFace= nChannelTypeFace;
	m_CustomInfo.m_nResverd3[0] = 0xf0;
	hr = WriteData();
	return hr;
}

/****************************************************************************************************
* @函数名称:	MakeDog
* @功  能：		制作视频分析记录仪加密狗
* @参  数：		
* @nChannelsPlate	牌识通道数
* @nChannelsFace 人脸识别通道数
* @nPlatForm			设备类型
* @返回值：		S_OK 操作成功 
* @说  明：		
****************************************************************************************************/
HRESULT CIVMSDogMaker::MakeDog( 
								 BYTE nChannelsPlate,
								 BYTE nChannelsFace,
								 BYTE nPlatForm)
{
	HRESULT hr = S_OK;
	memset( m_pMemoryBuf,0,sizeof(m_pMemoryBuf) );
	hr = CIVMSDog::ReadData();
	InitAllParam( );
	if( S_OK != hr  )
	{
		return hr;
	}
	m_InfoBody.m_bBind = FALSE;
	m_CustomInfo.m_nChannels = nChannelsPlate;
	m_CustomInfo.m_nChannelsFace= nChannelsFace;
	m_CustomInfo.m_bConnPlatForm= nPlatForm;
	m_CustomInfo.m_nResverd3[0] = 0xf0;
	hr = WriteData();
	return hr;
}

					
