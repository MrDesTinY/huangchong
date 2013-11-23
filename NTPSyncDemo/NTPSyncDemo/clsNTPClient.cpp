#include "stdafx.h"
#include "clsNTPClient.h"

/* How to multiply by 4294.967296 quickly (and not quite exactly)
* without using floating point or greater than 32-bit integers.
* If you want to fix the last 12 microseconds of error, add in
* (2911*(x))>>28)
*/
unsigned int clsNTPClient::m_siJAN1970 = 0X83AA7E80;
bool clsNTPClient::m_threadflag;
CRITICAL_SECTION clsNTPClient::m_csflag;

clsNTPClient::clsNTPClient()
{
	clsNTPClient::m_threadflag = false;
	InitializeCriticalSection(&m_csflag);
}

clsNTPClient::~clsNTPClient()
{
	DeleteCriticalSection(&m_csflag);
}

bool clsNTPClient::SetParams(char* chIP, int iPort, int iInterval)
{
	memset(m_chNTPServer,0,sizeof(m_chNTPServer));
	strcpy_s(m_chNTPServer,sizeof(m_chNTPServer),chIP);
	m_iNTPPort = iPort;
	m_iInterval = iInterval;
	return true;
}

unsigned int clsNTPClient::MicroSeconds2NTPFraction(unsigned int x)
{
	return (4294 * (x) + ((1981 * (x))>>11));
}

/* The reverse of the above, needed if we want to set our microsecond
* clock based on the incoming time in NTP format.
* Basically exact.
*/
unsigned int clsNTPClient::NTPFraction2MicroSeconds(unsigned int x)
{
	return (((x) >> 12) - 759 * ((((x) >> 10) + 32768) >> 16));
}

bool clsNTPClient::SendNTPPacket(SOCKET sock, const sockaddr* to)
{
	ntp_packet packet;
	memset(&packet, 0, sizeof(ntp_packet));
	packet.header.local_precision = -6;
	packet.header.poll_intervals = 4;
	packet.header.stratum = 0;
	packet.header.mode = 3;
	packet.header.version_number = 3;
	packet.header.leap_indicator = 0;

	packet.header.root_delay = 1<<16;
	packet.header.root_dispersion = 1<<16;
	__timeb32 now;
	_ftime32_s(&now);
	packet.transmit.secondsFrom1900 = now.time + m_siJAN1970;
	packet.transmit.fraction = MicroSeconds2NTPFraction(now.millitm);

	packet.header.noname = htonl(packet.header.noname);
	packet.header.root_delay = htonl(packet.header.root_delay);
	packet.header.root_dispersion = htonl(packet.header.root_dispersion);
	packet.transmit.secondsFrom1900 = htonl(packet.transmit.secondsFrom1900);
	packet.transmit.fraction = htonl(packet.transmit.fraction);
	int bytesSent = sendto(sock, (const char*)&packet, sizeof(packet), 0, to, sizeof(struct sockaddr_in));
	return bytesSent != SOCKET_ERROR;
}

bool clsNTPClient::RecvNTPPacket(SOCKET sock, ntp_packet& packet)
{
	int bytesRead = recvfrom(sock, (char*)&packet, sizeof(ntp_packet), 0, NULL, NULL);
	return SOCKET_ERROR != bytesRead;
}

void printDateTime()
{
	__time32_t now;
	_time32(&now);
	tm ptm;
	_localtime32_s(&ptm,&now);
	char message[100] = {0};
	asctime_s(message,sizeof(message),&ptm);
	printf(message);
	OutputDebugString(message);
	OutputDebugString("\n");
}

bool clsNTPClient::MySetLocalTime(const ntp_packet& packet)
{
	ntp_timestamp server_transmit_time;
	server_transmit_time.secondsFrom1900 = ntohl(packet.transmit.secondsFrom1900);
	server_transmit_time.fraction = ntohl(packet.transmit.fraction);
	timeval newtime;
	newtime.tv_sec = server_transmit_time.secondsFrom1900 - m_siJAN1970;
	newtime.tv_usec = NTPFraction2MicroSeconds(server_transmit_time.fraction);
	tm ptm;
	_localtime32_s(&ptm,&newtime.tv_sec);
	//if(ptm == NULL)
	//{
	//	return false;
	//}
	SYSTEMTIME time;
	time.wYear = ptm.tm_year + 1900;
	time.wMonth = ptm.tm_mon + 1;
	time.wDay = ptm.tm_mday;
	time.wHour = ptm.tm_hour;
	time.wMinute = ptm.tm_min;
	time.wSecond = ptm.tm_sec;
	time.wMilliseconds = 0;

#ifdef _DEBUG
	printDateTime();
#else
	SetLocalTime(&time);
#endif
	return true;
}

