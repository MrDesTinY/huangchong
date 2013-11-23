
// WaveComDemoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WaveComDemo.h"
#include "WaveComDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CWaveComDemoDlg 对话框




CWaveComDemoDlg::CWaveComDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWaveComDemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWaveComDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CWaveComDemoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BNCONNECT, &CWaveComDemoDlg::OnBnClickedBnconnect)
    ON_BN_CLICKED(IDC_BNSEND, &CWaveComDemoDlg::OnBnClickedBnsend)
    ON_BN_CLICKED(IDC_BNRECIEVE, &CWaveComDemoDlg::OnBnClickedBnrecieve)
    ON_BN_CLICKED(IDC_BNDELINDEX, &CWaveComDemoDlg::OnBnClickedBndelindex)
    ON_BN_CLICKED(IDC_BNCHECKAUTO, &CWaveComDemoDlg::OnBnClickedBncheckauto)
    ON_BN_CLICKED(IDC_BNCHECKNEW, &CWaveComDemoDlg::OnBnClickedBnchecknew)
    ON_BN_CLICKED(IDC_BNDISCONNECT, &CWaveComDemoDlg::OnBnClickedBndisconnect)
    ON_WM_CLOSE()
END_MESSAGE_MAP()


// CWaveComDemoDlg 消息处理程序

BOOL CWaveComDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
    SetDlgItemText(IDC_CBBAUD,_T("9600"));
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CWaveComDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CWaveComDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CWaveComDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CWaveComDemoDlg::OnBnClickedBnconnect()
{
    // TODO: Add your control notification handler code here
    CString strType[256];
    int iPort = GetDlgItemInt(IDC_EDCOMPORT);
    int iBaud = GetDlgItemInt(IDC_CBBAUD);
    if(!m_clsSMSModem.SmsConnenction(iPort,iBaud,strType))
    {
        MessageBox(_T("连接短信猫失败！"));
        return;
    }
    MessageBox(_T("连接短信猫成功！"));
    SetDlgItemText(IDC_EDTYPE,*strType);
    return;
}

void CWaveComDemoDlg::OnBnClickedBnsend()
{
    // TODO: Add your control notification handler code here
    CString cstrTel;
    GetDlgItemText(IDC_EDPHONENUM,cstrTel);
    CString cstrContent;
    GetDlgItemText(IDC_EDSENDCONTENT,cstrContent);
    if(!m_clsSMSModem.SmsSend(cstrTel,cstrContent))
    {
        MessageBox(_T("发送短信失败！"));
    }
    else
    {
        MessageBox(_T("发送短信成功！"));
    }
    return;
}

void CWaveComDemoDlg::OnBnClickedBnrecieve()
{
    // TODO: Add your control notification handler code here
    CString cstrRecType;
    GetDlgItemText(IDC_CBRECTYPE,cstrRecType);
    CString strRec;
    if(!m_clsSMSModem.SmsRecieve(cstrRecType,&strRec))
    {
        MessageBox(_T("接收短信失败！"));
        return;
    }
    else
    {
        MessageBox(_T("接收短信成功！"));
    }
    return;
}

void CWaveComDemoDlg::OnBnClickedBndelindex()
{
    // TODO: Add your control notification handler code here
    CString cstrIndex;
    GetDlgItemText(IDC_EDSMSINDEX,cstrIndex);
    if(!m_clsSMSModem.SmsDelete(cstrIndex))
    {
        MessageBox(_T("删除短信失败！"));
    }
    else
    {
        MessageBox(_T("删除短信成功！"));
    }
    return;
}

void CWaveComDemoDlg::OnBnClickedBncheckauto()
{
    // TODO: Add your control notification handler code here
    if(!m_clsSMSModem.SmsAutoFlag())
    {
        SetDlgItemText(IDC_EDAUTO,_T("不支持"));
    }
    else
    {
        SetDlgItemText(IDC_EDAUTO,_T("支持"));
    }
}

void CWaveComDemoDlg::OnBnClickedBnchecknew()
{
    // TODO: Add your control notification handler code here
    if(!m_clsSMSModem.SmsNewFlag())
    {
        SetDlgItemText(IDC_EDNEW,_T("无新信息"));
    }
    else
    {
        SetDlgItemText(IDC_EDNEW,_T("有新信息"));
    }
}

void CWaveComDemoDlg::OnBnClickedBndisconnect()
{
    // TODO: Add your control notification handler code here
    if(!m_clsSMSModem.SmsDisconnection())
    {
        MessageBox(_T("断开短信猫失败！"));
    }
    else
    {
        MessageBox(_T("断开短信猫成功！"));
    }
    return;
}

void CWaveComDemoDlg::OnClose()
{
    // TODO: Add your message handler code here and/or call default
    m_clsSMSModem.SmsDisconnection();
    CDialog::OnClose();
}
