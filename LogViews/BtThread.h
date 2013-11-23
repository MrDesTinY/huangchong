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

#ifndef __BT_THREAD_H__
#define __BT_THREAD_H__

#include <QThread>
#include <Winsock2.h>
#include <Ws2bth.h>

class MainWindow;

typedef enum AcceptResultCode
{
    ACCEPT_OK = 0,
    ACCEPT_RETRY = 1,
    ACCEPT_ERROR = 2
} AcceptResultCode;

class BtHandler : public QObject
{
    Q_OBJECT

friend class BtThread;

public:
    BtHandler(const MainWindow& guiNotifier);
    ~BtHandler();

    int startBtServer();
    int stopBtServer();
    int acceptClient();
    int disconnect();
    int rcvLogMsg();

private:
    int initWinSock2();

signals:
    void btStarted(bool success);
    void btConnected();
    void btDisconnected();
    void message(const QString& msg);
    void rcvdMsgs(const QStringList& msgs);

private:
    // Data:
    SOCKET serverSocket;
    SOCKET clientSocket;
    BYTE* rcvBuffer;
    bool isConnected;
    static const GUID LOG_VIEWS_BT_SERVICE_CLASS_GUID;
    static const int RCV_BUFFER_MAX_SIZE;
};

class BtThread : public QThread
{
    Q_OBJECT

public:
    BtThread(MainWindow* notifier);
    ~BtThread();
    void stop();
protected:
    void run();
private:
    MainWindow* guiNotifier;
    BtHandler* btHandler;
    bool isAlive;
};

#endif//__UDP_THREAD_H__
