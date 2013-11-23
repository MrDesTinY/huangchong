// HVActiveX2PropPage.cpp : Implementation of the CHVActiveX2PropPage property page class.

#include "stdafx.h"
#include "HVActiveX2.h"
#include "HVActiveX2PropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CHVActiveX2PropPage, COlePropertyPage)



// Message map

BEGIN_MESSAGE_MAP(CHVActiveX2PropPage, COlePropertyPage)
END_MESSAGE_MAP()



// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CHVActiveX2PropPage, "HVActiveX2.HVActiveX2PropPage.1",
	0x83840bed, 0xe5f7, 0x4aaa, 0xbd, 0xf3, 0xa1, 0xf2, 0xe5, 0xdd, 0xf5, 0xef)



// CHVActiveX2PropPage::CHVActiveX2PropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CHVActiveX2PropPage

BOOL CHVActiveX2PropPage::CHVActiveX2PropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_HVActiveX2_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// CHVActiveX2PropPage::CHVActiveX2PropPage - Constructor

CHVActiveX2PropPage::CHVActiveX2PropPage() :
	COlePropertyPage(IDD, IDS_HVActiveX2_PPG_CAPTION)
{
}



// CHVActiveX2PropPage::DoDataExchange - Moves data between page and properties

void CHVActiveX2PropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// CHVActiveX2PropPage message handlers
