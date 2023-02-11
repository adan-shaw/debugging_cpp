//#include <QtGui/QWidget>//qt4专用
#include <QtWidgets/QWidget>//qt5专用
class Widget:public QWidget{
	Q_OBJECT
	public:
		Widget(QWidget * parent=0);
};
