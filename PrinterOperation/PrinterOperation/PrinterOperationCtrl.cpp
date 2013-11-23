// PrinterOperationCtrl.cpp : Implementation of the CPrinterOperationCtrl ActiveX Control class.

#include "stdafx.h"
#include "PrinterOperation.h"
#include "PrinterOperationCtrl.h"
#include "PrinterOperationPropPage.h"
#include "comcat.h" 
#include "objsafe.h" 


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CPrinterOperationCtrl, COleControl)



// Message map

BEGIN_MESSAGE_MAP(CPrinterOperationCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()



// Dispatch map

BEGIN_DISPATCH_MAP(CPrinterOperationCtrl, COleControl)
	DISP_FUNCTION_ID(CPrinterOperationCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CPrinterOperationCtrl, "SendPrintData", dispidSendPrintData, SendPrintData, VT_I4, VTS_BSTR VTS_UI4 VTS_BSTR)
	DISP_FUNCTION_ID(CPrinterOperationCtrl, "PrintTest", dispidPrintTest, PrintTest, VT_I4, VTS_NONE)
END_DISPATCH_MAP()



// Event map

BEGIN_EVENT_MAP(CPrinterOperationCtrl, COleControl)
END_EVENT_MAP()



// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CPrinterOperationCtrl, 1)
	PROPPAGEID(CPrinterOperationPropPage::guid)
END_PROPPAGEIDS(CPrinterOperationCtrl)



// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CPrinterOperationCtrl, "PRINTEROPERATION.PrinterOperationCtrl.1",
	0x4fd00b5e, 0x4c1f, 0x47a1, 0xbb, 0x65, 0x5d, 0xb8, 0xc8, 0xbb, 0x74, 0xef)



// Type library ID and version

IMPLEMENT_OLETYPELIB(CPrinterOperationCtrl, _tlid, _wVerMajor, _wVerMinor)



// Interface IDs

const IID BASED_CODE IID_DPrinterOperation =
		{ 0x1C047CA9, 0xD613, 0x48B7, { 0xB8, 0xF2, 0x50, 0x0, 0xB2, 0x62, 0xFA, 0x9C } };
const IID BASED_CODE IID_DPrinterOperationEvents =
		{ 0x9AFD2FBF, 0x35BE, 0x4639, { 0xB4, 0x9B, 0xFB, 0x45, 0x1C, 0x49, 0x88, 0x1B } };



// Control type information

static const DWORD BASED_CODE _dwPrinterOperationOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CPrinterOperationCtrl, IDS_PRINTEROPERATION, _dwPrinterOperationOleMisc)

HRESULT CreateComponentCategory(CATID catid, WCHAR* catDescription) 
{ 

	ICatRegister* pcr = NULL ; 
	HRESULT hr = S_OK ; 

	hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr, 
		NULL, CLSCTX_INPROC_SERVER, IID_ICatRegister, (void**)&pcr); 
	if (FAILED(hr)) 
	{
		return hr; 
	}

	CATEGORYINFO catinfo; 
	catinfo.catid = catid; 
	catinfo.lcid = 0x0409 ; // english 

	int len = wcslen(catDescription); 
	if (len>127) 
		len = 127; 
	wcsncpy_s(catinfo.szDescription, catDescription, len); 
	// Make sure the description is null terminated 
	catinfo.szDescription[len] = '\0'; 
	hr = pcr->RegisterCategories(1, &catinfo); 
	pcr->Release(); 

	return hr; 
} 

HRESULT RegisterCLSIDInCategory(REFCLSID clsid, CATID catid) 
{ 
	ICatRegister* pcr = NULL ; 
	HRESULT hr = S_OK ; 
	hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr, 
		NULL, CLSCTX_INPROC_SERVER, IID_ICatRegister, (void**)&pcr); 
	if (SUCCEEDED(hr)) 
	{ 
		CATID rgcatid[1] ; 
		rgcatid[0] = catid; 
		hr = pcr->RegisterClassImplCategories(clsid, 1, rgcatid); 
	} 

	if (pcr != NULL) 
		pcr->Release(); 

	return hr; 
} 

