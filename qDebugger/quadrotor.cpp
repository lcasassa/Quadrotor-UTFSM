#include "quadrotor.h"

Quadrotor::Quadrotor(QObject *parent) : QObject(parent) {

    timer.setInterval(1000);
    connect(&timer, SIGNAL(timeout()), this, SLOT(recieve()));

}

void Quadrotor::send(QByteArray b) {
    unsigned char checksum=0;
    for(int i=0; i<b.size(); i++) {
        checksum ^= (unsigned char)b[i];
    }
    b = QByteArray("$") + b + QString("*%1\r\n").arg(checksum, 2, 16, QLatin1Char('0')).toAscii();
    sentBuffer.append(b);

    if(sentBuffer.size() == 1) {
        emit sendToQuadrotor(sentBuffer[0]);
//        qDebug("send 1");
        timer.start();
//        qDebug("timer start");
    }
}

// Recibi un packete
void Quadrotor::recieve(QByteArray b) {

    if(sentBuffer.isEmpty()) {
        qWarning("RX: %s", b.data());
        return;
    }

    if(b.isEmpty() || sentBuffer[0].size() != b.size()) {
        emit sendToQuadrotor(sentBuffer[0]);
//        qDebug("error1 send 2 %d %d", sentBuffer[0].size(), b.size());
        return;
    }

    for(int i=0; i<b.size() && i<sentBuffer[0].size(); i++) {
        if(b.at(i) != sentBuffer[0].at(i)) { // paquete no es igual, reenviando
            emit sendToQuadrotor(sentBuffer[0]);
//            qDebug("error2 send 2");
            return;
        }
    }
    // Eliminando paquete bien enviado.
    sentBuffer.removeFirst();

    // Enviando proximo paquete
    if(!sentBuffer.isEmpty()) {
        emit sendToQuadrotor(sentBuffer[0]);
//        qDebug("send 2");
    } else {
        timer.stop();
//        qDebug("timer stop");
    }

}

void Quadrotor::setX_P(quint16 value) {
    send(QString("XP%1").arg(value).toAscii());
}

void Quadrotor::setX_I(quint16 value) {
    send(QString("XI%1").arg(value).toAscii());
}

void Quadrotor::setX_D(quint16 value) {
    send(QString("XD%1").arg(value).toAscii());
}

void Quadrotor::setY_P(quint16 value) {
    send(QString("YP%1").arg(value).toAscii());
}

void Quadrotor::setY_I(quint16 value) {
    send(QString("YI%1").arg(value).toAscii());
}

void Quadrotor::setY_D(quint16 value) {
    send(QString("YD%1").arg(value).toAscii());
}

void Quadrotor::setZ_P(quint16 value) {
    send(QString("ZP%1").arg(value).toAscii());
}

void Quadrotor::setZ_I(quint16 value) {
    send(QString("ZI%1").arg(value).toAscii());
}

void Quadrotor::setZ_D(quint16 value) {
    send(QString("ZD%1").arg(value).toAscii());
}

void Quadrotor::saveParameters() {
    send(QString("W").toAscii());
}

void Quadrotor::getParameters() {
    send(QString("G").toAscii());
}

