// PrinterOperation.cpp : Implementation of CPrinterOperationApp and DLL registration.

#include "stdafx.h"
#include "PrinterOperation.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CPrinterOperationApp theApp;

const GUID CDECL BASED_CODE _tlid =
		{ 0x12435CCC, 0x411F, 0x4AA6, { 0xB7, 0x2A, 0xB8, 0x2A, 0x63, 0x15, 0x9C, 0x41 } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;



// CPrinterOperationApp::InitInstance - DLL initialization

BOOL CPrinterOperationApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: Add your own module initialization code here.
	}

	return bInit;
}



// CPrinterOperationApp::ExitInstance - DLL termination

int CPrinterOperationApp::ExitInstance()
{
	// TODO: Add your own module termination code here.

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
