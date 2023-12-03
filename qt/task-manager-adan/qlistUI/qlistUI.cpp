#include "qlistUI.h"
#include "ui_qlistUI.h"
#include <QVBoxLayout>
#include <QTextCodec>
#include <QComboBox>
#include <QTextBlock>
#include <QTextCodec>
#include <QTextCursor>
#include <QDebug>

//qt 控件变量new/delete 内存管理声明:
/*
	qt 控件, 一旦添加到QVBoxLayout 布局中, 程序退出时, 会自动释放内存,
	所以不需要担心内存泄漏? 析构函数delete ui; 会自动释放所有资源;
*/

qlistUI::qlistUI(QList<item_t> *p, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::qlistUI){

	ui->setupUi(this);
	row_cur = 0;
	row_running = 0;
	col_max = 5;
	col_pid = 4;
	textBrowser_count = 0;
	row_locked = 0;
	items_info = p;

	//创建1 个QTableWidget 控件, 初始化为: 0行5列
	tableWidget = new QTableWidget(0, col_max);
	tableWidget->setHorizontalHeaderLabels(QStringList() << tr("exe path") << tr("hide") << tr("logging") << tr("status") << tr("pid"));
	//qlist_items = tableWidget->selectedItems();			//获取qlist 的items list(方便后续批量操作)
	tableWidget->setColumnWidth(0, 512);							//手动修改第一列的宽度

	//创建3 个QDialogButtonBox 控件
	buttonBox = new QDialogButtonBox(Qt::Horizontal);
	addRowButton = buttonBox->addButton(tr("&add exe info row"), QDialogButtonBox::ActionRole);
	delRowButton = buttonBox->addButton(tr("&del exe info row"), QDialogButtonBox::ActionRole);
	lockButton = buttonBox->addButton(tr("&lock button and start all process"), QDialogButtonBox::ActionRole);
	unlockButton = buttonBox->addButton(tr("&unlock button and stop all process"), QDialogButtonBox::ActionRole);
	killButton = buttonBox->addButton(tr("&del current item && kill process"), QDialogButtonBox::ActionRole);
	buttonBox->addButton(QDialogButtonBox::Ok);
	buttonBox->addButton(QDialogButtonBox::Cancel);

	//创建1 个QLabel 控件
	label = new QLabel("all stderr:");

	//创建1 个QTextBrowser 控件
	textBrowser = new QTextBrowser();
	textBrowser->append("usage: ");
	textBrowser->append("    'add exe info row' first, push 'lock button and start process' button to lock all info and start run exe process");
	textBrowser->append("    stdout and stderr print to log file, if you make a choose to logging for this exe process");
	textBrowser->append("    empty value default reset to 0, 'exe path' can not be empty");
	textBrowser->append("    pid can not be input, any value input will be ignore, display only");
	textBrowser->append("    if you want to kill one process, please input the first-column number(not the pid number)");
	textBrowser->append("    stderr also print to here [default]");
	textBrowser->append("author: ");									//添加text to QTextBrowser控件[换行]
	textBrowser->insertPlainText("adan_shaw@qq.com");	//添加text to QTextBrowser控件[不换行]
	textBrowser->setStyleSheet("background-color:rgb(0,0,0); color:rgb(255,255,255); border:20px;");
	font.setFamily(font_style);
	font.setPixelSize(font_size);
	textBrowser->setFont(font);

	//绑定信号槽
	connect(addRowButton, SIGNAL(clicked()), this, SLOT(addRow()));
	connect(delRowButton, SIGNAL(clicked()), this, SLOT(delRow()));
	connect(lockButton, SIGNAL(clicked()), this, SLOT(lockRow()));
	connect(unlockButton, SIGNAL(clicked()), this, SLOT(unlockRow()));
	connect(killButton, SIGNAL(clicked()), this, SLOT(delCurItemKillProc()));
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

	//创建1 个QVBoxLayout 控件, 用作重新布局(底纸)
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(tableWidget);								//添加tableWidget 到布局中
	mainLayout->addWidget(buttonBox);									//添加buttonBox 到布局中
	mainLayout->addWidget(label);											//添加label 到布局中
	mainLayout->addWidget(textBrowser);								//添加textBrowser 到布局中
	setLayout(mainLayout);

	//设置标题
	setWindowTitle(tr("qlistUI for terminal control"));
	tableWidget->setEnabled(true);
	addRowButton->setEnabled(true);
	delRowButton->setEnabled(true);
	lockButton->setEnabled(true);
	unlockButton->setEnabled(false);
	killButton->setEnabled(false);
}

