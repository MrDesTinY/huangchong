
// ParkingDemoDlg.h : 头文件
//

#pragma once
#include "HCNetSDK.h"
#include "OutPutDlg.h"
#include "GeneralDef.h"
#include <vector>
#include "afxwin.h"
#include "afxcmn.h"
using namespace std;


#define TREE_NODE_ROOT   0
#define TREE_NODE_DEVICE 1
#define TREE_NODE_CHAN   2

typedef struct _tagPlayWindow
{
	DWORD dwChanIndex; //通道号数字通道
	int iLeftPlaceNo;
	BOOL bHasLeftVehicle;
	CString strLeftPlate;	
	int iRightPlaceNo;
	BOOL bHasRightVehicle;
	CString strRightPlate;	
	_tagPlayWindow()
	{
		dwChanIndex = 0;
		iLeftPlaceNo = 0;
		bHasLeftVehicle = FALSE;
		strLeftPlate = "";		
		iRightPlaceNo = 0;
		bHasRightVehicle = FALSE;
		strRightPlate = "";	
	}
}PlayWindow;

typedef struct _tagDeviceInfo
{
	int  iUserId;
	//LONG lPlayHandle;
	LONG lAlarmHandle;
	
	char szDeviceName[64];
	char szDVRIP[16];
	WORD wDVRPort;
	char szUserName[32];
	char szPassword[32];
	BOOL bHasLogined;  //是否已经登录
	BOOL bIsPlaying;   //是否正在播放

	HTREEITEM hDeviceItem; //当前DVR设备结点句柄

	PlayWindow struPlayWindow[MAX_OUTPUT_WINDOW_NUM];
	NET_DVR_DEVICEINFO_V30 lpDeviceInfo;	
	NET_DVR_IPPARACFG_V40 struIPParaCfgV40;
	STRU_CHANNEL_INFO struChanInfo[MAX_CHANNUM_V30*2]; //channel structure
	CHANNEL_INFO struZeroChan[16];	

	int iGroupNO;           //IP Group NO.
	int	iAnalogChanNum;		//analog channel number
	BOOL bIPRet;			//support IP conection
	LONG lFirstEnableChanIndex;	 //first enabled channel index

	 _tagDeviceInfo()
	 {
		 lAlarmHandle = -1;
		 iUserId = -1;
		 ZeroMemory(szDeviceName, sizeof(szDeviceName));
		 ZeroMemory(szDVRIP, sizeof(szDVRIP));
		 wDVRPort = 0;
		 ZeroMemory(szUserName, sizeof(szUserName));
		 ZeroMemory(szPassword, sizeof(szPassword));	
		 bHasLogined = FALSE;
		 bIsPlaying = FALSE; 
		 hDeviceItem = NULL;
		 ZeroMemory(&lpDeviceInfo, sizeof(lpDeviceInfo));
		 ZeroMemory(&struIPParaCfgV40, sizeof(struIPParaCfgV40));
		 ZeroMemory(&struChanInfo, sizeof(struChanInfo));
		 ZeroMemory(&struZeroChan, sizeof(struZeroChan));

		 iGroupNO = -1;
		 iAnalogChanNum = 0;
		 bIPRet	= FALSE;
		 lFirstEnableChanIndex = 0;
	 }
}DeviceInfo;

//CRITICAL_SECTION LOCK
class CGuard
{
public:
	CGuard(CRITICAL_SECTION *lpcsLock);
	~CGuard(void);
private:
	CRITICAL_SECTION *m_lpcsLock;
protected:
	CGuard(void);
};

inline CGuard::~CGuard(void)
{	
	LeaveCriticalSection(m_lpcsLock);	
}

inline CGuard::CGuard(CRITICAL_SECTION *lpcsLock)
: m_lpcsLock(lpcsLock)
{
	EnterCriticalSection(m_lpcsLock);
}

