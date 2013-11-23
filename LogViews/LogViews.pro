TARGET = LogViews
TEMPLATE = app
QT += network
QT += xml
RESOURCES += logviews.qrc \
    logviews.qrc

SOURCES += main.cpp \
    mainwindow.cpp \
    UdpThread.cpp \
    LogView.cpp \
    LogModel.cpp \
    LogItemDelegate.cpp \
    LogEntry.cpp \
    Util.cpp \
    GoToLineDlg.cpp \
    BtThread.cpp \
    SettingsDlg.cpp \
    UserSettings.cpp \
    CommThread.cpp

HEADERS += mainwindow.h \
    UdpThread.h \
    LogView.h \
    LogModel.h \
    LogItemDelegate.h \
    LogEntry.h \
    Util.h \
    GoToLineDlg.h \
    BtThread.h \
    SettingsDlg.h \
    UserSettings.h \
    CommThread.h

RC_FILE = LogViews.rc

win32:LIBS += ws2_32.lib
