#include "StdAfx.h"
#include "Server.h"


#define THREAD_EXIT_CODE 0xdead

/**
* @Fn: CloseSocket()
* @Brief: closesocket()的包裹函数
* @param c: s 待关闭的socket
* @param n: void
* @Return: void
*/


void CloseSocket(SOCKET &s)
{
    CString strTmp;
    if(s != INVALID_SOCKET)
    {
        int iRet=closesocket(s);
        if(iRet !=0)
        {
            strTmp.Format("关闭socket失败,Error Code:%d",WSAGetLastError());
            OutputDebugString(strTmp);
            CPowerInfo::WLog(strTmp);
            return;
        }

        s = INVALID_SOCKET;
    }
}

/*
@Fn: Recvn() 
@Brief: 接受len 字节后才返回
@param c: s socket pBuff: 接收buffer 头指针,len:接收的字节数
@param n: void
@return :成功 返回0,否则 -1
*/


int Recvn(SOCKET s, char * buff, int len)
{
    CString strTmp;
    int nToRead = len;
    int nRecv = 0;

    while (nToRead > 0)
    {
        nRecv = recv(s, buff, nToRead, 0);

        if (nRecv <= 0)
        {
            strTmp.Format("recvn errcode %d \n",WSAGetLastError());
            OutputDebugString(strTmp);
            CPowerInfo::WLog(strTmp);
            return -1;
        }

        buff += nRecv;
        nToRead -= nRecv;
    }

    return 0;
}

Server* Server::m_pServer = NULL;

Server::Server(void):m_sListen(INVALID_SOCKET)
,m_hListenThread(NULL)
,m_bShutDown(FALSE)
,m_pClientCtx(NULL)
,m_hCheckThread(NULL)
{
    Server::m_pServer = this;
    InitializeCriticalSectionAndSpinCount(&m_csList, 4000);
    

}

Server::~Server(void)
{
    DeleteCriticalSection(&m_csList);

}

BOOL Server::StartServer()
{
    CString strTmp;
    m_sListen = socket(AF_INET,SOCK_STREAM,0);
    if(m_sListen == INVALID_SOCKET)
    {
        strTmp.Format("create listen socket fail,with error code:%d",WSAGetLastError());
        OutputDebugString(strTmp);
        CPowerInfo::WLog(strTmp);
        return FALSE;
    }
    
     int iRet = 0;
    
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(CMD_LISTEN_PORT);
    
    BOOL bSet = TRUE;
    iRet = setsockopt(m_sListen,SOL_SOCKET,SO_REUSEADDR,(const char *)&bSet,sizeof(BOOL));
    
    if(iRet != 0)
    {
        strTmp.Format("skid---set listen socket addr reuse fail,with error code:%d",WSAGetLastError());
        OutputDebugString(strTmp);
        CPowerInfo::WLog(strTmp);
        return FALSE;
    }
    
    iRet = bind(m_sListen,(const sockaddr*)&addr,sizeof(addr));
    
    if(iRet != 0)
    {
        strTmp.Format("skid--bind socket fail,with error code:%d",WSAGetLastError());
        OutputDebugString(strTmp);
        CPowerInfo::WLog(strTmp);
        return FALSE;
    }
    
    iRet = listen(m_sListen,SOMAXCONN);
    
    if(iRet != 0)
    {
        strTmp.Format("skid--------listen() fail,with error code:%d",WSAGetLastError());
        OutputDebugString(strTmp);
        CPowerInfo::WLog(strTmp);
        return FALSE;
    }
    
    m_bShutDown = FALSE;
    
    m_hListenThread = CreateThread(NULL,0,ListenThreadFunc,this,0,NULL);
    if(m_hListenThread == NULL)
    {
        strTmp.Format("skid----启动监听线程失败,with error code :%d\n",GetLastError());
        OutputDebugString(strTmp);
        CPowerInfo::WLog(strTmp);
        return FALSE;
    }
    m_hCheckThread = CreateThread(NULL,0,CheckThreadFunc,this,0,NULL);
    if(m_hCheckThread == NULL)
    {
        strTmp.Format("skid----接收线程启动失败,with error code：%d\n",GetLastError());
        OutputDebugString(strTmp);
        CPowerInfo::WLog(strTmp);
        return FALSE;
    }
    return TRUE;
    
        
}

