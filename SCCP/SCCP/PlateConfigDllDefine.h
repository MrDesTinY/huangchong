/** @file    PlateConfigDllDefine.h
  * @note    HangZhou Hikvision Software Co.,Ltd.All Right Reserved.
  * @brief   本文件是车牌识别配置dll头文件
  *
  * @author  杨威yf2
  * @date    2011/10/27
  */
#ifndef PLATE_CONFIG_DLL_DEFINE_H
#define PLATE_CONFIG_DLL_DEFINE_H

#ifdef  PLATE_CONFIG_DLL_EXMPORT
#define AFX_PLATECONFIG_API extern "C" _declspec(dllexport)
#else
#define AFX_PLATECONFIG_API extern "C" _declspec(dllimport)
#endif

#define DEVVCATAB		1			                      //通道配置信息界面
#define PLAYWNDWIDTH  352
#define PLAYWNDHEIGHT 288
#define MINWIDTH	850
#define MINHEIGHT	690

#define NAME_LEN			    32                       //用户名长度
#define PASSWD_LEN			    16                       //密码长度
#define CHAN_INFO_LEN           128                      //监控点信息长度

#define WNDREMAINLEN	7	                             //对话框边框保留长度
#define WM_PLATECFGDLG_DETROY	WM_USER + 1110

//定义语言版本 --byshm
enum{LANG_CHI = 0,                                       //简体中文
     LANG_ENG,		                                     //英文
     LANG_TRAD,	                                         //繁体
     LANG_RUS,
     LANG_PLS,
     LANG_FRCH, 
     LANG_SPA,
     LANG_CORA,
     LANG_GMN,
     LANG_ITAL,
     LANG_POR,                                           //葡萄牙文
};

//tab页配置窗口序号
typedef enum tag_Sub_PlateConfig_Tab
{
	SUB_CONFIG_TAB_ARITH = 0,			                //车牌识别配置
	SUB_CONFIG_TAB_PLATE,				                //算法库参数配置
}SUB_PLATECONFIG_TAB, *LPSUB_PLATECONFIG_TAB;


//窗口参数
typedef struct
{
	int			iTabShow;								//远程智能配置窗口选择显示的属性页
	int			iLanguage;								//远程智能配置窗口的语言种类
	HWND		hParWnd;								//远程智能配置窗口的父窗口
	CRect		rcWndRect;								//远程智能配置窗口的大小
	bool		bFixedSize;								//窗口位移是否固定

	COLORREF	clrPen;									//远程智能配置窗口中列表间隔线的画笔
	LOGBRUSH	lbBackBrush;							//远程智能配置窗口中的背景底色画刷
	LOGBRUSH	lbTitleBkBrush;							//远程智能配置窗口中的标题底色画刷
	LOGBRUSH	lbListTitleBkBr;						//远程智能配置窗口中的列表表头底色画刷
	LOGBRUSH	lbStaticBkBrush;						//远程智能配置窗口的静态框底色画刷
	LOGBRUSH	lbEditBrush;							//远程智能配置窗口的编辑框底色画刷
	LOGBRUSH	lbSelBkBrush;							//远程智能配置窗口的列表选中底色画刷

	//预留位
	LOGBRUSH	lbRes1;									
	LOGBRUSH	lbRes2;
	DWORD		dwRes1;												
	DWORD		dwRes2;
	void NET_INTELCFG_WNDPARAM()
	{
		iTabShow			=	DEVVCATAB;
		iLanguage			=	LANG_CHI;
		hParWnd				=	NULL;
		rcWndRect.SetRect(0,0,MINWIDTH,MINHEIGHT);
		bFixedSize			=	true;

		clrPen				= RGB(107,107,107);
		lbBackBrush.lbColor = RGB(204,204,204);
		lbBackBrush.lbStyle = BS_SOLID;
		lbTitleBkBrush.lbColor = RGB(148,148,148);
		lbTitleBkBrush.lbStyle = BS_SOLID;
		lbListTitleBkBr.lbColor = RGB(174,174,174);
		lbListTitleBkBr.lbStyle = BS_SOLID;
		lbStaticBkBrush.lbColor = RGB(209,209,209);
		lbStaticBkBrush.lbStyle = BS_SOLID;
		lbEditBrush.lbColor = RGB(228,228,228);
		lbEditBrush.lbStyle = BS_SOLID;
		lbSelBkBrush.lbColor = RGB(49,106,197);
		lbSelBkBrush.lbStyle = BS_SOLID;

		ZeroMemory(&lbRes1, sizeof(LOGBRUSH));
		ZeroMemory(&lbRes2, sizeof(LOGBRUSH));
		dwRes1               = 0;
		dwRes2               = 0;
	}
}NET_INTELCFG_WNDPARAM,*LPNET_INTELCFG_WNDPARAM;
//登陆设备信息
typedef struct tag_NET_INTELCFG_DEVLOGININFO
{
	char	chDevIP[16];				//设备IP
	char	chUserName[NAME_LEN];		//用户名
	char	chPassword[PASSWD_LEN];		//密码
	int		iChanNum;					//从1开始
	int		iMediaSrvNum;				//流媒体个数（现在暂不支持）
	WORD	wDevPort;					//设备端口
	LONG	lConnectType;				//连接sdk的方式，是否通过流媒体的标志
	LPVOID	pMediaSrvDir;				//流媒体路径指针
	BYTE	res[8];
	tag_NET_INTELCFG_DEVLOGININFO()
	{
		iChanNum = 1;
		wDevPort = 8000;
		lConnectType = 0;
		iMediaSrvNum = 0;
		pMediaSrvDir = NULL;
		ZeroMemory(chDevIP, 16);
		ZeroMemory(chUserName, NAME_LEN);
		ZeroMemory(chPassword, PASSWD_LEN);
		ZeroMemory(res, 8);
	}
}NET_INTELCFG_DEVLOGININFO,*LPNET_INTELCFG_DEVLOGININFO ;

