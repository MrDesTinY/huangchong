#if !defined(AFX_IMAGESHOWSTATIC_H__312F30A9_712D_4E61_92FA_D8A2B7101F7D__INCLUDED_)
#define AFX_IMAGESHOWSTATIC_H__312F30A9_712D_4E61_92FA_D8A2B7101F7D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImageShowStatic.h : header file
//
#include <Windows.h>
/////////////////////////////////////////////////////////////////////////////
// CImageShowStatic window

class CImageShowStatic : public CStatic
{
// Construction
public:
	CImageShowStatic();

// Attributes
public:

protected:

	unsigned char  *m_pImageData; //存储图像数据管理类实例指针，不需要销毁
	int m_iImageWidth;
	int m_iImageHeight;

public:
	BITMAPINFO   *m_pBmpInfo;
	void       SetImagePointer(unsigned char  *pImage,int width,int height); //设置图像数据管理类实例指针函数
    unsigned char*     GetImagePointer();             //获取图像数据管理类实例指针函数

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageShowStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CImageShowStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CImageShowStatic)
	afx_msg void OnPaint();
	afx_msg void OnDraw();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGESHOWSTATIC_H__312F30A9_712D_4E61_92FA_D8A2B7101F7D__INCLUDED_)