DWORD Server::ListenThreadFunc(LPVOID lpParam)
{
    Server *pThis = (Server*)lpParam;
    
    CString strTmp;
    
    while(!pThis->m_bShutDown)
    {
        sockaddr_in addr;
        int addrLen = sizeof(sockaddr_in);
        
        SOCKET sClient = accept(pThis->m_sListen,(sockaddr*)&addr,&addrLen);
        
        if(INVALID_SOCKET == sClient)
        {
            return THREAD_EXIT_CODE;
        }
        
        //2 分钟超时
        int timeout = 2* 1000 * 60;
        
        setsockopt(sClient,SOL_SOCKET,SO_SNDTIMEO,(char*)&timeout,sizeof(timeout));
        setsockopt(sClient,SOL_SOCKET,SO_RCVTIMEO,(char*)&timeout,sizeof(timeout));
        
        pThis->ProcessAccepted(sClient,addr);
        
        strTmp.Format("skid----数据处理服务器%s连接!\n",inet_ntoa(addr.sin_addr));
        OutputDebugString(strTmp);
        CPowerInfo::WLog(strTmp);
        
        
    }
    
    strTmp.Format("skid------监听线程退出\n");
    OutputDebugString(strTmp);
    CPowerInfo::WLog(strTmp);
    return THREAD_EXIT_CODE;
    
}


void Server::ProcessAccepted(SOCKET sClient,sockaddr_in &addr)
{
    CString strTmp;
    ClientContext *pClientCtx = new ClientContext;
    
    if(pClientCtx == NULL)
    {
        CloseSocket(sClient);
        strTmp.Format("skid----new ClientContext fail\n");
        OutputDebugString(strTmp);
        CPowerInfo::WLog(strTmp);
        return;
    }
    pClientCtx->pServer = this;
    pClientCtx->sClient = sClient;
    strcpy_s(pClientCtx->chClientIp,16,inet_ntoa(addr.sin_addr));
    
    strTmp.Format("skid----接收新的连接:%s\n",pClientCtx->chClientIp);
    OutputDebugString(strTmp);
    CPowerInfo::WLog(strTmp);
    
    HANDLE hRecvThread = CreateThread(NULL,0,RecvThreadFunc,pClientCtx,CREATE_SUSPENDED,NULL);
    
    if(hRecvThread == NULL)
    {
        CloseSocket(sClient);
        if(pClientCtx)
        {
            delete pClientCtx;
            pClientCtx = NULL;
            strTmp.Format("skid---启动接收线程失败\n");
            OutputDebugString(strTmp);
            CPowerInfo::WLog(strTmp);
            return ;
        }
    }
    
    pClientCtx->hRecvThread = hRecvThread;
    AddClientToList(pClientCtx);
    ResumeThread(pClientCtx->hRecvThread);
    
}

void Server::AddClientToList(ClientContext *pClientCtx)
{
    EnterCriticalSection(&m_csList);
    ClientContext * p = m_pClientCtx;
    m_pClientCtx = pClientCtx;
    m_pClientCtx->pNext = p;
    LeaveCriticalSection(&m_csList);
}

void Server::RemoveAllClients()
{
    ClientContext *pHead = m_pClientCtx;
    
    while(pHead)
    {
        CloseSocket(pHead->sClient);
        if(pHead->hRecvThread)
        {
            WaitForSingleObject(pHead->hRecvThread,INFINITE);
            CloseHandle(pHead->hRecvThread);
            pHead->hRecvThread = NULL;
        }
       
        ClientContext *p = pHead;
        pHead = pHead->pNext;
        if(p)
        {
            delete p;
            p = NULL;
        }         
      
    }
    m_pClientCtx = NULL;

}

void Server::StopServer()
{
    CString strTmp;
    CloseSocket(m_sListen);
    strTmp.Format("skid---服务器停止接收数据");
    OutputDebugString(strTmp);
    CPowerInfo::WLog(strTmp);
    
    m_bShutDown = TRUE;
    
    WaitForSingleObject(m_hListenThread,INFINITE);
    if(m_hListenThread)
    {
        CloseHandle(m_hListenThread);
        m_hListenThread = NULL;
    }
    WaitForSingleObject(m_hCheckThread,INFINITE);
    if(m_hCheckThread)
    {
        CloseHandle(m_hCheckThread);
        m_hCheckThread = NULL;
    }
    
    RemoveAllClients();
    
}

DWORD Server::CheckThreadFunc(LPVOID lpParam)
{
    Server *pThis = (Server*)lpParam;
    while(!pThis->m_bShutDown)
    {
        Sleep(200);
        pThis->DoCheck();
    }
    return THREAD_EXIT_CODE;
}

