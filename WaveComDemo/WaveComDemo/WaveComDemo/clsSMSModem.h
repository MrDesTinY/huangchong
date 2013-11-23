#pragma once
#include <winbase.h>
class clsSMSModem
{
public:
    clsSMSModem(void);
    ~clsSMSModem(void);
public:
    typedef int ( __stdcall *pfuncSmsConnenction)(CString strCopyRight,int iPort,int iBaud,CString *strMobileType);
    typedef int ( __stdcall *pfuncSmsSend)(CString strPhoneNum,CString strText);
    typedef int ( __stdcall *pfuncSmsRecieve)(CString strSmsType,CString *strText);
    typedef int ( __stdcall *pfuncSmsDelete)(CString strTextIndex);
    typedef int ( __stdcall *pfuncSmsAutoFlag)();
    typedef int ( __stdcall *pfuncSmsNewFlag)();
    typedef int ( __stdcall *pfuncSmsDisconnection)();
public:
    int SmsConnenction(int iPort,int iBaud,CString *strMobileType);
    int SmsSend(CString strPhoneNum,CString strText);
    int SmsRecieve(CString strSmsType,CString *strText);
    int SmsDelete(CString strTextIndex);
    int SmsAutoFlag();
    int SmsNewFlag();
    int SmsDisconnection();
private:
    pfuncSmsConnenction m_pfuncSmsConnenction;
    pfuncSmsSend m_pfuncSmsSend;
    pfuncSmsRecieve m_pfuncSmsRecieve;
    pfuncSmsDelete m_pfuncSmsDelete;
    pfuncSmsAutoFlag m_pfuncSmsAutoFlag;
    pfuncSmsNewFlag m_pfuncSmsNewFlag;
    pfuncSmsDisconnection m_pfuncSmsDisconnection;
    HMODULE m_hSMSModule;//ø‚∂‘œÛ
    BOOL m_bCanDisConnect;
};
