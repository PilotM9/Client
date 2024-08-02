#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    socket = new QTcpSocket(this);
    socket->connectToHost("localhost", 1234);

    setupConnections();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupConnections()
{
  //  connect(ui->addButton, &QPushButton::clicked, this, &MainWindow::on_addButton_clicked);
   // connect(ui->sendButton, &QPushButton::clicked, this, &MainWindow::on_sendButton_clicked);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::on_readyRead);
}

void MainWindow::on_addButton_clicked()
{
    QString configuration = ui->configComboBox->currentText();
    QString priority = ui->priorityComboBox->currentText();
    QString request = QString("%1;%2").arg(configuration).arg(priority);
    ui->textEdit->append("Added request: " + request);
    requests.append(request);
}

void MainWindow::on_sendButton_clicked()
{
    for (const QString &request : requests) {
        sendRequest(request);
    }
    requests.clear();
    ui->textEdit->append("All requests sent");
}

void MainWindow::sendRequest(const QString &request)
{
    if (socket->state() == QTcpSocket::ConnectedState) {
        socket->write(request.toUtf8());
        socket->flush();  // Добавьте flush здесь
        ui->textEdit->append("Sent request: " + request);  // Отладочное сообщение
    } else {
        ui->textEdit->append("Error: Not connected to server");
    }
}

void MainWindow::on_readyRead()
{
    QByteArray response = socket->readAll();
    ui->textEdit->append("Server response: " + QString(response)+"\n");
}