int clsNTPClient::AdjustTimeUDP()
{
	int errorNo = 0;
	//校验数据结构长度
	_ASSERTE(sizeof(ntp_header) == 16);
	_ASSERTE(sizeof(ntp_packet) == 48);
	if(sizeof(ntp_packet) != 48)
		return -1;

	// Initialize Winsock.
	WSADATA wsaData;
	int iResult = WSAStartup( MAKEWORD(2,2), &wsaData );
	if ( iResult != NO_ERROR )
	{
		printf("Error at WSAStartup()\n");
		return -2;
	}

	// Create a socket.
	SOCKET m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
	if ( m_socket == INVALID_SOCKET )
	{
		printf( "Error at socket(): %ld\n", WSAGetLastError() );
		WSACleanup();
		return -3;
	}

	/* bind local address. */
	sockaddr_in addr_src;
	int addr_len = sizeof(struct sockaddr_in);
	memset(&addr_src, 0, addr_len);
	addr_src.sin_family = AF_INET;
	addr_src.sin_addr.s_addr = htonl(INADDR_ANY);
	addr_src.sin_port = htons(0);
	if (SOCKET_ERROR == bind(m_socket, (struct sockaddr*)&addr_src, addr_len))
	{
		printf( "Error at bind(): %ld\n", WSAGetLastError() );
		errorNo = -4;
		goto Cleanup;
	}

	//fill addr_dst
	sockaddr_in addr_dst;
	memset(&addr_dst, 0, addr_len);
	addr_dst.sin_family = AF_INET;
	{
		struct hostent* host = gethostbyname(m_chNTPServer);
		if (NULL == host || 4 != host->h_length)
		{
			printf( "Error at gethostbyname()\n");
			errorNo = -5;
			goto Cleanup;
		}
		memcpy(&(addr_dst.sin_addr.s_addr), host->h_addr_list[0], 4);
	}
	addr_dst.sin_port = htons(m_iNTPPort);

	//设置读取超时
	fd_set fds_read;
	timeval timeout;
	fds_read.fd_count = 1;
	fds_read.fd_array[0] = m_socket;
	timeout.tv_sec = 2;
	timeout.tv_usec = 0;
	if (SOCKET_ERROR == select(0, &fds_read, NULL, NULL, &timeout))
	{
		printf( "Error at select(): %ld\n", WSAGetLastError() );
		errorNo = -6;
		goto Cleanup;
	}

	if(SendNTPPacket(m_socket, (sockaddr*)&addr_dst) == false)
	{
		printf( "Error at send(): %ld\n", WSAGetLastError() );
		errorNo = -7;
		goto Cleanup;
	}

	ntp_packet packet;
	if(RecvNTPPacket(m_socket, packet) == false)
	{
		printf( "Error at recvfrom(): %ld\n", WSAGetLastError() );
		errorNo = -8;
		goto Cleanup;
	}

	if(MySetLocalTime(packet) == false)
	{
		printf("now is after 03:14:07, January 19, 2038, UTC");
		errorNo = -9;
	}

Cleanup:
	closesocket(m_socket);
	WSACleanup();
	return errorNo;
}


int clsNTPClient::AdjustTimeTCP()
{
	//Initial Socket
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2,2),&wsaData);  
	if(iResult != NO_ERROR)
	{
		OutputDebugString("Error at WSAStartup!\n");
		return -1;
	}

	sockaddr_in addr_dst;
	addr_dst.sin_family = AF_INET;
	{
		struct hostent* host = gethostbyname(m_chNTPServer);
		if(NULL == host || 4 != host->h_length)
		{
			OutputDebugString("Error at gethostbyname!!\n");
			WSACleanup();
			return -2;
		}
		memcpy(&(addr_dst.sin_addr.s_addr),host->h_addr_list[0], host->h_length);
	}
	addr_dst.sin_port = htons(m_iNTPPort);

	SOCKET m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if(m_socket == INVALID_SOCKET)
	{
		OutputDebugString("Error at Create Socket!\n");
		closesocket(m_socket);
		WSACleanup();
		return -3;
	}

	int status = connect(m_socket,(LPSOCKADDR)&addr_dst,sizeof(addr_dst));
	if(status == SOCKET_ERROR)
	{
		OutputDebugString("Error at connect!!!\n");
		closesocket(m_socket);
		WSACleanup();
		return -4;
	}

	char recvbuf[64] = {0};
	recv(m_socket,recvbuf,sizeof(recvbuf),0);
	if(recvbuf == 0)
	{
		OutputDebugString("Error at recv!!!\n");
		closesocket(m_socket);
		WSACleanup();
		return -5;
	}
	
    UINT uiServerTimeSec;
	char chServerTime[64] = {0};
	memcpy_s(chServerTime,strlen(recvbuf)-4,recvbuf,strlen(recvbuf)-4);
	uiServerTimeSec = atoi(chServerTime);
	timeval newtime;
	newtime.tv_sec = uiServerTimeSec;
	tm ptm;
	_localtime32_s(&ptm,&newtime.tv_sec);
	//if(ptm == NULL)
	//{
	//	return false;
	//}
	SYSTEMTIME time;
	time.wYear = ptm.tm_year + 1900;
	time.wMonth = ptm.tm_mon + 1;
	time.wDay = ptm.tm_mday;
	time.wHour = ptm.tm_hour;
	time.wMinute = ptm.tm_min;
	time.wSecond = ptm.tm_sec;
	time.wMilliseconds = 0;