//监控点信息
typedef struct tag_NET_INTELCFG_MONITORINFO
{
	char chChanName[CHAN_INFO_LEN];      //监控点名字
	char chDevName[CHAN_INFO_LEN];       //所属设备名字
}NET_INTELCFG_MONITORINFO, *LPNET_INTELCFG_MONITORINFO;


/** @fn       NET_DVR_SetMonitorInfo(LPNET_INTELCFG_MONITORINFO pMonitorInfo)
  * @brief    设置监控点信息  
  * @param    pMonitorInfo  :监控点信息
  * @return   void   
  */
AFX_PLATECONFIG_API void __stdcall NET_DVR_SetMonitorInfo(LPNET_INTELCFG_MONITORINFO pMonitorInfo);

/** @fn       NET_DVR_CreateWnd(LPNET_INTELCFG_WNDPARAM pIntelWndParam)
  * @brief    创建远程配置窗口  
  * @param    pIntelWndParam  :远程智能配置窗口画刷，父窗口句柄，窗口大小，语言种类等信息
  * @return   true : 创建窗口成功 ;false: 创建窗口失败 
  */
AFX_PLATECONFIG_API bool __stdcall NET_DVR_CreateWnd(LPNET_INTELCFG_WNDPARAM pIntelWndParam);

/** @fn       NET_DVR_SetIntelligentCfg
  * @brief    初始化智能设备配置界面信息并显示界面  
  * @param    pDevLoginInfo  :设备登陆信息
  * @return   true : 智能设备界面初始化后成功显示; false: 智能设备界面未成功显示  
  */
AFX_PLATECONFIG_API bool __stdcall NET_DVR_SetIntelligentCfg(LPNET_INTELCFG_DEVLOGININFO pDevLoginInfo);

/** @fn       NET_DVR_SetChanIntelligentCfg(LPNET_INTELCFG_DEVLOGININFO pDevLoginInfo)
  * @brief    初始化智能通道配置界面信息并显示界面  
  * @param    LPNET_INTELCFG_DEVLOGININFO pDevLoginInfo  :设备登陆信息
  * @return   true : 智能通道界面初始化后成功显示; false: 智能通道界面未成功显示    
  */
AFX_PLATECONFIG_API bool __stdcall NET_DVR_SetChanIntelligentCfg(LPNET_INTELCFG_DEVLOGININFO pDevLoginInfo);

/** @fn       NET_DVR_DllPreTranslateMessage(MSG *pMsg)
  * @brief    截取父窗口的PreTranslateMessage消息，在动态库中进行相应  
  * @param    MSG * pMsg  :截取的消息
  * @return   true ： 截取消息成功; false： 截取消息失败
  */
AFX_PLATECONFIG_API bool __stdcall NET_DVR_DllPreTranslateMessage(MSG *pMsg);

/** @fn       NET_DVR_DestroyDlg(bool bExit = false)
  * @brief    关闭远程配置窗口  
  * @param    bool bExit  :是否是程序退出
  * @return   true ： 销毁窗口成功; false： 销毁窗口失败  
  */
AFX_PLATECONFIG_API bool __stdcall NET_DVR_DestroyDlg(bool bExit = false);

#endif