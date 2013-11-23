#pragma once
#include <Windows.h>
#include <StdIO.h>
#include <winspool.h>
// PrinterOperationCtrl.h : Declaration of the CPrinterOperationCtrl ActiveX Control class.


// CPrinterOperationCtrl : See PrinterOperationCtrl.cpp for implementation.

class CPrinterOperationCtrl : public COleControl
{
	DECLARE_DYNCREATE(CPrinterOperationCtrl)

// Constructor
public:
	CPrinterOperationCtrl();

// Overrides
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

// Implementation
protected:
	~CPrinterOperationCtrl();

	DECLARE_OLECREATE_EX(CPrinterOperationCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CPrinterOperationCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CPrinterOperationCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CPrinterOperationCtrl)		// Type name and misc status

// Message maps
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// Event maps
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
		dispidPrintTest = 2L,
		dispidSendPrintData = 1L
	};
private:
	void PrintError( DWORD dwError, LPCTSTR lpString );
	BOOL ReadFileWithAlloc( LPTSTR szFileName, LPDWORD pdwSize, LPBYTE *ppBytes );
	BOOL RawDataToPrinter( LPTSTR szPrinterName, LPBYTE lpData, DWORD dwCount );
public:
	LONG SendPrintData(LPCTSTR byPrintData, ULONG ulDataCount, LPCTSTR bsPrinterName = "GK888t");
protected:
	LONG PrintTest(void);
};

