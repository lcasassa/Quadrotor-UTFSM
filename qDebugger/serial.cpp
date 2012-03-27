#include "serial.h"

Serial::Serial(QString serial_port, QObject *parent) : QObject(parent) {

    port = new QextSerialPort(serial_port, QextSerialPort::EventDriven);
    port->setBaudRate(BAUD115200);
    port->setFlowControl(FLOW_HARDWARE);
    port->setParity(PAR_NONE);
    port->setDataBits(DATA_8);
    port->setStopBits(STOP_1);
    port->open(QIODevice::ReadWrite);
    if(port->isOpen()) {
        qWarning("Puerto serial %s abierto", serial_port.toAscii().data());
    } else {
        qWarning("No se pudo abrir puerto serial %s", serial_port.toAscii().data());
    }

    connect(port, SIGNAL(readyRead()), this, SLOT(readyRead()));


    f = new QFile("out.txt");
    if (!f->open(QIODevice::Append | QIODevice::Text)) {
        qWarning("No se pudo abrir out.txt");
        return;
    }

    out = new QTextStream(&(*f));
    (*out) << "Nueva medición " << QDateTime::currentDateTime().toString() << "\n";

}

Serial::~Serial() {
    if(f->isOpen()) {
        delete out;
        f->close();
    }
    delete f;
    delete port;
}

void Serial::send(QByteArray s) {
    port->write(s);
    emit serialTextSend(s);
}

bool Serial::isOpen() {
    return port->isOpen();
}

#define START_COMMAND_CHAR '$'
#define END_CHAR '\n'
#define SPLIT_CHAR ' '

void Serial::readyRead() {
    static QByteArray b;
    bool ok;
    b += port->readAll();
    if(b.contains(END_CHAR)) {
        QList<QByteArray> list = b.split(END_CHAR);
        for(int i=0; i<(list.size()-1); i++) {
            QList<QByteArray> arg = list[i].split(SPLIT_CHAR);
            QList<int> datos;

            b = b.remove(0, list[i].size()+1);

            if(list[i].at(0) == '$') { // Packete es de comando
                emit serialTextReceive(list[i]+END_CHAR);
                continue;
            }

            (*out) << QDateTime::currentMSecsSinceEpoch();

            for(int j=0; j<arg.size(); j++) {
                QString s(arg[j]);
                int dato;
                dato = s.toInt(&ok);
                if(!ok) {
                    qWarning("Error1: %s", list[i].data());
                    break;
                }
                datos.append(dato);
                (*out) << " " << dato;
            }

            (*out) << "\n";

            if(ok) {
                if(datos.size() == 4)
                        qWarning("%s", list[i].data());
                else
                emit nuevosDatos(datos);
            }

        }
    }

}
