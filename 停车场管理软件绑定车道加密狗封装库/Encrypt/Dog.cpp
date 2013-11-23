#include "Dog.h"

short int DogAddr,DogBytes;
unsigned long DogPassword, NewPassword,DogResult;
unsigned char DogCascade;
void * DogData;

CDog::CDog()
{
	memset( m_pMemoryBuf,0,sizeof( m_pMemoryBuf ) );
	m_BufLen = 0;
}
CDog::~CDog()
{

}
/****************************************************************************************************
* @函数名称:	WriteData
* @功  能：		往加密狗写数据
* @参  数：		无
* @返回值：		S_OK 操作成功,否则 返回对应的错误代码
* @说  明：		往加密狗前128内存写数据
****************************************************************************************************/
HRESULT CDog::WriteData()
{	
	DogPassword = 0;
	DogBytes = MAX_PRIVATE_MEMORY_LEN;
	DogData = &m_pMemoryBuf[0];
	DogAddr = 0x00;
	m_BufLen = DogBytes;
	//对数据进行和校验保证其不被改动
	if( S_OK != SetSum() )
	{
		//WriteLog( "写加密狗-校验和失败\r\n" );
		return S_FALSE;
	}
	//写数据
	if( 0 != WriteDog() )
	{
		//WriteLog( "写加密狗-写狗失败\r\n" );
		return S_FALSE;
	}
	return S_OK;
}
/****************************************************************************************************
* @函数名称:	ReadData
* @功  能：		从加密狗中读取数据 并取需要的 MAX_PRIVATE_MEMORY_LEN 长度的数据放入缓存中
* @参  数：		无
* @返回值：		S_OK 操作成功,否则 返回对应的错误代码
* @说  明：		读取的数据是普通编辑区--前128字节
****************************************************************************************************/
HRESULT CDog::ReadData()
{
	DogAddr = 0;
	DogBytes = MAX_PRIVATE_MEMORY_LEN;
	DogData = &m_pMemoryBuf[0];
	//读取数据
	if( 0 != ReadDog() )
	{	
		//WriteLog( "读取加密狗-读取失败\r\n" );
		return -1;
	}
	m_BufLen = DogBytes;
	//和校验 保证数据不是胡乱写上去的
	/*if( S_OK != CheckSum() )
	{
		WriteLog( "读取加密狗-校验和失败\r\n" );
		return -2;
	}*/

	return S_OK;
}
/****************************************************************************************************
* @函数名称:	CheckSum
* @功  能：		对一元数组进行和校验
* @参  数：		无
* @返回值：		S_OK 操作成功,否则 返回对应的错误代码
* @说  明：		要校验和之前需要对m_pMemoryBuf进行复值，也就是说需要成功调用 ReadData 
				对buf中的指定信息进行和校验 --前128个字节
****************************************************************************************************/
HRESULT CDog::CheckSum( )
{
	BYTE nSum = 0;
	if( 0 >= m_BufLen || MAX_PRIVATE_MEMORY_LEN <m_BufLen )
	{
		return -1;
	}

	for( int i=0;i<m_BufLen-2;i++ )
	{
		nSum += m_pMemoryBuf[ i ];
	}

	if( nSum != m_pMemoryBuf[ m_BufLen-1] )
	{
		return -2;
	}
	return S_OK;
}
/****************************************************************************************************
* @函数名称:	SetSum
* @功  能：		对一元数组设置和校验
* @参  数：		无
* @返回值：		S_OK 操作成功,否则 返回对应的错误代码
* @说  明：		对buf中的指定信息 --前128个字节设置校验和
****************************************************************************************************/
HRESULT CDog::SetSum( )
{
	BYTE nSum = 0;
	if( 0 >= m_BufLen || MAX_PRIVATE_MEMORY_LEN <m_BufLen )
	{
		return S_FALSE;
	}

	for( int i=0;i<m_BufLen-2;i++ )
	{
		nSum += m_pMemoryBuf[ i ];
	}
	 m_pMemoryBuf[ m_BufLen-1] = nSum;
	return S_OK;
}