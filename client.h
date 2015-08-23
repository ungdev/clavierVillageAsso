#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QMessageBox>

class Client : public QObject
{
    Q_OBJECT

public:
    explicit Client(QObject* parent, QString host, int port);
    void rawSend(QString message);

private slots:
    void readMessage();
    void displayError(QAbstractSocket::SocketError socketError);
    void connected();

signals:
    void ready();
    void start();
    void ping();

private:
    QTcpSocket* socket;
    quint16 blockSize;
};

#endif // CLIENT_H