// CParkingDemoDlg 对话框
class CParkingDemoDlg : public CDialog
{
// 构造
public:
	CParkingDemoDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CParkingDemoDlg(void);

// 对话框数据
	enum { IDD = IDD_PARKINGDEMO_DIALOG };

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
	COutPutDlg m_OutPutWindow[MAX_OUTPUT_WINDOW_NUM];

	HWND hWnd;
	LONG lRealPlayHandle;
	afx_msg void OnBnClickedOk();

	CString GetLocalIPAddr();
	BOOL CheckIPStr(CString ip);

	void SetOutputWindow(int n);
	//void SelectOutputWindow(int num);

	int m_iTotalDeviceIndex; //设备序号
	int m_iCurDeviceIndex;   //当前设备序号、索引
	int m_iCurChanIndex;     //当前通道序号、索引
	DWORD	  m_dwNodeData;  //结点值
	CString   m_strNodeText; //结点的名称

	vector<DeviceInfo> m_vDevices;

	CTreeCtrl m_treeDevices;
	HTREEITEM m_hRoot;			//根节点句柄
	HTREEITEM m_hCurDeviceItem; //当前DVR设备结点句柄
	HTREEITEM m_hCurChanItem;  //当前通道结点句柄
	HTREEITEM m_hSelect;
	HTREEITEM m_hParent;
	HTREEITEM m_hItem;      //将点击的点转化为当前选项的句柄

	HTREEITEM m_hDeviceItem;//插入的设备结点句柄
	afx_msg void OnNMClickTreeDevice(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickTreeDevice(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkTreeDevice(NMHDR *pNMHDR, LRESULT *pResult);
	int GetCurDeviceIndex(void);

	void LoginAll(); //登录当前DVR下的全部相机
	void LogoutAll();//注销当前DVR下的全部相机

	void ResetOutPutWindow();
	void DeleteTreeChanNode(HTREEITEM hDeviceItem);

	void PlayAll(void); //播放当前DVR下的全部相机
	void StopAll(void);	//停止当前DVR下的全部相机
	BOOL UpdateUI(void);

	afx_msg void OnTreeDevicePlay();
	afx_msg void OnTreeDeviceStopPlay();
	afx_msg void OnTreeDeviceDelete();
	afx_msg void OnTreedeviceLogin();
	afx_msg void OnTreedeviceLogout();

	LONG m_lListenHandle;

	void StartListen();
	void StopListen(); 

	CRITICAL_SECTION m_csLock;
	HANDLE m_hParkThread;     //停车场数据刷新线程句柄	    
	CEvent m_evParkInfo;  //退出线程事件

	BOOL m_IsThreadRun;
	void FreshParkData(int iIndex);
	afx_msg LRESULT OnUpdateOutPutWindow(WPARAM wParam,LPARAM lParam);
	
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTreedeviceProperty();

	BOOL DoSetIPAccessCfg(int iDeviceIndex);
	BOOL DoGetDeviceResoureCfg(int iDeviceIndex, int iChanIndex);
	void RefreshIPDevLocalCfg(int iDeviceIndex);


	afx_msg void OnChanPlaceInfo();
	// 用于标示是否开启牌识上传的变量
	BOOL m_bListenning;
	afx_msg void OnBnClickedButtonEnableUploadData();

	afx_msg void OnBnClickedButtonSetUploadCfg();
	CString m_strHostIP;
	int m_iHostPort;
	//CComboBox m_comboUploadStrategy;
	int m_iUploadInterval;
	int m_iUploadTimeOut;
	BOOL m_bSuspendUpload;

	NET_ITS_UPLOAD_CFG m_struUploadCfg;
	LONG m_lChannel;
	void OnBtnGet();
	void DrawWindowPicture(int iWndIndex);
	
	BOOL m_checkPicData;
	void DrawWindowPicture(int iWndIndex,int iPicSize, unsigned char* pic);
	afx_msg void OnBnClickedCheckPicData();
};
