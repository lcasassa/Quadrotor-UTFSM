#include "protocol.h"
#include <QDebug>

#define START_COMMAND_CHAR '$'
#define START_COMMAND_ACK_CHAR '#'
#define END_CHAR '\n'
#define SPLIT_CHAR ' '
#define SPLIT_CHECKSUM '*'

Protocol::Protocol(QObject *parent) :
    QObject(parent)
{
    timer.setInterval(1000);
    connect(&timer, SIGNAL(timeout()), this, SLOT(receiveCommand()));
}

unsigned char Protocol::checksum(QByteArray b) {
    unsigned char result=0;
    for(int i=0; i<b.size(); i++) {
        result ^= (unsigned char)b[i];
    }
    return result;
}

void Protocol::sendCommand(QByteArray b) {
    QByteArray out;

    out += START_COMMAND_CHAR;
    out += b;
    out += SPLIT_CHECKSUM;
    out += QString("%1").arg(checksum(b), 2, 16, QLatin1Char('0')).toAscii();
    out += END_CHAR;
    sentBuffer.append(out);

    if(sentBuffer.size() == 1) {
        emit sendToSerial(sentBuffer[0]);
//        qDebug("send 1");
        timer.start();
//        qDebug("timer start");
    }
}

// Recibi un packete
void Protocol::receiveCommand(QByteArray b) {

    if(sentBuffer.isEmpty()) {
//            qDebug("error0 bad checksum %d %d", sentBuffer[0].size(), b.size());
            qWarning("RX: %s", b.data());
        return;
    }

    if(b.isEmpty() || sentBuffer[0].size() != b.size()) {
        emit sendToSerial(sentBuffer[0]);
//        qDebug("error1 send 2 %d %d", sentBuffer[0].size(), b.size());
        return;
    }

    for(int i=0; i<b.size() && i<sentBuffer[0].size(); i++) {
        if(b.at(i) != sentBuffer[0].at(i)) { // paquete no es igual, reenviando
            emit sendToSerial(sentBuffer[0]);
//            qDebug("error2 send 2");
            return;
        }
    }
    // Eliminando paquete bien enviado.
    sentBuffer.removeFirst();

    // Enviando proximo paquete
    if(!sentBuffer.isEmpty()) {
        emit sendToSerial(sentBuffer[0]);
//        qDebug("send 2");
    } else {
        timer.stop();
//        qDebug("timer stop");
    }

}

void Protocol::receiveFromSerial(QByteArray newb) {
    static QByteArray b;
    bool ok;
    b += newb;
    if(b.contains(END_CHAR)) {
        QList<QByteArray> list = b.split(END_CHAR);
        for(int i=0; i<(list.size()-1); i++) {
            QList<QByteArray> arg = list[i].split(SPLIT_CHAR);
            QList<int> datos;

            b = b.remove(0, list[i].size()+1);

            if(list[i].at(0) == START_COMMAND_CHAR) { // Packete es de comando
                QList<QByteArray> l = list[i].mid(1).split(SPLIT_CHECKSUM);

//                QString checksumString = l[1];
//                int checksum = checksumString.toInt(&ok, 10);

//                if(checksum == (int)this->checksum(l[0])) {
                qWarning() << l[0];

                emit receivedNewCommand(l[0]);
//                }

                continue;
            }

            if(list[i].at(0) == START_COMMAND_ACK_CHAR) { // Packete es de comando
                qWarning() << list[i];
                list[i][0] = START_COMMAND_CHAR;
                receiveCommand(list[i]+END_CHAR);
                continue;
            }

            for(int j=0; j<arg.size(); j++) {
                QString s(arg[j]);
                int dato;
                dato = s.toInt(&ok);
                if(!ok) {
                    break;
                }
                datos.append(dato);
            }

            if(ok) {
                emit receivedNewData(datos);
            } else {
                list[i].chop(1);
                qWarning() << list[i];
                emit receivedNewString(list[i]);
            }

        }
    }

}
