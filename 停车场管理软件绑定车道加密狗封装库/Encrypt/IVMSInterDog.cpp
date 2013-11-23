#include "IVMSInterDog.h"

CIVMSInterDog::CIVMSInterDog()
{

}
CIVMSInterDog::~CIVMSInterDog()
{

}

HRESULT CIVMSInterDog::GetBindHardWareInfo( BYTE * const pStr,int  nStrLen )
{

	return  S_OK;
}
//检测加密狗是否还在
HRESULT CIVMSInterDog::CheckDogAlive()
{
	return S_OK;
}
HRESULT CIVMSInterDog::GetDogAccess( CUSTOMINFO &nCustomInfo )
{
	memcpy( &nCustomInfo,&m_CustomInfo,sizeof( nCustomInfo ) );
	return S_OK;
}
/****************************************************************************************************
* @函数名称:	CheckDogBind
* @功  能：		分析加密狗绑定行为
* @参  数：		
* @返回值：		S_OK 已绑定 -2  狗未绑定-4 已经绑定其它电脑 -5 狗过期 其它--读取加密狗错误
* @说  明：		制作狗的时候，默认是未绑定的，当程序第一次调用此函数的时候，会提示用户是否绑定，如果选择绑定
				则加密狗永远与此电脑绑定
****************************************************************************************************/
HRESULT CIVMSInterDog::CheckDogBind( )
{
	HRESULT hr =  CHikDog::CheckDogBind(  );
	if( S_OK == hr )
	{
		///Do Nothing
	}
	else if ( -1 == hr || -3 == hr )
	{
		#ifdef LINUX
				printf("读取加密信息失败");
		#else 
		#ifdef WIN32
				/*MessageBox(NULL,"无法读取加密狗，请确认是否插入海康加密狗!",
					"加密狗提示信息",MB_OK|MB_ICONWARNING );*/
		#endif
		#endif
	}
	else if ( -2 == hr )
	{
		int nRet = 0;
		#ifdef LINUX
				printf("读取加密信息失败");
		#else 
		#ifdef WIN32
				

				/*nRet= MessageBox(NULL,"是否确认绑定此加密狗?\n一旦绑定此电脑，其它电脑将无法使用此狗!",
					"加密狗提示信息",MB_YESNO|MB_ICONQUESTION );
				if( IDYES == nRet )
				{
					hr = BindDog();
					if( S_OK == hr )
					{
						MessageBox(NULL,"加密狗已绑定本机",
							"加密狗提示信息",MB_OK|MB_ICONWARNING);
					}
					else
					{
						MessageBox(NULL,"加密狗绑定本机失败",
							"加密狗提示信息",MB_OK|MB_ICONWARNING);
					}
				}*/
		#endif
		#endif
	}
	else if ( -4 == hr )
	{
		#ifdef LINUX
				printf("此狗已经被别的电脑绑定");
		#else 
		#ifdef WIN32
				/*MessageBox(NULL,"您现在使用的加密狗已经绑定其它电脑，不能在此机上使用!",
					"加密狗提示信息",MB_OK|MB_ICONWARNING);*/
		#endif 
		#endif
	}
	else if ( -5 == hr )
	{
		#ifdef LINUX
				printf("加密狗已过期");
		#else 
		#ifdef WIN32
				/*MessageBox(NULL,"您目前使用的加密狗已经过期，请与海康联系!",
					"加密狗提示信息",MB_OK|MB_ICONWARNING);*/
		#endif
		#endif
	}
	else
	{
		#ifdef LINUX
			printf("无法识别的加密狗");
		#else 
		#ifdef WIN32
				/*MessageBox(NULL,"无法读取加密狗，请确认是否插入海康加密狗!",
					"加密狗提示信息",MB_OK|MB_ICONWARNING);*/
		#endif
		#endif
	}	
	return hr;
}

