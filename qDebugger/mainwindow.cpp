#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "plot.h"
#include <qextserialport.h>

#include <qglobal.h>
#include <QTimer>
#include <QList>
#include <QMessageBox>
#include <QCoreApplication>

#include "kalman.h"
#include "kalman2.h"
#include <stdio.h>


float gyro_offset[2] = {0,0};
float gyro_integral[2] = {0,0};

float motores_integral[2] = {0,0};

float k_motores, offset_motores;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->showMaximized();

    gyro_offset[0] = ui->doubleSpinBox_offset_gyro_0->value();
//    gyro_offset[1] = ui->doubleSpinBox_offset_gyro_1->value();
    kalman->Q_angle = ui->doubleSpinBox_var_theta->value();
    kalman->Q_gyro = ui->doubleSpinBox_var_bias->value();
    kalman->R_angle = ui->doubleSpinBox_var_acelerometro->value();
    (kalman+1)->Q_angle = ui->doubleSpinBox_var_theta->value();
    (kalman+1)->Q_gyro = ui->doubleSpinBox_var_bias->value();
    (kalman+1)->R_angle = ui->doubleSpinBox_var_acelerometro->value();
/*
    Kalman2_init(kalman2);
    Kalman2_init(kalman2+1);
    kalman2->Pbias = ui->doubleSpinBox_var_bias->value();
    kalman2->Pomega = ui->doubleSpinBox_var_omega->value();
    kalman2->Ptheta = ui->doubleSpinBox_var_theta->value();
    kalman2->PsensorTheta = ui->doubleSpinBox_var_acelerometro->value();
    kalman2->PsensorOmega = ui->doubleSpinBox_var_giroscopio->value();
    (kalman2+1)->Pbias = ui->doubleSpinBox_var_bias->value();
    (kalman2+1)->Pomega = ui->doubleSpinBox_var_omega->value();
    (kalman2+1)->Ptheta = ui->doubleSpinBox_var_theta->value();
    (kalman2+1)->PsensorTheta = ui->doubleSpinBox_var_acelerometro->value();
    (kalman2+1)->PsensorOmega = ui->doubleSpinBox_var_giroscopio->value();

    k_motores = ui->doubleSpinBox_k_motores->value();
    offset_motores = ui->doubleSpinBox_offset_motores->value();
*/
    ui->widget_plot->addVariable(Qt::blue);
    ui->widget_plot->addVariable(Qt::red);

    ui->widget_plot2->addVariable(Qt::blue);
    ui->widget_plot2->addVariable(Qt::red);
    ui->widget_plot2->addVariable(Qt::green);

    ui->widget_plot3->addVariable(Qt::blue);
    ui->widget_plot3->addVariable(Qt::red);
    ui->widget_plot3->addVariable(Qt::yellow);
    ui->widget_plot3->addVariable(Qt::gray);


    ui->widget_plot_2->addVariable(Qt::blue);
    ui->widget_plot_2->addVariable(Qt::red);
    ui->widget_plot_2->addVariable(Qt::gray);

    ui->widget_plot2_2->addVariable(Qt::blue);
    ui->widget_plot2_2->addVariable(Qt::red);
    ui->widget_plot2_2->addVariable(Qt::gray);
    ui->widget_plot2_2->addVariable(Qt::yellow);

    ui->widget_plot_X_PID->addVariable(Qt::blue);
    ui->widget_plot_X_PID->addVariable(Qt::red);
    ui->widget_plot_X_PID->addVariable(Qt::green);
    ui->widget_plot_Y_PID->addVariable(Qt::blue);
    ui->widget_plot_Y_PID->addVariable(Qt::red);
    ui->widget_plot_Y_PID->addVariable(Qt::green);

/*
    Plot *d_plot = new Plot(ui->widget_plot);

    QHBoxLayout *hLayout = new QHBoxLayout(ui->widget_plot);
    hLayout->addWidget(ui->widget_panel);
    hLayout->addWidget(d_plot, 10);
*/
//    setCentralWidget();


    quadrotor = new Quadrotor();
    serial = new Serial();
    connect(serial, SIGNAL(nuevosDatos(QList<int>)), this, SLOT(nuevosDatos(QList<int>)));
