#pragma once


// CIntelConfig dialog

//#define   DLL_TEST	
#undef	  DLL_TEST

class CIntelConfig : public CDialog
{
	DECLARE_DYNAMIC(CIntelConfig)

public:
	CIntelConfig(CWnd* pParent = NULL);   // standard constructor
	virtual ~CIntelConfig();

	void SetIntelCfgDeviceInfo(NET_INTELCFG_DEVLOGININFO *pDevInfo);

	// Dialog Data
	enum { IDD = IDD_DIALOG_INTEL_CONFIG };
	typedef enum _INTELE_ERR_CODE
	{      
		INTEL_ERR_NO = 0,                 //无错误
		INTEL_ERR_MODULE_NOTFOUND,        //未找到智能配置模块文件
		INTEL_ERR_LOADMODULE_FAILED,      //加载配置模块失败
		INTEL_ERR_LOADFUNCTION_FAILED,    //初始化配置模块函数库失败
		INTEL_ERR_SYSCHANEXCEPTION,       //通道异常
		INTEL_ERR_NOTHIKDEVICE,           //非海康设备
		INTEL_ERR_INVALIDCHANTYPE,        //设备通道类型不合法
		INTEL_ERR_GETCTRLCFG_FAILED       //获取通道控制信息失败
	}INTEL_ERR_CODE;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg LRESULT OnPlateCfgDlgDestroy(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
	void SetDlgRect(CRect rtDlg);
	bool CreateIntelCfgModuleWnd();
	bool ShowIntelCfgModuleChanCfg();
	bool ShowIntelCfgModuleDevCfg();
	bool InitIntelCfgModFunc();


private:
	NET_INTELCFG_DEVLOGININFO		m_stDevLoginInfo;

	/*智能配置库导出函数类型定义*/
	typedef bool(__stdcall *NET_DVR_CreateWndFunc)(LPNET_INTELCFG_WNDPARAM pIntelWndParam);
	typedef bool(__stdcall *NET_DVR_SetIntelDevCfgFunc)(LPNET_INTELCFG_DEVLOGININFO pDevLoginInfo);
	typedef bool(__stdcall *NET_DVR_SetIntelChanCfgFunc)(LPNET_INTELCFG_DEVLOGININFO pDevLoginInfo);
	typedef bool(__stdcall *NET_DVR_DllPreTranslateMessageFunc)(MSG *pMsg);
	typedef bool(__stdcall *NET_DVR_DestroyDlgFunc)(bool bExit /* = false */);
	typedef bool(__stdcall *NET_DVR_SetShowRuleTabFunc)(bool bShowRuleTab /* = false */);
	typedef void(__stdcall *NET_DVR_SetMonitorInfoFunc)(LPNET_INTELCFG_MONITORINFO pMonitorInfo);



	//智能配置库导出函数指针
	HMODULE								m_hIntelCfgModule;
	NET_DVR_CreateWndFunc				m_pNET_DVR_CreateWndFunc;
	NET_DVR_SetIntelDevCfgFunc			m_pNET_DVR_SetIntelDevCfgFunc;
	NET_DVR_SetIntelChanCfgFunc			m_pNET_DVR_SetIntelChanCfgFunc;
	NET_DVR_DllPreTranslateMessageFunc	m_pNET_DVR_DllPreTranslateMessageFunc;
	NET_DVR_DestroyDlgFunc				m_pNET_DVR_DestroyDlgFunc;
	NET_DVR_SetShowRuleTabFunc			m_pNET_DVR_SetShowRuleTabFunc;
	NET_DVR_SetMonitorInfoFunc          m_pNET_DVR_SetMonitorInfoFunc;

public:
	afx_msg void OnDestroy();
	afx_msg void OnClose();
};
