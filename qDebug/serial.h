#ifndef SERIAL_H
#define SERIAL_H

#include <QObject>
#include <qextserialport.h>
#include <qextserialenumerator.h>
#include <QStringList>
#include <QList>


#define SERIAL_PORT "/dev/ttyUSB0"
//#define SERIAL_PORT "COM1"

class Serial : public QextSerialPort
{
    Q_OBJECT
public:
    Serial(QString serial_port = SERIAL_PORT);
    QStringList list();

public slots:
    void sendBytes(QByteArray s);
private slots:
    void receiveBytes();

signals:
    void receiveFromSerial(QByteArray b);

};

#endif // SERIAL_H
