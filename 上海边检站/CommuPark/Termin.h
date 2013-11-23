
/**
@FileName: Termin.h
@ClassName: Termin
@Brief: 出入口终端类，负责启动数据上传服务以及监听命令通道等待 数据处理服务器连接
*/
#pragma once

#include "Client.h"
#include "Server.h"
#include "Config.h"
class Termin
{
    public:
        Termin(CConfig &config);
        virtual ~Termin(void);
        Server m_server;
        CClient m_client; 
        
        BOOL Init(CConfig &config);
        
        BOOL Start();
        
        BOOL Stop();
        
        BOOL CheckTime(); 
	BOOL SendUCIData();
        BOOL SaveTmpUCI();

    private:
        BOOL m_bUpData;
        BOOL m_bListen;
};
