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

#include "CommThread.h"
#include "mainwindow.h"

#include <QtDebug>

#define TIMEOUT_RETURN_CODE -2

const int CommHandler::RCV_BUFFER_MAX_SIZE = 4096;

CommThread::CommThread(MainWindow* notifier):
    guiNotifier(notifier),
    commHandler(NULL),
    isAlive(false)
{
    commHandler = new CommHandler(guiNotifier);
}

CommThread::~CommThread()
{
    qDebug() << "CommThread::~CommThread()" << endl;
    stop();
    delete commHandler;
    commHandler = NULL;
}

void CommThread::stop()
{
    qDebug() << "CommThread::stop()" << endl;
    isAlive = false;
    wait();
}

void CommThread::run()
{
    qDebug() << "CommThread::run()" << endl;
    isAlive = true;
    int errCode = 0;
    if(commHandler->openCommPort() != 0)
    {
        qDebug() << "CommThread exiting: can not open " + guiNotifier->getSettings().commPort + " port" << endl;
        goto EXIT;
    }

    while(isAlive)
    {
        errCode = commHandler->rcvLogMsg();
        if(errCode != 0 && errCode != TIMEOUT_RETURN_CODE)
        {
            qDebug() << "CommThread exiting: can not read from the " + guiNotifier->getSettings().commPort + " port" << endl;
            isAlive = false;
            goto EXIT;
        }
    }

    qDebug() << "CommThread exiting normal" << endl;

EXIT:
    commHandler->closeCommPort();
}

CommHandler::CommHandler(MainWindow* notifier):
    guiNotifier(notifier),
    rcvBuffer(NULL),
    hCommPort(NULL)
{
    connect(this, SIGNAL(commPortOpened(bool)),
            guiNotifier, SLOT(commPortOpenedSlot(bool)));

    connect(this, SIGNAL(message(const QString&)),
            guiNotifier, SLOT(showMessage(const QString&)));
    connect(this, SIGNAL(rcvdMsgs(const QStringList&)),
            guiNotifier, SLOT(processMsgs(const QStringList&)));

    rcvBuffer = new unsigned char[RCV_BUFFER_MAX_SIZE];
}

CommHandler::~CommHandler()
{
    qDebug() << "CommHandler::~CommHandler()" << endl;
    delete[] rcvBuffer;
}

/*int CommHandler::rcv(quint32 numBytes)
{
    qDebug() << "CommHandler::rcv(): numBytes=" << numBytes << endl;
    bool result;
    quint32 rcvdBytesCount = 0;
    while(rcvdBytesCount < numBytes)
    {
        result = ReadFile(hCommPort,
                          ((char*) rcvBuffer) + rcvdBytesCount,
                          numBytes - rcvdBytesCount,
                          (LPDWORD)&rcvdBytesCount,
                          NULL);
        if(!isAli)
        if(!result)
        {
            emit message("Unable to to read from COM7\nClosed the serial port");
            return -1;
        }
    }
    return 0;
}*/

int CommHandler::rcv(quint32 numBytes)
{
    qDebug() << "CommHandler::rcv(): numBytes=" << numBytes << endl;
    bool result;
    quint32 rcvdBytesCount = 0;
    result = ReadFile(hCommPort,
                      (char*) rcvBuffer,
                      numBytes,
                      (LPDWORD)&rcvdBytesCount,
                      NULL);
    if(!result)
    {
        emit message("Unable to to read from COM7\nClosed the serial port");
        return -1;
    }
    if(rcvdBytesCount < numBytes)
    {
        qDebug() << "CommHandler::rcv(): TIMEOUT: rcvdBytesCount=" << rcvdBytesCount << endl;
        return TIMEOUT_RETURN_CODE;
    }
    return 0;
}


int CommHandler::rcvLogMsg()
{
    qDebug() << "CommHandler::rcvLogMsg()" << endl;
    int retCode = 0;
    QStringList msgs;
    QString msgsStr;

    retCode = rcv(2);
    if(retCode != 0)
    {
        goto EXIT;
    }
    quint8 logMsgLenMSB = (quint8)rcvBuffer[0];
    quint8 logMsgLenLSB = (quint8)rcvBuffer[1];
    quint32 logMsgLen = ((quint32)logMsgLenMSB << 8) | ((quint32)logMsgLenLSB);
    qDebug() << "CommHandler::rcvLogMsg(): logMsgLen=" << logMsgLen << endl;

    retCode = rcv(logMsgLen);
    if(retCode != 0)
    {
        goto EXIT;
    }
    msgsStr = QString::fromUtf8((const char*)rcvBuffer, logMsgLen);
    qDebug() << "CommHandler::rcvLogMsg(): msgsStr=" << msgsStr << endl;
    msgs << msgsStr;
    emit rcvdMsgs(msgs);
    msgs.clear();

EXIT:
    return retCode;
}

int CommHandler::openCommPort()
{
    qDebug() << "CommHandler::openCommPort()" << endl;
    int result = 0;
    DCB dcbSerialParams = {0};
    COMMTIMEOUTS timeouts = {0};
    QString commPortStr = QString("\\\\.\\") + guiNotifier->getSettings().commPort;
    hCommPort = CreateFile(commPortStr.utf16(),
                    GENERIC_READ | GENERIC_WRITE,
                    0,
                    0,
                    OPEN_EXISTING,
                    FILE_ATTRIBUTE_NORMAL,
                    0);
    if(hCommPort == INVALID_HANDLE_VALUE)
    {
        hCommPort = NULL;
        emit message("Unable to open connection to " + guiNotifier->getSettings().commPort + "\nCould not open serial port");
        result = -1;
        goto EXIT;
    }

    // get comm port config
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if(!GetCommState(hCommPort, &dcbSerialParams))
    {
        emit message("Unable to read configuration for " + guiNotifier->getSettings().commPort + " port");
        result = -1;
        goto EXIT;
    }
    dcbSerialParams.BaudRate = CBR_115200;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    // no flow control
    dcbSerialParams.fOutxCtsFlow = FALSE;
    dcbSerialParams.fRtsControl = RTS_CONTROL_DISABLE;
    dcbSerialParams.fInX = FALSE;
    dcbSerialParams.fOutX = FALSE;

    if(!SetCommState(hCommPort, &dcbSerialParams))
    {
        emit message("Unable to set configuration for " + guiNotifier->getSettings().commPort + " port");
        result = -1;
        goto EXIT;
    }

    timeouts.ReadIntervalTimeout = 300;
    timeouts.ReadTotalTimeoutConstant = 300;
    timeouts.ReadTotalTimeoutMultiplier = 300;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;
    if(!SetCommTimeouts(hCommPort, &timeouts))
    {
        emit message("Unable to set timeouts for " + guiNotifier->getSettings().commPort + " port");
        result = -1;
        goto EXIT;
    }

EXIT:
    if(result != 0)
    {
        closeCommPort();
    }
    emit commPortOpened(result == 0);
    return result;
}

int CommHandler::closeCommPort()
{
    qDebug() << "CommHandler::closeCommPort()" << endl;
    if(hCommPort != NULL)
    {
        CloseHandle(hCommPort);
        hCommPort = NULL;
    }
    return 0;
}
