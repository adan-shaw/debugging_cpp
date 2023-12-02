#-------------------------------------------------
#
# Project created by QtCreator 2023-11-29T01:30:19
#
#-------------------------------------------------



QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TEMPLATE = app
DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11

# qmake 编译选项
QMAKE_CFLAGS += "-w -O2 -g3"
QMAKE_CXXFLAGS += "-w -O2 -g3"

# qmake 链接选项(linux 专用? windows 通过LIBS += "" 指定库路径, windows 平台貌似没有链接选项这个概念)
QMAKE_LFLAGS += ""



TARGET = task-manager-adan
HEADERS += qlistUI/qlistUI.h
SOURCES += qlistUI/main.cpp qlistUI/qlistUI.cpp
FORMS += qlistUI/qlistUI.ui



# Default rules for deployment(设置默认的安装路径, 不用管)
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
