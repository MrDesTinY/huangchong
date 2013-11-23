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

#include <QtNetwork>

#include "UdpThread.h"
#include "mainwindow.h"

UdpThread::UdpThread(MainWindow* notifier):
    guiNotifier(notifier)
{
}

UdpThread::~UdpThread()
{
}

void UdpThread::stop()
{
    exit();
    wait();
}

void UdpThread::run()
{
    UdpHandler* udpHandler = new UdpHandler(guiNotifier);
    if(false == udpHandler->bind())
    {
        return;
    }
    exec();
    delete udpHandler;
    udpHandler = NULL;
}

UdpHandler::UdpHandler(MainWindow* notifier):
    guiNotifier(notifier),
    udpSocket(NULL)
{
    connect(this, SIGNAL(udpBinded(bool)),
            guiNotifier, SLOT(udpBindedSlot(bool)));
    connect(this, SIGNAL(message(const QString&)),
            guiNotifier, SLOT(showMessage(const QString&)));
    connect(this, SIGNAL(rcvdMsgs(const QStringList&)),
            guiNotifier, SLOT(processMsgs(const QStringList&)));

    udpSocket = new QUdpSocket(this);
    connect(udpSocket, SIGNAL(readyRead()),
            this, SLOT(readPendingDgrams()));
}

UdpHandler::~UdpHandler()
{
}

bool UdpHandler::bind()
{
    bool result = udpSocket->bind(guiNotifier->getSettings().udpPort, QUdpSocket::DontShareAddress);
    emit udpBinded(result);
    return result;
}

void UdpHandler::readPendingDgrams()
{
    QStringList dgrams;
    while(udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;
        udpSocket->readDatagram(datagram.data(), datagram.size(),
                                &sender, &senderPort);
        dgrams<<datagram.data();
    }
    emit rcvdMsgs(dgrams);
    dgrams.clear();
}
