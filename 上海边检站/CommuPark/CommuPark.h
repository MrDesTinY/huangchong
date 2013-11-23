// CommuPark.h : main header file for the CommuPark DLL
//

#pragma once



#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

#include "Termin.h"


// CCommuParkApp
// See CommuPark.cpp for the implementation of this class
//

class CCommuParkApp : public CWinApp
{
public:
	CCommuParkApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
