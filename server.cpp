#include "server.h"

Server::Server(QObject *parent) : QObject(parent)
{
    server = new QTcpServer(this);
    if (!server->listen(QHostAddress::Any, 50185)) {
        QMessageBox* error = new QMessageBox(QMessageBox::Critical, "Server", "Unable to start the server : " + server->errorString(), QMessageBox::Ok);
        error->show();
        return;
    }

    parentW = (MainWindow*) parent;

    QString addr = getLocalIP();
    QString port = QVariant(server->serverPort()).toString();
    QMessageBox* success = new QMessageBox(QMessageBox::Information, "Server", "Server listening on " + addr + ":" + port, QMessageBox::Ok);
    success->show();

    blockSize = 0;

    connect(server, &QTcpServer::newConnection, this, &Server::sessionOpened);
}

void Server::sessionOpened()
{
    QTcpSocket* client = server->nextPendingConnection();
    clients << client;
    connect(client, &QTcpSocket::readyRead, this, &Server::broadcastMessage);
    connect(client, &QTcpSocket::disconnected, this, &Server::clientDisconnected);
}

void Server::broadcastMessage()
{
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if (socket == 0)
    {
        return;
    }

    QDataStream in(socket);

    if (blockSize == 0)
    {
        if (socket->bytesAvailable() < (int)sizeof(quint16))
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

    parentW->log("server got" + message);

    if (message == "ready")
    {
        emit ready();
    }
    else if (message == "ping")
    {
        emit ping();
        rawSend(message);
    }
    else if (message == "start")
    {
        emit start();
    }
    else if (message.startsWith("speed-"))
    {
        int value = QVariant(message.mid(6, 3)).toInt();
        emit clientSpeed(value);
    }

    blockSize = 0;
}

void Server::clientDisconnected()
{
    QTcpSocket* socket = qobject_cast<  QTcpSocket*>(sender());
    if (socket == 0)
    {
        return;
    }


    clients.removeOne(socket);
    socket->deleteLater();
}

void Server::rawSend(QString message)
{
    QByteArray packet;
    QDataStream out(&packet, QIODevice::WriteOnly);

    out << (quint16) 0;
    out << message;
    out.device()->seek(0);
    out << (quint16) (packet.size() - sizeof(quint16));

    for(int i = 0; i < clients.size(); ++i)
    {
        clients[i]->write(packet);
    }
}

QString Server::getLocalIP()
{
    foreach (const QHostAddress &address, QNetworkInterface::allAddresses())
    {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
        {
            return address.toString();
        }
    }

    return "";
}
