#-------------------------------------------------
#
# Project created by QtCreator 2023-11-29T01:30:19
#
#-------------------------------------------------



QT += core gui
QMAKE_LFLAGS += "-w -O2"
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TEMPLATE = app
DEFINES += QT_DEPRECATED_WARNINGS
CONFIG += c++11



TARGET = task-manager-adan
HEADERS += qlistUI/qlistUI.h
SOURCES += qlistUI/main.cpp qlistUI/qlistUI.cpp
FORMS += qlistUI/qlistUI.ui



# Default rules for deployment(设置默认的安装路径, 不用管)
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
