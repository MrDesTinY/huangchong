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

#include "LogItemDelegate.h"

#include <QtDebug>
#include <QLineEdit>
#include <QPainter>
#include <QSortFilterProxyModel>

#include "LogModel.h"
#include "LogEntry.h"

void LogItemDelegate::paint(
        QPainter *painter, const QStyleOptionViewItem &option,
        const QModelIndex &index) const
{
    QRect rect = option.rect;
    const QAbstractItemModel* model = index.model();
    QSortFilterProxyModel* proxyModel = (QSortFilterProxyModel*)model;
    LogModel* logModel = (LogModel*) (proxyModel->sourceModel());
    int row = proxyModel->mapToSource(index).row();
    QVector<LogEntry*> logEntries = logModel->getLogEntries();
    LogEntry* crtEntry = logEntries.at(row);

    if(crtEntry->logColor.isValid())
    {
        painter->fillRect(rect.x(), rect.y(), rect.width(), rect.height(), crtEntry->logColor);
    }

    const QPen& origPen = painter->pen();

    int totalRowCount1 = logModel->rowCount() + 1;
    QString totalRowCount1Str = QString::number(totalRowCount1);
    QFontMetrics fm(option.fontMetrics);
    QRect totalRowCount1StrRect = fm.boundingRect(totalRowCount1Str);
    QRect lineNoRect(rect.x(), rect.y(), totalRowCount1StrRect.width() + 10, rect.height());

    // draw line no column
    QColor lineNoColor = QColor(235, 234, 219);
    if(crtEntry->tagColor.isValid())
        lineNoColor = crtEntry->tagColor;
    painter->fillRect(lineNoRect, lineNoColor);
    QString crtRow1Str = QString::number(row + 1);
    painter->setPen(QColor(Qt::black));
    painter->drawText(lineNoRect, Qt::AlignCenter, crtRow1Str);

    painter->setPen(QColor(72, 72, 85));
    painter->drawRect(rect.x(), rect.y()-1, lineNoRect.width(), lineNoRect.height());

    // draw log line
    painter->save();
    painter->translate(lineNoRect.width() + 3, 0);
    QItemDelegate::paint(painter, option, index);
    painter->restore();

    // draw bottom border
    painter->setPen(QColor(106, 110, 169));
    painter->drawLine(rect.x() + lineNoRect.width(), rect.y() + rect.height() - 1, rect.x() + rect.width(), rect.y() + rect.height() - 1);
    painter->setPen(origPen);
}

QSize LogItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize s = QItemDelegate::sizeHint(option, index);
    QFontMetrics fm(option.fontMetrics);
    s.setHeight(fm.height() + 6);
    return s;
}

QWidget* LogItemDelegate::createEditor(
        QWidget *parent, const QStyleOptionViewItem &option,
        const QModelIndex &index) const
{
    QLineEdit* editor = new QLineEdit(parent);
    editor->setText(index.data().toString());
    editor->setReadOnly(true);
    return editor;
}

void LogItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QItemDelegate::setEditorData(editor, index);
}

void LogItemDelegate::setModelData(
        QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QItemDelegate::setModelData(editor, model, index);
}
