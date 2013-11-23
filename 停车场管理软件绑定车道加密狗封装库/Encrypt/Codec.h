#ifndef  __CODEC_H__
#define  __CODEC_H__
#include "Encrypt.h"


class  CCodec
{

public:
#define KEY_STR "0123456789abcdefg"
#define MAX_CODE_KEY_LEN	18
#define MAX_CODE_LEN		1000

	CCodec();
		~CCodec();
		//装载密钥
		HRESULT InitializeKey( const BYTE * pKey,int nLen );
		//设置密源数据
		HRESULT SetCryptData( const BYTE * pString ,int nLen );
		//得到密源数据
		HRESULT GetCryptData( BYTE * const pString ,int &nLen );
		//加密函数
		HRESULT Encrypt( );
		//解密函数
		HRESULT Decrypt( );

	protected:
		//密钥
		BYTE m_pKey[ MAX_CODE_KEY_LEN ];
		//存放数据的空间
		BYTE m_pStr[ MAX_CODE_LEN ] ;
		//数据长度
		int  m_nStrLen;
};



#endif//__CODEC_H__