void Server::DoCheck()
{
    EnterCriticalSection(&m_csList);
    ClientContext *pClientCtx = m_pClientCtx;
    DWORD dwExitCode  =0;
    CString strTmp;
    while(pClientCtx)
    {
        if(!GetExitCodeThread(pClientCtx->hRecvThread,&dwExitCode))
        {
            break;
        }
        
        if(dwExitCode == STILL_ACTIVE)
        {
            pClientCtx = pClientCtx->pNext;
        }
        else if(dwExitCode == THREAD_EXIT_CODE)
        {
            CloseSocket(pClientCtx->sClient);
            
            if(pClientCtx->hRecvThread)
            {
                CloseHandle(pClientCtx->hRecvThread);
                pClientCtx->hRecvThread = NULL;
            }
            
            strTmp.Format("数据处理服务器%s断开连接\n",pClientCtx->chClientIp);
            OutputDebugString(strTmp);
            
            //移除对应客户端上下文
            
            ClientContext *pHead = m_pClientCtx;
            while(pHead)
            {
                if(pHead == pClientCtx)
                {
                    if(m_pClientCtx == pClientCtx)
                    {
                        m_pClientCtx = pClientCtx->pNext;
                    }
                    pClientCtx = pClientCtx->pNext; //移动当前指针
                    
                    if(pHead)
                    {
                        delete pHead;
                        pHead = NULL;
                    }
                    break;
                }
                
                else if(pHead->pNext == pClientCtx)
                {
                    pHead->pNext = pClientCtx->pNext;
                    
                    ClientContext *p =  pClientCtx;
                    pClientCtx = pClientCtx->pNext;
                    if(p)
                    {
                        delete p;
                        p = NULL;
                    }
                    break;
                }
                pHead = pHead->pNext;
            }
        }
        
        else
        {
            strTmp.Format("接收数据线程出错退出,ip:%s,socket：%d",pClientCtx->chClientIp,pClientCtx->sClient);
            OutputDebugString(strTmp);
            break;
        }
    
    }
    
    LeaveCriticalSection(&m_csList);
    

}

DWORD WINAPI Server::RecvThreadFunc(LPVOID lpParam)
{
    CString strTmp;
    ClientContext *pClientCtx = (ClientContext*) lpParam;
    
    if(!pClientCtx)
    {
        return THREAD_EXIT_CODE;
    }
    
    fd_set fdread;
    FD_ZERO(&fdread);
    FD_SET(pClientCtx->sClient,&fdread);
    timeval tv;
    
    tv.tv_sec = 2;
    tv.tv_usec = 0;
    
    int iRet = select(0,&fdread,NULL,NULL,&tv);
    
    if(iRet > 0)
    {
        if(FD_ISSET(pClientCtx->sClient,&fdread))
        {
          //  DWORD dwTime = GetTickCount();
            strTmp.Format("进入函数:ProcessRecv()\n");
            OutputDebugString(strTmp);
            CPowerInfo::WLog(strTmp);
            for(;;)
            {
                if(pClientCtx->pServer->ProcessRecv(pClientCtx) != 0)
                {
                    break;
                }
            }
            strTmp.Format("离开函数:ProcessRecv()\n");
            OutputDebugString(strTmp);
            CPowerInfo::WLog(strTmp);
            
        }
    }
    
    else
    {
        strTmp.Format("Recv data timeout\n");
        OutputDebugString(strTmp);
        CPowerInfo::WLog(strTmp);
    }
    return THREAD_EXIT_CODE;
    
}

int Server::ProcessRecv(ClientContext *pClientCtx)
{
    CString strTmp;
    char *pRecvBuffer = pClientCtx->chData;
    
    SOCKET s = pClientCtx->sClient;
    
    int ilen = Recvn(s,pRecvBuffer,SYN_HEADER_SIZE);
    
    if(ilen != 0)
    {
        strTmp.Format("接收同步头失败\n");
        OutputDebugString(strTmp);
        CPowerInfo::WLog(strTmp);
        return -1;
    }
    
    for(int i =0;i <SYN_HEADER_SIZE;i++)
    {
        if(pRecvBuffer[i] != SynInfo[i])
        {
           OutputDebugString("同步头错误\n");
           CPowerInfo::WLog("同步头错误\n");
            return -1;
        }
    }
    GDW_PacketHead packetHead;
    ilen = Recvn(s,(char*)&packetHead,sizeof(packetHead));
    if(ilen != 0)
    {
        OutputDebugString("接受包头错误\n");
        CPowerInfo::WLog("接受包头错误\n");
        return -1;
    }

    if(CheckSum((UINT16*)&packetHead,sizeof(packetHead)) != 0)
    {
        OutputDebugString("校验包头数据错误\n");
        CPowerInfo::WLog("校验包头数据错误\n");
        return -1;
    }

    DWORD dwDatalen = packetHead.dwDataLen;
    // LOG_INFO("接收到的数据大小:%d",dwDatalen);

    if(dwDatalen >MAX_DATA_LEN)
    {
        strTmp.Format("接收数据体大小错误，datasize:%d\n",dwDatalen);
        OutputDebugString(strTmp);
        CPowerInfo::WLog(strTmp);
        return -1;
    }

    ilen = Recvn(s,pRecvBuffer,dwDatalen);
    if(ilen != 0)
    {
        OutputDebugString("接受数据体失败\n");
        CPowerInfo::WLog("接受数据体失败\n");
        return -1;
    }
    ilen = SendResponse(s,packetHead.wDataType);
    if(ilen != 0)
    {
        OutputDebugString("发送回馈包失败\n");
        CPowerInfo::WLog("发送回馈包失败\n");
        return -1;
    }
    return 0;
}

/* 发送回馈的函数 有待协议扩充后实现，目前 未实现
*/
int Server::SendResponse(SOCKET /*s*/,short /*sType*/)
{
    return 0;
    
}