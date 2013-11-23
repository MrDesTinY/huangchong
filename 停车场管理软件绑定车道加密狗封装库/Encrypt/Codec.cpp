#include "Codec.h"
CCodec::CCodec()
{
	m_nStrLen = 0;
	memset( m_pStr,0,sizeof( m_pStr ) );
	memset( m_pKey,0,sizeof( m_pKey ) );
	memcpy( m_pKey,KEY_STR,MAX_CODE_KEY_LEN);
}

CCodec::~CCodec()
{
	
}
/****************************************************************************************************
* @函数名称:	SetData
* @功  能：		设置加密解密过程中的密钥
* @参  数：		
* @pKey			密钥指针
* @nLen			密钥长度					
* @返回值：		S_OK 操作成功,否则 返回对应的错误代码
* @说  明：		
****************************************************************************************************/
HRESULT CCodec::InitializeKey( const BYTE * pKey,int nLen )
{
	if( NULL == pKey )
	{
		return CODEC_INITKEY_ERROR_BUF_OUT;
	}
	if(	 nLen <=0 || nLen >=MAX_CODE_KEY_LEN )
	{
		return CODEC_INITKEY_ERROR_BUF_OUT;
	}
	memcpy( m_pKey,pKey,nLen );
	return S_OK;
}
/****************************************************************************************************
* @函数名称:	SetEncryptData
* @功  能：		设置需要加密的数据源
* @参  数：		
* @pString		数据源指针
* @nLen         数据源长度
* @返回值：		S_OK 操作成功,否则 返回对应的错误代码
* @说  明：		将要加密的对象放入加密buf中
****************************************************************************************************/

HRESULT CCodec::SetCryptData( const BYTE * pString ,int nLen )
{
	if( NULL == pString )
	{
		return CODEC_CRYPT_EROR_BUF_NULL;
	}
	if( nLen <=0 || nLen >=MAX_CODE_LEN )
	{
		return CODEC_CRYPT_ERROR_BUF_OUT;
	}
	m_nStrLen = nLen;
	memcpy( m_pStr,pString,nLen );
	return S_OK;
}
/****************************************************************************************************
* @函数名称:	GetCryptData
* @功  能：		获取数据
* @参  数：		
* @pString		用于输出的数据指针
* @nLen         用于输出的数据长度
* @返回值：		S_OK 操作成功,否则 返回对应的错误代码
* @说  明：		从加密BUF中获取数据
****************************************************************************************************/
HRESULT CCodec::GetCryptData( BYTE * const pString ,int &nLen )
{
	if( NULL == pString )
	{
		return -1;
	}
	memcpy( pString,m_pStr,m_nStrLen );
	nLen = m_nStrLen;
	return S_OK;
}
/****************************************************************************************************
* @函数名称:	Encrypt
* @功  能：		加密数据
* @参  数：		无
* @返回值：		S_OK 操作成功,否则 返回对应的错误代码
* @说  明：		将数据源中的数据进行加密 
****************************************************************************************************/
HRESULT  CCodec::Encrypt( )
{
	BYTE nData = 0;
	for( int i=0;i<MAX_CODE_LEN;i++ )
	{
		nData = m_pKey[ i%MAX_CODE_KEY_LEN ];
		m_pStr[i] += nData;
	}

	return S_OK;
}
/****************************************************************************************************
* @函数名称:	Decrypt
* @功  能：		加密数据
* @参  数：		无
* @返回值：		S_OK 操作成功,否则 返回对应的错误代码
* @说  明：		将数据源中的数据进行解密 
				函数调用顺序    InitializeKey->SetData->Encrypt ->Decrypt  	
****************************************************************************************************/
HRESULT CCodec:: Decrypt( )
{
	BYTE nData = 0;
	for( int i=0;i<MAX_CODE_LEN;i++ )
	{
		nData = m_pKey[ i%MAX_CODE_KEY_LEN ];
		m_pStr[i] -= nData;
	}
	return  S_OK;
}