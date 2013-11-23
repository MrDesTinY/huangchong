#ifndef __LINUX_HARDWARE_INFO_H__
#define __LINUX_HARDWARE_INFO_H__

#include "Encrypt.h"

class CLinuxHardWareInfo
{
	public:
		CLinuxHardWareInfo();
		~CLinuxHardWareInfo();
		BOOL GetBios( );
		BOOL GetCpuID();
		BOOL GetSerialNum( );
		BOOL GetMAcAddr();
	protected:
		BYTE m_MacInfo[ 1000 ];
		BYTE m_BiosInfo[ 1000 ];
		BYTE m_CpuInfo[ 1000 ];
		BYTE m_SerialInfo[ 1000 ];
};
#endif //__LINUX_HARDWARE_INFO_H__