qlistUI::~qlistUI(){
	if(row_locked)
		clean_items_info();
	delete ui;
}

void qlistUI::set_main_pid(long long pid){
	mainPid = pid;
}

long long qlistUI::get_main_pid(){
	return mainPid;
}

void qlistUI::update_main_pid(){
	label->setText("[main() process pid: " + QString::number(mainPid,'g',17) + "], all stderr: ");
}

const char *killStr = "kill -s 9 ";

void qlistUI::clean_items_info(void){
	char str[64];
	for(int row=0; row < row_running; row++){
		//items_info->first().proc->kill();						//startDetached() QProcess 进程, 直接kill() 不会有任何作用(用task-manager-adan 打开task-manager-adan 即可测试)
		//items_info->first().proc->waitForFinished();//阻塞等待杀死完成
		//使用pid 大法杀死程序:
		snprintf(str, sizeof(str) - 1, "%s %ld", killStr, items_info->first().pid);
		system(str);
		//textBrowser->append(QString("*%1* - clean_items_info(): %2 ").arg(textBrowser_count++).arg(str));//debug only
		items_info->removeFirst();
		tableWidget->item(row,3)->setText("");				//status 显示复原
		tableWidget->item(row,col_pid)->setText("");	//pid 显示复原
	}
	row_locked = 0;
	row_running = 0;
}

void qlistUI::kill_item_process(int row){
	char str[64];
	//items_info->at(row).proc->kill();
	//items_info->at(row).proc->waitForFinished();
	snprintf(str, sizeof(str) - 1, "%s %ld", killStr, items_info->at(row).pid);
	system(str);
	//textBrowser->append(QString("*%1* - kill_item_process(): %2 ").arg(textBrowser_count++).arg(str));//debug only
	items_info->removeAt(row);
	row_running-=1;
}



//done的作用是: 当用户按下OK/cancel/x 按钮后, 执行的动作
void qlistUI::done(int result){
	//done() 接收到Accepted 信号
	if(result == QDialog::Accepted){
		textBrowser->append(QString("*%1* - done(if(QDialog::Accepted == result)): adan shaw is the most handsome person in the world 0.0").arg(textBrowser_count++));
		return;									//拒绝退出对话框
	}

	//done() 接收到Rejected 信号(cancel/x 按钮, 都属于Rejected 信号)
	if(result == QDialog::Rejected){
		textBrowser->append("QDialog::Rejected: botton hit");
		QDialog::done(result);	//退出对话框(qt默认操作)
		return;
	}

	qDebug() << "QDialog::done(): unexpected window signal!!";//qt 即便退出, 也会在debug terminal 中打印debug info
	QDialog::done(result);		//意外操作: 退出对话框
}

