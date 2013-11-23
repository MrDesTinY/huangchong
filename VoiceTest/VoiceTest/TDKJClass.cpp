#include "stdafx.h"
#include "TDKJClass.h"
clsTDKJClass::clsTDKJClass()
{
    m_hTDBJQ = NULL;
    m_funcDsbdll = NULL;
    m_hTDBJQ = LoadLibrary(L"Tdbjq.dll");
    if(m_hTDBJQ != NULL)
    {
        m_funcDsbdll = (DSBDLL)GetProcAddress(m_hTDBJQ, "dsbdll");  
    }
}
clsTDKJClass::~clsTDKJClass()
{
    if (m_hTDBJQ != NULL)
    {
        FreeLibrary(m_hTDBJQ);
        m_hTDBJQ = NULL;
    }
    m_funcDsbdll = NULL;
}
//等待函数
void clsTDKJClass::TDKJ_Wait()
{
    if(m_funcDsbdll != NULL)
    {
        m_funcDsbdll(0,"&Sc$");
        m_funcDsbdll(0,"&C24您好,请稍等！$");
        m_funcDsbdll(0,"W");
    }
}
//付费函数
void clsTDKJClass::TDKJ_VoiceShow(int iMinutes,double iShouldPay,double iFinalPay)
{
    if(m_funcDsbdll != NULL && iMinutes > 0)
    {
        int iDay = (int)(iMinutes/1440);
        int iHour = (int)((iMinutes%1440)/60);
        int iMinute = (int)(((iMinutes%1440)%60));
        CString cstrParkingTime;
        CString cstrShouldPay;
        CString cstrFinalPay;
        CString cstrShouldPayVoice;
        CString cstrFinalPayVoice;
        cstrParkingTime.Format(_T("&C11停车时长："));
        if(iDay > 0)
        {
            cstrParkingTime.AppendFormat(_T("%d天"),iDay);
        }
        if(iHour > 0)
        {
            cstrParkingTime.AppendFormat(_T("%d小时"),iHour);
        }
        if(iMinute > 0)
        {
            cstrParkingTime.AppendFormat(_T("%d分"),iMinute);
        }
        cstrParkingTime.AppendFormat(_T("$"));
        cstrShouldPay.Format(_T("&C31应收：%0.2f元$"),iShouldPay);
        cstrFinalPay.Format(_T("&C41实收：%0.2f元$"),iFinalPay);
        cstrShouldPayVoice.Format(_T("%fP"),iShouldPay);
        cstrFinalPayVoice.Format(_T("%fP"),iFinalPay);
        m_funcDsbdll(0,"&Sc$");//清屏
        USES_CONVERSION;
        m_funcDsbdll(0,W2A(cstrParkingTime.GetBuffer(0)));
        m_funcDsbdll(0,W2A(cstrShouldPay.GetBuffer(0)));
        m_funcDsbdll(0,"u");
        m_funcDsbdll(0,W2A(cstrShouldPayVoice.GetBuffer(0)));
        m_funcDsbdll(0,W2A(cstrFinalPay.GetBuffer(0)));
        m_funcDsbdll(0,"A");
        m_funcDsbdll(0,W2A(cstrFinalPayVoice.GetBuffer(0)));
        m_funcDsbdll(0,"X");
    }
}
//清屏函数
void clsTDKJClass::TDKJ_Clear()
{
    if(m_funcDsbdll != NULL)
    {
        m_funcDsbdll(0,"&Sc$");
        m_funcDsbdll(0,"&C25谢谢惠顾！$");
    }
}
//任意命令
void clsTDKJClass::TDKJ_Cmd(char * OutString)
{
    if(m_funcDsbdll != NULL)
    {
        m_funcDsbdll(0,OutString);
    }
}