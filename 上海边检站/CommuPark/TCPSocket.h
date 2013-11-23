#pragma once
#include <winsock2.h>

//#pragma comment(lib,"ws2_32.lib")

#pragma warning(push)
#pragma warning(disable:4127)
extern CString g_strErr;
class TCPSocket
{
public:

    TCPSocket(void)
        : m_socket(INVALID_SOCKET)
    {
		InitializeCriticalSection(&m_errMsg);

    }

	CRITICAL_SECTION m_errMsg;
    BOOL Connect(const char * ip, WORD wPort);
    void DisConnect()
    {
        if (INVALID_SOCKET != m_socket)
        {
			struct linger so_linger;			
			so_linger.l_onoff = 1;
			so_linger.l_linger =  5;
			setsockopt(m_socket,SOL_SOCKET,SO_LINGER,(const char*)&so_linger,sizeof(so_linger));
			shutdown(m_socket,SD_BOTH);
            closesocket(m_socket);			
            m_socket = INVALID_SOCKET;
        }
    }
    BOOL SendData(char * pBuff, int nDataLen);
    BOOL RecvData(char * pBuff, int nDataLen);

	
    ~TCPSocket(void)
    {
        DisConnect();
		DeleteCriticalSection(&m_errMsg);
    }

protected:
    SOCKET m_socket;
};

inline BOOL TCPSocket::Connect(const char * ip, WORD wPort)
{
    if (m_socket != INVALID_SOCKET)
    {
        DisConnect();
    }
    
    m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    
    if (m_socket == INVALID_SOCKET)
    {
        return FALSE;
    }

    u_long ulNonBlock = 1;
    ioctlsocket(m_socket, FIONBIO, &ulNonBlock);

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(wPort);
    addr.sin_addr.s_addr = inet_addr(ip);

    connect(m_socket, (sockaddr*)&addr, sizeof(sockaddr));

    fd_set fdwrite;
    FD_ZERO(&fdwrite);
    FD_SET(m_socket, &fdwrite);

    /*这个函数监视一系列文件描述符，特别是 readfds、writefds 和 exceptfds。如果你想知道你是否能够从标准输入和套接字描述符 sockfd 读入数据，你只要将文件描述符 0 和 sockfd 加入到集合 readfds 中。*/
    // 超时数据结构
    timeval timeout;
    timeout.tv_sec = 3; // 1秒内发送请求响应到客户端
    timeout.tv_usec = 0;

    int nRetSelect = select(0, NULL, &fdwrite, NULL, &timeout);

    if (nRetSelect <= 0) // 是否可写
    {
        return FALSE;
    }

    int nError;
    int nErrorLen = sizeof(int);
    getsockopt(m_socket, SOL_SOCKET, SO_ERROR, (char*)&nError, &nErrorLen); ////retrieve a socket option
    if (nError == 0)
    {
        u_long ulBlock = 0;
        ioctlsocket(m_socket, FIONBIO, &ulBlock);   ///controls the I/O mode of a socket
        return TRUE;
    }

    return FALSE;
}

inline BOOL TCPSocket::SendData(char * pBuff, int nDataLen)
{

    int   reuse; 
    int nsize = sizeof(int);
    getsockopt(m_socket,SOL_SOCKET,SO_SNDBUF,(char   *)&reuse,&nsize); 



   // if (nDataLen <= 8192) //每次发送4K
    {
		int Templen = send(m_socket,pBuff,nDataLen,0);
		CString Tmpstr;
		Tmpstr.Format("The nDataLen is %d, The Send Length is %d.\n", nDataLen, Templen);
		OutputDebugString(Tmpstr);
		CPowerInfo::WLog(Tmpstr);
        if  (nDataLen != Templen)
        {
            CString strTmp;
		    char buf[256];
		    USES_CONVERSION;
		    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,(void*)FORMAT_MESSAGE_FROM_STRING,WSAGetLastError(),0,buf,256,0);
		    EnterCriticalSection(&m_errMsg);
		    g_strErr.Format("code:%d  Message:%s",WSAGetLastError(),buf);
		    if (nDataLen == 6)
		    {
			   // PostMessage(AfxGetApp()->m_pMainWnd->GetSafeHwnd(),WM_TCP_SEND_FAILED,(WPARAM)(LPCTSTR)g_strErr,(LPARAM)0);
			   strTmp.Format("skid---发送同步头失败,%s",g_strErr);
			   OutputDebugString(strTmp);
			   CPowerInfo::WLog(strTmp);
			   
		    }
		    else if (nDataLen == 16)
		    {
			   // PostMessage(AfxGetApp()->m_pMainWnd->GetSafeHwnd(),WM_TCP_SEND_FAILED,(WPARAM)(LPCTSTR)g_strErr,(LPARAM)1);
			   strTmp.Format("skid----发送数据包头失败,%s",g_strErr);
			   OutputDebugString(strTmp);
			   CPowerInfo::WLog(strTmp);
		    }
		    else 
		    {
			    //PostMessage(AfxGetApp()->m_pMainWnd->GetSafeHwnd(),WM_TCP_SEND_FAILED,(WPARAM)(LPCTSTR)g_strErr,(LPARAM)2);
			   strTmp.Format("skid------发送数据体失败,%s",g_strErr);
			   OutputDebugString(strTmp);
			    CPowerInfo::WLog(strTmp);
			   
		    }
    		
		    LeaveCriticalSection(&m_errMsg);
            return FALSE;
        }
    }

  //  else
  //  {
  //      int nSended = 0;
  //      
  //      while (nSended < nDataLen)
  //      {
  //          int nWantSend = 8196;

  //          if (nSended + nWantSend > nDataLen)
  //          {
  //              nWantSend = nDataLen - nSended;
  //          }

  //          if (nWantSend != send(m_socket,pBuff+nSended,nWantSend,0))
  //          {
  //              CString str;
  //              str.Format(_T("TerminalSimulator: error %d"), WSAGetLastError());
  //              OutputDebugString(str);
  //              CPowerInfo::WLog(str);
  //              
  //              return FALSE;
  //          }

  //          nSended += nWantSend;
  //      }
  //  }

    return TRUE;
}

