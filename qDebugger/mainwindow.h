#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include "kalman.h"
#include "quadrotor.h"
#include "serial.h"

class QextSerialPort;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    /*
    void on_doubleSpinBox_k_motores_valueChanged(double );
    void on_doubleSpinBox_offset_motores_valueChanged(double );
    void on_doubleSpinBox_var_giroscopio_valueChanged(double );
    void on_doubleSpinBox_var_omega_valueChanged(double );
*/
    void on_pushButton_3_released();
    void on_pushButton_Z_D_released();
    void on_pushButton_Z_I_released();
    void on_pushButton_Z_P_released();
    void on_textEdit_textChanged();
    void on_pushButton_Y_D_released();
    void on_pushButton_Y_I_released();
    void on_pushButton_Y_P_released();
    void on_pushButton_X_D_released();
    void on_pushButton_X_I_released();
    void on_pushButton_X_P_released();
    void on_pushButton_2_clicked();
    void on_doubleSpinBox_var_acelerometro_valueChanged(double );
    void on_doubleSpinBox_var_bias_valueChanged(double );
    void on_doubleSpinBox_var_theta_valueChanged(double );
//    void on_doubleSpinBox_offset_gyro_1_valueChanged(double );
    void on_doubleSpinBox_offset_gyro_0_valueChanged(double );
    void on_pushButton_clicked();
//    void readyRead(void);
    void nuevosDatos(QList<int> datos);
    void serialTextReceive(QByteArray s);
    void serialTextSend(QByteArray s);

    void on_pushButton_4_clicked();

private:
    Ui::MainWindow *ui;
    Serial *serial;
    Quadrotor *quadrotor;
};

#endif // MAINWINDOW_H
