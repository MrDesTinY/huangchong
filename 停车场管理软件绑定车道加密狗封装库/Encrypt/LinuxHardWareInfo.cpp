#include "LinuxHardWareInfo.h"
CLinuxHardWareInfo::CLinuxHardWareInfo()
{
	memset( m_MacInfo,0,sizeof( m_MacInfo ) );
	memset( m_BiosInfo,0,sizeof( m_BiosInfo ) );
	memset( m_CpuInfo,0,sizeof( m_CpuInfo ) );
	memset( m_SerialInfo,0,sizeof( m_SerialInfo ) );
}
CLinuxHardWareInfo::~CLinuxHardWareInfo( )
{
	
}
BOOL CLinuxHardWareInfo::GetBios( )
{
	return TRUE;
}
BOOL CLinuxHardWareInfo::GetCpuID()
{
	return TRUE;
}
BOOL CLinuxHardWareInfo::GetSerialNum( )
{
	return TRUE;
}
BOOL CLinuxHardWareInfo::GetMAcAddr()
{
	return TRUE;
}