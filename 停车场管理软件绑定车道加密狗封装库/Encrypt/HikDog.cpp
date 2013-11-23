#include "HikDog.h"
#include "HikDate.h"
CHikDog::CHikDog()
{
	//InitAllParam();
}
CHikDog::~CHikDog()
{

}
/****************************************************************************************************
* @函数名称:	ReadData
* @功  能：		读取加密狗信息并解析成海康加密狗格式
* @参  数：		无
* @返回值：		S_OK 操作成功,否则 返回对应的错误代码
* @说  明：		读狗--->解密狗--->获取基本信息
****************************************************************************************************/
HRESULT CHikDog::ReadData()
{	

	if( S_OK != CDog::ReadData() )							//顺序3
	{
		//WriteLog("海康狗-读取数据失败");
		return -1;
	}

	if( S_OK != Decrypt() )									//顺序2
	{
		//WriteLog("海康狗-读取数据后解密失败");
		return -2;
	}

	if( S_OK != GetHikInfo() )								//顺序1
	{
		//WriteLog("海康狗-读取数据后获取基本信息失败");
		return -3;
	}
	
	return S_OK;
}
/****************************************************************************************************
* @函数名称:	WriteData
* @功  能：		将数据写入加密狗
* @参  数：		无
* @返回值：		S_OK 操作成功,否则 返回对应的错误代码
* @说  明：		获取基本信息--->加密数据--->写狗
****************************************************************************************************/
HRESULT CHikDog::WriteData()
{
	if( S_OK != SetHikInfo() )								//顺序1
	{		
		//WriteLog("海康狗-设置基本信息失败");
		return S_FALSE;
	}

	if( S_OK != Encrypt() )									//顺序2
	{
		//WriteLog("海康狗-加密数据失败");
		return S_FALSE;
	}

	if( S_OK != CDog::WriteData() )						//顺序3
	{
		//WriteLog("海康狗-写数据失败");
		return  S_FALSE;
	}
	return  S_OK;
}
/****************************************************************************************************
* @函数名称:	WriteData
* @功  能：		解析基本信息
* @参  数：		无
* @返回值：		S_OK 成功
* @说  明：		从已读取信息的缓冲区中解析基本信息
****************************************************************************************************/

HRESULT CHikDog::GetHikInfo( )
{
	memcpy( &m_InfoHeader,m_pMemoryBuf,sizeof( m_InfoHeader ) );
	memcpy( &m_InfoBody,m_pMemoryBuf+m_InfoHeader.m_BaseInfoAddr ,sizeof( m_InfoBody ) );
	return S_OK;
}
/****************************************************************************************************
* @函数名称:	SetHikInfo
* @功  能：		解析基本信息
* @参  数：		无
* @返回值：		S_OK 成功
* @说  明：		将基本信息写入待写缓冲区
****************************************************************************************************/

HRESULT CHikDog::SetHikInfo( )
{
	HRESULT hr = S_OK;
	memcpy( m_pMemoryBuf,&m_InfoHeader,sizeof( m_InfoHeader ) );
	memcpy( m_pMemoryBuf+m_InfoHeader.m_BaseInfoAddr,&m_InfoBody,sizeof( m_InfoBody ) );
	return  hr;
}

