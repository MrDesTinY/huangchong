#ifndef  __IVMSDOG_H__
#define  __IVMSDOG_H__
#include "HikDog.h"

typedef struct  _tagCUSTOMINFO
{
	BYTE m_nChannels;
	BYTE m_nLanes;
	BOOL m_bConnPlatForm;
	BYTE m_nChannelType;
    
    BYTE m_nChannelsFace;
	BYTE m_nLanesFace;
	BOOL m_bConnPlatFormFace;
	BYTE m_nChannelTypeFace;
    
	BYTE m_nResverd3[4];
}CUSTOMINFO,*PCUSTOMINFO;
class CIVMSDog:public CHikDog
{
#define  MAX_IVMS_INFO_LEN   200
public:
	CIVMSDog();
	~CIVMSDog();

protected:
	//读取加密狗--多态
	virtual HRESULT ReadData( );
	//写加密狗--多态
	virtual HRESULT WriteData( );
	//从BUF中解析自定义信息
	virtual HRESULT GetCustomInfo( );
	//将自定义信息写入BUF
	virtual HRESULT SetCustomInfo( );
	//初始化参数--多态
	virtual HRESULT InitAllParam();
private:
	BYTE m_IVMSInfo[ MAX_IVMS_INFO_LEN ];

protected:
	CUSTOMINFO m_CustomInfo;

};


#endif//__IVMSDOG_H__