//    connect(serial, SIGNAL(serialTextSend(QByteArray)), this, SLOT(serialTextSend(QByteArray)));
    connect(quadrotor, SIGNAL(sendToQuadrotor(QByteArray)), serial, SLOT(send(QByteArray)));
    connect(quadrotor, SIGNAL(sendToQuadrotor(QByteArray)), this, SLOT(serialTextSend(QByteArray)));
    connect(serial, SIGNAL(serialTextReceive(QByteArray)), quadrotor, SLOT(recieve(QByteArray)));
    connect(serial, SIGNAL(serialTextReceive(QByteArray)), this, SLOT(serialTextReceive(QByteArray)));

    if(!serial->isOpen()) {
        QMessageBox msgBox;
        msgBox.setText("No se pudo abrir la puerta serial");
        msgBox.exec();
        exit(-1);
    }

//    serial->send("Ja!!");
}

void MainWindow::nuevosDatos(QList<int> datos) {

    if(datos.size()==4) {

//            qWarning("%s", list[i].data());

        datos[0] += gyro_offset[0];
        datos[1] += gyro_offset[1];

        QList<double> data3;
        data3.append(-1*((float)datos[2])/100.0);
        data3.append(-1*((float)datos[3])/100.0);
//            data3.append(0);
//            data3.append(0);
        data3.append(((float)datos[0])/100.0);
        data3.append(((float)datos[1])/100.0);
        ui->widget_plot2_2->newData(data3);

        QList<double> data;
        data.append(((float)datos[2])/100.0);
        data.append(((float)datos[3])/100.0);
        ui->widget_plot2->newData(data);

        QList<double> data_2;
        data_2.append(0);
        data_2.append(0);
        data_2.append(0);
        ui->widget_plot_2->newData(data_2);

/*
    } else if(datos.size() == 2) {

        qWarning("%d %d", datos[0], datos[1]);

        QList<double> data;
        data.append(((float)datos[0])/100.0);
        data.append(((float)datos[1])/100.0);
        ui->widget_plot->newData(data);
*/

    } else if(datos.size() == 9) {

//            qWarning("%s", list[i].data());

        datos[0] -= gyro_offset[0];
        datos[1] -= gyro_offset[1];

        KalmanStateUpdate(kalman, ((float)datos[0])/100/7.5);
        KalmanStateUpdate(kalman+1, ((float)-datos[1])/100/7.5);

        KalmanUpdate(kalman,   atan2(datos[3] , sqrtf(powf(((float)datos[2]),2) + powf(((float)datos[4]),2))) *180/M_PI);
        KalmanUpdate(kalman+1, atan2(datos[2] , sqrtf(powf(((float)datos[3]),2) + powf(((float)datos[4]),2))) *180/M_PI);


/*
        Kalman2(kalman2,   k_motores*((float)(datos[6]-datos[8]+offset_motores)), atan2(datos[2] , sqrtf(powf(((float)datos[3]),2) + powf(((float)datos[4]),2))) *180/M_PI, ((float)-datos[0])/100);
        Kalman2(kalman2+1, k_motores*((float)(datos[5]-datos[7]+offset_motores)), atan2(datos[3] , sqrtf(powf(((float)datos[2]),2) + powf(((float)datos[4]),2))) *180/M_PI, ((float)-datos[1])/100);
*/
        gyro_integral[0] += ((float)datos[0])/100/7.5;
        gyro_integral[1] += ((float)-datos[1])/100/7.5;
//            motores_integral[0] += k_motores*((float)(datos[6]-datos[8]+offset_motores));



        QList<double> data;
        data.append(((float)datos[0])/100.0);
        data.append(((float)datos[1])/100.0);
        ui->widget_plot->newData(data);

        QList<double> data2;
        data2.append(datos[2]);
        data2.append(datos[3]);
        data2.append(datos[4]);
        ui->widget_plot2->newData(data2);


        QList<double> data3;
        data3.append(((float)datos[7])/100.0);
        data3.append(((float)datos[8])/100.0);
        data3.append(((float)datos[5])/100.0);
        data3.append(((float)datos[6])/100.0);
        ui->widget_plot3->newData(data3);


        QList<double> data_2;

        static int init = 50;
        if(init>0) {
            init--;
            data_2.append(0);
        }else
            data_2.append(kalman[0].angle);

        data_2.append(gyro_integral[0]);
        data_2.append(atan2(datos[3] , sqrtf(powf(((float)datos[2]),2) + powf(((float)datos[4]),2))) *180/M_PI);
        ui->widget_plot_2->newData(data_2);

    } else if(datos.size() == 6) {

//        qWarning("%d %d", datos[0], datos[1]);

        QList<double> data;
        data.append(((float)datos[0]));
        data.append(((float)datos[1]));
        data.append(((float)datos[2]));
        ui->widget_plot_X_PID->newData(data);
        QList<double> data2;
        data2.append(((float)datos[3]));
        data2.append(((float)datos[4]));
        data2.append(((float)datos[5]));
        ui->widget_plot_Y_PID->newData(data2);

    } else if(datos.size() == 5) {

//        qWarning("%d %d", datos[0], datos[1]);

        QList<double> data;
        data.append(((float)datos[0])/100);
        data.append(((float)datos[1])/100);
        data.append(0);
        ui->widget_plot_X_PID->newData(data);
        QList<double> data2;
        data2.append(((float)datos[2]));
        data2.append(((float)datos[3])/100);
        data2.append(((float)datos[4])/100);
        ui->widget_plot_Y_PID->newData(data2);


    } else {
        qDebug("Size %d", datos.size());
    }
}


