// MakeDogToolDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "config.h"
#ifndef BYTE
typedef unsigned char BYTE;
#endif 

#ifndef  WORD
typedef  unsigned short WORD;
#endif
// CMakeDogToolDlg 对话框
class CMakeDogToolDlg : public CDialog
{
// 构造
public:
	CMakeDogToolDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CMakeDogToolDlg();
// 对话框数据
	enum { IDD = IDD_MAKEDOGTOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	HBRUSH m_hbrush;
	bool m_bChangeStatus;
	CComboBox m_ccbDeviceType;
	CComboBox m_ccbSetPlatform;
	CComboBox m_ccbGetPlatform;
	CConfig *m_pConfig;
	afx_msg void OnBnClickedTestDog();
	afx_msg void OnBnClickedMakeDog();
	//afx_msg void OnBnClickedMakeFRDog();
	afx_msg void OnBnClickedReadDog();
	//afx_msg void OnBnClickedReadFRDog();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
