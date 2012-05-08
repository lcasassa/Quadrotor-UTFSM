#include "configquadrotor.h"
#include "ui_configquadrotor.h"
#include <QDebug>

configQuadrotor::configQuadrotor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::configQuadrotor)
{
    ui->setupUi(this);

    ui->widget_plot_angularControl_up->addVariable(Qt::red);
    ui->widget_plot_angularControl_up->addVariable(Qt::blue);
    ui->widget_plot_angularControl_up->addVariable(Qt::green);

    ui->widget_plot_angularControl_down->addVariable(Qt::red);
    ui->widget_plot_angularControl_down->addVariable(Qt::blue);
    ui->widget_plot_angularControl_down->addVariable(Qt::green);

    ui->widget_plot_angularVelocityControl_up->addVariable(Qt::red);
    ui->widget_plot_angularVelocityControl_up->addVariable(Qt::blue);
    ui->widget_plot_angularVelocityControl_up->addVariable(Qt::green);

    ui->widget_plot_angularVelocityControl_down->addVariable(Qt::red);
    ui->widget_plot_angularVelocityControl_down->addVariable(Qt::blue);
    ui->widget_plot_angularVelocityControl_down->addVariable(Qt::green);

    ui->widget_plot_imu_up->addVariable(Qt::red);
    ui->widget_plot_imu_up->addVariable(Qt::blue);
    ui->widget_plot_imu_up->addVariable(Qt::green);

    ui->widget_plot_imu_down->addVariable(Qt::red);
    ui->widget_plot_imu_down->addVariable(Qt::blue);
    ui->widget_plot_imu_down->addVariable(Qt::green);

}

configQuadrotor::~configQuadrotor()
{
    quadrotor->selectOutput(0);
    delete ui;
}

void configQuadrotor::setQuadrotor(Quadrotor *quadrotor_) {
    quadrotor = quadrotor_;
    connect(quadrotor->protocol, SIGNAL(receivedNewData(QList<int>)), this, SLOT(receivedNewData(QList<int>)));
}

void configQuadrotor::receivedNewData(QList<int> data_int) {
    QList<double> data;

    for(int i=0; i<data_int.size(); i++) {
        data.append((double)data_int[i]/100);
    }

    switch(ui->tabWidget->currentIndex()) {
    case 0: // IMU
        ui->widget_plot_imu_up->newData(data.mid(0, 3));
        ui->widget_plot_imu_down->newData(data.mid(3, 3));
        break;
    case 1:
        ui->widget_plot_angularVelocityControl_up->newData(data.mid(0, 3));
        ui->widget_plot_angularVelocityControl_down->newData(data.mid(3, 3));
        break;
    case 2:
        ui->widget_plot_angularControl_up->newData(data.mid(0, 3));
        ui->widget_plot_angularControl_down->newData(data.mid(3, 3));
        break;
    }

}

void configQuadrotor::on_tabWidget_currentChanged(int index)
{
    quadrotor->selectOutput(index+1);
}

void configQuadrotor::on_pushButton_saveToFlash_released()
{
    quadrotor->saveParameters();
}

void configQuadrotor::on_pushButton_readFromQuadrotor_released()
{
    quadrotor->getParameters();
}

void configQuadrotor::on_pushButton_IMU_cleanPlots_released()
{
    ui->widget_plot_imu_up->clean();
    ui->widget_plot_imu_down->clean();
}

void configQuadrotor::on_doubleSpinBox_Gyro_X_valueChanged(double arg1)
{
    quadrotor->setX_GyroGain((quint16)(arg1*1000));
}

void configQuadrotor::on_doubleSpinBox_Gyro_Y_valueChanged(double arg1)
{
    quadrotor->setY_GyroGain((quint16)(arg1*1000));
}

void configQuadrotor::on_doubleSpinBox_Gyro_Z_valueChanged(double arg1)
{
    quadrotor->setZ_GyroGain((quint16)(arg1*1000));
}

void configQuadrotor::on_doubleSpinBox_filter_X_P_valueChanged(double arg1)
{
    quadrotor->setX_AlfaBeta_P((quint16)(arg1*1000));
}
