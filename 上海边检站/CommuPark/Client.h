/**
@FileName: Client.h
@Brief:  这个类负责连接数据处理服务器，并按照停车场终端服务器上传过车数据 并作出应答
@ClassName: CClient
@Author: skid
@Date:   20110927
@History:  
*/

#pragma once

#include "TCPSocket.h"
#include "Config.h"
#include "DataConvert.h"




class CClient
{
    public:
        CClient(CConfig &config);
        virtual ~CClient(void);
        
        BOOL Init(CConfig &config);
        
        
        static DWORD WINAPI ReConnectThread(LPVOID lpParam); //重连线程
        static DWORD WINAPI SendThread(LPVOID lpParam);     // 发送线程
        static DWORD WINAPI SendUCIThread(LPVOID lpParam);     // 发送线程
        
        BOOL MakeDataPacket(vector<char> &data,UINT16 wCommand,GDW_PacketHead& packetHead); // 组包
         
        BOOL SendData(TCPSocket &sock,UINT16 wCommand); // 发送数据包
     
        BOOL m_bConnect; // 是否连接上
        
        void DoConnect(); // 重连线程的实现
        void DoProcessData(); // 发送数据
        
        void DoUpdateUCI(); // 发送联检车辆数据
        
        BOOL GetConnect()
        {return m_bConnect;}
        
        TCPSocket m_tcpSocket;
        
        CString  m_strPort;      // 8682
        CString m_strServerIP;   // 数据处理服务器IP
        int m_nSendInterval; //发送数据的间隔时间
        int m_nSendUCIInterval;
        int m_nUploadMode;       // 上传模式
        
        DataConvert m_DataConvert;
        
        vector<char> m_data;
        
        CRITICAL_SECTION m_csData; //buffer的临界区
        
        HANDLE m_hConnectEvent; //重连事件
        HANDLE m_hSendEvent;   // 发送事件
        
        HANDLE m_hSendUCIEvent;
        
        HANDLE m_hConnectThread;
        HANDLE m_hSendThread;
        
        HANDLE m_hSendUCIThread;
        
        
        BOOL DoStart();  // 启动连接开始发送数据
        
        BOOL DoStop();  // 关闭连接
        
        BOOL DoCheckTime();//校时功能

		BOOL DoSendUCIData();//上传联检车辆信息

        BOOL DoSaveTmpUCI();
        
        
        
};
