#pragma once


#include "hiksadp.h"
#include <vector>

#define SADP_TEST

/** @struct	 _SADPINFO_
* @brief	 SADP信息
*/
#define		WM_SADP							WM_USER + 26	//设备巡检
typedef struct _SADPINFO_
{
	CString		strSeries;				///< 服务名				
	CString		strDeviceID;			///< 设备ID
	CString		strNetworkAddr;			///< 网络地址
	CString		strDevIP;				///< 设备IP
	CString		strSubnetMask;			///< 子网掩码
	CString		strSoftwareVersion;		///< 软件版本
	CString		strDspVersion;			///< Dsp版本
	CString		strStartTime;			///< 开启时间
	int			nDevType;				///< 设备类型
	int			nDevPort;				///< 设备端口
	int			nChanNum;				///< 通道数目
	int			nDiskNum;				///< 磁盘数目
	int			nResult;				///< 结果
}SADPINFO,*PSADPINFO;

typedef BOOL(__stdcall *PFN_SADP_START)(void(CALLBACK *fHandleDataCallBack)(
										char*        szSeries,
										char*        szDeviceid,
										char*        szHwaddr,
										char*        szPraddr,
										char*        szSubnetmask,
										unsigned int nDevtype,
										unsigned int nPort,
										unsigned int nEnccnt,
										unsigned int nHdiskcnt,
										char*        szSfwversion, 
										char*        szDspversion, 
										char*        szStarttime,
										int          result));
typedef BOOL(__stdcall *PFN_SADP_STOP)();
typedef BOOL(__stdcall *PFN_SADP_SENDINQUIRY)();
typedef BOOL(__stdcall *PFN_SADP_CONFIGLOAD)(char* devFilename);
typedef BOOL(__stdcall *PFN_SADP_SENDUPDATEIP)(char* desDLC,char *desIP,char* subnetmask,char * passwd,char * port);
typedef PINFO_ADAPTER (__stdcall *PFN_SADP_SYSINFO)();
typedef LONG (__stdcall *PFN_SADP_GETADAPNUM)();
typedef BOOL (__stdcall *PFN_SADP_SENDSETUSERPASSWORD)(char* desDLC,char* desIP,char* subnetmask,char* passwd,char* port);

// CSadpDlg dialog
class CSadpDlg : public CDialog
{
	DECLARE_DYNAMIC(CSadpDlg)

public:
	CSadpDlg(HMODULE	 hSadpModule,CWnd* pParent = NULL);   // standard constructor
	//inline CSadpDlg()
	//{
	//	this->m_hSadpModule = hSadpModule;
	//}
	virtual ~CSadpDlg();

// Dialog Data
	enum { IDD = IDD_DLG_SADP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg LRESULT OnMsgSadp(WPARAM wParam, LPARAM lParam);
	afx_msg void OnLvnDeleteItemListDevice(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListDevice(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickListDevice(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnModify();
	afx_msg void OnBnClickedBtnCancelModify();
	afx_msg void OnBnClickedBtnSaveDevParam();
	afx_msg void OnBnClickedBtnSelDevice();
	afx_msg void OnBnClickedBtnRefreshList();

	DECLARE_MESSAGE_MAP()
	void InitCtrlState();
	void UpdateCtrlState();
	void InitDevListCols();
	void ArrangeDevListSize();
	void Add2DevList(const SADPINFO *pSadpInfo);
	void DelDeviceFromList(const SADPINFO* pSadpInfo);
	void UpdateDeviceList(const SADPINFO* pSadpInfo,int nDeviceIndex = -1);
	
	CString GetDeviceTypeName(int nDevType);
	int  GetDeviceIndex(const SADPINFO* pSadpInfo);
	BOOL ValidateParams();
	void ClearSadpInfo();
public:
	CString GetSelDeviceIP()   const;
	int     GetSelDevicePort() const;
	BOOL	InitSadpModuleFuncs();

public:
	CString						m_strDeviceID;			///< 设备ID
	CString						m_strSoftwareVer;		///< 软件版本
	CString						m_strDevIP;				///< 设备IP
	CString						m_strSubnetMask;		///< 子网掩码
	CString						m_strDevMac;			///< MAC地址
	CString						m_strAdminPwd;			///< 密码
	int							m_nDevPort;				///< 端口
	BOOL						m_bModifyDevice;		///< 是否修改
	CIPAddressCtrl				m_ctrlDevIP;			///< 设备IP控件
	CIPAddressCtrl				m_ctrlSubnetMask;		///< 子网掩码控件
	CEdit						m_editDevPort;			///< 设备端口控件
	CListCtrl					m_listDevice;			///< 设备列表控件
	std::vector<SADPINFO *>		m_vSadpInfo;			///< 设备信息



	HMODULE						m_hSadpModule;
	PFN_SADP_STOP				m_pfnSadpStop;
	PFN_SADP_START				m_pfnSadpStart;
	PFN_SADP_SENDUPDATEIP		m_pfnSadpSendUpdateIP;

	afx_msg void OnBnClickedBtnModify2();
};
