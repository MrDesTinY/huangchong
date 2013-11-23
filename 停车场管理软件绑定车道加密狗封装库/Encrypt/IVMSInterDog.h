#ifndef __IVMS_INTER_DOG_H__
#define __IVMS_INTER_DOG_H__
#include "IVMSDog.h"
class CIVMSInterDog: public CIVMSDog
{
public:
	CIVMSInterDog();
	~CIVMSInterDog();
	HRESULT GetBindHardWareInfo( BYTE * const pStr,int  nStrLen );
	HRESULT CheckDogAlive();
	HRESULT GetDogAccess( CUSTOMINFO &nCustomInfo );
	//检验加密狗是否已经绑定--多态
	virtual HRESULT  CheckDogBind( );
protected:
};



#endif //__IVMS_INTER_DOG_H__