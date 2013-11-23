// CamConfig.h : main header file for the CamConfig DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
//MyDefine
#include "clsNetIONvr.h"

// CCamConfigApp
// See CamConfig.cpp for the implementation of this class
//

class CCamConfigApp : public CWinApp
{
public:
	CCamConfigApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