//信号槽函数
void qlistUI::addRow(){
	int n;
	QTableWidgetItem *item = NULL;
	QComboBox *pComboBox = NULL;

	//在row后面插入一行, 此时tableWidget为新增的一行
	tableWidget->insertRow(row_cur);

	//设置第n 列
	for(n=0; n < col_max; n++){
		switch(n){
			case 0:
			case 4:
			case 3:
				item = new QTableWidgetItem;
				item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
				if(n == 3 || n == 4){
					item->setText("0.0");
					item->setFlags(Qt::ItemIsEnabled);									//禁止item 编辑
					//item->setFlags(Qt::ItemIsEditable);								//恢复item 编辑
					//item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEnabled);//恢复item 编辑(方法2)
					item->setFlags(Qt::ItemIsSelectable);								//禁止item 被选中
				}
				tableWidget->setItem(row_cur, n, item);
				break;
			case 1:
			case 2:
				pComboBox = new QComboBox();//删除item 时, 会自动释放, 不用担心
				pComboBox->addItem("0");
				pComboBox->addItem("1");
				tableWidget->setCellWidget(row_cur, n, pComboBox);
				break;
			default:
				textBrowser->append(QString("*%1* - tableWidget->addRow(%2): switch(n){} error!! default: is not alloc!! ").arg(textBrowser_count++).arg(n));
				break;
		}
	}
	row_cur+=1;
	textBrowser->append(QString("*%1* - tableWidget->addRow(%2): add item").arg(textBrowser_count++).arg(row_cur));
}

void qlistUI::delRow(){
	int row = tableWidget->currentRow();
	if(row == -1)																								//不选中时, tableWidget->currentRow() = -1, 选中时, 可直接tableWidget->removeRow(tableWidget->currentRow());
		row = row_cur - 1;
	if(row_cur > 0){
		textBrowser->append(QString("*%1* - tableWidget->delRow(%2): item remove!!").arg(textBrowser_count++).arg(row+1));
		//textBrowser->append(QString("*%1* - tableWidget->delRow(%2/%3): item remove!!").arg(textBrowser_count++).arg(row).arg(row_cur));//test only
		row_cur-=1;
		tableWidget->removeRow(row);
	}
	else
		textBrowser->append(QString("*%1* - tableWidget->delRow(): empty table!!").arg(textBrowser_count++));
}

void qlistUI::lockRow(){
	int row;
	QString str;
	QByteArray tmp;
	item_t *p;
	QTableWidgetItem *item;
	QComboBox *pComboBox;
	QWidget *widget;
	QStringList strList;

	//tableWidget->setEnabled(false);
	addRowButton->setEnabled(false);
	delRowButton->setEnabled(false);
	lockButton->setEnabled(false);															//设置禁用按钮控件
	unlockButton->setEnabled(true);															//设置激活按钮控件
	killButton->setEnabled(true);
	textBrowser->append(QString("*%1* - tableWidget->lockRow(): lock all item and start all process").arg(textBrowser_count++));

	//遍历更新*items_info 数据容器(先清空, 再重新添加, 最简单暴躁的方式, 强制刷新所有数据!! 反正数据量不大)
	if(row_locked)
		clean_items_info();

	//items_info->size() == row_cur == 0
	for(row=0; row < row_cur; row++){
		str = tableWidget->item(row,0)->text();										//第一行为空, 则报错!!(崩溃点)
		if(!str.isEmpty()){
			p = new item_t();																				//创建新节点item_t 容器
			tmp = str.toLocal8Bit();
			strncpy(p->exe_path, tmp.data(), EXE_PATH_MAX);					//拷贝字符串
			//p->hide = tableWidget->item(row,1)->text().toBool();	//QString 没有toBool() 方法?
			//p->hide = tableWidget->item(row,1)->text().toInt();
			widget = tableWidget->cellWidget(row,1);
			pComboBox = dynamic_cast<QComboBox*>(widget);
			p->hide = pComboBox->currentText().toInt();
			if(p->hide != 0) p->hide = 1;														//一切非0 的unsigned int, 统一都视为true[空也视为0]
			widget = tableWidget->cellWidget(row,2);
			pComboBox = dynamic_cast<QComboBox*>(widget);
			p->logging = pComboBox->currentText().toInt();
			if(p->logging != 0) p->logging = 1;

			p->status = 1;
			tableWidget->item(row,3)->setText(QString::number(p->status,'g',17));						//修改status 所在的item 单元的值

			//开始all 程序(创建QProcess 进程, 将stderr 重定向到textBrowser, 如果启动了p->no_stdout, 则抛弃stdout; 否则根据p->logging 决定将stdout, stderr 打印到指定的地方)
			p->proc = new QProcess();
			p->proc->setProgram(p->exe_path);												//设置启动的exe 的path
			p->proc->setArguments(strList);													//设置启动的exe 的启动参数
			//p->proc->start(p->exe_path, strList);									//阻塞启动exe
			p->proc->startDetached(&p->pid);												//detached 式分离exe 启动, 并获取返回的pid
			connect(p->proc,SIGNAL(readyReadStandardError()),this,SLOT(myReadStderrSlot()));//标准错误重定向
			//p->pid = p->proc->processId();
			tableWidget->item(row,col_pid)->setText(QString::number(p->pid,'g',17));				//修改pid 所在的item 单元的值

			textBrowser->append(QString("*%1* - tableWidget->lockRow(%2): %3, %4, %5 statue=1, pid=%6").arg(textBrowser_count++).arg(row).arg(p->exe_path).arg(p->hide).arg(p->logging).arg(p->pid));
			textBrowser->append(QString("*%1* - tableWidget->lockRow(%2): QProcess started!!").arg(textBrowser_count++).arg(row));

			items_info->insert(row, *p);
			row_running+=1;
		}
		else{
			textBrowser->append(QString("*%1* - tableWidget->lockRow(%2): exe path can not be empty!!").arg(textBrowser_count++).arg(row));
			row_locked = 1;
			row_running = row;																			//强行为执行clean_items_info()伪造数据!! 不伪造数据, 会崩溃
			clean_items_info();
			return;																									//有空exe path, 则终止并清空退出
		}
	}

	if(row_running > 0)
		row_locked = 1;																						//当row_running > 0, 明确items_info 已经被占用
}

