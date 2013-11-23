// SCCPPropPage.cpp : Implementation of the CSCCPPropPage property page class.

#include "stdafx.h"
#include "SCCP.h"
#include "SCCPPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CSCCPPropPage, COlePropertyPage)



// Message map

BEGIN_MESSAGE_MAP(CSCCPPropPage, COlePropertyPage)
END_MESSAGE_MAP()



// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CSCCPPropPage, "SCCP.SCCPPropPage.1",
	0xa3941d2c, 0x5f95, 0x409a, 0x95, 0xa8, 0x1b, 0xc, 0xf0, 0x37, 0x5e, 0xd0)



// CSCCPPropPage::CSCCPPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CSCCPPropPage

BOOL CSCCPPropPage::CSCCPPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_SCCP_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// CSCCPPropPage::CSCCPPropPage - Constructor

CSCCPPropPage::CSCCPPropPage() :
	COlePropertyPage(IDD, IDS_SCCP_PPG_CAPTION)
{
}



// CSCCPPropPage::DoDataExchange - Moves data between page and properties

void CSCCPPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// CSCCPPropPage message handlers
