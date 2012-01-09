#ifndef QUADROTOR_H
#define QUADROTOR_H

#include <QObject>
#include <QTimer>

class Quadrotor : public QObject
{
    Q_OBJECT
public:
    explicit Quadrotor(QObject *parent = 0);

signals:
    void sendToQuadrotor(QByteArray);

public slots:
    void recieve(QByteArray b = "");
    void send(QByteArray b);
    void setX_P(quint16 value);
    void setX_I(quint16 value);
    void setX_D(quint16 value);
    void setY_P(quint16 value);
    void setY_I(quint16 value);
    void setY_D(quint16 value);
    void setZ_P(quint16 value);
    void setZ_I(quint16 value);
    void setZ_D(quint16 value);
    void saveParameters(void);
    void getParameters(void);

private:
    QList<QByteArray> sentBuffer;
    QTimer timer;
    int X_P;
    int X_I;
    int X_D;
    int Y_P;
    int Y_I;
    int Y_D;
    int Z_P;
    int Z_I;
    int Z_D;

};

#endif // QUADROTOR_H
