#pragma once
#include "GeneralDef.h"
//#include "ParkingDemoDlg.h"

// CIPAccessCfgDlg 对话框
 char *GetDeviceAddress(LPNET_DVR_IPDEVINFO_V31 lpDeviceInfo);
 void SetDeviceAddress(LPNET_DVR_IPDEVINFO_V31 lpDevInfo, char *lpAddress);

class CIPAccessCfgDlg : public CDialog
{
	DECLARE_DYNAMIC(CIPAccessCfgDlg)

public:
	CIPAccessCfgDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CIPAccessCfgDlg();

	DWORD m_dwDevIndex;

	int m_iCurSel;//the select item index in the list
	int m_iCurChanNO;    //0-MAX_CHANNUM_V30 + groupNO*64
	int m_iCurChanIndex; //0-MAX_CHANNUM_V30
	BOOL m_bCurEnable;   //is the channel added or not
	int m_iSelAccessType;
	BOOL m_bAnalogChange;

	CImageList m_TreeImage;
	NET_DVR_IPPARACFG_V40 m_struIPParaCfgV40;
	NET_DVR_IPALARMOUTCFG m_struIPAlarmOutCfg;
	NET_DVR_IPALARMINCFG m_struIPAlarmInCfg;
	NET_DVR_IPC_PROTO_LIST m_struProtoList;
	void CheckInitParam();
	void UpdateGroup(int iGroupNO);
	void EnableInfoCtrl(BOOL bEnable);
	void SetCtrlStat(int iStat);
	//void SetCtrlStat(int iStat);
	DWORD m_dwStartChan;
	char m_szProType[64];//协议类型字符串
	char* ProIndexToStr(int iProType);

// 对话框数据
	enum { IDD = IDD_IPACCESS_CFG_DIALOG };

	CComboBox	m_comboTransProtTypeUrl;
	CComboBox	m_comboChannelGroupNO;
	CComboBox	m_comboStreamType;
	CComboBox	m_comboProType;
	CListCtrl	m_listIPChan;
	CString	m_csDevIP;
	UINT	m_uDevPort;
	CString	m_csDevPwd;
	CString	m_ceDevUserName;
	int		m_iIPDevChan;
	BOOL	m_bStreamUrlEnable;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg void OnNMClickListIpChan(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnSelchangeComboChannelGroupNo();
	afx_msg void OnCbnSelchangeComboStreamType();
	afx_msg void OnBnClickedOk();

	afx_msg void OnBtnAddIPChan();

	afx_msg void OnBtnDelete();
	afx_msg void OnBtnEnable();
	afx_msg void OnBtnRefresh();
	afx_msg void OnBtnSave();
	afx_msg void OnBtnQuickAdd();
	afx_msg void OnBtnStreamUrlSet();
	afx_msg void OnButtonUpdateProList();
	afx_msg void OnButtonCustomPro();

	DECLARE_MESSAGE_MAP()

public:
	void ShowInfoByGetStreamType(int iType);
	void ShowDeviceInfo(BOOL bShow);
	void ShowStreamUrl(BOOL bShow);

	//Set channel information by the type of getting stream.
	void SetChannelInfobyStreamType(BOOL bAnalog);

	//Is enable or online?
	void ShowIsEnableOrOnlineInList(int iRowIndex, int iColumnIndex, int iArrayIndex);
	void GetIsEnableString(BOOL bEnable, char *pStrEnable);

	void SetChannelEnable(BOOL bEnable);

public:
// 	CDlgIPAccessCfgDDNS *m_pDlgIPAccessDDNS;
// 	CDlgIPAccessCfgIPServer *m_pDlgIPAccessIPServer;
// 	CDlgIPAccessCfgStreamServer *m_pDlgIPAccessStreamServer;
// 	DlgIPAccessCfgEasyDDNS *m_pDlgIPAccessCfgEasyDDNS;
	
	afx_msg void OnBnClickedBtnEnable();
	afx_msg void OnBnClickedBtnAddIpChan();
	afx_msg void OnBnClickedBtnDelete();
	afx_msg void OnBnClickedButtonUpdateProList();
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnBnClickedBtnRefresh();

	
};
