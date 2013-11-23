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

#include "mainwindow.h"

#include <QListView>
#include <QStringListModel>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QtDebug>
#include <QTimerEvent>
#include <QStatusBar>
#include <QLabel>
#include <QSortFilterProxyModel>
#include <QLineEdit>
#include <QToolBar>

#include <QAction>
#include <QIcon>
#include <QFileDialog>
#include <QItemSelection>
#include <QFrame>
#include <QItemSelectionRange>
#include <QColorDialog>

#include "UdpThread.h"
#include "BtThread.h"
#include "CommThread.h"
#include "LogModel.h"
#include "LogView.h"
#include "LogItemDelegate.h"
#include "GoToLineDlg.h"
#include "SettingsDlg.h"

#define REFRESH_LIST_TIME_MILLIS 300

MainWindow::MainWindow(QWidget *parent):
        QMainWindow(parent),
        startAction(NULL),
        stopAction(NULL),
        colorTagAction(NULL),
        colorLogAction(NULL),
        delSelRowsAction(NULL),
        searchAction(NULL),
        clearSearchAction(NULL),

        listView(NULL),
        listModel(NULL),
        proxyModel(NULL),
        udpThread(NULL),
        btThread(NULL),
        commThread(NULL),
        refreshListTimerId(0),
        batchMsgs(),
        statusLblNoOfLines(NULL),
        statusLblOnlineMode(NULL),
        searchEdit(NULL),
        crtFullFileName(""),
        crtSearchStr(""),
        settings()
{
    UserSettings::LoadSettings(&settings);
    LogItemDelegate* logDelegate = new LogItemDelegate(this);
    listView = new LogView(this);
    listView->setItemDelegate(logDelegate);

    listModel = new LogModel(this);

    proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setDynamicSortFilter(true);
    proxyModel->setSourceModel(listModel);
    proxyModel->setFilterKeyColumn(0);

    listView->setModel(proxyModel);

    //startAction = new QAction(tr("Listen"), this);
    //startAction->setIcon(QIcon(":/images/start.png"));
    //startAction->setStatusTip(tr("Start listening"));
    //connect(startAction, SIGNAL(triggered()), this, SLOT(startListeningSlot()));
    //stopAction = new QAction(tr("Stop"), this);
    //stopAction->setIcon(QIcon(":/images/stop.png"));
    //stopAction->setStatusTip(tr("Stop listening"));
    //stopAction->setEnabled(false);
    //connect(stopAction, SIGNAL(triggered()), this, SLOT(stopListeningSlot()));
    QToolBar* toolBar = addToolBar(tr("Toolbar"));
    toolBar->setFloatable(false);
    toolBar->setMovable(false);
    toolBar->setAllowedAreas(Qt::TopToolBarArea);
    //toolBar->addAction(startAction);
    //toolBar->addAction(stopAction);
    toolBar->addSeparator();
//-----------------------------------------------------------
    QAction* openAction = new QAction(tr("Open file"), this);
    openAction->setIcon(QIcon(":/images/open.png"));
    openAction->setStatusTip(tr("Open log file"));
    connect(openAction, SIGNAL(triggered()), this, SLOT(openFileSlot()));
	QAction* saveAction = new QAction(tr("Save file"), this);
	saveAction->setIcon(QIcon(":/images/save.png"));
	saveAction->setStatusTip(tr("Save to log file"));
	connect(saveAction, SIGNAL(triggered()), this, SLOT(saveFileSlot()));
	toolBar->addAction(openAction);
    toolBar->addAction(saveAction);
    toolBar->addSeparator();
//-----------------------------------------------------------
    QAction* goToLineAction = new QAction(tr("Go to line"), this);
    goToLineAction->setIcon(QIcon(":/images/go_to_line_no.png"));
    goToLineAction->setStatusTip(tr("Go to line number"));
    goToLineAction->setShortcut(tr("Ctrl+G"));
    connect(goToLineAction, SIGNAL(triggered()), this, SLOT(goToLineSlot()));
    toolBar->addAction(goToLineAction);

    QAction* deleteAllAction = new QAction(tr("Delete all"), this);
    deleteAllAction->setIcon(QIcon(":/images/clearlist.png"));
    deleteAllAction->setStatusTip(tr("Delete all messages"));
    connect(deleteAllAction, SIGNAL(triggered()), this, SLOT(deleteAllSlot()));
    toolBar->addAction(deleteAllAction);
    toolBar->addSeparator();
//-----------------------------------------------------------
    //QAction* settingsAction = new QAction(tr("Settings"), this);
    //settingsAction->setIcon(QIcon(":/images/settings.png"));
    //settingsAction->setStatusTip(tr("Settings"));
    //connect(settingsAction, SIGNAL(triggered()), this, SLOT(showSettingsDlgSlot()));
    //toolBar->addAction(settingsAction);
    //toolBar->addSeparator();
//-----------------------------------------------------------
    searchEdit = new QLineEdit();
    connect(searchEdit, SIGNAL(textChanged(const QString&)), this, SLOT(searchFilterChanged(const QString&)));
    toolBar->addWidget(searchEdit);
    searchAction = new QAction(tr("Search"), this);
    searchAction->setIcon(QIcon(":/images/search.png"));
    searchAction->setEnabled(false);
    connect(searchEdit, SIGNAL(returnPressed()), this, SLOT(returnPressedInFilter()));
    connect(searchAction, SIGNAL(triggered()), this, SLOT(searchSlot()));
    toolBar->addAction(searchAction);
    clearSearchAction = new QAction(tr("Clear search"), this);
    clearSearchAction->setIcon(QIcon(":/images/search_clear.png"));
    clearSearchAction->setEnabled(false);
    connect(clearSearchAction, SIGNAL(triggered()), this, SLOT(clearSearchSlot()));
    toolBar->addAction(clearSearchAction);
    toolBar->addSeparator();
//-----------------------------------------------------------
    QAction* aboutAction = new QAction(tr("About"), this);
    aboutAction->setIcon(QIcon(":/images/about.png"));
    aboutAction->setStatusTip(tr("About LogViews"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(showAbout()));
    toolBar->addAction(aboutAction);
    toolBar->addSeparator();
//-----------------------------------------------------------
    colorTagAction = new QAction(tr("Set tag color"), this);
    colorTagAction->setStatusTip(tr("Set tag colors for selected messages"));
    connect(colorTagAction, SIGNAL(triggered()), this, SLOT(setSelRowsTagColorSlot()));
    colorLogAction = new QAction(tr("Set line color"), this);
    colorLogAction->setStatusTip(tr("Set line colors for selected messages"));
    connect(colorLogAction, SIGNAL(triggered()), this, SLOT(setSelRowsLogColorSlot()));

    delSelRowsAction = new QAction(tr("Delete"), this);
    delSelRowsAction->setStatusTip(tr("Delete selected messages"));

    setCentralWidget(listView);

    statusLblNoOfLines = new QLabel();
    statusLblNoOfLines->setText(tr("Lines: 0"));
    statusBar()->addPermanentWidget(statusLblNoOfLines);

    statusLblOnlineMode = new QLabel();
    statusLblOnlineMode->setText(tr("Offline"));
    statusBar()->addPermanentWidget(statusLblOnlineMode);

    udpThread = new UdpThread(this);
    connect(udpThread, SIGNAL(finished()), this, SLOT(connectionThreadFinishedSlot()));
    connect(udpThread, SIGNAL(terminated()), this, SLOT(connectionThreadFinishedSlot()));

    btThread = new BtThread(this);
    connect(btThread, SIGNAL(finished()), this, SLOT(connectionThreadFinishedSlot()));
    connect(btThread, SIGNAL(terminated()), this, SLOT(connectionThreadFinishedSlot()));

    commThread = new CommThread(this);
    connect(commThread, SIGNAL(finished()), this, SLOT(connectionThreadFinishedSlot()));
    connect(btThread, SIGNAL(terminated()), this, SLOT(connectionThreadFinishedSlot()));

    setWindowIcon(QIcon(":/images/log.png"));
}

MainWindow::~MainWindow()
{
    UserSettings::SaveSettings(&settings);

    delete udpThread;
    udpThread = NULL;

    delete btThread;
    btThread = NULL;

    delete commThread;
    commThread = NULL;
}

void MainWindow::timerEvent(QTimerEvent* timerEvt)
{
    if(!timerEvt)
        return;
    if(timerEvt->timerId() == refreshListTimerId)
    {
        int rowCount = listModel->rowCount();
        int batchCount = batchMsgs.count();
        if(batchCount > 0)
        {
            listModel->insertRows(rowCount, batchCount);
            int i = 0;
            foreach(QString msg, batchMsgs)
            {
                listModel->setData(listModel->index(rowCount + i, 0), msg.trimmed());
                ++i;
            }
            statusLblNoOfLines->setText(tr("Lines: ") + QString::number(rowCount + batchCount));
            batchMsgs.clear();
        }
    }
}

void MainWindow::udpBindedSlot(bool success)
{
    if(!success)
    {
        statusLblOnlineMode->setText(tr("Offline"));
        startAction->setEnabled(true);
        stopAction->setEnabled(false);
        QMessageBox::warning(this, tr("LogViews"), tr("bind() error"), QMessageBox::Ok);
    }
    else
    {
        statusLblOnlineMode->setText(tr("Listening"));
        startAction->setEnabled(false);
        stopAction->setEnabled(true);
        refreshListTimerId = startTimer(REFRESH_LIST_TIME_MILLIS);
    }
}

void MainWindow::connectionThreadFinishedSlot()
{
    killTimer(refreshListTimerId);
    statusLblOnlineMode->setText(tr("Offline"));
    startAction->setEnabled(true);
    stopAction->setEnabled(false);
}

void MainWindow::commPortOpenedSlot(bool success)
{
    if(!success)
    {
        statusLblOnlineMode->setText(tr("Offline"));
        startAction->setEnabled(true);
        stopAction->setEnabled(false);
    }
    else
    {
        refreshListTimerId = startTimer(REFRESH_LIST_TIME_MILLIS);
        statusLblOnlineMode->setText("Listening on " + settings.commPort + " port");
        startAction->setEnabled(false);
        stopAction->setEnabled(true);
    }
}

void MainWindow::btStartedSlot(bool success)
{
    if(!success)
    {
        statusLblOnlineMode->setText(tr("Offline"));
        startAction->setEnabled(true);
        stopAction->setEnabled(false);
        QMessageBox::warning(this, tr("LogViews"), tr("Could not start bluetooth"), QMessageBox::Ok);
    }
    else
    {
        statusLblOnlineMode->setText(tr("Listening on bluetooth"));
        startAction->setEnabled(false);
        stopAction->setEnabled(true);
    }
}

void MainWindow::btConnectedSlot()
{
    statusLblOnlineMode->setText(tr("Connected on bluetooth"));
    refreshListTimerId = startTimer(REFRESH_LIST_TIME_MILLIS);
}

void MainWindow::btDisconnectedSlot()
{
    statusLblOnlineMode->setText(tr("Listening on bluetooth"));
    killTimer(refreshListTimerId);
}

void MainWindow::showMessage(const QString& msg)
{
    QMessageBox::warning(this, tr("LogViews"), msg, QMessageBox::Ok);
}

void MainWindow::processMsgs(const QStringList& msgs)
{
    qDebug() << "MainWindow::processMsgs()" << endl;
    batchMsgs.append(msgs);
}

void MainWindow::searchSlot()
{
    crtSearchStr = searchEdit->text();
    searchAction->setEnabled(false);
    statusBar()->clearMessage();
	regExp.setPattern(crtSearchStr);
	regExp.setCaseSensitivity(Qt::CaseInsensitive);
	regExp.setPatternSyntax(QRegExp::FixedString);
    //QRegExp regExp(crtSearchStr, Qt::CaseInsensitive, QRegExp::FixedString);
    proxyModel->setFilterRegExp(regExp);
}

void MainWindow::clearSearchSlot()
{
    searchEdit->clear();
    statusBar()->clearMessage();
    crtSearchStr = "";
    int currentSrcIdx0 = 0;
    QItemSelection srcSelRanges = proxyModel->mapSelectionToSource(listView->selectionModel()->selection());
    QModelIndex currentProxyModelIdx = listView->selectionModel()->currentIndex();
    if(currentProxyModelIdx.isValid())
        currentSrcIdx0 = proxyModel->mapToSource(currentProxyModelIdx).row();

    delete proxyModel;
    proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setDynamicSortFilter(true);
    proxyModel->setSourceModel(listModel);
    proxyModel->setFilterKeyColumn(0);

    listView->setModel(proxyModel);

    QModelIndex restoreSrcCrtIdx = listModel->index(currentSrcIdx0, 0);
    QModelIndex restoreProxyCrtIdx = proxyModel->mapFromSource(restoreSrcCrtIdx);
    listView->scrollTo(restoreProxyCrtIdx, QAbstractItemView::PositionAtCenter);
    listView->selectionModel()->select(srcSelRanges, QItemSelectionModel::Select);

    listView->raise();
    listView->activateWindow();
    listView->setFocus();
}

void MainWindow::searchFilterChanged(const QString& filterStr)
{
    if(filterStr.isEmpty())
    {
        searchAction->setEnabled(false);
        clearSearchAction->setEnabled(false);
    }
    else
    {
        searchAction->setEnabled(true);
        clearSearchAction->setEnabled(true);
    }
    if(crtSearchStr.isEmpty())
    {
        statusBar()->showMessage(tr("No current filter"));
    }
    else
    {
        statusBar()->showMessage(tr("Current filter: ") + crtSearchStr);
    }
}

void MainWindow::startListeningSlot()
{
    startAction->setEnabled(false);
    stopAction->setEnabled(false);

    switch(settings.connectionType)
    {
    case ConnectionType::CONN_UDP:
        udpThread->start();
        break;
    case ConnectionType::CONN_BLUETOOTH:
        btThread->start();
        break;
    case ConnectionType::CONN_COMM:
        commThread->start();
        break;
    }
}

void MainWindow::stopListeningSlot()
{
    stopAction->setEnabled(false);
    startAction->setEnabled(false);
    switch(settings.connectionType)
    {
    case ConnectionType::CONN_UDP:
        udpThread->stop();
        break;
    case ConnectionType::CONN_BLUETOOTH:
        btThread->stop();
        break;
    case ConnectionType::CONN_COMM:
        commThread->stop();
        break;
    }
}

void MainWindow::goToLineSlot()
{
    int totalRowCount = listModel->rowCount();
    int currentSrcIdx0 = -2;
    QModelIndex currentProxyIdx = listView->selectionModel()->currentIndex();
    if(currentProxyIdx.isValid())
        currentSrcIdx0 = proxyModel->mapToSource(currentProxyIdx).row();

    GoToLineDlg dlg(this, 1, totalRowCount, currentSrcIdx0 + 1);
    dlg.exec();
    int selLineNo1 = dlg.getLineNo();

    if(selLineNo1 > 0)
    {
        listView->selectionModel()->clearSelection();
        QModelIndex newSrcCrtIdx = listModel->index(selLineNo1 - 1, 0);
        QModelIndex newProxyCrtIdx = proxyModel->mapFromSource(newSrcCrtIdx);
        listView->selectionModel()->select(newProxyCrtIdx, QItemSelectionModel::Select);
        listView->selectionModel()->setCurrentIndex(newProxyCrtIdx, QItemSelectionModel::Current);
        listView->scrollTo(newProxyCrtIdx, QAbstractItemView::PositionAtCenter);
        listView->raise();
        listView->activateWindow();
        listView->setFocus();
    }
}

void MainWindow::deleteAllSlot()
{
    listModel->deleteAll();
    statusLblNoOfLines->setText(tr("Lines: 0"));
}

void MainWindow::showSettingsDlgSlot()
{
    UserSettings newSettings = settings;
    SettingsDlg dlg(this, &newSettings);
    int result = dlg.exec();
    if(result == QDialog::Accepted)
    {
        if(newSettings != settings)
        {
            stopListeningSlot();
            settings = newSettings;
            UserSettings::SaveSettings(&settings);
        }
    }
}

void MainWindow::showAbout()
{
    QMessageBox::about(this, tr("About LogViews"),
            tr("<h2>LogViews 0.1</h2>"
               "<p>Copyright &copy; 2010 Ionut Dediu."
               "<p>License: Apache License, Version 2.0"
               "<p>LogViews is a small application that "
               "helps in debugging syslog messages through "
               "basic filtering, custom colors and comments."
               "<p>Support: <a href=\"http://sourceforge.net/projects/logviews\">http://sourceforge.net/projects/logviews</a>"));
}

void MainWindow::setSelRowsTagColorSlot()
{
    QColor tagColor = QColorDialog::getColor();
    if(!tagColor.isValid())
        return;
    QItemSelection srcSelRanges = proxyModel->mapSelectionToSource(listView->selectionModel()->selection());
    foreach(QItemSelectionRange crtSrcSelRange, srcSelRanges)
    {
        QModelIndexList srcSelIndexes = crtSrcSelRange.indexes();
        foreach(QModelIndex srcSelIdx, srcSelIndexes)
        {
            listModel->colorTagLine(srcSelIdx.row(), tagColor);
        }
    }
    listView->selectionModel()->select(listView->selectionModel()->selection(), QItemSelectionModel::Deselect);
}

void MainWindow::setSelRowsLogColorSlot()
{
    QColor logColor = QColorDialog::getColor();
    if(!logColor.isValid())
        return;
    QItemSelection srcSelRanges = proxyModel->mapSelectionToSource(listView->selectionModel()->selection());
    foreach(QItemSelectionRange crtSrcSelRange, srcSelRanges)
    {
        QModelIndexList srcSelIndexes = crtSrcSelRange.indexes();
        foreach(QModelIndex srcSelIdx, srcSelIndexes)
        {
            listModel->colorLogLine(srcSelIdx.row(), logColor);
        }
    }
    listView->selectionModel()->select(listView->selectionModel()->selection(), QItemSelectionModel::Deselect);
}

void MainWindow::returnPressedInFilter()
{
    if(searchAction->isEnabled())
        searchAction->trigger();
}

void MainWindow::openFileSlot()
{
	QString fullFileName = QFileDialog::getOpenFileName(this, tr("Open log file"),"E:\\",tr("Log Files (*.log *.txt)"));//, ".");
    if(!fullFileName.isEmpty())
        loadFile(fullFileName);
}

void MainWindow::loadFile(const QString &fullFileName)
{
    QFile file(fullFileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, tr("LogViews"),
                             tr("Cannot open file %1:\n%2.")
                             .arg(file.fileName())
                             .arg(file.errorString()));
        return;
    }
    crtFullFileName = fullFileName;
    QString shownName = QFileInfo(fullFileName).fileName();//fullFileName;
    listModel->loadLog(file);
    file.close();
    statusLblNoOfLines->setText(tr("Lines: ") + QString::number(listModel->rowCount()));
    setWindowTitle(tr("%1 - %2").arg(shownName).arg(tr("LogViews")));
    // load xml meta data (color info)
    //loadXmlMetaFile(crtFullFileName + ".lml");
    listModel->repaintAll();
}

