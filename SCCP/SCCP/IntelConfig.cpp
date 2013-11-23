// IntelConfig.cpp : implementation file
//

#include "stdafx.h"
#include "SCCP.h"
#include "IntelConfig.h"
#define DLL_TEST

// CIntelConfig dialog

IMPLEMENT_DYNAMIC(CIntelConfig, CDialog)

CIntelConfig::CIntelConfig(CWnd* pParent /*=NULL*/)
	: CDialog(CIntelConfig::IDD, pParent)
	,m_hIntelCfgModule(NULL)
	, m_pNET_DVR_CreateWndFunc(NULL)
	, m_pNET_DVR_SetIntelDevCfgFunc(NULL)
	, m_pNET_DVR_SetIntelChanCfgFunc(NULL)
	, m_pNET_DVR_DllPreTranslateMessageFunc(NULL)
	, m_pNET_DVR_DestroyDlgFunc(NULL)
	, m_pNET_DVR_SetMonitorInfoFunc(NULL)
{

}

CIntelConfig::~CIntelConfig()
{
}

void CIntelConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CIntelConfig, CDialog)
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_MESSAGE(WM_PLATECFGDLG_DETROY, &CIntelConfig::OnPlateCfgDlgDestroy)
END_MESSAGE_MAP()


// CIntelConfig message handlers

BOOL CIntelConfig::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CenterWindow();

	SetWindowText(_T("智能配置"));
	SetDlgRect(CRect(0, 0, MINWIDTH + 7, MINHEIGHT + 30));

#ifdef DLL_TEST
	if (InitIntelCfgModFunc() == FALSE)
	{
		AfxMessageBox(_T("智能配置失败(失败原因:链接库函数加载失败)"));
		EndDialog(1);
		return false;
	}
#endif

	//智能配置模块创建窗口
	if (!CreateIntelCfgModuleWnd())
	{
		AfxMessageBox(_T("智能配置失败(失败原因:创建窗口失败)"));
		EndDialog(1);
		return false;
	}

	//显示智能配置模块通道配置界面
	ShowIntelCfgModuleChanCfg();


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

/** @fn		void CIntelConfig::SetIntelCfgDeviceInfo(NET_INTELCFG_DEVLOGININFO *pDevInfo)
 *	@brief	设置配置信息
 *	@param	[IN]	pDevInfo	配置信息
 *	@return	void
 */
void CIntelConfig::SetIntelCfgDeviceInfo(NET_INTELCFG_DEVLOGININFO *pDevInfo)
{
	if (pDevInfo == NULL)
	{
		return;
	}

	memcpy(&m_stDevLoginInfo, pDevInfo, sizeof(NET_INTELCFG_DEVLOGININFO));
}



/** @fn		void CIntelConfig::SetDlgRect(CRect rtDlg)
 *	@brief	设置对话框
 *	@param	[IN]	rtDlg	对话框位置
 *	@return void
 */
void CIntelConfig::SetDlgRect(CRect rtDlg)
{
	//初始化标题栏的位置
	CRect rtWnd(0, 0, 0, 0);
	GetWindowRect(&rtWnd);
	rtWnd.right = rtWnd.left + rtDlg.Width();
	rtWnd.bottom = rtWnd.top + rtDlg.Height();

	MoveWindow(rtWnd);
}

/** @fn		bool CIntelConfig::CreateIntelCfgModuleWnd()
 *	@brief	创建智能配置库窗口
 *	@return	bool	true表示成功，否则失败
 */
