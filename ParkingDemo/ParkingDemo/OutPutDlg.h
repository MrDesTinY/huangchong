#pragma once
#include "HCNetSDK.h"
#include "afxwin.h"
#include <gdiplus.h>
#pragma  comment (lib, "gdiplus.lib")
using namespace Gdiplus;

class COutPutDlg : public CDialog
{
	DECLARE_DYNAMIC(COutPutDlg)

public:
	COutPutDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COutPutDlg();

// 对话框数据
	enum { IDD = IDD_OUTPUT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	CString m_strLeftPlateNo;
	CString m_strRightPlateNo;
	int m_iLeftPlaceNo;
	int m_iRightPlaceNo;
	BOOL m_bHasLeftVehicle;
	BOOL m_bHasRightVehicle;

	int m_iNum;
	BOOL m_bSeleted;
	int m_iUserId;
	int m_iChanId;
	long m_lPlayHandle;

	// 选中此画面
	void SelectThis(void);
	// 取消选中此画面
	void CancelSelect(void);
	// 在此画面播放
	void PlayThis(int iUserId, int iChanId);
	// 停止此画面播放
	void StopThis(void);
	// 强制刷新窗口
	void RefreshThis(void);

	void UpdateWindow(void);
	void ResetWindow(void);

	NET_DVR_DISPLAY_PARA m_struCardPlayParam;//m_struCardPlayPara


	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	COLORREF m_RedColor;  //红色
	COLORREF m_GreenColor;//绿色
	COLORREF m_BlackColor;//黑色

	CBrush m_RedBrush;    // 红色画刷
	CBrush m_GreenBrush;  // 绿色画刷


	//*****************************


	
	

	CStatic m_Static_Pic;
	virtual BOOL OnInitDialog();

	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();



	CDC*             g_DC;
	CRect            g_rect;
	CRITICAL_SECTION g_cs;
	BOOL    m_bCsDlg;

	void DrawPicture(int iPicSize, unsigned char* pic); 

	
};
