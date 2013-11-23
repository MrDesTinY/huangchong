#ifndef __HARDWARE_INFO_H__
#define __HARDWARE_INFO_H__


#ifdef WIN32
	#include "WindowsHardWareInfo.h"
class CHardWareInfo:public CWindowsHardWareInfo
#else
#ifdef LINUX
	#include "LinuxHardWareInfo.h"
class CHardWareInfo:public CLinuxHardWareInfo
#endif 
#endif
{
	#define MAX_INFO_LEN	32

	public:
		CHardWareInfo();
		virtual ~CHardWareInfo();
		virtual HRESULT GetHardWareInfo();
	protected:
		//存储获取到的信息
		BYTE  m_HardWareInfo[ MAX_INFO_LEN ];
	 
};







#endif//__HARDWARE_INFO_H__