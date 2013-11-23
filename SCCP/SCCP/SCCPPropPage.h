#pragma once

// SCCPPropPage.h : Declaration of the CSCCPPropPage property page class.


// CSCCPPropPage : See SCCPPropPage.cpp for implementation.

class CSCCPPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CSCCPPropPage)
	DECLARE_OLECREATE_EX(CSCCPPropPage)

// Constructor
public:
	CSCCPPropPage();

// Dialog Data
	enum { IDD = IDD_PROPPAGE_SCCP };

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	DECLARE_MESSAGE_MAP()
};

