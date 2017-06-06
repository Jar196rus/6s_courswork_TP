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
    in >> Sendmessage;
    qca_decode();
    ui->textinfo->append(Sendmessage);
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
        qca_encode();
        QByteArray data;
        QDataStream out(&data, QIODevice::WriteOnly);
        out << qint64(0);
        out << str;
        out.device()->seek(qint64(0));
        out << qint64(data.size() - sizeof(qint64));
        udpChat->writeDatagram(data, QHostAddress::Broadcast, 33334);
        ui->messeger->clear();
    }

}

void MainWindow::qca_encode()
{
    QCA::Initializer init;
    if (QCA::isSupported("des-ecb")) {
        QCA::SecureArray arg = QVariant(Resievemessage).toByteArray();
        QByteArray Arr = "F1F1F1F1F1F1F1F1";
        QCA::SymmetricKey key(Arr);
        QCA::InitializationVector iv(16);
        QCA::Cipher cipher(QString("des"), QCA::Cipher::ECB, QCA::Cipher::DefaultPadding, QCA::Encode, key, iv);
        QCA::SecureArray u = cipher.update(arg);
        QCA::SecureArray f = cipher.final();
        Resievemessage = QString(f.data());
    }

}


void MainWindow::qca_decode()
{
    QCA::Initializer init;
    if (QCA::isSupported("des-ecb")) {
        QCA::SecureArray arg = QVariant(Sendmessage).toByteArray();
        QByteArray Arr = "F1F1F1F1F1F1F1F1";
        QCA::SymmetricKey key(Arr);
        QCA::InitializationVector iv(16);
        QCA::Cipher cipher(QString("des"), QCA::Cipher::ECB, QCA::Cipher::DefaultPadding, QCA::Decode, key, iv);
        QCA::SecureArray u = cipher.update(arg);
        QCA::SecureArray f = cipher.final();
        Sendmessage = QString(f.data());
    }
}

