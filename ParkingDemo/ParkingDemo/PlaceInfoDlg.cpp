// PlaceInfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ParkingDemo.h"
#include "PlaceInfoDlg.h"


// CPlaceInfoDlg 对话框

IMPLEMENT_DYNAMIC(CPlaceInfoDlg, CDialog)

CPlaceInfoDlg::CPlaceInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPlaceInfoDlg::IDD, pParent)
	, m_iChanNo(0)
	, m_iLeftPlaceNo(0)
	, m_iRightPlaceNo(0)
{

}

CPlaceInfoDlg::~CPlaceInfoDlg()
{
}

void CPlaceInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CHAN_NO, m_iChanNo);
	DDX_Text(pDX, IDC_EDIT_LEFT_PLACE_NO, m_iLeftPlaceNo);
	DDX_Text(pDX, IDC_EDIT_RIGHT_PLACE_NO, m_iRightPlaceNo);
}


BEGIN_MESSAGE_MAP(CPlaceInfoDlg, CDialog)
END_MESSAGE_MAP()


// CPlaceInfoDlg 消息处理程序