HRESULT UnRegisterCLSIDInCategory(REFCLSID clsid, CATID catid) 
{ 
	ICatRegister* pcr = NULL ; 
	HRESULT hr = S_OK ; 
	hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr, 
		NULL, CLSCTX_INPROC_SERVER, IID_ICatRegister, (void**)&pcr); 
	if (SUCCEEDED(hr)) 
	{ 
		// Unregister this category as being "implemented" by 
		// the class. 
		CATID rgcatid[1] ; 
		rgcatid[0] = catid; 
		hr = pcr->UnRegisterClassImplCategories(clsid, 1, rgcatid); 
	} 

	if (pcr != NULL) 
	{
		pcr->Release(); 
	}

	return hr; 
} 


// CParkingOCXCtrl::CParkingOCXCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CParkingOCXCtrl

BOOL CPrinterOperationCtrl::CPrinterOperationCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegApartmentThreading to 0.

	//if (bRegister)
	//	return AfxOleRegisterControlClass(
	//		AfxGetInstanceHandle(),
	//		m_clsid,
	//		m_lpszProgID,
	//		IDS_PARKINGOCX,
	//		IDB_PARKINGOCX,
	//		afxRegApartmentThreading,
	//		_dwParkingOCXOleMisc,
	//		_tlid,
	//		_wVerMajor,
	//		_wVerMinor);
	//else
	//	return AfxOleUnregisterClass(m_clsid, m_lpszProgID);


	if (bRegister) 
	{ 
		HRESULT hr = S_OK ; 

		// register as safe for scripting 
		hr = CreateComponentCategory(CATID_SafeForScripting, 
			L"Controls that are safely scriptable"); 

		if (FAILED(hr)) 
		{
			return FALSE; 
		}

		hr = RegisterCLSIDInCategory(m_clsid, CATID_SafeForScripting); 

		if (FAILED(hr)) 
		{
			return FALSE; 
		}

		// register as safe for initializing 
		hr = CreateComponentCategory(CATID_SafeForInitializing, 
			L"Controls safely initializable from persistent data"); 

		if (FAILED(hr)) 
		{
			return FALSE; 
		}

		hr = RegisterCLSIDInCategory(m_clsid, CATID_SafeForInitializing); 

		if (FAILED(hr)) 
		{
			return FALSE; 
		}

		return AfxOleRegisterControlClass( 
			AfxGetInstanceHandle(), 
			m_clsid, 
			m_lpszProgID, 
			//IDS_CROSSINGACTIVEX,
			IDS_PRINTEROPERATION,
			IDB_PRINTEROPERATION,
			//IDB_CROSSINGACTIVEX,
			afxRegInsertable | afxRegApartmentThreading, 
			//_dwCrossingActiveXOleMisc,
			_dwPrinterOperationOleMisc,
			_tlid, 
			_wVerMajor, 
			_wVerMinor); 
	} 
	else 
	{ 
		HRESULT hr = S_OK ; 

		hr = UnRegisterCLSIDInCategory(m_clsid, CATID_SafeForScripting); 

		if (FAILED(hr)) 
		{
			return FALSE; 
		}

		hr = UnRegisterCLSIDInCategory(m_clsid, CATID_SafeForInitializing); 

		if (FAILED(hr)) 
		{
			return FALSE; 
		}

		return AfxOleUnregisterClass(m_clsid, m_lpszProgID); 
	}
}
// CPrinterOperationCtrl::CPrinterOperationCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CPrinterOperationCtrl

//BOOL CPrinterOperationCtrl::CPrinterOperationCtrlFactory::UpdateRegistry(BOOL bRegister)
//{
//	// TODO: Verify that your control follows apartment-model threading rules.
//	// Refer to MFC TechNote 64 for more information.
//	// If your control does not conform to the apartment-model rules, then
//	// you must modify the code below, changing the 6th parameter from
//	// afxRegApartmentThreading to 0.
//
//	if (bRegister)
//		return AfxOleRegisterControlClass(
//			AfxGetInstanceHandle(),
//			m_clsid,
//			m_lpszProgID,
//			IDS_PRINTEROPERATION,
//			IDB_PRINTEROPERATION,
//			afxRegApartmentThreading,
//			_dwPrinterOperationOleMisc,
//			_tlid,
//			_wVerMajor,
//			_wVerMinor);
//	else
//		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
//}



