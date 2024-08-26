#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , socket(new QUdpSocket(this))
    , statusServer(new QLabel(this))
    , statusTimer(new QTimer(this))
    , busyRadioButton(new QRadioButton("Imitation", this))
    , pushButton(new QPushButton("Connect", this))
    , textEdit_4(new QLineEdit(this))
{
    ui->setupUi(this);

    statusServer->setGeometry(440, 40, 20, 20);
    statusServer->setStyleSheet("background-color: red;");
    statusServer->setToolTip("Server Connection Status");

    statusTimer->setInterval(500); // Обновление каждые 500 миллисекунд
    connect(statusTimer, &QTimer::timeout, this, &MainWindow::updateConnectionStatus);
    statusTimer->start();

    busyRadioButton->setGeometry(440, 80, 100, 20);
    connect(busyRadioButton, &QRadioButton::toggled, this, &MainWindow::on_busyRadioButton_toggled);

    pushButton->setGeometry(440, 120, 100, 30);
    connect(pushButton, &QPushButton::clicked, this, &MainWindow::on_pushButton_clicked);

    textEdit_4->setGeometry(440, 160, 100, 30);

    socket->bind(QHostAddress::LocalHost, 1234);

    // Настройка соединения сокета
    setupConnections();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupConnections()
{
    connect(socket, &QUdpSocket::readyRead, this, &MainWindow::on_readyRead);
}

void MainWindow::sendRequest(const QString &id, const QString &configuration, const QString &priority, const QHostAddress &address, quint16 port)
{
    QJsonObject jsonParams;
    jsonParams["id"] = id;
    jsonParams["configuration"] = configuration;
    jsonParams["priority"] = priority;

    QJsonObject jsonRequest;
    jsonRequest["jsonrpc"] = "2.0";
    jsonRequest["method"] = "processRequest"; // Убедитесь, что метод правильно указан
    jsonRequest["params"] = jsonParams;
    jsonRequest["id"] = id.toInt(); // Используем ID как числовой идентификатор

    sendJsonRpcRequest(jsonRequest, address, port);
}

void MainWindow::sendJsonRpcRequest(const QJsonObject &request, const QHostAddress &address, quint16 port)
{
    QJsonDocument jsonDoc(request);
    QByteArray datagram = jsonDoc.toJson();

    QString jsonString = jsonDoc.toJson(QJsonDocument::Indented);  // Преобразуем JSON в отформатированную строку
    qDebug() << "Request JSON:" << jsonString;  // Выводим отформатированный JSON

    // Отправляем датаграмму
    socket->writeDatagram(datagram, address, port);
    ui->textEdit->append("Sent request: " + QString(request["id"].toString()));
}

void MainWindow::processResponse(const QJsonObject &response)
{
    QString result = response["result"].toString();
    ui->textEdit->append("Server response: " + result);

    if (result.contains("Accepted")) {
        ui->textEdit_3->setTextColor(Qt::green);
    } else {
        ui->textEdit_3->setTextColor(Qt::red);
    }

    ui->textEdit_3->append(result);
}

void MainWindow::on_addButton_clicked()
{
    QString configuration = ui->configComboBox->currentText();
    QString priority = ui->priorityComboBox->currentText();
    QString id = QString::number(QDateTime::currentMSecsSinceEpoch());

    QJsonObject request;
    request["id"] = id;
    request["configuration"] = configuration;
    request["priority"] = priority;

    requests.append(request);
    ui->textEdit->append("ID: " + id);
    ui->textEdit_2->append("Added request: " + id);
}

void MainWindow::on_sendButton_clicked()
{
    if (requests.isEmpty()) {
        ui->textEdit->append("No requests to send");
        return;
    }

    QHostAddress serverAddress("localhost");
    quint16 serverPort = 1234;

    QJsonObject startRequest;
    startRequest["jsonrpc"] = "2.0";
    startRequest["method"] = "startProcessing";
    startRequest["id"] = 1; // Уникальный ID для команды START
    sendJsonRpcRequest(startRequest, serverAddress, serverPort);

    // Отправляем все запросы
    for (const QJsonObject &request : requests) {
        QJsonObject jsonRequest;
        jsonRequest["jsonrpc"] = "2.0";
        jsonRequest["method"] = "processRequest"; // Убедитесь, что метод правильно указан
        jsonRequest["params"] = request; // Включаем тело запроса как параметры
        jsonRequest["id"] = request["id"].toString(); // Используем ID как строковый идентификатор

        sendJsonRpcRequest(jsonRequest, serverAddress, serverPort);
    }
    requests.clear();
}

void MainWindow::on_readyRead()
{
    while (socket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(socket->pendingDatagramSize());
        QHostAddress senderAddress;
        quint16 senderPort;

        socket->readDatagram(datagram.data(), datagram.size(), &senderAddress, &senderPort);

        QJsonDocument jsonDoc = QJsonDocument::fromJson(datagram);
        if (jsonDoc.isNull() || !jsonDoc.isObject()) {
            ui->textEdit->append("Received invalid JSON");
            return;
        }

        QJsonObject jsonResponse = jsonDoc.object();
        processResponse(jsonResponse);
    }
}

void MainWindow::updateConnectionStatus()
{
    statusServer->setStyleSheet("background-color: green;");
}

void MainWindow::on_busyRadioButton_toggled(bool checked)
{
    QHostAddress serverAddress("localhost");
    quint16 serverPort = 1234;

    QJsonObject busyStatus;
    busyStatus["jsonrpc"] = "2.0";
    busyStatus["method"] = checked ? "setBusy" : "setAvailable";
    busyStatus["id"] = 1; // Уникальный ID для команды занятости

    sendJsonRpcRequest(busyStatus, serverAddress, serverPort);
    qDebug() << "Sent" << (checked ? "BUSY" : "AVAILABLE") << "to server";
}

void MainWindow::on_pushButton_clicked()
{
    bool ok;
    QString portText = textEdit_4->text();
    quint16 port = portText.toUInt(&ok);

    qDebug() << "Port text:" << portText;

    if (!ok || port < 1 || port > 65535) {
        ui->textEdit->append("Invalid port number. Please enter a number between 1 and 65535.");
        statusServer->setStyleSheet("background-color: red;");
        return;
    }

    socket->abort();  // Закрываем старое соединение
    if (!socket->bind(QHostAddress::LocalHost, port)) {
        ui->textEdit->append("Failed to bind to port " + QString::number(port));
        statusServer->setStyleSheet("background-color: red;");
    } else {
        ui->textEdit->append("Successfully bound to port " + QString::number(port));
        statusServer->setStyleSheet("background-color: green;");
    }
    textEdit_4->text()=0;
}