inline BOOL TCPSocket::RecvData(char * pBuff, int nDataLen)
{
    fd_set fdread;
    FD_ZERO(&fdread);
    FD_SET(m_socket, &fdread);

    timeval timeout;
    timeout.tv_sec = 3/*0*/; // for test
    timeout.tv_usec = 0; // 250 ms    
	
    TRACE("TerminalSimulator wait select");

    int nRet = select(0, &fdread, NULL, NULL, &timeout);

    if (nRet <= 0)
    {
        TRACE("TerminalSimulator select:%d",WSAGetLastError());
		EnterCriticalSection(&m_errMsg);
		g_strErr.Format("code:%d Message:%s",WSAGetLastError(),"select timeout");
		if (nDataLen == 6)
		{
		//	PostMessage(AfxGetApp()->m_pMainWnd->GetSafeHwnd(),WM_TCP_SEND_FAILED,(WPARAM)(LPCTSTR)g_strErr,(LPARAM)3);
		    OutputDebugString("skid----recv syn select timeout\n");
		    CPowerInfo::WLog("skid----recv syn select timeout");
		}
		else if (nDataLen == 16)
		{
		    //	PostMessage(AfxGetApp()->m_pMainWnd->GetSafeHwnd(),WM_TCP_SEND_FAILED,(WPARAM)(LPCTSTR)g_strErr,(LPARAM)4);
		    OutputDebugString("skid------recv packetHead select timeout\n");
		    CPowerInfo::WLog("skid------recv packetHead select timeout");
		}
		LeaveCriticalSection(&m_errMsg);
        return FALSE;
    }

    int nRecv = recv(m_socket, pBuff, nDataLen, 0);
	if (nRecv <=0)
	{
		char buf[128];
		USES_CONVERSION;
		FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM,(void*)FORMAT_MESSAGE_FROM_STRING,WSAGetLastError(),0,buf,128,0);
		EnterCriticalSection(&m_errMsg);
		g_strErr.Format("code:%d Message:%s",WSAGetLastError(),buf);
		if (nDataLen == 6)
		{
//			PostMessage(AfxGetApp()->m_pMainWnd->GetSafeHwnd(),WM_TCP_SEND_FAILED,(WPARAM)(LPCTSTR)g_strErr,(LPARAM)3);
            OutputDebugString("skid-------recv syn error\n");
            CPowerInfo::WLog("skid-------recv syn error\n");
		}
		else if (nDataLen == 16)
		{
//			PostMessage(AfxGetApp()->m_pMainWnd->GetSafeHwnd(),WM_TCP_SEND_FAILED,(WPARAM)(LPCTSTR)g_strErr,(LPARAM)4);
            OutputDebugString("skid-----recv packethead error\n");
            CPowerInfo::WLog("skid-----recv packethead error\n");
		}
		LeaveCriticalSection(&m_errMsg);
		return FALSE;
	}

    TRACE("TerminalSimulator recv:%d",nRecv);

    // 接收回传数据
    return (nDataLen == nRecv);
}

#pragma warning(pop)
