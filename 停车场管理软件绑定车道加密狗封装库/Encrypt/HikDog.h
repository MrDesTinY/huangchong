#ifndef  __HIKLOG_H__
#define  __HIKLOG_H__

//#include "Log.h"
#include "Codec.h"
#include "Dog.h"
#include "HardWareInfo.h"
#define HIK_COMPANY_NAME "HIK"
//存储信息的信息头
typedef struct tagINFO_HEADER	//4字节对齐
{
	BYTE m_BaseInfoAddr;//基本信息地址
	BYTE m_BaseInfoLen;//基本信息长度
	BYTE m_CustomInfoAddr;//自定义信息地址
	BYTE m_CustomInfoLen;//自定义信息长度
	BYTE m_Resver1[ 4 ];//预留信息1
}INFO_HEADER;
//存储的基本信息的信息实体
typedef struct tagBASEINFO_BODY
{
	BYTE m_CompanyName[4];//公司名称
	BYTE m_HareWareID[MAX_INFO_LEN];//硬件ID
	BYTE m_bBind;//是否绑定
	BYTE m_BindDate[4];//绑定日期
	WORD m_UseDays;//使用日期
	BYTE m_UserGroup;//用户组
	BYTE m_Resver2[4];//预留信息2
}BASEINFO_BODY;

class  CHikDog:/*public CLog,*/public CCodec,public CDog,CHardWareInfo
{
	public:
		CHikDog();
		~CHikDog();
	public:
		//绑定加密狗
		virtual HRESULT  BindDog( );
		//检验加密狗是否已经绑定
		virtual HRESULT  CheckDogBind( );
	protected:
		//读数据--多态
		virtual HRESULT  ReadData( );
		//写数据--多态
		virtual HRESULT  WriteData( );
		//设置海康基本信息到要写入的buf中
		virtual HRESULT  SetHikInfo( );
		//从读取信息中获取海康基本信息
		virtual HRESULT  GetHikInfo( );
		//加密数据-多态
		virtual HRESULT  Encrypt();
		//解密数据--多态
		virtual HRESULT  Decrypt();
		//使用权限是否过期
		BOOL ISDateOff( BYTE nDate[4] ,WORD nDays );
	protected:
	
		//初始化所有参数
		HRESULT InitAllParam();
	protected:
		INFO_HEADER m_InfoHeader;
		BASEINFO_BODY m_InfoBody;
};


#endif//__HIKLOG_H__