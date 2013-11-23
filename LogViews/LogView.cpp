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

#include "LogView.h"
#include "mainwindow.h"

#include <QMenu>
#include <QContextMenuEvent>
#include <QDebug>
#include <QHeaderView>

LogView::LogView(QWidget *parent):
        QListView(parent)
{
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setWordWrap(false);
    setUniformItemSizes(true);
    setAlternatingRowColors(true);
}

void LogView::contextMenuEvent(QContextMenuEvent *event)
{
    if(selectionModel()->selectedIndexes().count() != 0)
    {
        MainWindow* mainWnd = static_cast<MainWindow *>(QObject::parent());
        QMenu menu(this);
        menu.addAction(mainWnd->GetColorTagAction());
        menu.addAction(mainWnd->GetColorLogAction());
        menu.addSeparator();
        menu.addAction(mainWnd->GetDelSelRowsAction());
        menu.exec(mapToGlobal(event->pos()));
    }
}
