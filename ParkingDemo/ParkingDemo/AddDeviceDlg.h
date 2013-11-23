#pragma once


// CAddDeviceDlg 对话框

class CAddDeviceDlg : public CDialog
{
	DECLARE_DYNAMIC(CAddDeviceDlg)

public:
	CAddDeviceDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAddDeviceDlg();

// 对话框数据
	enum { IDD = IDD_ADDDEVICE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strDeviceName;
	CString m_strDeviceIP;
	long m_lPort;
	CString m_strUserName;
	CString m_strPassWord;
	int m_iBeginPlaceNo;

	virtual BOOL OnInitDialog();

};
