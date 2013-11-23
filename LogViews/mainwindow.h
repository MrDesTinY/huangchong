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

#ifndef __MAIN_WINDOW_H__
#define __MAIN_WINDOW_H__

#include <QDialog>
#include <QtGui/QMainWindow>
#include <QStringListModel>
#include <QRegExp>

#include "UserSettings.h"

class QLabel;
class QSortFilterProxyModel;
class QAction;
class QLineEdit;
class QItemSelection;
class QListView;

class LogView;
class UdpThread;
class BtThread;
class CommThread;
class LogModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QAction* GetColorTagAction();
    QAction* GetColorLogAction();
    QAction* GetDelSelRowsAction();
    QMenu* createPopupMenu();
    UserSettings getSettings();

private slots:
    // UDP
    void udpBindedSlot(bool success);
    // COMM
    void commPortOpenedSlot(bool success);
    // BT
    void btStartedSlot(bool success);
    void btConnectedSlot();
    void btDisconnectedSlot();

    void connectionThreadFinishedSlot();

    void showMessage(const QString& msg);
    void processMsgs(const QStringList& msgs);
    void searchSlot();
    void clearSearchSlot();
    void searchFilterChanged(const QString& filterStr);
    void openFileSlot();
    void saveFileSlot();
    void startListeningSlot();
    void stopListeningSlot();
    void goToLineSlot();
    void deleteAllSlot();
    void showSettingsDlgSlot();
    void showAbout();
    void setSelRowsTagColorSlot();
    void setSelRowsLogColorSlot();
    void returnPressedInFilter();

protected:
    void timerEvent(QTimerEvent * timerEvt);
private:
    // Methods:
    void loadFile(const QString& fullFileName);
    void loadXmlMetaFile(const QString& fullFileName);
    void saveFile(const QString& fullFileName);
    void saveXmlMetaFile(const QString& fullXmlFileName);
    void saveFileAs();

    // Data:
    QAction* startAction;
    QAction* stopAction;
    QAction* colorTagAction;
    QAction* colorLogAction;
    QAction* delSelRowsAction;
    QAction* searchAction;
    QAction* clearSearchAction;

    LogView* listView;
    LogModel* listModel;
    QSortFilterProxyModel* proxyModel;
    UdpThread* udpThread;
    BtThread* btThread;
    CommThread* commThread;
    int refreshListTimerId;
    QStringList batchMsgs;
    QLabel* statusLblNoOfLines;
    QLabel* statusLblOnlineMode;
    QLineEdit* searchEdit;
    QString crtFullFileName;
    QString crtSearchStr;
    UserSettings settings;
	QRegExp regExp;
};

#endif//__MAIN_WINDOW_H__