MainWindow::~MainWindow()
{
    delete ui;
    delete serial;
    delete quadrotor;
}

void MainWindow::on_pushButton_clicked()
{
    gyro_integral[0]=0;
    gyro_integral[1]=0;
    motores_integral[0] = 0;
    motores_integral[1] = 0;
}

void MainWindow::on_doubleSpinBox_offset_gyro_0_valueChanged(double value)
{
    gyro_offset[0]=value;
}
/*
void MainWindow::on_doubleSpinBox_offset_gyro_1_valueChanged(double value)
{
    gyro_offset[1]=value;
}*/

void MainWindow::on_doubleSpinBox_var_theta_valueChanged(double value)
{
    kalman->Q_angle = value;
    (kalman+1)->Q_angle = value;
    kalman2->Ptheta = value;
    (kalman2+1)->Ptheta = value;
}

void MainWindow::on_doubleSpinBox_var_bias_valueChanged(double value)
{
    kalman->Q_gyro = value;
    (kalman+1)->Q_gyro = value;
    kalman2->Pbias = value;
    (kalman2+1)->Pbias = value;
}

void MainWindow::on_doubleSpinBox_var_acelerometro_valueChanged(double value)
{
    kalman->R_angle = value;
    (kalman+1)->R_angle = value;
    kalman2->PsensorTheta = value;
    (kalman2+1)->PsensorTheta = value;

}

void MainWindow::on_pushButton_2_clicked()
{
    kalman->angle=0.0;
    (kalman+1)->angle=0.0;
    kalman2->x[0] = 0.0;
    kalman2->x[1] = 0.0;
    kalman2->x[2] = 0.0;
    (kalman2+1)->x[0] = 0.0;
    (kalman2+1)->x[1] = 0.0;
    (kalman2+1)->x[2] = 0.0;
}
/*
void MainWindow::on_doubleSpinBox_var_omega_valueChanged(double value)
{
    kalman2->Pomega = value;
    (kalman2+1)->Pomega = value;
}

void MainWindow::on_doubleSpinBox_var_giroscopio_valueChanged(double value)
{
    kalman2->PsensorOmega = value;
    (kalman2+1)->PsensorOmega = value;
}

void MainWindow::on_doubleSpinBox_offset_motores_valueChanged(double value)
{
    offset_motores = value;

}

void MainWindow::on_doubleSpinBox_k_motores_valueChanged(double value)
{
    k_motores = value;
}
*/

void MainWindow::serialTextReceive(QByteArray b) {
    QString s(b);
    s.remove('\r');
    s.remove('\n');
    ui->textEdit->append("> " + s);
}

