
// ToHvshowDemoDlg.h : header file
//

#pragma once
#include "tohvshowctrl1.h"
#include "afxwin.h"


// CToHvshowDemoDlg dialog
class CToHvshowDemoDlg : public CDialog
{
// Construction
public:
	CToHvshowDemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TOHVSHOWDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CTohvshowctrl1 m_OcxToHwShow;
	CTohvshowctrl1 m_OcxToHwShow1;
	afx_msg void OnBnClickedLogin();
	afx_msg void OnBnClickedGetstatus();
	afx_msg void OnBnClickedSetdefpath();
	afx_msg void OnBnClickedLoginout();
	afx_msg void OnBnClickedGetplate();
	afx_msg void OnBnClickedGetplatecolor();
	afx_msg void OnBnClickedGetaddinfo();
	afx_msg void OnBnClickedSavecarfullpic();
	afx_msg void OnBnClickedSavefacepic();
	afx_msg void OnBnClickedSaveplatepic();
	afx_msg void OnBnClickedSaveplatebinpic();
	afx_msg void OnBnClickedSavebinbmppic();
	afx_msg void OnBnClickedGetsnaptime();
	afx_msg void OnBnClickedGetsavepicpath();
	afx_msg void OnBnClickedIffullpic();
	afx_msg void OnBnClickedIfplatepic();
	afx_msg void OnBnClickedIfbinpic();
	afx_msg void OnBnClickedIffacepic();
	afx_msg void OnBnClickedIfautosave();
	afx_msg void OnBnClickedIfprintlog();
	afx_msg void OnBnClickedGetplatecontrol();
	afx_msg void OnBnClickedGetfullcontrol();
	afx_msg void OnBnClickedGetfacecontrol();
	afx_msg void OnBnClickedGetplatenocontrol();
	afx_msg void OnBnClickedSetplatecontrol();
	afx_msg void OnBnClickedSetfullcontrol();
	afx_msg void OnBnClickedSetfacecontrol();
	afx_msg void OnBnClickedSetplatenocontrol();
	DECLARE_EVENTSINK_MAP()
	void OnReceivePlateTohvshowctrl1();
	// 车辆原图
	CStatic m_originalpic;
	// 车脸图
	CStatic m_facepic;
	// 车牌图片
	CStatic m_platepic;
	// 车牌二值图
	CStatic m_binpic;

public:
	BOOL IsFileExists(CString csPath);
	void Show_picture(CStatic & PicShow,CString csPath);
	void OnReceivePlateTohvshowctrl2();
};
