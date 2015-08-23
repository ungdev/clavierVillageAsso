#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMessageBox>
#include <QNetworkInterface>
#include <QDebug>

class Server : public QObject
{
    Q_OBJECT

public:
    Server(QObject* parent = 0);
    void rawSend(QString message);

signals:
    void ready();
    void ping();

private slots:
    void clientDisconnected();
    void sessionOpened();
    void broadcastMessage();

private:
    QTcpServer* server;
    QList<QTcpSocket*> clients;
    quint16 blockSize;

    QString getLocalIP();
};

#endif // SERVER_H