// CPrinterOperationCtrl::CPrinterOperationCtrl - Constructor

CPrinterOperationCtrl::CPrinterOperationCtrl()
{
	InitializeIIDs(&IID_DPrinterOperation, &IID_DPrinterOperationEvents);
	// TODO: Initialize your control's instance data here.
}



// CPrinterOperationCtrl::~CPrinterOperationCtrl - Destructor

CPrinterOperationCtrl::~CPrinterOperationCtrl()
{
	// TODO: Cleanup your control's instance data here.
}



// CPrinterOperationCtrl::OnDraw - Drawing function

void CPrinterOperationCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;

	// TODO: Replace the following code with your own drawing code.
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	pdc->Ellipse(rcBounds);
}



// CPrinterOperationCtrl::DoPropExchange - Persistence support

void CPrinterOperationCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.
}



// CPrinterOperationCtrl::OnResetState - Reset control to default state

void CPrinterOperationCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}



// CPrinterOperationCtrl::AboutBox - Display an "About" box to the user

void CPrinterOperationCtrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_PRINTEROPERATION);
	dlgAbout.DoModal();
}

// **********************************************************************
// PrintError - uses printf() to display error code information
// 
// Params:
//   dwError       - the error code, usually from GetLastError()
//   lpString      - some caller-defined text to print with the error info
// 
// Returns: void
// 
void CPrinterOperationCtrl::PrintError( DWORD dwError, LPCTSTR lpString )
{
#define MAX_MSG_BUF_SIZE 512
	TCHAR 	*msgBuf;
	DWORD	cMsgLen;

	cMsgLen = FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 40, NULL, dwError,
		MAKELANGID(0, SUBLANG_ENGLISH_US), (LPTSTR) &msgBuf,
		MAX_MSG_BUF_SIZE, NULL );
	printf( TEXT("%s Error [%d]:: %s\n"), lpString, dwError, msgBuf );
	LocalFree( msgBuf );
#undef MAX_MSG_BUF_SIZE
}
// end PrintError
// **********************************************************************

