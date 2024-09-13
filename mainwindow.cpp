#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>
#include <QDateTime>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , socket(new QUdpSocket(this))
    , statusServer(new QLabel(this))
    , statusTimer(new QTimer(this))
    , pushButton(new QPushButton("Connect", this))
    , textEdit_4(new QLineEdit(this))
    , timeoutTimer(new QTimer(this))
{
    ui->setupUi(this);

    // Инициализация элементов интерфейса
    statusServer->setGeometry(440, 40, 20, 20);
    statusServer->setStyleSheet("background-color: red;");
    statusServer->setToolTip("Server Connection Status");

    statusTimer->setInterval(500); // Обновление каждые 500 миллисекунд
    connect(statusTimer, &QTimer::timeout, this, &MainWindow::updateConnectionStatus);
    statusTimer->start();

    pushButton->setGeometry(440, 120, 100, 30);
    connect(pushButton, &QPushButton::clicked, this, &MainWindow::on_pushButton_clicked);

    textEdit_4->setGeometry(440, 160, 100, 30);

    socket->bind(QHostAddress::LocalHost, 12345);

    // Настройка соединений
    setupConnections();

    // Таймер для отслеживания времени ожидания ответа от сервера
    timeoutTimer->setInterval(3000); // Тайм-аут 3 секунды
    connect(timeoutTimer, &QTimer::timeout, this, &MainWindow::onResponseTimeout);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupConnections()
{
    connect(socket, &QUdpSocket::readyRead, this, &MainWindow::on_readyRead);
    connect(socket, &QUdpSocket::errorOccurred, this, &MainWindow::onSocketError);
}

void MainWindow::sendRequest(const QString &configuration, const QString &priority, const QHostAddress &address, quint16 port)
{
    QJsonObject jsonParams;
    jsonParams["configuration"] = configuration;
    jsonParams["priority"] = priority;

    QJsonObject jsonRequest;
    jsonRequest["jsonrpc"] = "2.0";
    jsonRequest["method"] = "processRequest";
    jsonRequest["params"] = jsonParams;

    sendJsonRpcRequest(jsonRequest, address, port);
}

void MainWindow::sendJsonRpcRequest(const QJsonObject &request, const QHostAddress &address, quint16 port)
{
    QJsonDocument jsonDoc(request);
    QByteArray datagram = jsonDoc.toJson();

    QString jsonString = jsonDoc.toJson(QJsonDocument::Indented);  // Преобразуем JSON в отформатированную строку
    qDebug() << "Request JSON:" << jsonString;  // Выводим отформатированный JSON

    qint64 bytesSent = socket->writeDatagram(datagram, address, port);
    if (bytesSent == -1) {
        ui->textEdit->append("Error sending request: " + socket->errorString());
        return;
    }
    ui->textEdit->append("Sent request");

    // Запуск таймера ожидания ответа
    timeoutTimer->start();
}

void MainWindow::processResponse(const QJsonObject &response)
{
    QString result = response["result"].toString();
    QString id = response["id"].toString();
    QJsonObject requestBody = response["requestBody"].toObject();  // Извлекаем тело заявки из ответа

    // Извлекаем детали заявки для отображения
    QString configuration = requestBody["configuration"].toString();
    QString priority = requestBody["priority"].toString();
    QString requestDetails = QString("Request ID: %1 - Configuration: %2, Priority: %3").arg(id, configuration, priority);

    // Устанавливаем цвет текста в зависимости от результата
    if (result.contains("Accepted")) {
        ui->textEdit_3->setTextColor(Qt::green);
    } else {
        ui->textEdit_3->setTextColor(Qt::red);
    }

    // Отображаем результат и детали заявки в `textEdit_3`
    ui->textEdit_3->append(requestDetails);
    ui->textEdit_3->append("Server response: " + result);
}


void MainWindow::on_addButton_clicked()
{
    QString configuration = ui->configComboBox->currentText();
    QString priority = ui->priorityComboBox->currentText();

    // Создаем объект заявки
    QJsonObject request;
    request["configuration"] = configuration;
    request["priority"] = priority;

    // Добавляем заявку в список
    requests.append(request);

    // Обновляем текстовые поля
    ui->textEdit_2->append("Added request: Configuration: " + configuration + ", Priority: " + priority);
}

void MainWindow::on_sendButton_clicked()
{
    if (requests.isEmpty()) {
        ui->textEdit->append("No requests to send");
        return;
    }

    QHostAddress serverAddress("localhost");
    quint16 serverPort = 12345;

    // Отправляем команду startProcessing перед отправкой первой заявки
    QJsonObject startRequest;
    startRequest["jsonrpc"] = "2.0";
    startRequest["method"] = "startProcessing";
    startRequest["id"] = 1; // Уникальный ID для команды START
    sendJsonRpcRequest(startRequest, serverAddress, serverPort);

    // Копируем заявки в очередь для обработки по одной
    requestsQueue = requests;
    requests.clear();

    // Начинаем отправку первой заявки
    sendNextRequest();
}

void MainWindow::sendNextRequest()
{
    if (requestsQueue.isEmpty()) {
        ui->textEdit->append("All requests have been sent");
        return;
    }

    QHostAddress serverAddress("localhost");
    quint16 serverPort = 12345;

    // Берем следующую заявку из очереди
    QJsonObject request = requestsQueue.takeFirst();
    sendRequest(request["configuration"].toString(), request["priority"].toString(), serverAddress, serverPort);
}

void MainWindow::on_readyRead()
{
    // Останавливаем таймер, если получили ответ
    timeoutTimer->stop();

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

        // Получение и отображение текущего тика сервера
        if (jsonResponse.contains("currentTick")) {
            QString currentTick = jsonResponse["currentTick"].toString();
            ui->textEdit->append("Current Tick: " + currentTick);
        }

        // После обработки ответа отправляем следующую заявку, если они еще есть
        if (!requestsQueue.isEmpty()) {
            sendNextRequest();
        }
    }
}

void MainWindow::updateConnectionStatus()
{
    statusServer->setStyleSheet("background-color: green;");
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
    textEdit_4->clear();
}

void MainWindow::onSocketError(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError);
    ui->textEdit->append("Socket error occurred: " + socket->errorString());
}

void MainWindow::onResponseTimeout()
{
    ui->textEdit->append("No response from server within the timeout period.");
}
