
QT += core gui
QT += xml

QT += gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += qt

TARGET = simvdcmtk
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    loadfile.cpp \
    db.cpp \
    sendfile.cpp

HEADERS  += mainwindow.h \
    dcmtklib.h \
    loadfile.h \
    db.h \
    sendfile.h

FORMS    += mainwindow.ui

unix:!macx:!symbian: LIBS += -ldcmimgle
unix:!macx:!symbian: LIBS += -ldcmdata
unix:!macx:!symbian: LIBS += -loflog
unix:!macx:!symbian: LIBS += -lofstd
unix:!macx:!symbian: LIBS += -ldcmfg
unix:!macx:!symbian: LIBS += -ldcmimage
unix:!macx:!symbian: LIBS += -ldcmimgle
unix:!macx:!symbian: LIBS += -ldcmiod
unix:!macx:!symbian: LIBS += -ldcmjpeg
unix:!macx:!symbian: LIBS += -ldcmjpls
unix:!macx:!symbian: LIBS += -ldcmnet
unix:!macx:!symbian: LIBS += -ldcmpstat
unix:!macx:!symbian: LIBS += -ldcmqrdb
unix:!macx:!symbian: LIBS += -ldcmrt
unix:!macx:!symbian: LIBS += -ldcmseg
unix:!macx:!symbian: LIBS += -ldcmsr
unix:!macx:!symbian: LIBS += -ldcmtls
unix:!macx:!symbian: LIBS += -ldcmwlm

QMAKE_CXXFLAGS += -DHAVE_CONFIG_H

unix:!macx: LIBS += -L$$PWD/../../../../usr/local/lib64/ -lOdbcFb
LIBS += -L/usr/lib -lodbc
INCLUDEPATH += $$PWD/../../../../usr/local/lib64
DEPENDPATH += $$PWD/../../../../usr/local/lib64
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../usr/local/lib64/release/ -lOdbcFb
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../usr/local/lib64/debug/ -lOdbcFb
else:unix: LIBS += -L$$PWD/../../../../usr/local/lib64/ -lOdbcFb
INCLUDEPATH += $$PWD/../../../../usr/local/lib64
DEPENDPATH += $$PWD/../../../../usr/local/lib64
