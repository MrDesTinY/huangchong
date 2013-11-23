
// SyncTimeTestMFC.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CSyncTimeTestMFCApp:
// See SyncTimeTestMFC.cpp for the implementation of this class
//

class CSyncTimeTestMFCApp : public CWinAppEx
{
public:
	CSyncTimeTestMFCApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CSyncTimeTestMFCApp theApp;