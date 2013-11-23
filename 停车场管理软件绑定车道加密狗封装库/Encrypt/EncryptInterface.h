//作者:周勇
//日期:2011-08-18
//内容:加密狗方案接口头文件
//版权信息：CopyRight HIKVISION 
#ifndef ___ENCRYPT_INTERFACE_H__
#define ___ENCRYPT_INTERFACE_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef LINUX
	
#else
	#include <Windows.h>
#endif
//////////////////////////////////////////////////////////////////////////
//数据类型定义
//////////////////////////////////////////////////////////////////////////

#ifdef LINUX
	#ifndef NULL
	#define  NULL 0
	#endif 

	#ifndef S_OK  
	#define S_OK  1
	#endif 

	#ifndef S_FALSE  
	#define S_FALSE   0
	#endif 

	#ifndef TRUE  
	#define TRUE   1
	#endif 

	#ifndef FALSE  
	#define FALSE   0
	#endif 

	#ifndef BYTE
		typedef unsigned char BYTE;
	#endif 

	#ifndef  WORD
		typedef  unsigned short WORD;
	#endif

	#ifndef DWORD
		typedef unsigned long DWORD;
	#endif 

	#ifndef HRESULT
		typedef int  HRESULT;
	#endif

	#ifndef BOOL
		typedef unsigned char BOOL;
	#endif

	#ifndef UINT
		typedef unsigned int UINT;
	#endif 

	#ifndef USHORT
		typedef unsigned short  USHORT;
	#endif 

	#ifndef TCHAR 
		typedef char TCHAR, *PTCHAR;
	#endif
#endif




//用户组
enum  USER_GROUP
{
	USERGROUP_ADMIN,//超级管理员组
	USERGROUP_CUSTOMER,	//普通用户组
	USERGROUP_DEVELOP	//开发人员组
};
//产品组
enum PRODUCT_GROUP
{
	PRODUCTGROUP_VIMS,//视频电警产品
	PRODUCTGROUP_PARK//停车场产品
};
//接收视频电警图片和车流量信息的平台
enum  PLATFORM_TYPE
{
	PLATEFORM_NONE,//不连接平台
	PLATEFORM_8620//8620平台
};
//错误代码
#define  CODEC_ERR	1000
#define  DOG_ERR	2000
enum  ERROR_CODE
{
	CODEC_INITKEY_ERROR_BUF_NULL = CODEC_ERR,//CCodec::InitializeKey缓冲区为空
	CODEC_INITKEY_ERROR_BUF_OUT,//CCodec::InitializeKey缓冲区溢出
	CODEC_CRYPT_EROR_BUF_NULL,// CCodec::SetCryptData缓冲区为空
	CODEC_CRYPT_ERROR_BUF_OUT,// CCodec::SetCryptData缓冲区溢出
	CODEC_GETCRYPTDATA_ERROR_1		,//CCodec::GetCryptData
	CODEC_ENCRYPT_ERROR_1,//CCodec::Encrypt
	CODEC_DENCRYPT_ERROR_1//CCodec::DEcrypt
};

enum CRYPT_STATE
{
	STATE_ENCRYPT,//加密状态
	STATE_DECRYPT,//解密状态
	STATE_UNINIT//未初始化状态

};
enum DOG_STATE
{

	STATE_BIND = 2, //狗处于绑定状态
	STATE_NOT_BIND=3//狗处于未绑定状态

};
//////////////////////////////////////////////////////////////////////////
//导出DLL的设置定义
//////////////////////////////////////////////////////////////////////////

#ifndef ENCRYPT_API
	#ifdef  ENCRYPT_EXPORTS
		#define ENCRYPT_API __declspec(dllexport)
	#else
		#define ENCRYPT_API __declspec(dllimport)
	#endif
#endif

//////////////////////////////////////////////////////////////////////////
//函数接口定义
//////////////////////////////////////////////////////////////////////////



/****************************************************************************************************
* @函数名称:MakePRDog
* @功  能：制作加密狗
* @参  数：
* @wDays		拥有的使用日期6
* @nChannels	可配置的最大通道数   
* @nLanes		可配置的最大车道数
* @nPlatForm   	连接平台  的类型  TRUE 是 FALSE 否
* @nChannelType	通道类型	
* @pResverd		预留字段
* @返回值：		S_OK 制作加密狗成功  S_FALSE 制作加密狗失败
* @说  明：		此函数针对MicroDog进行操作,在做狗的过程中请确认狗已经插入计算机并安装了对应的驱动
****************************************************************************************************/
ENCRYPT_API	HRESULT _cdecl MakePRDog(	 WORD wDays,
							   BYTE nChannels,
							   BYTE nLanes,
							   BYTE nPlatForm,
							   BYTE nChannelType );
