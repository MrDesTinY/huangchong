#ifndef __IVMSDOG_MAKER_H__
#define __IVMSDOG_MAKER_H__
#include "IVMSDog.h"
class CIVMSDogMaker:public CIVMSDog
{
public:
	CIVMSDogMaker();
	~CIVMSDogMaker();
public:
	//ÖÆ×÷¼ÓÃÜ¹·
	HRESULT _cdecl MakePRDog( 
		WORD wDays,
		BYTE nChannels,
		BYTE nLanes,
		BYTE nPlatForm,
		BYTE nChannelType  );
    	HRESULT _cdecl MakeFRDog( 
		WORD wDaysFace,
		BYTE nChannelsFace,
		BYTE nLanesFace,
		BYTE nPlatFormFace,
		BYTE nChannelTypeFace  );
		HRESULT _cdecl MakeDog( 
			BYTE nChannelsPlate,
			BYTE nChannelsFace,
			BYTE nPlatForm);
protected:
	
};



#endif//__IVMSDOG_MAKER_H__