void qlistUI::unlockRow(){
	//tableWidget->clear();																			//清空显示(不能这样用, 这样会还原全部东西, 不值得)
	clean_items_info();																					//自动终止all 程序, 并清空资源
	tableWidget->setEnabled(true);
	addRowButton->setEnabled(true);
	delRowButton->setEnabled(true);
	lockButton->setEnabled(true);
	unlockButton->setEnabled(false);
	killButton->setEnabled(false);
	textBrowser->append(QString("*%1* - tableWidget->unlockRow(): unlock all item and stop all process").arg(textBrowser_count++));
}

void qlistUI::delCurItemKillProc(){
	int row = tableWidget->currentRow();
	if(row == -1)
		row = row_cur - 1;
	if(row_cur > 0){
		textBrowser->append(QString("*%1* - tableWidget->delCurItemKillProc(%2): item remove!!").arg(textBrowser_count++).arg(row+1));
		row_cur-=1;
		tableWidget->removeRow(row);
	}
	else
		textBrowser->append(QString("*%1* - tableWidget->delCurItemKillProc(): empty table!!").arg(textBrowser_count++));
	if(row_running > 0){
		kill_item_process(row);
		textBrowser->append(QString("*%1* - tableWidget->delCurItemKillProc(%2): QProcess killed!!").arg(textBrowser_count++).arg(row));
	}
}

void qlistUI::myReadStderrSlot()
{
	QString output;
	QByteArray ba;
	item_t *p;
#ifdef defined(Q_OS_WIN)
	QTextCodec *textCodec = QTextCodec::codecForName("GBK");
#elif defined(Q_OS_LINUX)
	QTextCodec *textCodec = QTextCodec::codecForName("UTF-8");
#else
	assert(false);
#endif
	for(int i=0; i < row_running; i++){
		*p = items_info->at(i);
		ba = p->proc->readAllStandardError();
		output = textCodec->toUnicode(ba);
		if(output.length() > 0){
			textBrowser->setTextColor(color_stderr);
			textBrowser->append(output.trimmed());
			textBrowser->append("");
			lastPosition = textBrowser->textCursor().position();
			textBrowser->setTextColor(color_stdout);
		}
		else {
			textBrowser->append("");
			lastPosition = textBrowser->textCursor().position();
		}
	}
}
