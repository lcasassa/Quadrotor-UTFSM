#include <QMessageBox>
#include <QDebug>
#include "quadrotor.h"

Quadrotor::Quadrotor(QObject *parent) : QObject(parent) {

    protocol = new Protocol();
    serial = new Serial();

    connect(serial, SIGNAL(receiveFromSerial(QByteArray)), protocol, SLOT(receiveFromSerial(QByteArray)));
    connect(protocol, SIGNAL(sendToSerial(QByteArray)), serial, SLOT(sendBytes(QByteArray)));    

    serial->open(QIODevice::ReadWrite);

    if(!serial->isOpen()) {
        QMessageBox msgBox;
        msgBox.setText("No se pudo abrir la puerta serial");
        msgBox.exec();
//        exit(-1);
    }

//    connect(protocol, SIGNAL(receivedNewData(QList<int>)), this, SLOT());

}

Quadrotor::~Quadrotor() {
    delete protocol;
    delete serial;
}

void Quadrotor::selectSerialAndOpen(QWidget *parent) {
    QStringList items = serial->list();

    bool ok;
    QString item = QInputDialog::getItem(parent, tr("Select a Serial Port"),
                                         tr("Serial Port:"), items, 0, false, &ok);
    if (ok && !item.isEmpty()) {
        serial->setPortName(item);
        serial->open(QIODevice::ReadWrite);
        qWarning("open serial %s", item.toAscii().data());
        if(!serial->isOpen()) {
            QMessageBox msgBox;
            msgBox.setText("No se pudo abrir la puerta serial");
            msgBox.exec();
    //        exit(-1);
        } else {
            qWarning("Serial abierto!");
        }
    }
}

void Quadrotor::selectOutput(quint16 value) {
    protocol->sendCommand(QString("O%1").arg(value).toAscii());
}

void Quadrotor::setX_PA(quint16 value) {
    protocol->sendCommand(QString("XP%1").arg(value).toAscii());
}

void Quadrotor::setX_IA(quint16 value) {
    protocol->sendCommand(QString("XI%1").arg(value).toAscii());
}

void Quadrotor::setX_PG(quint16 value) {
    protocol->sendCommand(QString("XG%1").arg(value).toAscii());
}

void Quadrotor::setX_IG(quint16 value) {
    protocol->sendCommand(QString("XA%1").arg(value).toAscii());
}

void Quadrotor::setY_PA(quint16 value) {
    protocol->sendCommand(QString("YP%1").arg(value).toAscii());
}

void Quadrotor::setY_IA(quint16 value) {
    protocol->sendCommand(QString("YI%1").arg(value).toAscii());
}

void Quadrotor::setY_PG(quint16 value) {
    protocol->sendCommand(QString("YG%1").arg(value).toAscii());
}

void Quadrotor::setY_IG(quint16 value) {
    protocol->sendCommand(QString("YA%1").arg(value).toAscii());
}

void Quadrotor::setZ_P(quint16 value) {
    protocol->sendCommand(QString("ZP%1").arg(value).toAscii());
}

void Quadrotor::setZ_I(quint16 value) {
    protocol->sendCommand(QString("ZI%1").arg(value).toAscii());
}

void Quadrotor::setZ_D(quint16 value) {
    protocol->sendCommand(QString("ZD%1").arg(value).toAscii());
}

void Quadrotor::setX_AlfaBeta_P(quint16 value) {
    protocol->sendCommand(QString("XO%1").arg(value).toAscii());
}

void Quadrotor::setX_AlfaBeta_I(quint16 value) {
    protocol->sendCommand(QString("XL%1").arg(value).toAscii());
}

void Quadrotor::setY_AlfaBeta_P(quint16 value) {
    protocol->sendCommand(QString("YO%1").arg(value).toAscii());
}

void Quadrotor::setY_AlfaBeta_I(quint16 value) {
    protocol->sendCommand(QString("YL%1").arg(value).toAscii());
}

void Quadrotor::setE1_offset(quint16 value) {
    protocol->sendCommand(QString("E1O%1").arg(value).toAscii());
}

void Quadrotor::setE1_ganancia(quint16 value) {
    protocol->sendCommand(QString("E1G%1").arg(value).toAscii());
}

void Quadrotor::setE2_offset(quint16 value) {
    protocol->sendCommand(QString("E2O%1").arg(value).toAscii());
}

void Quadrotor::setE2_ganancia(quint16 value) {
    protocol->sendCommand(QString("E2G%1").arg(value).toAscii());
}

void Quadrotor::saveParameters() {
    protocol->sendCommand(QString("W").toAscii());
}

void Quadrotor::getParameters() {
    protocol->sendCommand(QString("G").toAscii());
}

void Quadrotor::setX_GyroGain(quint16 value) {
    protocol->sendCommand(QString("XE%1").arg(value).toAscii());
}

void Quadrotor::setY_GyroGain(quint16 value) {
    protocol->sendCommand(QString("YE%1").arg(value).toAscii());
}

void Quadrotor::setZ_GyroGain(quint16 value) {
    protocol->sendCommand(QString("ZE%1").arg(value).toAscii());
}

