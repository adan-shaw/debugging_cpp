//#include <QtGui/QApplication>//qt4 专用
#include <QtWidgets/QApplication>//qt5专用
#include <QDebug>
#include "widget.h"
int main(int argc, char **argv){
	QApplication app(argc, argv);
	Widget w;
	w.show();
	app.exec();
	return 0;
}
