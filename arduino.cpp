#include "arduino.h"

Arduino::Arduino(QObject *parent) : QObject(parent)
{
    arduinoIsAvailable = false;
    arduinoPortName = "";
    arduino = new QSerialPort(this);

    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {
        if (serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier())
        {
            if (serialPortInfo.vendorIdentifier() == arduinoUnoVendorId)
            {
                if (serialPortInfo.productIdentifier() == arduinoUnoProductId)
                {
                    arduinoPortName = serialPortInfo.portName();
                    arduinoIsAvailable = true;
                }
            }
        }
    }

    if (arduinoIsAvailable)
    {
        // open and configure the serialport
        arduino->setPortName(arduinoPortName);
        arduino->open(QSerialPort::WriteOnly);
        arduino->setBaudRate(QSerialPort::Baud9600);
        arduino->setDataBits(QSerialPort::Data8);
        arduino->setParity(QSerialPort::NoParity);
        arduino->setStopBits(QSerialPort::OneStop);
        arduino->setFlowControl(QSerialPort::NoFlowControl);

        QMessageBox* success = new QMessageBox(QMessageBox::Information, "Arduino", "Connected to the Arduino !", QMessageBox::Ok);
        success->show();
    }
    else
    {
        // give error message if not available
        QMessageBox* warn = new QMessageBox(QMessageBox::Warning, "Arduino", "Couldn't find the Arduino !", QMessageBox::Ok);
        warn->show();
    }
}

Arduino::~Arduino()
{
    if (arduino->isOpen())
    {
        arduino->close();
    }
}

void Arduino::rawSend(QString message)
{
    if (arduinoIsAvailable && arduino->isWritable())
    {
        arduino->write(message.toStdString().c_str());
    }
    else
    {
        QMessageBox* err = new QMessageBox(QMessageBox::Critical, "Arduino", "Couldn't write on the serial", QMessageBox::Ok);
        err->show();
    }
}

bool Arduino::isAvailable()
{
    return arduinoIsAvailable;
}
