#ifndef CLS_NTP_CLIENT
#define CLS_NTP_CLIENT


#include <stdio.h>
#include <Windows.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#include <crtdbg.h>
#include <sys/timeb.h>
#include <time.h>

//#ifndef _DEBUG
//#define printf(x)
//#define printf(x, y)
//#endif

struct ntp_timestamp
{
	unsigned int secondsFrom1900;//1900年1月1日0时0分以来的秒数
	unsigned int fraction;//微秒的4294.967296(=2^32/10^6)倍
};

struct ntp_header
{
	union
	{
		struct 
		{
			char local_precision;//表示本机时钟精度为2^local_precision秒。local_precision通常为负数。
			char poll_intervals;//表示测试间隔为2^poll_intervals秒。
			unsigned char stratum;//NTP服务器阶级，0表示不指定，1表示校准用原子钟。应该为0。
			unsigned char mode : 3;//通信模式。应该为3，表示是client。
			unsigned char version_number : 3;//NTP协议版本号。应该为3。
			unsigned char leap_indicator : 2;//闰秒指示，一般填0。
		};
		int noname;
	};

	int root_delay;//可正可负。2^16表示一秒。具体含义参见RFC1305。
	int root_dispersion;//只可为正。2^16表示一秒。具体含义参见RFC1305。
	int reference_clock_identifier;//具体含义参见RFC1305。一般填0。
};//没有错误的话，ntp_header的大小应该为16字节。

struct ntp_packet
{
	ntp_header header;
	//以下四个时间均为本地时间。即考虑了时区位置的。
	ntp_timestamp reference;//具体含义参见RFC1305。一般填0。
	ntp_timestamp originate;//上次发出时刻
	ntp_timestamp receive;//接收时刻
	ntp_timestamp transmit;//发出时刻
};//没有错误的话，ntp_header的大小应该为48字节。


class clsNTPClient
{
public:
	clsNTPClient();
	~clsNTPClient();
private:
	inline unsigned int MicroSeconds2NTPFraction(unsigned int x);
	inline unsigned int NTPFraction2MicroSeconds(unsigned int x);
	bool SendNTPPacket(SOCKET sock, const sockaddr* to);
	bool RecvNTPPacket(SOCKET sock, ntp_packet& packet);
	bool MySetLocalTime(const ntp_packet& packet);
public:
	bool SetParams(char* chIP, int iPort, int iInterval);
	int AdjustTimeUDP();
	int AdjustTimeTCP();
	bool Start(CListBox *listbox);
	bool Stop();
public:
	static DWORD WINAPI ThreadProc(LPVOID lpParameter);
private:
	char m_chNTPServer[20];
	int m_iNTPPort;
	int m_iInterval;
public:
	static unsigned int m_siJAN1970;// = 0X83AA7E80;//=2208988800(1970/1/1 - 1900/1/1 in seconds)*/
	static bool m_threadflag;
	static CRITICAL_SECTION m_csflag;
	HANDLE m_hThreadEvent;
	HANDLE m_hThread;
	CListBox *m_lbListBox;
};

#endif