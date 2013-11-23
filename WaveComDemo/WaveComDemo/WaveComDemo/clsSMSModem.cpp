#include "StdAfx.h"
#include "clsSMSModem.h"

clsSMSModem::clsSMSModem(void):
m_hSMSModule(NULL),
m_pfuncSmsConnenction(NULL),
m_pfuncSmsSend(NULL),
m_pfuncSmsRecieve(NULL),
m_pfuncSmsDelete(NULL),
m_pfuncSmsAutoFlag(NULL),
m_pfuncSmsNewFlag(NULL),
m_pfuncSmsDisconnection(NULL),
m_bCanDisConnect(FALSE)
{
    m_hSMSModule = LoadLibrary(_T("sms.dll"));
    if(m_hSMSModule)
    {
        m_pfuncSmsConnenction = (pfuncSmsConnenction)GetProcAddress(m_hSMSModule,"Sms_Connection");
        m_pfuncSmsSend = (pfuncSmsSend)GetProcAddress(m_hSMSModule,"Sms_Send");
        m_pfuncSmsRecieve = (pfuncSmsRecieve)GetProcAddress(m_hSMSModule,"Sms_Receive");
        m_pfuncSmsDelete = (pfuncSmsDelete)GetProcAddress(m_hSMSModule,"Sms_Delete");
        m_pfuncSmsAutoFlag = (pfuncSmsAutoFlag)GetProcAddress(m_hSMSModule,"Sms_AutoFlag");
        m_pfuncSmsNewFlag = (pfuncSmsNewFlag)GetProcAddress(m_hSMSModule,"Sms_NewFlag");
        m_pfuncSmsDisconnection = (pfuncSmsDisconnection)GetProcAddress(m_hSMSModule,"Sms_Disconnection");
    }
}

clsSMSModem::~clsSMSModem(void)
{
    if(m_hSMSModule)
    {
        FreeLibrary(m_hSMSModule);
        m_hSMSModule = NULL;
    }
}

int clsSMSModem::SmsConnenction(int iPort,int iBaud,CString *strMobileType)
{
    if(m_pfuncSmsConnenction)
    {
        CString strCopyRight = _T("//上海迅赛信息技术有限公司,网址www.xunsai.com//");
        int iValue = m_pfuncSmsConnenction(strCopyRight,iPort,iBaud,strMobileType);
        if(iValue == 1)
        {
            m_bCanDisConnect = TRUE;
        }
        return iValue;
    }
    return 0;
}

int clsSMSModem::SmsSend(CString strPhoneNum,CString strText)
{
    if(m_pfuncSmsSend)
    {
        return m_pfuncSmsSend(strPhoneNum,strText);
    }
    return 0;
}

int clsSMSModem::SmsRecieve(CString strSmsType,CString *strText)
{
    if(m_pfuncSmsRecieve)
    {
        return m_pfuncSmsRecieve(strSmsType,strText);
    }
    return 0;
}

int clsSMSModem::SmsDelete(CString strTextIndex)
{
    if(m_pfuncSmsDelete)
    {
        return m_pfuncSmsDelete(strTextIndex);
    }
    return 0;
}

int clsSMSModem::SmsAutoFlag()
{
    if(m_pfuncSmsAutoFlag)
    {
        return m_pfuncSmsAutoFlag();
    }
    return 0;
}

int clsSMSModem::SmsNewFlag()
{
    if(m_pfuncSmsNewFlag)
    {
        return m_pfuncSmsNewFlag();
    }
    return 0;
}

int clsSMSModem::SmsDisconnection()
{
    if(m_pfuncSmsDisconnection && m_bCanDisConnect)
    {
        m_bCanDisConnect = FALSE;
        m_pfuncSmsDisconnection();
        return 1;
    }
    return 0;
}