#include "IVMSDog.h"
CIVMSDog::CIVMSDog()
{
	InitAllParam();
};

CIVMSDog::~CIVMSDog()
{

}
/****************************************************************************************************
* @函数名称:	ReadData
* @功  能：		视频分析记录仪读加密信息
* @参  数：		无
* @返回值：		S_OK 操作成功,否则 返回对应的错误代码
* @说  明：		先读海康信息--->解析视频分析记录仪的信息
****************************************************************************************************/
HRESULT CIVMSDog::ReadData()
{
	HRESULT hr = CHikDog::ReadData();
	if( S_OK != hr )
	{
		return -1;
	}
	hr = GetCustomInfo( );
	if( S_OK != hr  )
	{
		return -2;
	}
	return S_OK;
}
/****************************************************************************************************
* @函数名称:	ReadData
* @功  能：		视频分析记录仪读写加密信息
* @参  数：		无
* @返回值：		S_OK 操作成功,否则 返回对应的错误代码
* @说  明：		向待写区设置视频分析记录仪的信息-->写读海康信息---->写狗
****************************************************************************************************/
HRESULT CIVMSDog::WriteData()
{
	HRESULT hr = S_OK;
	hr = SetCustomInfo( );
	if( S_OK != hr  )
	{
		return hr;
	}

	hr = CHikDog::WriteData();
	if( S_OK != hr )
	{
		return hr;
	}
	
	return S_OK;
}
/****************************************************************************************************
* @函数名称:	GetCustomInfo
* @功  能：		获取定制信息--视频分析记录仪的参数
* @参  数：		无
* @返回值：		S_OK 操作成功 
* @说  明：		从解密信息buf中抽取出定制信息(视频分析记录仪的参数)
****************************************************************************************************/
HRESULT CIVMSDog::GetCustomInfo( )
{
	HRESULT hr = S_OK;
	memcpy( &m_CustomInfo,m_pMemoryBuf+m_InfoHeader.m_CustomInfoAddr,sizeof( m_CustomInfo ) );
	return hr;
}
/****************************************************************************************************
* @函数名称:	SetCustomInfo
* @功  能：		设置定制信息
* @参  数：		无
* @返回值：		S_OK 操作成功 
* @说  明：		向待写缓冲区中设置视频分析记录仪的参数
****************************************************************************************************/
HRESULT CIVMSDog::SetCustomInfo( )
{
	HRESULT hr = S_OK;
	memcpy( m_pMemoryBuf+m_InfoHeader.m_CustomInfoAddr,&m_CustomInfo,sizeof( m_CustomInfo ) );
	return hr;
}
HRESULT CIVMSDog::InitAllParam( )
{
	HRESULT hr = S_OK;
	hr = CHikDog::InitAllParam();
	m_InfoHeader.m_CustomInfoLen = (BYTE)sizeof( m_CustomInfo );
	memset( &m_CustomInfo,0,sizeof( m_CustomInfo ) );
	//数据头+海康信息+视频电警信息+校验位
	m_BufLen = sizeof( m_InfoHeader )+sizeof( m_InfoBody ) +sizeof( m_CustomInfo ) + 1;//
	return hr;
}