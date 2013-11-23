// ToHvShow.cpp : Implementation of CToHvShowApp and DLL registration.

#include "stdafx.h"
#include "ToHvShow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CToHvShowApp theApp;

const GUID CDECL BASED_CODE _tlid =
		{ 0xD2FE37DE, 0x9320, 0x4DC7, { 0x9B, 0x78, 0xBC, 0x77, 0x6A, 0xD, 0x69, 0x25 } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;



// CToHvShowApp::InitInstance - DLL initialization

BOOL CToHvShowApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: Add your own module initialization code here.
		bInit = NET_DVR_Init();
	}

	return bInit;
}



// CToHvShowApp::ExitInstance - DLL termination

int CToHvShowApp::ExitInstance()
{
	// TODO: Add your own module termination code here.
	NET_DVR_Cleanup();
	return COleControlModule::ExitInstance();
}



// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}



// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}
