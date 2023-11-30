#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QPushButton>
#include <QVBoxLayout>
#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTextBrowser>


class MainWindow : public QMainWindow
{
Q_OBJECT

public:
MainWindow(QWidget *parent = nullptr);
~MainWindow();

private slots:
void startServer();
void stopServer();
void startClient();
void stopClient();
void handleMessage();

private:
void printMessage(const QString& message);

QTcpServer *m_server;
QTcpSocket *m_client;
QTextBrowser *m_textBrowser;
QPushButton *startServerButton;
QPushButton *stopServerButton;
QPushButton *startClientButton;
QPushButton *stopClientButton;
};

#endif // MAINWINDOW_H
