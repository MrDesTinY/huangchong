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

#include "LogModel.h"

#include <QTextStream>
#include <QFile>
#include <QDomDocument>
#include <QMessageBox>
#include <QTextCodec>

#include "LogEntry.h"
#include "Util.h"
#include "mainwindow.h"

QString LogModel::indent("  ");
const QString LogModel::XML_ROOT_TAG = "metaLog";
const QString LogModel::XML_XLINE_TAG = "xLine";
const QString LogModel::XML_XLINE_LINE_ATTR = "line";
const QString LogModel::XML_XLINE_TAGCOLOR_ATTR = "tagColor";
const QString LogModel::XML_XLINE_LOGCOLOR_ATTR = "logColor";

LogModel::LogModel(MainWindow* parent):
        QAbstractListModel(parent),
        mainWnd(parent)
{
}

LogModel::~LogModel()
{
    deleteAll();
}

int LogModel::rowCount(const QModelIndex &parent) const
{
    return logEntries.count();
}

int LogModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant LogModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    int row = index.row();
    if(row < 0 || row >= logEntries.count())
    {
        return QVariant();
    }
    if(role == Qt::EditRole || role == Qt::DisplayRole)
        return logEntries.at(row)->log;
    return QVariant();
}

bool LogModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid())
        return false;
    int row = index.row();
    if(row < 0 || row >= logEntries.count())
        return false;
    if(index.column() != 0)
        return false;
    if(role == Qt::EditRole || role == Qt::DisplayRole)
    {
        if(logEntries[row] == NULL)
            logEntries[row] = new LogEntry();
        logEntries[row]->log = value.toString();
        emit(dataChanged(index, index));
        return true;
    }
    return false;
}

