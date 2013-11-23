// HVActiveX2.cpp : Implementation of CHVActiveX2App and DLL registration.

#include "stdafx.h"
#include "HVActiveX2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CHVActiveX2App theApp;

const GUID CDECL BASED_CODE _tlid =
		{ 0x4157D2EB, 0x7391, 0x4182, { 0xA5, 0x2A, 0xA, 0x55, 0x20, 0x5D, 0xD4, 0x7A } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;



// CHVActiveX2App::InitInstance - DLL initialization

BOOL CHVActiveX2App::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: Add your own module initialization code here.
		bInit = NET_DVR_Init();
	}

	return bInit;
}



// CHVActiveX2App::ExitInstance - DLL termination

int CHVActiveX2App::ExitInstance()
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