/****************************************************************************************************
* @函数名称:MakeFRDog
* @功  能：制作加密狗
* @参  数：
* @wDays		拥有的使用日期6
* @nChannels	可配置的最大通道数   
* @nLanes		可配置的最大车道数
* @nPlatForm   	连接平台  的类型  TRUE 是 FALSE 否
* @nChannelType	通道类型	
* @pResverd		预留字段
* @返回值：		S_OK 制作加密狗成功  S_FALSE 制作加密狗失败
* @说  明：		此函数针对MicroDog进行操作,在做狗的过程中请确认狗已经插入计算机并安装了对应的驱动
****************************************************************************************************/
ENCRYPT_API	HRESULT _cdecl MakeFRDog(	 WORD wDays,
							   BYTE nChannels,
							   BYTE nLanes,
							   BYTE nPlatForm,
							   BYTE nChannelType );

ENCRYPT_API HRESULT _cdecl MakeDog(	BYTE nChannelsPlate,
					   BYTE nChannelsFace,
					   BYTE nPlatForm);
/****************************************************************************************************
* @函数名称:	CheckDogBind
* @功  能：		判断加密狗是否已经绑定
* @参  数：		无
* @返回值：		S_OK 已经绑定  S_FALSE 未绑定 
* @说  明：		调用此函数可以读取加密狗的绑定信息，判断此狗是否已经和别的硬件绑定
				如果已经绑定，可以通过调用 GetBindHardWareInfo来获取绑定的硬件信息

****************************************************************************************************/
ENCRYPT_API HRESULT _cdecl CheckDogBind( void );


/****************************************************************************************************
* @函数名称:	StatusValidate
* @功  能：		判断加密狗是否存在
* @参  数：		无
* @返回值：		0 存在  1 不存在 
* @说  明：		判断狗是否存在，0为存在并已经绑定，1为不存在或者没绑定
****************************************************************************************************/
ENCRYPT_API HRESULT _cdecl StatusValidate( void );




/****************************************************************************************************
* @函数名称:	GetBindHardWareInfo
* @功  能：		获取绑定的硬件信息，以防止用户在混淆加密狗的情况下无法识别加密狗的绑定对象
* @参  数：		
* @pStr			输出参数，返回硬件ID
* @nStrLen      标记可写入的信息量最大长度
* @返回值：		S_OK 获取成功  S_FALSE 获取识别 
* @说  明：		如果加密狗未绑定，则会返回失败，如果加密狗已经绑定，正常情况下会由pStr传出硬件ID号
****************************************************************************************************/
ENCRYPT_API HRESULT _cdecl GetBindHardWareInfo( BYTE * const pStr,int  nStrLen );

/****************************************************************************************************
* @函数名称:GetDogPRAccess
* @功  能：		获取加密狗中的车牌识别信息
* @参  数： 
* @nPRChannels   	输出可配置的最大通道数
* @nPRLanes		输出可配置的最大车道数
* @bPRConnPlatForm输出是否可连接平台 TRUE 是 FALSE 否
* @nPRChannelType	输出通道类型
* @bPRFlag	          输出牌识狗是否被制作 1是已制作，0是未制作
* @返回值：		S_OK 获取信息成功  S_FALSE 获取信息失败
* @说  明：		获取加密狗绑定的权限信息
****************************************************************************************************/
ENCRYPT_API HRESULT _cdecl GetDogPRAccess( 
	BYTE &nPRChannels,
	BYTE &nPRLanes,
	BOOL &bPRConnPlatForm,
	BYTE &nPRChannelType,
	BOOL &bPRFlag);


/****************************************************************************************************
* @函数名称:GetDogFRAccess
* @功  能：		获取加密狗中人脸识别信息
* @参  数： 
* @nFRChannels   	输出可配置的最大通道数
* @nFRLanes		输出可配置的最大车道数
* @bFRConnPlatForm输出是否可连接平台 TRUE 是 FALSE 否
* @nFRChannelType	输出通道类型
* @bFRFlag	          输出人脸识别狗是否被制作 1是已制作，0是未制作
* @返回值：		S_OK 获取信息成功  S_FALSE 获取信息失败
* @说  明：		获取加密狗绑定的权限信息
****************************************************************************************************/
ENCRYPT_API HRESULT _cdecl GetDogFRAccess( 
	BYTE &nFRChannels,
	BYTE &nFRLanes,
	BOOL &bFRConnPlatForm,
	BYTE &nFRChannelType,
	BOOL &bFRFlag);

/****************************************************************************************************
* @函数名称:GetDogAccess
* @功  能：		获取加密狗中的权限
* @参  数： 
* @nUserGroup	输出用户类型   
* @nUseDate		输出拥有的使用日期
* @nChannels   	输出可配置的最大通道数
* @nLanes		输出可配置的最大车道数
* @bConnPlatForm输出是否可连接平台 TRUE 是 FALSE 否
* @nChannelType	输出通道类型
* @pResverd		输出预留字段
* @返回值：		S_OK 获取信息成功  S_FALSE 获取信息失败
* @说  明：		获取加密狗绑定的权限信息
****************************************************************************************************/
ENCRYPT_API HRESULT _cdecl GetDogAccess(BYTE &nChannelsPlate,
							BYTE &nChannelsFace,
							BYTE &nPlatForm,
							BOOL &bFRFlag);

#ifdef __cplusplus
}
#endif

#endif //___ENCRYPT_INTERFACE_H__