#include "client.h"

Client::Client(QObject *parent, QString host, int port) : QObject(parent)
{
    socket = new QTcpSocket(this);
    blockSize = 0;

    connect(socket, &QTcpSocket::readyRead, this, &Client::readMessage);
    // Can't write this with new styel
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
    connect(socket, &QTcpSocket::connected, this, &Client::connected);
    (socket)->connectToHost(host, port);
}

void Client::readMessage()
{
    QDataStream in(socket);

    if (blockSize == 0)
    {
        if (socket->bytesAvailable() < (int) sizeof(quint16))
        {
            return;
        }

        in >> blockSize;
    }

    if (socket->bytesAvailable() < blockSize)
    {
        return;
    }

    QString message;
    in >> message;
    blockSize = 0;

    qDebug() << "client got" << message;

    if (message == "ready")
    {
        emit ready();
    }
    else if (message == "start")
    {
        emit start();
    }
    else if (message == "ping")
    {
        emit ping();
    }
}

void Client::displayError(QAbstractSocket::SocketError socketError)
{
    QMessageBox* err = new QMessageBox(QMessageBox::Critical, "Client", "Unable to conenct to host : "+ socket->errorString(), QMessageBox::Ok);

    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        err = NULL;
        break;
    case QAbstractSocket::HostNotFoundError:
        err = new QMessageBox(QMessageBox::Warning, "Client", "Host not found", QMessageBox::Ok);
        break;
    case QAbstractSocket::ConnectionRefusedError:
        err = new QMessageBox(QMessageBox::Critical, "Client", "Connection refused", QMessageBox::Ok);
        break;
    default:
        break;
    }

    if (err != NULL)
    {
        err->show();
    }
}

void Client::connected()
{
    rawSend("ping");
}

void Client::rawSend(QString message)
{
    QByteArray packet;
    QDataStream out(&packet, QIODevice::WriteOnly);

    qDebug() << "client sends" << message;

    out << (quint16) 0;
    out << message;
    out.device()->seek(0);
    out << (quint16) (packet.size() - sizeof(quint16));

    socket->write(packet);
}
