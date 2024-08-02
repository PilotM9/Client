#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QVector>

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

private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    QVector<QString> requests;

    void setupConnections();
    void sendRequest(const QString &request);
};

#endif // MAINWINDOW_H
