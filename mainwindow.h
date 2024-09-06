#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QLabel>
#include <QTimer>
#include <QRadioButton>
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
    // void on_busyRadioButton_toggled(bool checked);
    void on_pushButton_clicked();
    void onSocketError(QAbstractSocket::SocketError socketError);  // Новый слот для обработки ошибок сокета
    void onResponseTimeout();  // Новый слот для обработки тайм-аута ответа от сервера

private:
    void setupConnections();
    void sendRequest(const QString &configuration, const QString &priority, const QHostAddress &address, quint16 port);
    void sendJsonRpcRequest(const QJsonObject &request, const QHostAddress &address, quint16 port);
    void processResponse(const QJsonObject &response);

    Ui::MainWindow *ui;
    QUdpSocket *socket;
    QLabel *statusServer;
    QTimer *statusTimer;
    QTimer *timeoutTimer;  // Новый объект таймера для отслеживания времени ожидания ответа
    // QRadioButton *busyRadioButton;
    QPushButton *pushButton;
    QLineEdit *textEdit_4;
    QList<QJsonObject> requests;
};

#endif // MAINWINDOW_H
