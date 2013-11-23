#include "Log.h"
CLog::CLog()
{
	m_nMsgLen = 0;
	m_pFile = NULL;
	memset( m_pMsgBuf,0,sizeof( m_pMsgBuf ) );
}
CLog::~CLog()
{
	if( m_pFile != NULL )
	{
		fclose ( m_pFile );
	}
}
/****************************************************************************************************
 * @函数名称:	WriteError
 * @功  能：	将错误信息写入文件
 * @参  数：	无
 * @返回值：	S_OK  成功  S_FALSE 失败
 * @说  明：	内向使用函数 不对外开放
 ****************************************************************************************************/
HRESULT  CLog::WriteError(  )
{
	HRESULT hr = S_FALSE;

	if( NULL != m_pFile )
	{
		fprintf_s( m_pFile,m_pMsgBuf );
		hr = S_OK;
	}

	return hr;
}
/****************************************************************************************************
* @函数名称:PostError
* @功  能：	传入错误消息
* @参  数：	
* @nError	错误代码
* @返回值：	S_OK  成功  S_FALSE 失败
* @说  明：	内向使用函数 不对外开放
****************************************************************************************************/
HRESULT  CLog::PostError( HRESULT nError )
{

	HRESULT hr = S_FALSE;
	//先翻译消息
	hr = TranslateError( nError  );
	if( S_OK == hr )
	{
		//再传递消息	
		hr = WriteError( );
	}
	
	return hr;
}
/****************************************************************************************************
* @函数名称:TranslateError
* @功  能：	翻译错误消息
* @参  数：	
* @nError	错误代码
* @返回值：	S_OK  成功  S_FALSE 失败
* @说  明：	内向使用函数 不对外开放
****************************************************************************************************/
HRESULT CLog::TranslateError( HRESULT nError )
{
	int nLen = 0,nIndex = 0;
	HRESULT hr = S_FALSE;
	static char * szMsg[3] = {
							"操作成功",
							"内存溢出",
							"参数错误"};
	//////////////////////////////////////////////////////////////////////////


	//TODO:在这里实现

	//////////////////////////////////////////////////////////////////////////
	if( nIndex < 0 && nIndex > 3 )
	{
		return hr;
	}
	
	nLen = (int)strlen( szMsg[ nIndex ] );
	if( nLen >=  MAX_LOG_MSG_SIZE )
	{
		return  hr;
	}
	strcpy_s( m_pMsgBuf,nLen,szMsg[ nIndex ] );
	
	m_nMsgLen = nLen;
	
	hr = S_OK;
	
	return hr;  
}
/****************************************************************************************************
* @函数名称:PostError
* @功  能：	翻译错误消息
* @参  数：	
* @nError	错误代码
* @返回值：	S_OK  成功  S_FALSE 失败
* @说  明：	内向使用函数 不对外开放
****************************************************************************************************/
HRESULT CLog::WriteLog( const char * pChError )
{
	DWORD dwFileSize = 0;
	struct tm  when; 
	time_t  now; 
	WORD  nYear = 0;
	HRESULT hr = S_OK;
	if( NULL == m_pFile )
	{
		fopen_s( &m_pFile, LOG_FILE_NAME,"a");
		if( NULL == m_pFile )
		{
			return -1;
		}
	}

	fseek( m_pFile,0,SEEK_END);
	dwFileSize = ftell( m_pFile );
	//如果文件大小超过限制则清空
	if( dwFileSize > MAX_LOG_SIZE )
	{
		fclose( m_pFile );
		fopen_s( &m_pFile,LOG_FILE_NAME,"w");
		if( NULL == m_pFile ) 
		{
			return -2;
		}
	}
	time(   &now   ); 
	localtime_s( &when,&now   );  
	nYear = when.tm_year+1900;
	fprintf_s( m_pFile,"%4.4d%2.2d%2.2d %2.2d:%2.2d:%2.2d %s\n",		
				when.tm_year,when.tm_mon,when.tm_yday,
				when.tm_hour,when.tm_min,when.tm_sec,
				pChError );
	fclose( m_pFile );	
	m_pFile = NULL;
	return hr;
}
