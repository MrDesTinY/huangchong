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

#ifndef __LOG_VIEW_H__
#define __LOG_VIEW_H__

#include <QListView>

class LogView : public QListView
{
public:
    LogView(QWidget* parent = 0);
protected:
    void contextMenuEvent(QContextMenuEvent *event);
    //void selectionChanged(const QItemSelection& selected, const QItemSelection& deselected);
};

#endif//__LOG_VIEW_H__
