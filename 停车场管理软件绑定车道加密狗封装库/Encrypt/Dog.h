#ifndef __DOG_H__
#define __DOG_H__
#include "gsmh.h"
#include "Encrypt.h"
#include "Log.h"

class   CDog:public CLog
{
public:
	//整个加密狗最大内存容量
	#define MAX_DOG_MEMORY_LEN  300
	//可供写入的私有字节数
	#define MAX_PRIVATE_MEMORY_LEN 0x80
		CDog();	
		~CDog();
	protected:
		//读取数据
		virtual HRESULT ReadData( );
		//写数据
		virtual HRESULT WriteData(  );
		//校验和
		virtual HRESULT CheckSum( );
		//设置校验和
		virtual HRESULT SetSum( );

		BYTE  m_pMemoryBuf[ MAX_DOG_MEMORY_LEN ];
		int  m_BufLen;
};

#endif //__DOG_H__