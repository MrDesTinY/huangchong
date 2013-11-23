// SCCP.cpp : Implementation of CSCCPApp and DLL registration.

#include "stdafx.h"
#include "SCCP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CSCCPApp theApp;

const GUID CDECL BASED_CODE _tlid =
		{ 0x2C648E7C, 0x84B3, 0x4893, { 0x98, 0xCB, 0x3E, 0x90, 0xAF, 0x89, 0x77, 0x86 } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;



// CSCCPApp::InitInstance - DLL initialization

BOOL CSCCPApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: Add your own module initialization code here.
		bInit = NET_DVR_Init();
	}

	return bInit;
}



// CSCCPApp::ExitInstance - DLL termination

int CSCCPApp::ExitInstance()
{
	// TODO: Add your own module termination code here.
	//NET_DVR_Cleanup();
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
