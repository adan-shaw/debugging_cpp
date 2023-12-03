#include "terminalwidget.h"

TerminalWidget::TerminalWidget(int x, int y)
{
	QStringList strList;
	long long pid;
	QTextCursor editCursor = textCursor();
	QTextBlockFormat textBlockFormat;

	sw_color = false;

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

	setWindowTitle("qt terminal[QTextEdit], input/output both");

	proc = new QProcess();						//创建一个新进程, 运行terminal [QProcess 子进程独立运行, 独享stdout, stderr, 但会重qt信号槽定向到main 主窗口, 信号槽支持跨进程通信;]
	#ifdef defined(Q_OS_WIN)
		proc->setProgram("cmd");				//设置启动的exe 的path
	#elif defined(Q_OS_LINUX)
		proc->setProgram("/usr/bin/bash");
	#else
		assert(false);
	#endif
	proc->setArguments(strList);			//设置启动参数
	// 将stdout, 重定向到QTextEdit 容器的信号槽里面(锁死操作)
	connect(proc,SIGNAL(readyReadStandardOutput()),this,SLOT(myReadStdoutSlot()));
	// 将stderr, 重定向到QTextEdit 容器的信号槽里面(锁死操作)
	connect(proc,SIGNAL(readyReadStandardError()),this,SLOT(myReadStderrSlot()));
	proc->start();										//阻塞启动 [阻塞, 直至QProcess 子进程结束, main 主进程100% 替换到子进程[包括stdout/stdin/stderr], 相当于替换式exec()]
	//proc->startDetached(&pid);			//分离启动 [非阻塞, 不等QProcess 子进程结束, main 主进程和QProcess 子进程互相独立] -- 找不到设备, 不能用
}

TerminalWidget::~TerminalWidget(){
	// 解除信号槽重定向(解锁操作)
	disconnect(proc,SIGNAL(readyReadStandardOutput()),this,SLOT(myReadStdoutSlot()));
	// 解除信号槽重定向(解锁操作)
	disconnect(proc,SIGNAL(readyReadStandardError()),this,SLOT(myReadStderrSlot()));
	delete proc;
	proc = NULL;
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
#ifdef defined(Q_OS_WIN)
		lastInput = str.toLocal8Bit() + '\r' + '\n';
#elif defined(Q_OS_LINUX)
		lastInput = str.toLocal8Bit() + '\n';
#else
		assert(false);
#endif
		proc->write(lastInput);
		return;
	}
	else if(e->key() == Qt::Key_Backspace && editCursor.position() <= lastPosition)
		return;// 点击黑屏
	else if(e->key() == Qt::Key_Delete && editCursor.position() <= lastPosition)
		return;// 清除键记录
	else
		return QTextEdit::keyPressEvent(e);//递归调用?
}

void TerminalWidget::myReadStdoutSlot()
{
	QByteArray ba = proc->readAllStandardOutput();
#ifdef defined(Q_OS_WIN)
	QTextCodec *ptmp = QTextCodec::codecForName("GBK");//System = GBK = windows
#elif defined(Q_OS_LINUX)
	QTextCodec *ptmp = QTextCodec::codecForName("UTF-8");//默认 = UTF-8 = linux
#else
	assert(false);
#endif
	assert(ptmp != nullptr);
	QString output = ptmp->toUnicode(ba);
	//if (output.length() > 0 && output != QString::fromLocal8Bit(lastInput))
	if (output.length() > 0)
	{
		if(sw_color){
			setTextColor(Qt::white);
			sw_color = false;
		}
		append(output.trimmed());				//打印stdout 的内容
		append("");											//打印完毕, 换行
		//moveCursor(QTextCursor::End);	//拒绝pos 移动(直接换行更方便)
		lastPosition = textCursor().position();
	}
	else{
		append("");											//输入空数据, 打印空行
		lastPosition = textCursor().position();
	}
}