bool CIntelConfig::CreateIntelCfgModuleWnd()
{
#ifdef DLL_TEST
	if (m_pNET_DVR_CreateWndFunc == NULL)
	{
		return false;
	}
#endif
	//窗口信息
	NET_INTELCFG_WNDPARAM struIntelCfgWndParam		=	{0};

	struIntelCfgWndParam.iTabShow					=	SUB_CONFIG_TAB_ARITH;
	struIntelCfgWndParam.iLanguage					=	LANG_CHI;
	struIntelCfgWndParam.hParWnd					=	m_hWnd;//GetSafeHwnd();
	struIntelCfgWndParam.rcWndRect.top				=	0;
	struIntelCfgWndParam.rcWndRect.bottom			=	struIntelCfgWndParam.rcWndRect.top + MINHEIGHT;
	struIntelCfgWndParam.rcWndRect.left				=	0;
	struIntelCfgWndParam.rcWndRect.right			=	struIntelCfgWndParam.rcWndRect.left + MINWIDTH;
	struIntelCfgWndParam.bFixedSize					=	true;

	struIntelCfgWndParam.clrPen						=	RGB(107,107,107);
	struIntelCfgWndParam.lbBackBrush.lbColor		=	RGB(204,204,204);
	struIntelCfgWndParam.lbBackBrush.lbStyle		=	BS_SOLID;
	struIntelCfgWndParam.lbTitleBkBrush.lbColor		=	RGB(148,148,148);
	struIntelCfgWndParam.lbTitleBkBrush.lbStyle		=	BS_SOLID;
	struIntelCfgWndParam.lbListTitleBkBr.lbColor	=	RGB(174,174,174);
	struIntelCfgWndParam.lbListTitleBkBr.lbStyle	=	BS_SOLID;
	struIntelCfgWndParam.lbStaticBkBrush.lbColor	=	RGB(209,209,209);
	struIntelCfgWndParam.lbStaticBkBrush.lbStyle	=	BS_SOLID;
	struIntelCfgWndParam.lbEditBrush.lbColor		=	RGB(228,228,228);
	struIntelCfgWndParam.lbEditBrush.lbStyle		=	BS_SOLID;
	struIntelCfgWndParam.lbSelBkBrush.lbColor		=	RGB(49,106,197);
	struIntelCfgWndParam.lbSelBkBrush.lbStyle		=	BS_SOLID;
#ifdef DLL_TEST
	return m_pNET_DVR_CreateWndFunc(&struIntelCfgWndParam);
#else
	return NET_DVR_CreateWnd(&struIntelCfgWndParam);
#endif
}

/** @fn		bool CIntelConfig::ShowIntelCfgModuleChanCfg()
 *	@brief	显示智能配置模块通道配置界面
 *	@return bool true 表示成功，否则失败
 */
bool CIntelConfig::ShowIntelCfgModuleChanCfg()
{
#ifdef DLL_TEST
	if (m_pNET_DVR_SetIntelChanCfgFunc == NULL)
	{
		return false;
	}
#endif
	NET_INTELCFG_MONITORINFO		stMonitorInfo;
	
	memset(&stMonitorInfo, 0, sizeof(NET_INTELCFG_MONITORINFO));
	sprintf_s(stMonitorInfo.chDevName,CHAN_INFO_LEN, "%s", m_stDevLoginInfo.chDevIP);
	sprintf_s(stMonitorInfo.chChanName,CHAN_INFO_LEN, "%d", m_stDevLoginInfo.iChanNum);
#ifdef DLL_TEST
	m_pNET_DVR_SetMonitorInfoFunc(&stMonitorInfo);
	
	return m_pNET_DVR_SetIntelChanCfgFunc(&m_stDevLoginInfo);
#else
	NET_DVR_SetMonitorInfo(&stMonitorInfo);
	return NET_DVR_SetChanIntelligentCfg(&m_stDevLoginInfo);
#endif
}

/** @fn		void CIntelConfig::OnDestroy()
 *	@brief	销毁窗口
 *	@return	void
 */
void CIntelConfig::OnDestroy()
{
#ifdef DLL_TEST
	if (m_pNET_DVR_DestroyDlgFunc != NULL)
	{
		m_pNET_DVR_DestroyDlgFunc(TRUE);
	}

	//卸载模块
	if (m_hIntelCfgModule != NULL)
	{
		FreeLibrary(m_hIntelCfgModule);
		m_hIntelCfgModule = NULL;
	}
#else
	NET_DVR_DestroyDlg(TRUE);
#endif
	CDialog::OnDestroy();
	// TODO: Add your message handler code here
}

/** @fn		bool CIntelConfig::ShowIntelCfgModuleDevCfg()
 *	@brief	显示智能设备资源配置界面
 *	@return	bool	true表示成功，否则失败
 */
