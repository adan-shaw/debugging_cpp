#include "terminalwidget.h"
#include <QApplication>

//
void changeValue(void){
	return ;
}
int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	//Terminal 运行demo:
	TerminalWidget w(1024,768);
	TerminalWidgetEx w2(1024,768);
	w.show();
	//w2.show();

	return app.exec();//执行QApplication() 类, 直接替换main() 主函数(相当于main() 主函数)
}
