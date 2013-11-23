
// PicInfoToolDlg.h : 头文件
//

#pragma once

#include "afxwin.h"
#include "DecProcess.h"
#include "ImageShowStatic.h"
// CPicInfoToolDlg 对话框
class CPicInfoToolDlg : public CDialog
{
// 构造
public:
	CPicInfoToolDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_PICINFOTOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
//	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
private:
	DecProcess m_DecProcess;
public:
	afx_msg void ClearAllContent();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	CImageShowStatic m_ShowImage;
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};
