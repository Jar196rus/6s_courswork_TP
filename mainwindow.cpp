#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    client_status=0;
}

void MainWindow::read() {
    QByteArray datagram;
    datagram.resize(udpChat->pendingDatagramSize());
    QHostAddress *address = new QHostAddress();
    udpChat->readDatagram(datagram.data(), datagram.size(), address);

    QDataStream in(&datagram, QIODevice::ReadOnly);

    // где-то тут алгоритм дешифровки

    qint8 type = 0;
    in >> type;
    QString str;
    in >> str;
    ui->textinfo->append(str);
}

void MainWindow::on_connecting_clicked()
{
    client_status=1;
    nickname=ui->Nickbar->toPlainText();
    udpChat = new QUdpSocket(this);
    udpChat->bind(QHostAddress::Any, 33334);
    connect(udpChat, SIGNAL(readyRead()), SLOT(read()));
}

void MainWindow::on_sending_clicked()
{
    if(client_status==1) {
        QString str=(nickname + " : " + ui->messeger->toPlainText());
        ui->textinfo->append(str);
        //тут алгоритм шифрования
        QByteArray data;
        QDataStream out(&data, QIODevice::WriteOnly);
        out << qint64(0);
        out << str;
        out.device()->seek(qint64(0));
        out << qint64(data.size() - sizeof(qint64));
        udpChat->writeDatagram(data, QHostAddress::Broadcast, 33334);
    }

}