void TerminalWidget::myReadStderrSlot()
{
	QByteArray ba = proc->readAllStandardError();
#ifdef defined(Q_OS_WIN)
	QTextCodec *ptmp = QTextCodec::codecForName("GBK");
#elif defined(Q_OS_LINUX)
	QTextCodec *ptmp = QTextCodec::codecForName("UTF-8");
#else
	assert(false);
#endif
	assert(ptmp != nullptr);
	QString output = ptmp->toUnicode(ba);
	//if (output.length() > 0 && output != QString::fromLocal8Bit(lastInput))
	if (output.length() > 0)
	{
		if(!sw_color){
			setTextColor(Qt::red);
			sw_color = true;
		}
		append(output.trimmed());
		append("");
		//moveCursor(QTextCursor::End);
		lastPosition = textCursor().position();
	}
	else{
		append("");
		lastPosition = textCursor().position();
	}
}



TerminalWidgetEx::TerminalWidgetEx(int x, int y)
{
	QStringList strList;
	long long pid;
	QTextCursor editCursor = textCursor();
	QTextBlockFormat textBlockFormat;

	sw_color = false;

	window_x = x;
	window_y = y;
	resize(window_x,window_y);

	setStyleSheet("background-color:rgb(0,0,0); color:rgb(255,255,255); border:20px;");
	textBlockFormat.setLineHeight(20, QTextBlockFormat::FixedHeight);
	editCursor.setBlockFormat(textBlockFormat);
	setTextCursor(editCursor);

	font.setFamily(font_style);
	font.setPixelSize(font_size);
	setFont(font);

	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	setWindowTitle("qt terminal[QTextBrowser], display only");

	proc = new QProcess();
#ifdef defined(Q_OS_WIN)
	proc->setProgram("cmd");
#elif defined(Q_OS_LINUX)
	proc->setProgram("/usr/bin/bash");
#else
	assert(false);
#endif
	proc->setArguments(strList);
	connect(proc,SIGNAL(readyReadStandardOutput()),this,SLOT(myReadStdoutSlot()));
	connect(proc,SIGNAL(readyReadStandardError()),this,SLOT(myReadStderrSlot()));
	proc->start();
	//proc->startDetached(&pid);
}

TerminalWidgetEx::~TerminalWidgetEx(){
	disconnect(proc,SIGNAL(readyReadStandardOutput()),this,SLOT(myReadStdoutSlot()));
	disconnect(proc,SIGNAL(readyReadStandardError()),this,SLOT(myReadStderrSlot()));
	delete proc;
	proc = NULL;
}

void TerminalWidgetEx::myReadStdoutSlot()
{
	QByteArray ba = proc->readAllStandardOutput();
#ifdef defined(Q_OS_WIN)
	QTextCodec *ptmp = QTextCodec::codecForName("GBK");
#elif defined(Q_OS_LINUX)
	QTextCodec *ptmp = QTextCodec::codecForName("UTF-8");
#else
	assert(false);
#endif
	assert(ptmp != nullptr);
	QString output = ptmp->toUnicode(ba);
	if(sw_color){
		setTextColor(color_stderr);
		sw_color = false;
	}
	append(output.trimmed());
	append("");
	//moveCursor(QTextCursor::End);
	lastPosition = textCursor().position();
}

void TerminalWidgetEx::myReadStderrSlot()
{
	QByteArray ba = proc->readAllStandardError();
#ifdef defined(Q_OS_WIN)
	QTextCodec *ptmp = QTextCodec::codecForName("GBK");
#elif defined(Q_OS_LINUX)
	QTextCodec *ptmp = QTextCodec::codecForName("UTF-8");
#else
	assert(false);
#endif
	assert(ptmp != nullptr);
	QString output = ptmp->toUnicode(ba);
	if(!sw_color){
		setTextColor(color_stderr);
		sw_color = true;
	}
	append(output.trimmed());
	append("");
	//moveCursor(QTextCursor::End);
	lastPosition = textCursor().position();
}
