#pragma once
#include <windef.h>

class clsTDKJClass{
public:
    clsTDKJClass();
    ~clsTDKJClass();

    typedef unsigned (WINAPI * DSBDLL)(unsigned ComPort,char * OutString);
private:
    HMODULE m_hTDBJQ;
    DSBDLL m_funcDsbdll;
public:
    void TDKJ_Wait();//等待函数
    void TDKJ_VoiceShow(int iMinutes,double iShouldPay,double iFinalPay);//付费函数
    void TDKJ_Clear();//清屏函数
    void TDKJ_Cmd(char * OutString);//任意命令
};