/*
* Copyright 2010 Ionut Dediu, deionut@yahoo.com
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include "BtThread.h"
#include "mainwindow.h"

#include <QtDebug>

// {D227B199-E3E1-44ab-82DA-001D41D6153E}
const GUID BtHandler::LOG_VIEWS_BT_SERVICE_CLASS_GUID = { 0xd227b199, 0xe3e1, 0x44ab, { 0x82, 0xda, 0x0, 0x1d, 0x41, 0xd6, 0x15, 0x3e } };

const int BtHandler::RCV_BUFFER_MAX_SIZE = 4096;

#define SELECT_TIMEOUT_MS 100

BtThread::BtThread(MainWindow* notifier):
guiNotifier(notifier),
btHandler(NULL),
isAlive(false)
{
btHandler = new BtHandler(*guiNotifier);
}

BtThread::~BtThread()
{
qDebug() << "BtThread::~BtThread()" << endl;
stop();
delete btHandler;
btHandler = NULL;
}

void BtThread::stop()
{
qDebug() << "BtThread::stop()" << endl;
isAlive = false;
wait();
}

void BtThread::run()
{
qDebug() << "BtThread::run()" << endl;
isAlive = true;
int errCode = 0;
if(btHandler->startBtServer() != 0)
{
qDebug() << "BtThread exiting: btHandler->start() ERROR" << endl;
goto EXIT;
}

FD_SET read_fds;
struct timeval timeout;
timeout.tv_sec = 0;
timeout.tv_usec = SELECT_TIMEOUT_MS * 1000;

ACCEPT_CLIENT:
while(isAlive)
{
FD_ZERO(&read_fds);
FD_SET(btHandler->serverSocket, &read_fds);

if((errCode = select(0, &read_fds, NULL, NULL, &timeout)) == SOCKET_ERROR)
{
qDebug() << "BtThread exiting: select() function returned with error " << WSAGetLastError() << endl;
goto EXIT;
}
else if(errCode == 0)
{
//qDebug() << "BtThread: SELECT TIMEOUT !!!!" << endl;
}
if(FD_ISSET(btHandler->serverSocket, &read_fds))
{
btHandler->acceptClient();
break;
}
}

while(isAlive)
{
FD_ZERO(&read_fds);
FD_SET(btHandler->clientSocket, &read_fds);

if((errCode = select(0, &read_fds, NULL, NULL, &timeout)) == SOCKET_ERROR)
{
qDebug() << "BtThread exiting: select() function returned with error " << WSAGetLastError() << endl;
goto EXIT;
}
else if(errCode == 0)
{
//qDebug() << "BtThread: SELECT TIMEOUT !!!!" << endl;
}
if(FD_ISSET(btHandler->clientSocket, &read_fds))
{
errCode = btHandler->rcvLogMsg();
if(errCode != 0)	// ERROR, disconnected
{
if(isAlive)
{
goto ACCEPT_CLIENT;
}
else
{
qDebug() << "BtThread exiting: while connected isAlive == false" << endl;
goto EXIT;
}
}
}
}

qDebug() << "BtThread exiting normal" << endl;

EXIT:
btHandler->stopBtServer();
}

BtHandler::BtHandler(const MainWindow& guiNotifier):
serverSocket(INVALID_SOCKET),
clientSocket(INVALID_SOCKET),
rcvBuffer(NULL),
isConnected(false)
{
connect(this, SIGNAL(btStarted(bool)),
&guiNotifier, SLOT(btStartedSlot(bool)));
connect(this, SIGNAL(btConnected()),
&guiNotifier, SLOT(btConnectedSlot()));
connect(this, SIGNAL(btDisconnected()),
&guiNotifier, SLOT(btDisconnectedSlot()));

connect(this, SIGNAL(message(const QString&)),
&guiNotifier, SLOT(showMessage(const QString&)));
connect(this, SIGNAL(rcvdMsgs(const QStringList&)),
&guiNotifier, SLOT(processMsgs(const QStringList&)));

rcvBuffer = new BYTE[RCV_BUFFER_MAX_SIZE];

int retCode = initWinSock2();
if(retCode)
{
// ERROR
}
}

BtHandler::~BtHandler()
{
qDebug() << "BtHandler::~BtHandler()" << endl;
delete[] rcvBuffer;
// finish with winsock2
WSACleanup();
}

int BtHandler::rcvLogMsg()
{
int retCode = recv(clientSocket, (char*) rcvBuffer, 2, 0);
if((retCode == 0) || (retCode == SOCKET_ERROR))
{
disconnect();
return -1;
}
if(retCode == 1)
{
retCode = recv(clientSocket, (char*) rcvBuffer + 1, 1, 0);
if((retCode == 0) || (retCode == SOCKET_ERROR))
{
disconnect();
return -1;
}
}
quint8 logMsgLenMSB = (quint8)rcvBuffer[0];
quint8 logMsgLenLSB = (quint8)rcvBuffer[1];
quint32 logMsgLen = ((quint32)logMsgLenMSB << 8) | ((quint32)logMsgLenLSB);

quint32 rcvdBytesCount = 0;
while(rcvdBytesCount < logMsgLen)
{
retCode = recv(clientSocket, ((char*) rcvBuffer) + rcvdBytesCount, logMsgLen - rcvdBytesCount, 0);
// Connection closed or socket error
if((retCode == 0) || (retCode == SOCKET_ERROR))
{
disconnect();
return -1;
}
// All went well, advance the byte count
else
{
rcvdBytesCount += retCode;
}
}

QStringList msgs;
QString msgsStr = QString::fromUtf8((const char*)rcvBuffer, logMsgLen);
msgs << msgsStr;
emit rcvdMsgs(msgs);
msgs.clear();
return 0;
}

int BtHandler::initWinSock2()
{
// initialize WinSock 2
WORD wVersionRequested;	// Highest version of Windows Sockets support that the caller can use
WSADATA wsaData;		// Pointer to the WSADATA data structure that is to receive details
// of the Windows Sockets implementation.
int err;

wVersionRequested = MAKEWORD( 2, 2 );

err = WSAStartup( wVersionRequested, &wsaData );
if(err != 0)
{
emit message("Could not find a usable WinSock DLL\n");
return err;
}

// Confirm that the WinSock DLL supports 2.2
// Note that if the DLL supports versions greater
// than 2.2 in addition to 2.2, it will still return
// 2.2 in wVersion since that is the version we requested

if(LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
{
// Tell the user that we could not find a usable WinSock DLL
emit message("Could not find a usable WinSock DLL v 2.2\n");
WSACleanup();
return -1;
}
// The WinSock DLL is acceptable. Return success
return 0;
}

int BtHandler::startBtServer()
{
qDebug() << "BtHandler::startBtServer()" << endl;
int retCode = 0;

WSAQUERYSET wsaQuerySet = {0};
SOCKADDR_BTH sockAddrBthLocal = {0};
int iAddrLen = sizeof(SOCKADDR_BTH);
CSADDR_INFO csAddrInfo = {0};

// Open a bluetooth socket using RFCOMM protocol
if((serverSocket = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM)) == INVALID_SOCKET)
{
retCode = WSAGetLastError();
//emit message("Could not create bluetooth socket: %d\n(%s)", retCode, Util::GetLastErrorMessage());
goto EXIT;
}
// Setting address family to AF_BTH indicates winsock2 to use Bluetooth port
sockAddrBthLocal.addressFamily = AF_BTH;
sockAddrBthLocal.port = BT_PORT_ANY;

// bind() associates a local address and port combination
// with the socket just created. This is most useful when
// the application is a server that has a well-known port
// that clients know about in advance.
if(bind(serverSocket, (struct sockaddr *) &sockAddrBthLocal, sizeof(SOCKADDR_BTH)) == SOCKET_ERROR)
{
retCode = WSAGetLastError();
//emit message("Could not bind bluetooth socket: %d\n(%s)", retCode, Util::GetLastErrorMessage());
goto EXIT;
}

if(getsockname(serverSocket, (struct sockaddr *)&sockAddrBthLocal, &iAddrLen) == SOCKET_ERROR)
{
retCode = WSAGetLastError();
//emit message("Could not access bluetooth socket: %d\n(%s)", retCode, Util::GetLastErrorMessage());
goto EXIT;
}

// CSADDR_INFO
csAddrInfo.LocalAddr.iSockaddrLength = sizeof( SOCKADDR_BTH );
csAddrInfo.LocalAddr.lpSockaddr = (LPSOCKADDR)&sockAddrBthLocal;
csAddrInfo.RemoteAddr.iSockaddrLength = sizeof( SOCKADDR_BTH );
csAddrInfo.RemoteAddr.lpSockaddr = (LPSOCKADDR)&sockAddrBthLocal;
csAddrInfo.iSocketType = SOCK_STREAM;
csAddrInfo.iProtocol = BTHPROTO_RFCOMM;

// If we got an address, go ahead and advertise it.
ZeroMemory(&wsaQuerySet, sizeof(WSAQUERYSET));
wsaQuerySet.dwSize = sizeof(WSAQUERYSET);
wsaQuerySet.lpServiceClassId = (LPGUID) &LOG_VIEWS_BT_SERVICE_CLASS_GUID;
wsaQuerySet.lpszServiceInstanceName = TEXT("SmartCam Server");
wsaQuerySet.lpszComment = TEXT("Webcam over bluetooth");
wsaQuerySet.dwNameSpace = NS_BTH;
wsaQuerySet.dwNumberOfCsAddrs = 1;     // Must be 1.
wsaQuerySet.lpcsaBuffer = &csAddrInfo; // Required.

// As long as we use a blocking accept(), we will have a race
// between advertising the service and actually being ready to
// accept connections.  If we use non-blocking accept, advertise
// the service after accept has been called.
if(WSASetService(&wsaQuerySet, RNRSERVICE_REGISTER, 0) == SOCKET_ERROR)
{
retCode = WSAGetLastError();
//emit message("Could not set service on bluetooth socket: %d\n(%s)", retCode, Util::GetLastErrorMessage());
goto EXIT;
}

// listen() call indicates winsock2 to listen on a given socket for any incoming connection.
if(listen(serverSocket, SOMAXCONN) == SOCKET_ERROR)
{
retCode = WSAGetLastError();
//emit message("Could not listen on bluetooth socket: %d\n(%s)", retCode, Util::GetLastErrorMessage());
goto EXIT;
}

EXIT:

emit btStarted(retCode == 0);
return retCode;
}

int BtHandler::acceptClient()
{
qDebug() << "acceptClient() - started " << endl;
int retCode = 0;
if((clientSocket = accept(serverSocket, NULL, NULL)) == INVALID_SOCKET)
{
retCode = WSAGetLastError();
}

if(retCode == 0)
{
emit btConnected();
isConnected = true;
}
return retCode;
}

int BtHandler::disconnect()
{
isConnected = false;
// close the sockets (if opened)
if(clientSocket != INVALID_SOCKET)
{
if(closesocket(clientSocket) == SOCKET_ERROR)
{
//QString msg = "closesocket(clientSocket) < Cleanup > failed : " + Util::GetLastErrorMessage();
return WSAGetLastError();
}
clientSocket = INVALID_SOCKET;
emit btDisconnected();
}

return 0;
}

int BtHandler::stopBtServer()
{
qDebug() << "BtHandler::stopBtServer()" << endl;
int retCode = disconnect();
if(retCode)
goto EXIT;
if(serverSocket != INVALID_SOCKET)
{
if(closesocket(serverSocket) == SOCKET_ERROR)
{
//string msg = string("closesocket(serverSocket) < Cleanup > failed : ") + Util::GetLastErrorMessage();
retCode = WSAGetLastError();
}
serverSocket = INVALID_SOCKET;
}

EXIT:
return retCode;
}
