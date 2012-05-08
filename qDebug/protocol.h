#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QObject>
#include <QList>
#include <QByteArray>
#include <QString>
#include <QTimer>
#include "serial.h"

class Protocol : public QObject
{
    Q_OBJECT
public:
    explicit Protocol(QObject *parent = 0);
    
signals:
    void receivedNewData(QList<int>);
    void receivedNewString(QString);
    void receivedNewCommand(QByteArray);
    void sendToSerial(QByteArray); // Serial::sendBytes()

public slots:
    void sendCommand(QByteArray b);
    void receiveFromSerial(QByteArray b); // Serial::readyRead()

private slots:
    void receiveCommand(QByteArray b = "");

private:
    unsigned char checksum(QByteArray b);

private:
    QList<QByteArray> sentBuffer;
    QTimer timer;
};

#endif // PROTOCOL_H
