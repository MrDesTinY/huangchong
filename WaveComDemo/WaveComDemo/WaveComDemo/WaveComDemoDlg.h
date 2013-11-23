
// WaveComDemoDlg.h : 头文件
//

#pragma once
#include "clsSMSModem.h"

// CWaveComDemoDlg 对话框
class CWaveComDemoDlg : public CDialog
{
// 构造
public:
	CWaveComDemoDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_WAVECOMDEMO_DIALOG };

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
    afx_msg void OnBnClickedBnconnect();
    afx_msg void OnBnClickedBnsend();
    afx_msg void OnBnClickedBnrecieve();
    afx_msg void OnBnClickedBndelindex();
    afx_msg void OnBnClickedBncheckauto();
    afx_msg void OnBnClickedBnchecknew();
    afx_msg void OnBnClickedBndisconnect();
    clsSMSModem m_clsSMSModem;
    afx_msg void OnClose();
};
