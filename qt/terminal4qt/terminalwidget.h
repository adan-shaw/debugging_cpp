#ifndef TERMINALWIDGET_H
#define TERMINALWIDGET_H

#include <QTextEdit>
#include <QTextBrowser>
#include <QKeyEvent>
#include <QFont>
#include <QProcess>
#include <QByteArray>
#include <QTextCursor>
#include <QTextBlock>
#include <QTextCodec>
//#include <assert.h>

//继承于QTextEdit 的可编辑terminal
class TerminalWidget: public QTextEdit
{
	Q_OBJECT														//必须包含的宏(里面有qt 的大量定义)
public:
	TerminalWidget(int x, int y);
	~TerminalWidget();

public slots:
	void myReadStdoutSlot();						//标准输出
	void myReadStderrSlot();						//错误输出

protected:
	void keyPressEvent(QKeyEvent *e);

private:
	QByteArray lastInput;								// 上一次的输入内容
	QFont font;													// 字体数据载体(必须为QFont 类型)
	const char* font_style = "Noto Sans Mono";
	const int font_size = 14;
	const QColor color_stdout = Qt::white;
	const QColor color_stderr = Qt::red;
	QProcess* proc;											// 命令行终端进程
	long long lastPosition = 0;					// 光标位置
	int window_x;												// 窗口长度
	int window_y;												// 窗口宽度
};



//继承于QTextBrowser 的显示terminal(不可编辑)
class TerminalWidgetEx: public QTextBrowser
{
	Q_OBJECT
public:
	TerminalWidgetEx(int x, int y);
	~TerminalWidgetEx();

public slots:
	void myReadStdoutSlot();						//标准输出
	void myReadStderrSlot();						//错误输出

private:
	QByteArray lastInput;								// 上一次的输入内容
	QFont font;													// 字体数据载体(必须为QFont 类型)
	const char* font_style = "Noto Sans Mono";
	const int font_size = 14;
	const QColor color_stdout = Qt::white;
	const QColor color_stderr = Qt::red;
	QProcess* proc;											// 命令行终端进程
	long long lastPosition = 0;					// 光标位置
	int window_x;												// 窗口长度
	int window_y;												// 窗口宽度
};

#endif // TERMINALWIDGET_H


