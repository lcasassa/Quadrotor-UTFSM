#ifndef CONFIGQUADROTOR_H
#define CONFIGQUADROTOR_H

#include <QWidget>
#include <QList>
#include "quadrotor.h"

namespace Ui {
class configQuadrotor;
}

class configQuadrotor : public QWidget
{
    Q_OBJECT
    
public:
    explicit configQuadrotor(QWidget *parent = 0);
    ~configQuadrotor();
    void setQuadrotor(Quadrotor *quadrotor_);
    
private slots:

    void on_tabWidget_currentChanged(int index);

    void on_pushButton_saveToFlash_released();

    void on_pushButton_readFromQuadrotor_released();

    void on_pushButton_IMU_cleanPlots_released();

    void on_doubleSpinBox_Gyro_X_valueChanged(double arg1);

    void on_doubleSpinBox_Gyro_Y_valueChanged(double arg1);

    void on_doubleSpinBox_Gyro_Z_valueChanged(double arg1);

    void on_doubleSpinBox_filter_X_P_valueChanged(double arg1);

    void on_doubleSpinBox_filter_X_I_valueChanged(double arg1);

    void on_doubleSpinBox_filter_Y_P_valueChanged(double arg1);

    void on_doubleSpinBox_filter_Y_I_valueChanged(double arg1);

    void on_pushButton_AngularVelocityControl_cleanPlots_released();

    void on_pushButton_testVelocityFlight_released();

    void on_doubleSpinBox_angularVelocityControl_X_P_valueChanged(double arg1);

    void on_doubleSpinBox_angularVelocityControl_X_I_valueChanged(double arg1);

    void on_doubleSpinBox_angularVelocityControl_Y_P_valueChanged(double arg1);

    void on_doubleSpinBox_angularVelocityControl_Y_I_valueChanged(double arg1);

    void on_doubleSpinBox_angularVelocityControl_Z_P_valueChanged(double arg1);

    void on_doubleSpinBox_angularVelocityControl_Z_I_valueChanged(double arg1);

    void on_pushButton_AngularControl_cleanPlots_released();

public slots:
    void receivedNewData(QList<int>);
    void receivedNewCommand(QByteArray);

private:
    Ui::configQuadrotor *ui;
    Quadrotor *quadrotor;
};

#endif // CONFIGQUADROTOR_H