// **********************************************************************
// ReadFileWithAlloc - allocates memory for and reads contents of a file
// 
// Params:
//   szFileName   - NULL terminated string specifying file name
//   pdwSize      - address of variable to receive file bytes size
//   ppBytes      - address of pointer which will be allocated and contain file bytes
// 
// Returns: TRUE for success, FALSE for failure.
//
// Notes: Caller is responsible for freeing the memory using GlobalFree()
// 
BOOL CPrinterOperationCtrl::ReadFileWithAlloc( LPTSTR szFileName, LPDWORD pdwSize, LPBYTE *ppBytes )
{
	HANDLE		hFile;
	DWORD		dwBytes;
	BOOL		bSuccess = FALSE;

	// Validate pointer parameters
	if( ( pdwSize == NULL ) || ( ppBytes == NULL ) )
		return FALSE;
	// Open the file for reading
	hFile = CreateFile( szFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	if( hFile == INVALID_HANDLE_VALUE )
	{
		PrintError( GetLastError(), TEXT("CreateFile()") );
		return FALSE;
	}
	// How big is the file?
	*pdwSize = GetFileSize( hFile, NULL );
	if( *pdwSize == (DWORD)-1 )
		PrintError( GetLastError(), TEXT("GetFileSize()") );
	else
	{
		// Allocate the memory
		*ppBytes = (LPBYTE)GlobalAlloc( GPTR, *pdwSize );
		if( *ppBytes == NULL )
			PrintError( GetLastError(), TEXT("Failed to allocate memory\n") );
		else
		{
			// Read the file into the newly allocated memory
			bSuccess = ReadFile( hFile, *ppBytes, *pdwSize, &dwBytes, NULL );
			if( ! bSuccess )
				PrintError( GetLastError(), TEXT("ReadFile()") );
		}
	}
	// Clean up
	CloseHandle( hFile );
	return bSuccess;
}
// End ReadFileWithAlloc
// **********************************************************************

// **********************************************************************
// RawDataToPrinter - sends binary data directly to a printer
// 
// Params:
//   szPrinterName - NULL terminated string specifying printer name
//   lpData        - Pointer to raw data bytes
//   dwCount       - Length of lpData in bytes
// 
// Returns: TRUE for success, FALSE for failure.
// 
BOOL CPrinterOperationCtrl::RawDataToPrinter( LPTSTR szPrinterName, LPBYTE lpData, DWORD dwCount )
{
	HANDLE     hPrinter;
	DOC_INFO_1 DocInfo;
	DWORD      dwJob;
	DWORD      dwBytesWritten;

	// Need a handle to the printer.
	if( ! OpenPrinter( szPrinterName, &hPrinter, NULL ) )
	{
		PrintError( GetLastError(), TEXT("OpenPrinter") );
		return FALSE;
	}

	// Fill in the structure with info about this "document."
	DocInfo.pDocName = TEXT("My Document");
	DocInfo.pOutputFile = NULL;
	DocInfo.pDatatype = TEXT("RAW");
	// Inform the spooler the document is beginning.
	if( (dwJob = StartDocPrinter( hPrinter, 1, (LPBYTE)&DocInfo )) == 0 )
	{
		PrintError( GetLastError(), TEXT("StartDocPrinter") );
		ClosePrinter( hPrinter );
		return FALSE;
	}
	// Start a page.
	if( ! StartPagePrinter( hPrinter ) )
	{
		PrintError( GetLastError(), TEXT("StartPagePrinter") );
		EndDocPrinter( hPrinter );
		ClosePrinter( hPrinter );
		return FALSE;
	}
	// Send the data to the printer.
	if( ! WritePrinter( hPrinter, lpData, dwCount, &dwBytesWritten ) )
	{
		PrintError( GetLastError(), TEXT("WritePrinter") );
		EndPagePrinter( hPrinter );
		EndDocPrinter( hPrinter );
		ClosePrinter( hPrinter );
		return FALSE;
	}
	// End the page.
	if( ! EndPagePrinter( hPrinter ) )
	{
		PrintError( GetLastError(), TEXT("EndPagePrinter") );
		EndDocPrinter( hPrinter );
		ClosePrinter( hPrinter );
		return FALSE;
	}
	// Inform the spooler that the document is ending.
	if( ! EndDocPrinter( hPrinter ) )
	{
		PrintError( GetLastError(), TEXT("EndDocPrinter") );
		ClosePrinter( hPrinter );
		return FALSE;
	}
	// Tidy up the printer handle.
	ClosePrinter( hPrinter );
	// Check to see if correct number of bytes were written.
	if( dwBytesWritten != dwCount )
	{
		printf( TEXT("Wrote %d bytes instead of requested %d bytes.\n"), dwBytesWritten, dwCount );
		return FALSE;
	}
	return TRUE;
}
// End RawDataToPrinter
// **********************************************************************

// CPrinterOperationCtrl message handlers

LONG CPrinterOperationCtrl::SendPrintData(LPCTSTR byPrintData, ULONG ulDataCount, LPCTSTR bsPrinterName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// TODO: Add your dispatch handler code here
	return RawDataToPrinter((LPTSTR)bsPrinterName,(LPBYTE)CString(byPrintData).GetBuffer(0),ulDataCount);
}

LONG CPrinterOperationCtrl::PrintTest(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// TODO: Add your dispatch handler code here
	BYTE *byPrintData = (BYTE *)"N\r\nA200,20,0,8,1,1,N,\"公务接待票\"\r\nA80,70,0,8,1,1,N,\"部门：财务部\"\r\nA80,110,0,8,1,1,N,\"有效期：\"\r\nA45,150,0,8,1,1,N,\"2013-10-30至2013-11-30\"\r\nB70,185,0,1,3,6,90,B,\"571-00010\"\r\nP1\r\n";
	LONG ulDataCount = sizeof("N\r\nA200,20,0,8,1,1,N,\"公务接待票\"\r\nA80,70,0,8,1,1,N,\"部门：财务部\"\r\nA80,110,0,8,1,1,N,\"有效期：\"\r\nA45,150,0,8,1,1,N,\"2013-10-30至2013-11-30\"\r\nB70,185,0,1,3,6,90,B,\"571-00010\"\r\nP1\r\n");
	LONG lRet = SendPrintData((LPTSTR)byPrintData,ulDataCount,"GK888t");
	return lRet;
}
