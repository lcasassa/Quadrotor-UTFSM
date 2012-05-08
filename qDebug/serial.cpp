#include "serial.h"
#include <QDebug>

Serial::Serial(QString serial_port) : QextSerialPort("", QextSerialPort::EventDriven) {

    this->setPortName(serial_port);
    this->setBaudRate(BAUD115200);
    this->setFlowControl(FLOW_OFF);
    this->setParity(PAR_NONE);
    this->setDataBits(DATA_8);
    this->setStopBits(STOP_1);

/*    this->open(QIODevice::ReadWrite);
    if(port->isOpen()) {
        qWarning("Puerto serial %s abierto", serial_port.toAscii().data());
    } else {
        qWarning("No se pudo abrir puerto serial %s", serial_port.toAscii().data());
    }*/

    connect(this, SIGNAL(readyRead()), this, SLOT(receiveBytes()));
}

void Serial::sendBytes(QByteArray s) {
    this->write(s);
}

void Serial::receiveBytes() {
    QByteArray b = this->readAll();
    emit receiveFromSerial(b);
}

QStringList Serial::list() {
    QStringList portNames;
    QList<QextPortInfo> ports = QextSerialEnumerator::getPorts();
        for (int i = 0; i < ports.size(); i++) {
            if(ports.at(i).physName.contains("USB"))
                portNames << ports.at(i).physName;

/*
            qDebug() << "port name:" << ports.at(i).portName;
            qDebug() << "friendly name:" << ports.at(i).friendName;
            qDebug() << "physical name:" << ports.at(i).physName;
            qDebug() << "enumerator name:" << ports.at(i).enumName;
            qDebug() << "vendor ID:" << QString::number(ports.at(i).vendorID, 16);
            qDebug() << "product ID:" << QString::number(ports.at(i).productID, 16);
            qDebug() << "===================================";
*/
        }
    return portNames;
}
