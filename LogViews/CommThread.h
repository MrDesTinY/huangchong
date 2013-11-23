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

#ifndef __COMM_THREAD_H__
#define __COMM_THREAD_H__

#include <QThread>
#include <windows.h>

class MainWindow;

class CommHandler : public QObject
{
    Q_OBJECT

friend class CommThread;

public:
    CommHandler(MainWindow* notifier);
    ~CommHandler();

    int openCommPort();
    int closeCommPort();
    int rcvLogMsg();

private:
    int rcv(quint32 numBytes);
signals:
    void commPortOpened(bool success);
    void message(const QString& msg);
    void rcvdMsgs(const QStringList& msgs);

private:
    // Data:
    MainWindow* guiNotifier;
    unsigned char* rcvBuffer;
    static const int RCV_BUFFER_MAX_SIZE;
    HANDLE hCommPort;
};

class CommThread : public QThread
{
    Q_OBJECT

public:
    CommThread(MainWindow* notifier);
    ~CommThread();
    void stop();
protected:
    void run();
private:
    // Data:
    MainWindow* guiNotifier;
    CommHandler* commHandler;
    bool isAlive;
};

#endif//__UDP_THREAD_H__
