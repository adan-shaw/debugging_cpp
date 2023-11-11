#include "terminalwidget.h"

TerminalWidget::TerminalWidget(int x, int y)
{
	QTextCursor editCursor = textCursor();
	QTextBlockFormat textBlockFormat;

	// 初始化窗口大小
	window_x = x;
	window_y = y;
	resize(window_x,window_y);

	// 初始化窗口背景颜色和内容颜色
	setStyleSheet("background-color:rgb(0,0,0); color:rgb(255,255,255); border:20px;");
	textBlockFormat.setLineHeight(24, QTextBlockFormat::FixedHeight);
	editCursor.setBlockFormat(textBlockFormat);
	setTextCursor(editCursor);

	font.setFamily(font_style);				//设置字体样式
	font.setPixelSize(font_size);			//设置字体大小
	setFont(font);										//设置字体数据载体

	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	setWindowTitle("qt terminal title");

	//创建一个新进程, 运行terminal
	proc = new QProcess();
	// connect(): 将stdout, 重定向到QTextEdit 容器的信号槽里面(锁死操作)
	connect(proc,SIGNAL(readyReadStandardOutput()),this,SLOT(myReadStdoutSlot()));
	// connect(): 将stderr, 重定向到QTextEdit 容器的信号槽里面(锁死操作)
	connect(proc,SIGNAL(readyReadStandardError()),this,SLOT(myReadStderrSlot()));

#ifdef Q_OS_WIN
	proc->start("cmd");
#elif Q_OS_LINUX
	proc->start("bash");
#endif
}

TerminalWidget::~TerminalWidget(){
	// disconnect(): 解除信号槽重定向(解锁操作)
	disconnect(proc,SIGNAL(readyReadStandardOutput()),this,SLOT(myReadStdoutSlot()));
	// disconnect(): 解除信号槽重定向(解锁操作)
	disconnect(proc,SIGNAL(readyReadStandardError()),this,SLOT(myReadStderrSlot()));
}

void TerminalWidget::keyPressEvent(QKeyEvent *e)
{
	QTextCursor editCursor = textCursor();
	QString str;
	// Qt::Key_Enter 是小键盘(数字键盘)的 Enter, 对应的虚拟键码为: 0x01000005
	// Qt::Key_Return 是大键盘区的 Enter, 对应的虚拟键码为: 0x01000004
	if(e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter)
	{
		e->ignore();
		editCursor.setPosition(lastPosition, QTextCursor::MoveAnchor);
		editCursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
		str = editCursor.selectedText();
		editCursor.clearSelection();
#ifdef Q_OS_WIN
		lastInput =  str.toLocal8Bit() + '\r' + '\n';
#elif Q_OS_LINUX
		lastInput =  str.toLocal8Bit() + '\n';
#endif
		proc->write(lastInput);
		return;
	}
	// 点击黑屏
	else if(e->key() == Qt::Key_Backspace && editCursor.position() <= lastPosition)
		return;
	// 清除键记录
	else if(e->key() == Qt::Key_Delete && editCursor.position() <= lastPosition)
		return;
	else
	//递归调用?
		return QTextEdit::keyPressEvent(e);
}

void TerminalWidget::myReadStdoutSlot()
{
	QByteArray ba = proc->readAllStandardOutput();
	QTextCodec * textCodec = QTextCodec::codecForName("System");
	//assert(textCodec != nullptr);
	QString output = textCodec->toUnicode(ba);

	if (output.length() > 0 && output != QString::fromLocal8Bit(lastInput))
	{
		setTextColor(Qt::white);
		append(output.trimmed());
		moveCursor(QTextCursor::End);
		lastPosition = textCursor().position();
	}
}

void TerminalWidget::myReadStderrSlot()
{
	QByteArray ba = proc->readAllStandardError();
	QTextCodec* textCodec = QTextCodec::codecForName("System");
	//assert(textCodec != nullptr);
	QString output = textCodec->toUnicode(ba);

	if (output.length() > 0 && output != QString::fromLocal8Bit(lastInput))
	{
		setTextColor(Qt::red);
		append(output.trimmed());
		moveCursor(QTextCursor::End);
		lastPosition = textCursor().position();
	}
}



TerminalWidgetEx::TerminalWidgetEx(int x, int y)
{
	QTextCursor editCursor = textCursor();
	QTextBlockFormat textBlockFormat;

	// 初始化窗口大小
	window_x = x;
	window_y = y;
	resize(window_x,window_y);

	// 初始化窗口背景颜色和内容颜色
	setStyleSheet("background-color:rgb(0,0,0); color:rgb(255,255,255); border:20px;");
	textBlockFormat.setLineHeight(20, QTextBlockFormat::FixedHeight);
	editCursor.setBlockFormat(textBlockFormat);
	setTextCursor(editCursor);

	font.setFamily(font_style);				//设置字体样式
	font.setPixelSize(font_size);			//设置字体大小
	setFont(font);										//设置字体数据载体

	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	setWindowTitle("qt terminal title");

	//创建一个新进程, 运行terminal
	proc = new QProcess();
	// connect(): 将stdout, 重定向到QTextBrowser 容器的信号槽里面(锁死操作)
	connect(proc,SIGNAL(readyReadStandardOutput()),this,SLOT(myReadStdoutSlot()));
	// connect(): 将stderr, 重定向到QTextBrowser 容器的信号槽里面(锁死操作)
	connect(proc,SIGNAL(readyReadStandardError()),this,SLOT(myReadStderrSlot()));

#ifdef Q_OS_WIN
	proc->start("cmd");
#elif Q_OS_LINUX
	proc->start("bash");
#endif
}

TerminalWidgetEx::~TerminalWidgetEx(){
	// disconnect(): 解除信号槽重定向(解锁操作)
	disconnect(proc,SIGNAL(readyReadStandardOutput()),this,SLOT(myReadStdoutSlot()));
	// disconnect(): 解除信号槽重定向(解锁操作)
	disconnect(proc,SIGNAL(readyReadStandardError()),this,SLOT(myReadStderrSlot()));
}

void TerminalWidgetEx::myReadStdoutSlot()
{
	QByteArray ba = proc->readAllStandardOutput();
	QTextCodec * textCodec = QTextCodec::codecForName("System");
	//assert(textCodec != nullptr);
	QString output = textCodec->toUnicode(ba);

	if (output.length() > 0 && output != QString::fromLocal8Bit(lastInput))
	{
		setTextColor(color_stdout);
		append(output.trimmed());
		moveCursor(QTextCursor::End);
		lastPosition = textCursor().position();
	}
}

void TerminalWidgetEx::myReadStderrSlot()
{
	QByteArray ba = proc->readAllStandardError();
	QTextCodec* textCodec = QTextCodec::codecForName("System");
	//assert(textCodec != nullptr);
	QString output = textCodec->toUnicode(ba);

	if (output.length() > 0 && output != QString::fromLocal8Bit(lastInput))
	{
		setTextColor(color_stderr);
		append(output.trimmed());
		moveCursor(QTextCursor::End);
		lastPosition = textCursor().position();
	}
}