Qt::ItemFlags LogModel::flags(const QModelIndex &index) const
{
    return  Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

bool LogModel::insertRows(int row, int count, const QModelIndex& parent)
{
    beginInsertRows(QModelIndex(), row, row + count - 1);

    for(int crtRow = 0; crtRow < count; crtRow++)
    {
        LogEntry* newLogEntry = new LogEntry();
        logEntries.append(newLogEntry);
    }

    endInsertRows();
    return true;
}

bool LogModel::removeRows(int row, int count, const QModelIndex& parent)
{
    beginRemoveRows(QModelIndex(), row, row + count - 1);

    for(int crtRow = 0; crtRow < count; crtRow++)
    {
        LogEntry* toRemove = logEntries.at(row + crtRow);
        logEntries.remove(row + crtRow);
        delete toRemove;
    }

    endRemoveRows();
    return true;
}

void LogModel::colorTagLine(int row, QColor color)
{
    LogEntry* logEntry = logEntries.at(row);
    logEntry->tagColor = color;
}

void LogModel::colorLogLine(int row, QColor color)
{
    LogEntry* logEntry = logEntries.at(row);
    logEntry->logColor = color;
}

QVector<LogEntry*> LogModel::getLogEntries() const
{
    return logEntries;
}

void LogModel::deleteAll()
{
    for(int i = 0; i < logEntries.count(); i++)
    {
        delete logEntries.at(i);
    }
    logEntries.clear();
    //logEntries = QList<LogEntry*>();
    reset();
}

void LogModel::addLogEntry(LogEntry* logEntry)
{
    //QTextCodec *codec = QTextCodec::codecForLocale();
	//logEntry->log = codec->toUnicode((char *)(logEntry->log.data()));
	logEntries.append(logEntry);
}

void LogModel::repaintAll()
{
    reset();
}

void LogModel::saveLog(QTextStream& textStream,QRegExp regExp)
{
    int pos = 0;
	foreach(LogEntry* entry, logEntries)
    {
        regExp.indexIn(entry->log);
		if((entry->log).count(regExp))
		{
			//QRegExp regExpTmp("\s+");
			//while ((pos = regExpTmp.indexIn(entry->log,pos)) != -1)
			//{
			//	
			//	pos += regExpTmp.matchedLength();
			//}
			(entry->log).replace(QRegExp(" +"),"\t");			
			textStream << entry->log << endl;
		}
    }
}

void LogModel::saveXmlMetaLog(QTextStream& textStream)
{
    textStream << "<" + XML_ROOT_TAG + " version=\"0.1\">\n";
    for(int i = 0; i < logEntries.count(); i++)
    {
        LogEntry* crtEntry = logEntries.at(i);
        QColor crtTagColor = crtEntry->tagColor;
        QColor crtLogColor = crtEntry->logColor;
        if(crtTagColor.isValid() || crtLogColor.isValid())
        {
            textStream << indent << "<" << XML_XLINE_TAG << " " << XML_XLINE_LINE_ATTR << "=\"" << (i + 1) << "\"";
            if(crtTagColor.isValid())
            {
                textStream << " " << XML_XLINE_TAGCOLOR_ATTR << "=\"" + Util::color2RRGGBB(crtTagColor) +"\"";
            }
            if(crtLogColor.isValid())
            {
                textStream << " " << XML_XLINE_LOGCOLOR_ATTR << "=\"" + Util::color2RRGGBB(crtLogColor) +"\"";
            }

            textStream << " />\n";
        }
    }
    textStream << "</metaLog>\n";
}

void LogModel::loadLog(QFile& file)
{
    deleteAll();
	QTextCodec *code=QTextCodec::codecForName("unicode");//本人采用的是UTF8格式显示的
	QTextStream stream(&file);
	stream.setCodec(code);//输出流的设置
    while(!stream.atEnd())
    {
        QString line = stream.readLine().trimmed();
        LogEntry* logEntry = new LogEntry(line);
        addLogEntry(logEntry);
    }
}

void LogModel::loadXmlMetaLog(QFile& xmlFile)
{
    QString errorStr;
    int errorLine;
    int errorColumn;
    QDomDocument doc;
    if(!doc.setContent(&xmlFile, true, &errorStr, &errorLine, &errorColumn))
    {
        QMessageBox::warning(mainWnd, QObject::tr("XML parser"),
                             QObject::tr("Parse error at line %1, column %2:\n%3")
                             .arg(errorLine)
                             .arg(errorColumn)
                             .arg(errorStr));
        return;
    }
    QDomElement root = doc.documentElement();
    if(root.tagName() != LogModel::XML_ROOT_TAG)
    {
        QMessageBox::warning(mainWnd, QObject::tr("XML parser"),
                             QObject::tr("Unknown root element (%1), expected %2.")
                             .arg(root.tagName())
                             .arg(LogModel::XML_ROOT_TAG));
        return;
    }
    QDomNode node = root.firstChild();
    while(!node.isNull())
    {
        if(node.isElement())
        {
            QDomElement crtElem = node.toElement();
            if(crtElem.tagName() == XML_XLINE_TAG)
            {
                int crtRowIdx0 = -1;
                QString crtLineNo1Str = crtElem.attribute(XML_XLINE_LINE_ATTR).trimmed();
                if(crtLineNo1Str.length() != 0)
                {
                    bool conversionOk = false;
                    int crtLineNo1 = crtLineNo1Str.toInt(&conversionOk, 10);
                    if(!conversionOk)
                        continue;
                    crtRowIdx0 = crtLineNo1 - 1;
                }
                if(crtRowIdx0 >= 0 && crtRowIdx0 < logEntries.count())
                {
                    LogEntry* crtLogEntry = logEntries.at(crtRowIdx0);
                    QString crtTagColorStr = crtElem.attribute(XML_XLINE_TAGCOLOR_ATTR).trimmed();
                    if(crtTagColorStr.length() != 0)
                        crtLogEntry->tagColor.setNamedColor(crtTagColorStr);

                    QString crtLogColorStr = crtElem.attribute(XML_XLINE_LOGCOLOR_ATTR).trimmed();
                    if(crtLogColorStr.length() != 0)
                        crtLogEntry->logColor.setNamedColor(crtLogColorStr);
                }
            }
        }//if(node.isElement())

        node = node.nextSibling();
    }
}
