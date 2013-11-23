#include "HardWareInfo.h"

CHardWareInfo::CHardWareInfo()
{

}

CHardWareInfo::~CHardWareInfo()
{

}
/****************************************************************************************************
* @函数名称:	GetHardWareInfo
* @功  能：		读取硬件序列号
* @参  数：		无
* @返回值：		S_OK 操作成功,否则 返回对应的错误代码
* @说  明：		硬件序列号由	MAX_INFO_LEN 字节的数据构成 分别由MAX_INFO_LEN/4长度的数据存储
				cup序列号、硬盘序列号、主板序列号、网卡序列号
****************************************************************************************************/
HRESULT CHardWareInfo::GetHardWareInfo()
{
	BYTE * pAddr = &m_HardWareInfo[0];
	UINT nLen = MAX_INFO_LEN/4;
	if( FALSE == GetCpuID() )
	{
		return -1;
	}
	if( FALSE == GetSerialNum() )
	{
		return -2;
	}
	if( FALSE == GetBios() )
	{
		return -3;
	}
	if( FALSE == GetMAcAddr() )
	{
		return -4;
	}
	memcpy( pAddr,m_CpuInfo,nLen );
	pAddr += nLen;
	memcpy( pAddr,m_SerialInfo,nLen );
	pAddr += nLen;
	memcpy( pAddr,m_BiosInfo,nLen );
	pAddr += nLen;
	memcpy( pAddr,m_MacInfo,nLen );
	pAddr += nLen;
	return S_OK;
}