#ifndef ARDUINO_H
#define ARDUINO_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QtWidgets>

class Arduino : public QObject
{
    Q_OBJECT
public:
    explicit Arduino(QObject *parent = 0);
    ~Arduino();
    void rawSend(QString message);
    bool isAvailable();

private:
    QSerialPort *arduino;
    static const quint16 arduinoUnoVendorId = 9025;
    static const quint16 arduinoUnoProductId = 67;
    QString arduinoPortName;
    bool arduinoIsAvailable;
};

#endif // ARDUINO_H
