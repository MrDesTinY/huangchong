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

#ifndef __UDP_THREAD_H__
#define __UDP_THREAD_H__

#include <QThread>
#include <QUdpSocket>

class MainWindow;

class UdpThread : public QThread
{
    Q_OBJECT

public:
    UdpThread(MainWindow* notifier);
    ~UdpThread();
    void stop();
protected:
    void run();
private:
    MainWindow* guiNotifier;
};

class UdpHandler : public QObject
{
    Q_OBJECT

public:
    UdpHandler(MainWindow* notifier);
    ~UdpHandler();
    bool bind();
signals:
    void udpBinded(bool success);
    void message(const QString& msg);
    void rcvdMsgs(const QStringList& msgs);

private slots:
    void readPendingDgrams();
private:
    // Data:
    MainWindow* guiNotifier;
    QUdpSocket* udpSocket;
};

#endif//__UDP_THREAD_H__
