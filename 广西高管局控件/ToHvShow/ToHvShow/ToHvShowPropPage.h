#pragma once

// ToHvShowPropPage.h : Declaration of the CToHvShowPropPage property page class.


// CToHvShowPropPage : See ToHvShowPropPage.cpp for implementation.

class CToHvShowPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CToHvShowPropPage)
	DECLARE_OLECREATE_EX(CToHvShowPropPage)

// Constructor
public:
	CToHvShowPropPage();

// Dialog Data
	enum { IDD = IDD_PROPPAGE_TOHVSHOW };

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	DECLARE_MESSAGE_MAP()
};

