#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QLabel>
#include <QTimer>
#include <QRadioButton>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_addButton_clicked();
    void on_sendButton_clicked();
    void on_readyRead();
    void on_timeOut();
    void on_busyRadioButton_toggled(bool checked);

    void onServerConnected();
    void onServerDisconnected();
    void onServerError(QAbstractSocket::SocketError socketError);

    void onTimeServerConnected();
    void onTimeServerDisconnected();
    void onTimeServerError(QAbstractSocket::SocketError socketError);

private:
    void setupConnections();
    void connectToServers();
    void sendStartRequest();
    void sendRequest(const QString &request);
    void updateConnectionStatus();

    Ui::MainWindow *ui;
    QTcpSocket *socket;
    QTcpSocket *timeSocket;
    QLabel *statusTimeServer;
    QLabel *statusMainServer;
    QTimer *statusTimer;
    QRadioButton *busyRadioButton;
    QStringList requests; // Хранение очереди заявок
};

#endif // MAINWINDOW_H
