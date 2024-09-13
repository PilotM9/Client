#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QLabel>
#include <QTimer>
#include <QPushButton>
#include <QLineEdit>
#include <QJsonObject>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_addButton_clicked();
    void on_sendButton_clicked();
    void on_readyRead();
    void updateConnectionStatus();
    void on_pushButton_clicked();
    void onSocketError(QAbstractSocket::SocketError socketError);  // Слот для обработки ошибок сокета
    void onResponseTimeout();  // Слот для обработки тайм-аута ответа от сервера

private:
    void setupConnections();
    void sendRequest(const QString &configuration, const QString &priority, const QHostAddress &address, quint16 port);
    void sendJsonRpcRequest(const QJsonObject &request, const QHostAddress &address, quint16 port);
    void processResponse(const QJsonObject &response);
    void sendNextRequest();  // Исправлено: добавлены круглые скобки для метода

    Ui::MainWindow *ui;
    QUdpSocket *socket;
    QLabel *statusServer;
    QTimer *statusTimer;
    QTimer *timeoutTimer;  // Объект таймера для отслеживания времени ожидания ответа
    QPushButton *pushButton;
    QLineEdit *textEdit_4;
    QList<QJsonObject> requests;
    QList<QJsonObject> requestsQueue;  // Очередь для последовательной обработки заявок
};

#endif // MAINWINDOW_H
