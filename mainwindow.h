#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCrypto/QtCrypto>
#include <QtCrypto/qca.h>
#include <QMainWindow>
#include <QtNetwork>
#include <QUdpSocket>
#include <QTcpSocket>
#include <QObject>
#include <QByteArray>
#include <QDebug>

namespace Ui {
    class MainWindow;
}

namespace Ui {
    class MainWindow;
}

class QTcpServer;
class QUdpChat;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void read();
    void on_connecting_clicked();
    void on_sending_clicked();
    void qca_encode();
    void qca_decode();

private:
    Ui::MainWindow *ui;
    QUdpSocket *udpChat;
    QTcpServer *tcpServer;
    int client_status;
    QString nickname;
    QString Sendmessage;
    QString Resievemessage;
    QMap<int,QTcpSocket *> SClients;
};
#endif // MAINWINDOW_H
