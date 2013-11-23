#pragma once

// PrinterOperationPropPage.h : Declaration of the CPrinterOperationPropPage property page class.


// CPrinterOperationPropPage : See PrinterOperationPropPage.cpp for implementation.

class CPrinterOperationPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CPrinterOperationPropPage)
	DECLARE_OLECREATE_EX(CPrinterOperationPropPage)

// Constructor
public:
	CPrinterOperationPropPage();

// Dialog Data
	enum { IDD = IDD_PROPPAGE_PRINTEROPERATION };

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	DECLARE_MESSAGE_MAP()
};