bool CIntelConfig::ShowIntelCfgModuleDevCfg()
{
#ifdef DLL_TEST
	if (m_pNET_DVR_SetIntelDevCfgFunc == NULL)
	{
		return false;
	}
	return m_pNET_DVR_SetIntelDevCfgFunc(&m_stDevLoginInfo);
#else
	return NET_DVR_SetIntelligentCfg(&m_stDevLoginInfo);
#endif
}

void CIntelConfig::OnBnClickedOk()
{

}
void CIntelConfig::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnClose();
}

bool CIntelConfig::InitIntelCfgModFunc()
{
	//初始化数据
	if (m_hIntelCfgModule != NULL)
	{
		FreeLibrary(m_hIntelCfgModule);
		m_hIntelCfgModule = NULL;
	}

	m_pNET_DVR_CreateWndFunc				=	NULL;
	m_pNET_DVR_SetIntelDevCfgFunc			=	NULL;
	m_pNET_DVR_SetIntelChanCfgFunc			=	NULL;
	m_pNET_DVR_DllPreTranslateMessageFunc	=	NULL;
	m_pNET_DVR_DestroyDlgFunc				=	NULL;
	m_pNET_DVR_SetShowRuleTabFunc			=   NULL;
	m_pNET_DVR_SetMonitorInfoFunc           =   NULL;



	CString strModuleName = _T("PlateConfigDll.dll");

	//加载dll
	m_hIntelCfgModule = LoadLibrary(strModuleName);
	if (m_hIntelCfgModule == NULL)
	{
		return false;
	}

	//获取配置库创建窗口函数地址
	m_pNET_DVR_CreateWndFunc = (NET_DVR_CreateWndFunc)GetProcAddress(m_hIntelCfgModule, "NET_DVR_CreateWnd");
	if (m_pNET_DVR_CreateWndFunc == NULL)
	{
		return false;
	}

	m_pNET_DVR_SetIntelDevCfgFunc = (NET_DVR_SetIntelDevCfgFunc)GetProcAddress(m_hIntelCfgModule, "NET_DVR_SetIntelligentCfg");
	if (m_pNET_DVR_SetIntelDevCfgFunc == NULL)
	{
		return false;
	}

	m_pNET_DVR_SetIntelChanCfgFunc = (NET_DVR_SetIntelChanCfgFunc)GetProcAddress(m_hIntelCfgModule, "NET_DVR_SetChanIntelligentCfg");
	if (m_pNET_DVR_SetIntelChanCfgFunc == NULL)
	{
		return false;
	}

	m_pNET_DVR_DllPreTranslateMessageFunc = (NET_DVR_DllPreTranslateMessageFunc)GetProcAddress(m_hIntelCfgModule, "NET_DVR_DllPreTranslateMessage");
	if (m_pNET_DVR_DllPreTranslateMessageFunc == NULL)
	{
		return false;
	}

	m_pNET_DVR_DestroyDlgFunc = (NET_DVR_DestroyDlgFunc)GetProcAddress(m_hIntelCfgModule,"NET_DVR_DestroyDlg");
	if (m_pNET_DVR_DestroyDlgFunc == NULL)
	{
		return false;
	}
	m_pNET_DVR_SetMonitorInfoFunc = (NET_DVR_SetMonitorInfoFunc)GetProcAddress(m_hIntelCfgModule,"NET_DVR_SetMonitorInfo");
	if (m_pNET_DVR_SetMonitorInfoFunc == NULL)
	{
		return false;
	}
	return true;
}

/** @fn       LRESULT CIntelConfig::OnPlateCfgDlgDestroy(WPARAM wParam, LPARAM lParam)
  * @brief    收到智能配置销毁时消息，弹出对话框也进行关闭  
  * @param    WPARAM wParam  :
  * @param    LPARAM lParam  :
  * @return   LRESULT  
  */
LRESULT CIntelConfig::OnPlateCfgDlgDestroy(WPARAM wParam, LPARAM lParam)
{
	EndDialog(1);
#ifdef DLL_TEST
	if (m_pNET_DVR_DestroyDlgFunc != NULL)
	{
		m_pNET_DVR_DestroyDlgFunc(false);
	}
#else
	NET_DVR_DestroyDlg(false);
#endif

	return 1;
}