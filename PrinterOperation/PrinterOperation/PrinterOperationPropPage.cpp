// PrinterOperationPropPage.cpp : Implementation of the CPrinterOperationPropPage property page class.

#include "stdafx.h"
#include "PrinterOperation.h"
#include "PrinterOperationPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CPrinterOperationPropPage, COlePropertyPage)



// Message map

BEGIN_MESSAGE_MAP(CPrinterOperationPropPage, COlePropertyPage)
END_MESSAGE_MAP()



// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CPrinterOperationPropPage, "PRINTEROPERATI.PrinterOperatiPropPage.1",
	0x70e428dd, 0x81e4, 0x4571, 0xb3, 0x14, 0x90, 0x50, 0x1, 0x8c, 0, 0xf9)



// CPrinterOperationPropPage::CPrinterOperationPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CPrinterOperationPropPage

BOOL CPrinterOperationPropPage::CPrinterOperationPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_PRINTEROPERATION_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// CPrinterOperationPropPage::CPrinterOperationPropPage - Constructor

CPrinterOperationPropPage::CPrinterOperationPropPage() :
	COlePropertyPage(IDD, IDS_PRINTEROPERATION_PPG_CAPTION)
{
}



// CPrinterOperationPropPage::DoDataExchange - Moves data between page and properties

void CPrinterOperationPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// CPrinterOperationPropPage message handlers
