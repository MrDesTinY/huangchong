
// NTPSyncDemoDlg.h : header file
//

#pragma once
#include "config.h"
#include "clsNTPClient.h"
#include "afxwin.h"
// CNTPSyncDemoDlg dialog
#define WM_NC WM_USER+1001

class CNTPSyncDemoDlg : public CDialog
{
// Construction
public:
	CNTPSyncDemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_NTPSYNCDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	afx_msg void OnTimer(UINT nIDEvent);
private:
	BOOL m_status;
	CConfig *m_pConfig;
	clsNTPClient m_clsNTPClient;
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnNotifyIcon(WPARAM wParam,LPARAM IParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnClose();
	afx_msg BOOL RegisterRunKey();
	afx_msg BOOL UnRegisterRunKey();
	afx_msg void ToTray();
	CListBox m_lbListBox;
	NOTIFYICONDATA m_NotifyIcon;
	afx_msg void OnBnClickedCancel2();
	afx_msg void OnBnClickedCancel3();
};
