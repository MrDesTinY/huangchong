#pragma once

// HVActiveX2PropPage.h : Declaration of the CHVActiveX2PropPage property page class.


// CHVActiveX2PropPage : See HVActiveX2PropPage.cpp for implementation.

class CHVActiveX2PropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CHVActiveX2PropPage)
	DECLARE_OLECREATE_EX(CHVActiveX2PropPage)

// Constructor
public:
	CHVActiveX2PropPage();

// Dialog Data
	enum { IDD = IDD_PROPPAGE_HVActiveX2 };

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	DECLARE_MESSAGE_MAP()
};

