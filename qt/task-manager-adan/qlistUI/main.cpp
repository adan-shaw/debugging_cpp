#include "qlistUI.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QList<item_t> m_qlist;
	QApplication a(argc, argv);
	qlistUI qlistUI(&m_qlist);
	//printf("main() pid: %ll", a.applicationPid());
	qlistUI.set_main_pid(a.applicationPid());
	qlistUI.update_main_pid();
	qlistUI.show();
	return a.exec();
}
