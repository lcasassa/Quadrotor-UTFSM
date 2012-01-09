#ifndef SERIAL_H
#define SERIAL_H

#include <QObject>
#include <qextserialport.h>
#include <QDateTime>
#include <QTextStream>
#include <QFile>

//#define SERIAL_PORT "COM1"
#define SERIAL_PORT "/dev/ttyUSB0"
//#define SERIAL_PORT "COM4"

class Serial : public QObject
{
    Q_OBJECT
public:
    Serial(QString serial_port = SERIAL_PORT, QObject *parent = 0);
    ~Serial();
    bool isOpen();

signals:
    void nuevosDatos(QList<int>);
    void serialTextReceive(QByteArray);
    void serialTextSend(QByteArray);

public slots:
    void send(QByteArray s);
private slots:
    void readyRead();
private:
    QextSerialPort * port;
    QTextStream *out;
    QFile *f;

};

#endif // SERIAL_H
