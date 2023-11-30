#include "mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent)
{
m_textBrowser = new QTextBrowser(this);

startServerButton = new QPushButton("Start Server", this);
connect(startServerButton, &QPushButton::clicked, this, &MainWindow::startServer);

stopServerButton = new QPushButton("Stop Server", this);
connect(stopServerButton, &QPushButton::clicked, this, &MainWindow::stopServer);

startClientButton = new QPushButton("Start Client", this);
connect(startClientButton, &QPushButton::clicked, this, &MainWindow::startClient);

stopClientButton = new QPushButton("Stop Client", this);
connect(stopClientButton, &QPushButton::clicked, this, &MainWindow::stopClient);

QVBoxLayout *layout = new QVBoxLayout;
layout->addWidget(m_textBrowser);
layout->addWidget(startServerButton);
layout->addWidget(stopServerButton);
layout->addWidget(startClientButton);
layout->addWidget(stopClientButton);

QWidget *centralWidget = new QWidget(this);
centralWidget->setLayout(layout);
setCentralWidget(centralWidget);

m_server = new QTcpServer(this);
connect(m_server, &QTcpServer::newConnection, this, &MainWindow::handleMessage);

m_client = new QTcpSocket(this);

// Set up initial states
stopServerButton->setEnabled(false);
startClientButton->setEnabled(false);
stopClientButton->setEnabled(false);
}

MainWindow::~MainWindow()
{
}

void MainWindow::startServer()
{
if (!m_server->listen(QHostAddress::Any, 12345)) {
printMessage("Failed to start server.");
return;
}

printMessage("Server started.");

// Update button states
startServerButton->setEnabled(false);
stopServerButton->setEnabled(true);
startClientButton->setEnabled(true);
}

void MainWindow::stopServer()
{
m_server->close();
printMessage("Server stopped.");

// Update button states
stopServerButton->setEnabled(false);
startClientButton->setEnabled(false);
stopClientButton->setEnabled(false);
}

void MainWindow::startClient()
{
m_client->connectToHost(QHostAddress::LocalHost, 12345);

if (!m_client->waitForConnected()) {
printMessage("Failed to connect to server.");
return;
}

printMessage("Client connected.");

// Update button states
startClientButton->setEnabled(false);
stopClientButton->setEnabled(true);
}

void MainWindow::stopClient()
{
m_client->disconnectFromHost();
printMessage("Client disconnected.");

// Update button states
stopClientButton->setEnabled(false);
startClientButton->setEnabled(true);
}

void MainWindow::handleMessage()
{
QTcpSocket *socket = m_server->nextPendingConnection();
QString message = socket->readAll();
printMessage("Received message: " + message);
}

void MainWindow::printMessage(const QString& message)
{
m_textBrowser->append(message);
}
