#ifndef qlistUI_H
#define qlistUI_H

#include <QDialog>
#include <QTableWidget>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QTextBrowser>
#include <QProcess>
#include <QLabel>

namespace Ui {
	class qlistUI;
}

//每行item 元素的数据结构描述
#define EXE_PATH_MAX (512)
typedef struct{
	char exe_path[EXE_PATH_MAX];
	unsigned int hide;
	unsigned int logging;
	unsigned int status;
	long long pid;
	QProcess* proc;
} item_t;

class qlistUI : public QDialog{
	Q_OBJECT

public:
	qlistUI(QList<item_t> *p, QWidget *parent = 0);
	~qlistUI();
	void done(int result);
	void set_main_pid(long long pid);
	long long get_main_pid();
	void update_main_pid();

private slots:
	void addRow();
	void delRow();
	void lockRow();
	void unlockRow();
	void delCurItemKillProc();
	void myReadStderrSlot();

private:
	QTableWidget *tableWidget;
	QDialogButtonBox *buttonBox;
	QPushButton *addRowButton;
	QPushButton *delRowButton;
	QPushButton *lockButton;
	QPushButton *unlockButton;
	QPushButton *killButton;
	QLabel* label;

	QTextBrowser *textBrowser;
	const QColor color_stdout = Qt::white;
	const QColor color_stderr = Qt::red;
	QFont font;									// 字体数据载体(必须为QFont 类型)
	const char* font_style = "Noto Sans Mono";
	const int font_size = 14;
	QByteArray lastInput;				// 上一次的输入内容
	long long lastPosition = 0;	// 光标位置
	//QList<QTableWidgetItem*> qlist_items;

	QList<item_t> *items_info;	//items_info = 进程info
	Ui::qlistUI *ui;
	unsigned int row_cur;				//可变(可增行), 只用作记录用户输入table-row
	unsigned int row_running;		//实际已启动的行, 一行一个进程(不支持!! 不知道动态新增/删除进程, 想动态新增额外的进程, 请自己启动terminal)
	unsigned int col_max;				//不变(固定列)
	unsigned int col_pid;				//pid 显示专用列
	unsigned int textBrowser_count;
	bool row_locked;						//由于*items_info 操作是整进整出, 每次都会重写刷新整个QList, 这里用作判断lockButton 前是否需要清空数据(防止清空空QList 导致内存崩溃)
	long long mainPid;

	void clean_items_info(void);
	void kill_item_process(int row);
};

#endif // qlistUI_H
