// ToHvShowPropPage.cpp : Implementation of the CToHvShowPropPage property page class.

#include "stdafx.h"
#include "ToHvShow.h"
#include "ToHvShowPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CToHvShowPropPage, COlePropertyPage)



// Message map

BEGIN_MESSAGE_MAP(CToHvShowPropPage, COlePropertyPage)
END_MESSAGE_MAP()



// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CToHvShowPropPage, "TOHVSHOW.ToHvShowPropPage.1",
	0x83840bed, 0xe5f7, 0x4aaa, 0xbd, 0xf3, 0xa1, 0xf2, 0xe5, 0xdd, 0xf5, 0xef)



// CToHvShowPropPage::CToHvShowPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CToHvShowPropPage

BOOL CToHvShowPropPage::CToHvShowPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_TOHVSHOW_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// CToHvShowPropPage::CToHvShowPropPage - Constructor

CToHvShowPropPage::CToHvShowPropPage() :
	COlePropertyPage(IDD, IDS_TOHVSHOW_PPG_CAPTION)
{
}



// CToHvShowPropPage::DoDataExchange - Moves data between page and properties

void CToHvShowPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// CToHvShowPropPage message handlers
