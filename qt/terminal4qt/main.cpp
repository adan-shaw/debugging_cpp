#include "terminalwidget.h"
#include <QApplication>

//失败的project, it's closed now!!
int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	//Terminal 运行demo:
	TerminalWidget w(1024,768);
	TerminalWidgetEx w2(1024,768);
	w.show();//(输入/输出没办法保证, 有比较大的缺陷, 少点用)
	w2.show();//只输出, 不能输入, 可以参考一下

	return app.exec();//执行QApplication() 类, 直接替换main() 主函数(相当于main() 主函数)
}
