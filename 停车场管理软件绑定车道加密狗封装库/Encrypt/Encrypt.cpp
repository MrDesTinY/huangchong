#include "Encrypt.h"
#include "IVMSInterDog.h"
#include "IVMSDogMaker.h"

#define DOGON 0
#define DOGOFF 1

CIVMSDogMaker  IvmsdogMaker;
CIVMSInterDog  IvmsdogInter;

HRESULT _cdecl MakePRDog(	 WORD wDays,
					   BYTE nChannels,
					   BYTE nLanes,
					   BYTE nPlatForm,
					   BYTE nChannelType )
{
	HRESULT hr = S_FALSE;

	hr = IvmsdogMaker.MakePRDog( wDays,nChannels,nLanes,nPlatForm,nChannelType );
	
	return hr;
}

HRESULT _cdecl MakeFRDog(	 WORD wDays,
					   BYTE nChannelsFace,
					   BYTE nLanesFace,
					   BYTE nPlatFormFace,
					   BYTE nChannelTypeFace )
{
	HRESULT hr = S_FALSE;

	hr = IvmsdogMaker.MakeFRDog( wDays,nChannelsFace,nLanesFace,nPlatFormFace,nChannelTypeFace );
	
	return hr;
}

HRESULT _cdecl MakeDog(	BYTE nChannelsPlate,
					   BYTE nChannelsFace,
					   BYTE nPlatForm)
{
	HRESULT hr = S_FALSE;

	hr = IvmsdogMaker.MakeDog(nChannelsPlate,nChannelsFace,nPlatForm );

	return hr;
}


HRESULT _cdecl CheckDogBind( void )
{
	//HRESULT hr = IvmsdogInter.BindDog();
	HRESULT hr = IvmsdogInter.CheckDogBind();
	if(hr == S_OK)
	{
		return S_OK;
	}
	else if(hr == -1 || hr == -2 || hr == -3)
	{
		return S_FALSE;
	}
	else
	{
		return -1;
	}
}

HRESULT _cdecl StatusValidate( void )
{
	HRESULT hr;
	//HRESULT hr = IvmsdogInter.CheckDogBind();
	//if(hr == -1)
	//	return DOGOFF;
	hr = (HRESULT)DogCheck();
	if(hr == (HRESULT)0)
		return (HRESULT)DOGON;
	else
		return (HRESULT)DOGOFF;
}

HRESULT _cdecl BindDog( void )
 {
	return IvmsdogInter.BindDog( );
}
HRESULT _cdecl GetBindHardWareInfo( BYTE * const pStr,int  nStrLen )
{
	return S_OK;
}

HRESULT _cdecl GetDogPRAccess( 
	BYTE &nPRChannels,
	BYTE &nPRLanes,
	BOOL &bPRConnPlatForm,
	BYTE &nPRChannelType,
	BOOL &bPRFlag)
{
		HRESULT hr = CheckDogBind();
		if( S_FALSE == hr || -1 == hr )
		return S_FALSE;
		CUSTOMINFO nCustomInfo;
		hr = IvmsdogInter.GetDogAccess(nCustomInfo);
		if(S_OK != hr)
			return S_FALSE;
		bPRConnPlatForm = nCustomInfo.m_bConnPlatForm;
		nPRChannels = nCustomInfo.m_nChannels;
		nPRChannelType = nCustomInfo.m_nChannelType;
		nPRLanes = nCustomInfo.m_nLanes;
		if(nCustomInfo.m_nResverd3[0] == 0xf0)
		{
			bPRFlag = 1;
		}
		else
		{
			bPRFlag = 0;
		}
		return S_OK;
}


HRESULT _cdecl GetDogFRAccess( 
	BYTE &nFRChannels,
	BYTE &nFRLanes,
	BOOL &bFRConnPlatForm,
	BYTE &nFRChannelType,
	BOOL &bFRFlag)
{
		HRESULT hr = CheckDogBind();
		if( S_FALSE == hr || -1 == hr )
		return S_FALSE;
		CUSTOMINFO nCustomInfo;
		hr = IvmsdogInter.GetDogAccess(nCustomInfo);
		if(S_OK != hr)
			return S_FALSE;
		bFRConnPlatForm = nCustomInfo.m_bConnPlatFormFace;
		nFRChannels = nCustomInfo.m_nChannelsFace;
		nFRChannelType = nCustomInfo.m_nChannelTypeFace;
		nFRLanes = nCustomInfo.m_nLanesFace;
		if(nCustomInfo.m_nResverd3[0] == 0xf0)
		{
			bFRFlag = 1;
		}
		else
		{
			bFRFlag = 0;
		}
		return S_OK;
}


HRESULT _cdecl GetDogAccess(BYTE &nChannelsPlate,
							BYTE &nChannelsFace,
							BYTE &nPlatForm,
							BOOL &bFRFlag)
{
	HRESULT hr = CheckDogBind();
	if( S_FALSE == hr || -1 == hr )
		return S_FALSE;
	CUSTOMINFO nCustomInfo;
	hr = IvmsdogInter.GetDogAccess(nCustomInfo);
	if(S_OK != hr)
		return S_FALSE;
	nChannelsPlate = nCustomInfo.m_nChannels;
	nChannelsFace = nCustomInfo.m_nChannelsFace;
	nPlatForm = nCustomInfo.m_bConnPlatForm;
	if(nCustomInfo.m_nResverd3[0] == 0xf0)
	{
		bFRFlag = 1;
	}
	else
	{
		bFRFlag = 0;
	}
	return S_OK;
}