/****************************************************************************************************
* @函数名称:	Encrypt
* @功  能：		对待写缓冲区进行加密
* @参  数：		无
* @返回值：		S_OK 操作成功,否则 返回对应的错误代码
* @说  明：		
****************************************************************************************************/
HRESULT CHikDog::Encrypt()
{
	HRESULT hr = S_OK;

	//设置加密对象
	hr = SetCryptData( m_pMemoryBuf,m_BufLen );
	if( S_OK != hr )
	{
		//WriteLog("海康狗--设置加密数据错误");
		return hr;
	}
	//开始加密
	hr = CCodec::Encrypt();
	if( S_OK != hr )
	{
		//WriteLog("海康狗--加密数据错误");
		return hr;
	}
	//获取加密后的信息
	hr = GetCryptData( m_pMemoryBuf ,m_BufLen );
	if( hr != S_OK )
	{
		//WriteLog("海康狗--获取加密后的数据错误");
		return hr;
	}

	return  hr;
}
/****************************************************************************************************
* @函数名称:	Decrypt
* @功  能：		对读取到的数据缓冲区进行解密
* @参  数：		无
* @返回值：		S_OK 操作成功,否则 返回对应的错误代码
* @说  明：		
****************************************************************************************************/
HRESULT CHikDog::Decrypt()
{
	HRESULT hr = S_OK;
	//设置解密对象
	hr = SetCryptData( m_pMemoryBuf,m_BufLen );
	if( S_OK != hr )
	{
		//WriteLog("海康狗--设置解密数据失败");
		return hr;
	}
	//数据解密
	hr = CCodec::Decrypt();
	if( S_OK != hr )
	{
		//WriteLog("海康狗--解密数据失败");
		return hr;
		
	}
	//获取解密后的信息
	hr = GetCryptData( m_pMemoryBuf,m_BufLen );
	if( S_OK != hr )
	{
		//WriteLog("海康狗--获取解密后的数据失败");
		return hr;
	}

	return  hr;
}
HRESULT CHikDog::InitAllParam( )
{
	memset( &m_InfoBody,0,sizeof(  m_InfoBody ) );
	memset( &m_InfoHeader,0,sizeof( m_InfoHeader ) );
	m_InfoHeader.m_BaseInfoAddr = 0x00 +sizeof( INFO_HEADER );
	m_InfoHeader.m_BaseInfoLen = sizeof( BASEINFO_BODY );
	m_InfoHeader.m_CustomInfoAddr = m_InfoHeader.m_BaseInfoAddr + m_InfoHeader.m_BaseInfoLen;
	m_InfoHeader.m_CustomInfoLen = 0;
	memset( m_InfoHeader.m_Resver1,0,sizeof( m_InfoHeader.m_Resver1 ) );
	//基本信息
	memcpy( m_InfoBody.m_CompanyName,HIK_COMPANY_NAME,4 );
	memset( m_pStr,0,sizeof( m_pStr ) );
	//数据头+海康信息+视频电警信息+校验位
	m_BufLen = sizeof( m_InfoHeader )+sizeof( m_InfoBody )  + 1;//

	return S_OK;
}
/****************************************************************************************************
* @函数名称:	CheckDogBind
* @功  能：		检验硬件是否被绑定
* @参  数：		无				
* @返回值：		S_OK 已经被本机绑定  -1 读取加密狗失败 -2 未绑定 
				-3 获取硬件信息失败 -4 被其他的电脑绑定			
* @说  明：		
****************************************************************************************************/
HRESULT CHikDog::CheckDogBind( )
{
	//解析内容  判断是否绑定
	if( S_OK != ReadData() )
	{
		//WriteLog("海康狗-检验绑定时读取狗失败");
		return -1;
	}
	/*if(  m_InfoBody.m_bBind != TRUE )
	{
		WriteLog("狗未绑定");
		return -2;
	}*/
	//如果获取本地硬件ID失败
	/*if( S_OK != GetHardWareInfo( ) )
	{
		WriteLog("海康狗-检验绑定时获取硬件信息失败");
		return -3;
	}
	//如果被其他的电脑绑定
	/*if( 0 != memcmp( m_HardWareInfo,m_InfoBody.m_HareWareID,MAX_INFO_LEN ) )
	{
		WriteLog("海康狗-绑定的硬件与本机硬件ID不匹配");
		return -4;
	}*/
	/*if( TRUE == ISDateOff( m_InfoBody.m_BindDate,m_InfoBody.m_UseDays ) )
	{
		WriteLog("海康狗-使用日期到期");
		return -5;
	}*/
	return  S_OK;
	
}
/****************************************************************************************************
* @函数名称:	BindDog
* @功  能：		将加密狗与本机电脑进行绑定
* @参  数：		无
* @返回值：		S_OK 操作成功,否则 返回对应的错误代码
* @说  明：		写 日期-绑定标记-硬件ID-海康信息-加密-写狗
****************************************************************************************************/
HRESULT CHikDog::BindDog( )
{
	HRESULT hr = S_OK;
	
	hr = CHikDog::CheckDogBind( );
	//如果已经绑定 -- 返回S_OK
	if( S_OK == hr  )
	{
		return hr;
	}
	///如果不是未绑定
	if( -2 != hr )
	{
		return hr;
	}
	//如果未绑定
	 
	struct tm  when; 
	time_t  now; 
	WORD  nYear = 0;
	//标记需要绑定
	m_InfoBody.m_bBind = TRUE;
	
	//记录绑定日期
	time(   &now   ); 
	localtime_s( &when,&now   );  
	//2011  08  20 [2011(2字节)][08(1字节)][20(1字节)] 共4个字节
	nYear = when.tm_year+1900;
	m_InfoBody.m_BindDate[0] = nYear/100;
	m_InfoBody.m_BindDate[1] = nYear%100;
	m_InfoBody.m_BindDate[2] = when.tm_mon+1;
	m_InfoBody.m_BindDate[3] = when.tm_mday;
	
	//标记硬件ID
	/*hr = GetHardWareInfo();
	if( S_OK != hr )
	{
		return hr;
	}
	memcpy( m_InfoBody.m_HareWareID,m_HardWareInfo,MAX_INFO_LEN );*/
	//设置
	hr = SetHikInfo();
	if( S_OK != hr )
	{
		return  hr;
	}
	//写狗
	hr = WriteData();
	if( S_OK != hr )
	{
		return  hr;
	}

	return  hr;
}
/****************************************************************************************************
* @函数名称:	ISDateOff
* @功  能：		判断是权限否过期
* @参  数：		TRUE
* @返回值：		TRUE 过期  FALSE 未过期
* @说  明：		
****************************************************************************************************/
BOOL CHikDog::ISDateOff( BYTE nDate[4] ,WORD nDays )
{
	struct tm  tmNow; 
	time_t  nNow; 
	time( &nNow ); 
	localtime_s( &tmNow,&nNow);  
	tmNow.tm_year += 1900;

	date dNow,dBegin;
	dNow.year = tmNow.tm_year;
	dNow.month = tmNow.tm_mon+1;
	dNow.day = tmNow.tm_mday;
	
	dBegin.year = 100*nDate[0]+nDate[1];
	dBegin.month = nDate[2];
	dBegin.day = nDate[3];
	int nDayElaps = DaysBetweenDate( dBegin,dNow );
	if(!( dNow.year == dBegin.year &&
		dNow.month == dBegin.month && 
		dNow.day == dBegin.day ) )
	{
		if( nDayElaps >= nDays || 0 == nDayElaps )
		{
			return TRUE;
		}	
	}
	
	return FALSE;
}