#ifdef _DEBUG
	printDateTime();
#else
	SetLocalTime(&time);
#endif

	closesocket(m_socket);
	WSACleanup();
	return 1;
}


DWORD WINAPI clsNTPClient::ThreadProc(LPVOID lpParameter)
{
	clsNTPClient *pclsTmpNTPClient = (clsNTPClient *)lpParameter;
	SYSTEMTIME stNow;
	char chBuf[128] = {0};
	while(WaitForSingleObject(pclsTmpNTPClient->m_hThreadEvent,pclsTmpNTPClient->m_iInterval*1000) != WAIT_OBJECT_0)
	{
		EnterCriticalSection(&m_csflag);
		bool bTmpflag = m_threadflag;
		LeaveCriticalSection(&m_csflag);
		if(bTmpflag)
		{	
			GetLocalTime(&stNow);
			if(1 == pclsTmpNTPClient->AdjustTimeTCP())
			{
				OutputDebugString("ThreadProc::AdjustTimeOK\n");
				sprintf_s(chBuf,"%4d-%0.2d-%0.2d %0.2d:%0.2d:%0.2d:校时成功！",stNow.wYear,stNow.wMonth,stNow.wDay,stNow.wHour,stNow.wMinute,stNow.wSecond);
				pclsTmpNTPClient->m_lbListBox->AddString(chBuf);
				if(pclsTmpNTPClient->m_lbListBox->GetCount() > 100)
				{
					pclsTmpNTPClient->m_lbListBox->DeleteString(0);
				}
				//return 0;
			}
			else
			{
				OutputDebugString("ThreadProc::AdjustTimeError1\n");
				sprintf_s(chBuf,"%4d-%0.2d-%0.2d %0.2d:%0.2d:%0.2d:校时失败！",stNow.wYear,stNow.wMonth,stNow.wDay,stNow.wHour,stNow.wMinute,stNow.wSecond);
				pclsTmpNTPClient->m_lbListBox->AddString(chBuf);
				if(pclsTmpNTPClient->m_lbListBox->GetCount() > 100)
				{
					pclsTmpNTPClient->m_lbListBox->DeleteString(0);
				}
				//return -1;
			}
		}
		else
		{
			OutputDebugString("ThreadProc::AdjustTimeError2\n");
			GetLocalTime(&stNow);
			sprintf_s(chBuf,"%4d-%0.2d-%0.2d %0.2d:%0.2d:%0.2d:线程关闭！",stNow.wYear,stNow.wMonth,stNow.wDay,stNow.wHour,stNow.wMinute,stNow.wSecond);
			pclsTmpNTPClient->m_lbListBox->AddString(chBuf);
			if(pclsTmpNTPClient->m_lbListBox->GetCount() > 100)
			{
				pclsTmpNTPClient->m_lbListBox->DeleteString(0);
			}
			return -2;
		}
	}
	return 0;
}

bool clsNTPClient::Start(CListBox *listbox)
{
	this->m_lbListBox = listbox;
	EnterCriticalSection(&m_csflag);
	bool bTmpflag = m_threadflag;
	LeaveCriticalSection(&m_csflag);
	if(bTmpflag)
	{
		OutputDebugString("Start::False\n");
		return false;
	}
	EnterCriticalSection(&m_csflag);
	m_threadflag = true;
	LeaveCriticalSection(&m_csflag);
	m_hThreadEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
	m_hThread = CreateThread(NULL,0,clsNTPClient::ThreadProc,this,0,NULL);
	OutputDebugString("Start::true\n");
	return true;
}

bool clsNTPClient::Stop()
{
	EnterCriticalSection(&m_csflag);
	bool bTmpflag = m_threadflag;
	LeaveCriticalSection(&m_csflag);
	if(!bTmpflag)
	{
		OutputDebugString("Stop::False\n");
		return false;
	}
	EnterCriticalSection(&m_csflag);
	m_threadflag = false;
	LeaveCriticalSection(&m_csflag);
	SetEvent(m_hThreadEvent);
	//WaitForSingleObject(m_hThread,INFINITE);
	if(m_hThreadEvent)
	{
		CloseHandle(m_hThreadEvent);
		m_hThreadEvent = NULL;
	}
	if(m_hThread)
	{
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}
	return true;
}