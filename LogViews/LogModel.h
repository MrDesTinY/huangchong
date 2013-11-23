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

#ifndef __LOG_MODEL_H__
#define __LOG_MODEL_H__

#include <QAbstractListModel>
#include <QColor>
#include <QVector>
#include <QRegExp>

#include "LogEntry.h"

class QTextStream;
class QFile;

class MainWindow;

class LogModel : public QAbstractListModel
{
    Q_OBJECT
public:
    LogModel(MainWindow* parent = 0);
    ~LogModel();
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    int columnCount(const QModelIndex& parent) const;

    QVariant data(const QModelIndex& index, int role) const;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);

    Qt::ItemFlags flags(const QModelIndex& index) const;

    bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex());

    void colorTagLine(int row, QColor color);
    void colorLogLine(int row, QColor color);
    QVector<LogEntry*> getLogEntries() const;
    void deleteAll();

    void addLogEntry(LogEntry* logEntry);
    void repaintAll();

    void saveLog(QTextStream& textStream,QRegExp regExp);
    void saveXmlMetaLog(QTextStream& textStream);

    void loadLog(QFile& file);
    void loadXmlMetaLog(QFile& xmlFile);

    // XML consts:
    static const QString XML_ROOT_TAG;
    static const QString XML_XLINE_TAG;
    static const QString XML_XLINE_LINE_ATTR;
    static const QString XML_XLINE_TAGCOLOR_ATTR;
    static const QString XML_XLINE_LOGCOLOR_ATTR;

private:
    Q_DISABLE_COPY(LogModel)
    // Data:
    static QString indent;
    MainWindow* mainWnd;
    QVector<LogEntry*> logEntries;
};

#endif//__LOG_MODEL_H__
