#ifndef QUADROTOR_H
#define QUADROTOR_H

#include <QObject>
#include <QWidget>
#include <QTimer>
#include <QInputDialog>
#include "serial.h"
#include "protocol.h"

class Quadrotor : public QObject
{
    Q_OBJECT
public:
    explicit Quadrotor(QObject *parent = 0);
    ~Quadrotor();
    void selectSerialAndOpen(QWidget *parent = 0);
    void selectOutput(quint16 value);

signals:

public slots:
    void setX_GyroGain(quint16 value);
    void setY_GyroGain(quint16 value);
    void setZ_GyroGain(quint16 value);
    void setX_AlfaBeta_P(quint16 value);
    void setX_AlfaBeta_I(quint16 value);
    void setY_AlfaBeta_P(quint16 value);
    void setY_AlfaBeta_I(quint16 value);
    void setX_PA(quint16 value);
    void setX_IA(quint16 value);
    void setX_PG(quint16 value);
    void setX_IG(quint16 value);
    void setY_PA(quint16 value);
    void setY_IA(quint16 value);
    void setY_PG(quint16 value);
    void setY_IG(quint16 value);
    void setZ_P(quint16 value);
    void setZ_I(quint16 value);
    void setZ_D(quint16 value);
    void setE1_offset(quint16 value);
    void setE1_ganancia(quint16 value);
    void setE2_offset(quint16 value);
    void setE2_ganancia(quint16 value);
    void saveParameters(void);
    void getParameters(void);
    void setX_AccOffset(quint16 value);
    void setY_AccOffset(quint16 value);
    void setZ_AccOffset(quint16 value);
    void calculate_AccOffset(void);

public:
    Protocol *protocol;
private:
    Serial *serial;
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
