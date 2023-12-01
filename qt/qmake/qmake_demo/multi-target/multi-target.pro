
TEMPLATE = app
INCLUDEPATH += ./widget
LIBS += -L ./widget -l Qt5Widgets


A {
	TARGET = multi-target
	HEADERS += widget/widget.h
	SOURCES += src/main.cpp widget/widget.cpp
	DEFINES += QT_DEPRECATED_WARNINGS
}

B {
  TARGET = test_main
  SOURCES += test_main.cpp
}

