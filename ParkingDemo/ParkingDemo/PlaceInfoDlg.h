#pragma once


// CPlaceInfoDlg 对话框

class CPlaceInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CPlaceInfoDlg)

public:
	CPlaceInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPlaceInfoDlg();

// 对话框数据
	enum { IDD = IDD_PLACEINFO_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_iChanNo;
	int m_iLeftPlaceNo;
	int m_iRightPlaceNo;
};
