#include <QDebug>
#include "widget.h"
Widget::Widget(QWidget *parent):QWidget(parent){
	qDebug()<<"this is in class construct!"<<endl;
}