void MainWindow::loadXmlMetaFile(const QString& fullFileName)
{
    QFile xmlFile(fullFileName);
    if(!xmlFile.exists())
    {
        qDebug() << "XML file " << fullFileName << " does not exist";
        return;
    }
    if(!xmlFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, tr("LogViews"),
                             tr("Cannot open file %1:\n%2.")
                             .arg(xmlFile.fileName())
                             .arg(xmlFile.errorString()));
        return;
    }
    listModel->loadXmlMetaLog(xmlFile);
    xmlFile.close();
}

void MainWindow::saveFileSlot()
{
    if(crtFullFileName.isEmpty())
    {
        return saveFileAs();
    }
    else
    {
        return saveFile(crtFullFileName);
    }
}

void MainWindow::saveFileAs()
{
	QString fullFileName = QFileDialog::getSaveFileName(this, tr("Save log file"),"E:\\",
		tr("Files (*.xls *.txt)"));//, ".");
    if(fullFileName.isEmpty())
        return;
    return saveFile(fullFileName);
}

void MainWindow::saveFile(const QString &fullFileName)
{
    //QFile file(fullFileName);
	QString fullFileNameAs = QFileDialog::getSaveFileName(this, tr("Save log file"),"E:\\",
		tr("Files (*.xls *.txt)"));//, ".");
	QFile file(fullFileNameAs);
    if(!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(this, tr("LogViews"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(file.fileName())
                             .arg(file.errorString()));
        return;
    }
    QTextStream out(&file);
    listModel->saveLog(out,regExp);
    file.close();
    crtFullFileName = fullFileName;
    //saveXmlMetaFile(crtFullFileName + ".lml");
}

void MainWindow::saveXmlMetaFile(const QString& fullXmlFileName)
{
    QFile xmlFile(fullXmlFileName);
    if(!xmlFile.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(this, tr("LogViews"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(xmlFile.fileName())
                             .arg(xmlFile.errorString()));
        return;
    }
    QTextStream xmlOut(&xmlFile);
    listModel->saveXmlMetaLog(xmlOut);
    xmlFile.close();
}

QAction* MainWindow::GetColorTagAction()
{
    return colorTagAction;
}

QAction* MainWindow::GetColorLogAction()
{
    return colorLogAction;
}

QAction* MainWindow::GetDelSelRowsAction()
{
    return delSelRowsAction;
}

QMenu* MainWindow::createPopupMenu()
{
    return NULL;
}

UserSettings MainWindow::getSettings()
{
    return settings;
}
