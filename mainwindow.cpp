#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , timeSocket(new QTcpSocket(this))
    , socket(new QTcpSocket(this))
    , statusTimeServer(new QLabel(this))
    , statusMainServer(new QLabel(this))
    , statusTimer(new QTimer(this))
    , busyRadioButton(new QRadioButton("Imitation", this))
{
    ui->setupUi(this);

    // Настройка соединений и подключение
    setupConnections();
    connectToServers();

    // Инициализация QLabel и установка начальных свойств
    statusTimeServer->setGeometry(440, 40, 20, 20);
    statusTimeServer->setStyleSheet("background-color: red;");
    statusTimeServer->setToolTip("Time Server Connection Status");

    statusMainServer->setGeometry(560, 40, 20, 20);
    statusMainServer->setStyleSheet("background-color: red;");
    statusMainServer->setToolTip("Main Server Connection Status");

    // Настройка таймера для обновления состояния
    statusTimer->setInterval(500); // Обновление каждые 500 миллисекунд
    connect(statusTimer, &QTimer::timeout, this, &MainWindow::updateConnectionStatus);
    statusTimer->start();

    // Настройка радиокнопки
    busyRadioButton->setGeometry(440, 80, 100, 20);
    connect(busyRadioButton, &QRadioButton::toggled, this, &MainWindow::on_busyRadioButton_toggled);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupConnections()
{
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::on_readyRead);
    connect(socket, &QTcpSocket::errorOccurred, this, &MainWindow::onServerError);
    connect(socket, &QTcpSocket::connected, this, &MainWindow::onServerConnected);
    connect(socket, &QTcpSocket::disconnected, this, &MainWindow::onServerDisconnected);

    connect(timeSocket, &QTcpSocket::readyRead, this, &MainWindow::on_timeOut);
    connect(timeSocket, &QTcpSocket::errorOccurred, this, &MainWindow::onTimeServerError);
    connect(timeSocket, &QTcpSocket::connected, this, &MainWindow::onTimeServerConnected);
    connect(timeSocket, &QTcpSocket::disconnected, this, &MainWindow::onTimeServerDisconnected);
}

void MainWindow::connectToServers()
{
    if (socket->state() != QTcpSocket::ConnectedState) {
        socket->connectToHost("localhost", 1234); // Основной сервер
    }

    if (timeSocket->state() != QTcpSocket::ConnectedState) {
        timeSocket->connectToHost("localhost", 1235); // Сервер времени
    }
}

void MainWindow::on_addButton_clicked()
{
    QString configuration = ui->configComboBox->currentText();
    QString priority = ui->priorityComboBox->currentText();
    QString id = QString::number(QDateTime::currentMSecsSinceEpoch());
    QString request = QString("%1;%2;%3").arg(id).arg(configuration).arg(priority);
    ui->textEdit->append("ID: " + request);
    ui->textEdit_2->append("Added request: " + request);
    requests.append(request);
}

void MainWindow::on_sendButton_clicked()
{
    if (requests.isEmpty()) {
        ui->textEdit->append("No requests to send");
        return;
    }

    sendStartRequest();
}

void MainWindow::sendStartRequest()
{
    if (timeSocket->state() == QTcpSocket::ConnectedState) {
        timeSocket->write("START");
        timeSocket->flush();
        ui->textEdit->append("Sent START request to time server");
    } else {
        ui->textEdit->append("Error: Not connected to time server");
    }
}

void MainWindow::sendRequest(const QString &request)
{
    if (socket->state() == QTcpSocket::ConnectedState) {
        socket->write(request.toUtf8());
        socket->flush();
        ui->textEdit->append("Sent request: " + request);
    } else {
        ui->textEdit->append("Error: Not connected to server");
    }
}

void MainWindow::on_readyRead()
{
    QByteArray response = socket->readAll();
    ui->textEdit->append("Server response: " + QString(response));
    if (response.contains("Accepted")) {
        ui->textEdit_3->setTextColor(Qt::green);
    } else {
        ui->textEdit_3->setTextColor(Qt::red);
    }
    ui->textEdit_3->append(QString(response));
}

void MainWindow::on_timeOut()
{
    QByteArray response = timeSocket->readAll();
    qDebug() << "Time server response:" << response;
    if (response == "TICK") {
        if (!requests.isEmpty()) {
            sendRequest(requests.takeFirst());
        } else {
            timeSocket->write("NO_MORE_REQUESTS");
            timeSocket->flush();
            ui->textEdit->append("No more requests, sent NO_MORE_REQUESTS to time server");
        }
    }
}

void MainWindow::updateConnectionStatus()
{
    if (timeSocket->state() == QTcpSocket::ConnectedState) {
        statusTimeServer->setStyleSheet("background-color: green;");
    } else {
        statusTimeServer->setStyleSheet("background-color: red;");
    }

    if (socket->state() == QTcpSocket::ConnectedState) {
        statusMainServer->setStyleSheet("background-color: green;");
    } else {
        statusMainServer->setStyleSheet("background-color: red;");
    }
}

void MainWindow::on_busyRadioButton_toggled(bool checked)
{
    if (socket->state() == QTcpSocket::ConnectedState) {
        if (checked) {
            socket->write("BUSY");
        } else {
            socket->write("AVAILABLE");
        }
        socket->flush();
        ui->textEdit->append(checked ? "Server is now busy" : "Server is now available");
    }
}

void MainWindow::onServerConnected()
{
    ui->textEdit->append("Connected to main server");
}

void MainWindow::onServerDisconnected()
{
    ui->textEdit->append("Disconnected from main server");
}

void MainWindow::onServerError(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError);
    ui->textEdit->append("Main server error: " + socket->errorString());
    // Попытаться переподключиться, если сервер отключен
    if (socket->state() != QTcpSocket::ConnectedState) {
        connectToServers();
    }
}

void MainWindow::onTimeServerConnected()
{
    ui->textEdit->append("Connected to time server");
}

void MainWindow::onTimeServerDisconnected()
{
    ui->textEdit->append("Disconnected from time server");
}

void MainWindow::onTimeServerError(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError);
    ui->textEdit->append("Time server error: " + timeSocket->errorString());
    // Попытаться переподключиться, если сервер времени отключен
    if (timeSocket->state() != QTcpSocket::ConnectedState) {
        connectToServers();
    }
}
