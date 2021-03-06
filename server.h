#ifndef SERVER_H
#define SERVER_H

#include "mainwindow.h"
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMessageBox>
#include <QNetworkInterface>
//#include <QDebug>

class MainWindow;

class Server : public QObject
{
    Q_OBJECT

public:
    Server(QObject* parent = 0);
    void rawSend(QString message);

signals:
    void ready();
    void ping();
    void clientSpeed(int speed);
    void start();

private slots:
    void clientDisconnected();
    void sessionOpened();
    void broadcastMessage();

private:
    MainWindow* parentW;
    QTcpServer* server;
    QList<QTcpSocket*> clients;
    quint16 blockSize;

    QString getLocalIP();
};

#endif // SERVER_H