void MainWindow::serialTextSend(QByteArray b) {
    QString s(b);
    s.remove('\r');
    s.remove('\n');
    ui->textEdit->append("< " + s);
}


//#define BACKGROUND_BUTTON_COLOR "background-color: rgb(255, 0, 0)"
void MainWindow::on_pushButton_X_PG_released() {
//    ui->pushButton_X_P->setStyleSheet(BACKGROUND_BUTTON_COLOR);
    quadrotor->setX_PG((quint16)ui->spinBox_X_PG->value());
 //   serial->send(QString("XP%1").arg(ui->spinBox_X_P->value()));
}

void MainWindow::on_pushButton_X_IG_released() {
//    ui->pushButton_X_I->setStyleSheet(BACKGROUND_BUTTON_COLOR);
    quadrotor->setX_IG((quint16)ui->spinBox_X_IG->value());
//    serial->send(QString("XI%1").arg(ui->spinBox_X_I->value()));
}

void MainWindow::on_pushButton_X_PA_released() {
//    ui->pushButton_X_D->setStyleSheet(BACKGROUND_BUTTON_COLOR);
    quadrotor->setX_PA((quint16)ui->spinBox_X_PA->value());
//    serial->send(QString("XD%1").arg(ui->spinBox_X_D->value()));
}

void MainWindow::on_pushButton_X_IA_released()
{
    quadrotor->setX_IA((quint16)ui->spinBox_X_IA->value());
}

void MainWindow::on_pushButton_Y_PG_released() {
//    ui->pushButton_Y_P->setStyleSheet(BACKGROUND_BUTTON_COLOR);
    quadrotor->setY_PG((quint16)ui->spinBox_Y_PG->value());
//    serial->send(QString("YP%1").arg(ui->spinBox_Y_P->value()));
}

void MainWindow::on_pushButton_Y_IG_released() {
//    ui->pushButton_Y_I->setStyleSheet(BACKGROUND_BUTTON_COLOR);
    quadrotor->setY_IG((quint16)ui->spinBox_Y_IG->value());
//    serial->send(QString("YI%1").arg(ui->spinBox_Y_I->value()));
}

void MainWindow::on_pushButton_Y_PA_released() {
//    ui->pushButton_Y_D->setStyleSheet(BACKGROUND_BUTTON_COLOR);
    quadrotor->setY_PA((quint16)ui->spinBox_Y_PA->value());
//    serial->send(QString("YD%1").arg(ui->spinBox_Y_D->value()));
}

void MainWindow::on_pushButton_Y_IA_released()
{
    quadrotor->setY_IA((quint16)ui->spinBox_Y_IA->value());
}

#define TEXT_SIZE 1000
void MainWindow::on_textEdit_textChanged() {
    if(ui->textEdit->toPlainText().size() > TEXT_SIZE)
        ui->textEdit->setPlainText(ui->textEdit->toPlainText().right(TEXT_SIZE));
}

void MainWindow::on_pushButton_Z_P_released() {
    quadrotor->setZ_P((quint16)ui->spinBox_Z_P->value());
}

void MainWindow::on_pushButton_Z_I_released() {
    quadrotor->setZ_I((quint16)ui->spinBox_Z_I->value());
}

void MainWindow::on_pushButton_Z_D_released() {
    quadrotor->setZ_D((quint16)ui->spinBox_Z_D->value());
}

void MainWindow::on_pushButton_3_released()
{
    quadrotor->saveParameters();
}

void MainWindow::on_pushButton_4_clicked()
{
    quadrotor->getParameters();
}

void MainWindow::on_spinBox_E1_offset_valueChanged(int arg1)
{
    quadrotor->setE1_offset((quint16)arg1);
}

void MainWindow::on_spinBox_E1_ganancia_valueChanged(int arg1)
{
    quadrotor->setE1_ganancia((quint16)arg1);
}

void MainWindow::on_spinBox_E2_offset_valueChanged(int arg1)
{
    quadrotor->setE2_offset((quint16)arg1);
}

void MainWindow::on_spinBox_E2_ganancia_valueChanged(int arg1)
{
    quadrotor->setE2_ganancia((quint16)arg1);
}

