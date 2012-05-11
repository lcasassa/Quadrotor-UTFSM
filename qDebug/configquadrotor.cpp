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

    ui->widget_plot_angularControl_down->addVariable(Qt::red);
    ui->widget_plot_angularControl_down->addVariable(Qt::blue);

    ui->widget_plot_angularVelocityControl_up->addVariable(Qt::red);
    ui->widget_plot_angularVelocityControl_up->addVariable(Qt::blue);

    ui->widget_plot_angularVelocityControl_mid->addVariable(Qt::red);
    ui->widget_plot_angularVelocityControl_mid->addVariable(Qt::blue);

    ui->widget_plot_angularVelocityControl_down->addVariable(Qt::red);
    ui->widget_plot_angularVelocityControl_down->addVariable(Qt::blue);

    ui->widget_plot_imu_up->addVariable(Qt::red);
    ui->widget_plot_imu_up->addVariable(Qt::blue);
    ui->widget_plot_imu_up->addVariable(Qt::green);

    ui->widget_plot_imu_down->addVariable(Qt::red);
    ui->widget_plot_imu_down->addVariable(Qt::blue);
    ui->widget_plot_imu_down->addVariable(Qt::green);
    ui->widget_plot_imu_down->addVariable(Qt::yellow);
    ui->widget_plot_imu_down->addVariable(Qt::cyan);

}

configQuadrotor::~configQuadrotor()
{
    quadrotor->selectOutput(0);
    delete ui;
}

void configQuadrotor::setQuadrotor(Quadrotor *quadrotor_) {
    quadrotor = quadrotor_;
    connect(quadrotor->protocol, SIGNAL(receivedNewData(QList<int>)), this, SLOT(receivedNewData(QList<int>)));
    connect(quadrotor->protocol, SIGNAL(receivedNewCommand(QByteArray)), this, SLOT(receivedNewCommand(QByteArray)));
    on_pushButton_readFromQuadrotor_released();
}

void configQuadrotor::receivedNewData(QList<int> data_int) {
    QList<double> data;

    for(int i=0; i<data_int.size(); i++) {
        data.append((double)data_int[i]/100);
    }

    switch(ui->tabWidget->currentIndex()) {
    case 0: // IMU
        ui->widget_plot_imu_up->newData(data.mid(0, 3));
        ui->widget_plot_imu_down->newData(data.mid(3, 5));
        break;
    case 1:
        ui->widget_plot_angularVelocityControl_up->newData(data.mid(0, 2));
        ui->widget_plot_angularVelocityControl_mid->newData(data.mid(2, 2));
        ui->widget_plot_angularVelocityControl_down->newData(data.mid(4, 2));
        break;
    case 2:
        ui->widget_plot_angularControl_up->newData(data.mid(0, 2));
        ui->widget_plot_angularControl_down->newData(data.mid(2, 2));
        break;
    case 3:
        ui->progressBar_thro->setValue(data_int.at(2));
        ui->progressBar_rudo->setValue(data_int.at(3));
        ui->progressBar_ale->setValue(data_int.at(0));
        ui->progressBar_elev->setValue(data_int.at(1));
        ui->progressBar_gear->setValue(data_int.at(4));
        ui->progressBar_aux1->setValue(data_int.at(5));
        ui->progressBar_aux2->setValue(data_int.at(6));
        ui->progressBar_aux3->setValue(data_int.at(7));
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

void configQuadrotor::on_pushButton_AngularVelocityControl_cleanPlots_released()
{
    ui->widget_plot_angularVelocityControl_up->clean();
    ui->widget_plot_angularVelocityControl_mid->clean();
    ui->widget_plot_angularVelocityControl_down->clean();
}

void configQuadrotor::on_pushButton_AngularControl_cleanPlots_released()
{
    ui->widget_plot_angularControl_up->clean();
    ui->widget_plot_angularControl_down->clean();
}

void configQuadrotor::on_doubleSpinBox_Gyro_X_valueChanged(double arg1)
{
    quadrotor->setX_GyroGain((quint16)((float)arg1*1000));
}

void configQuadrotor::on_doubleSpinBox_Gyro_Y_valueChanged(double arg1)
{
    quadrotor->setY_GyroGain((quint16)((float)arg1*1000));
}

void configQuadrotor::on_doubleSpinBox_Gyro_Z_valueChanged(double arg1)
{
    quadrotor->setZ_GyroGain((quint16)((float)arg1*1000));
}

void configQuadrotor::on_doubleSpinBox_filter_X_P_valueChanged(double arg1)
{
    quadrotor->setX_AlfaBeta_P((quint16)((float)arg1*1000));
}

void configQuadrotor::on_doubleSpinBox_filter_X_I_valueChanged(double arg1)
{
    quadrotor->setX_AlfaBeta_I((quint16)((float)arg1*1000));
}

void configQuadrotor::on_doubleSpinBox_filter_Y_P_valueChanged(double arg1)
{
    quadrotor->setY_AlfaBeta_P((quint16)((float)arg1*1000));
}

void configQuadrotor::on_doubleSpinBox_filter_Y_I_valueChanged(double arg1)
{
    quadrotor->setY_AlfaBeta_I((quint16)((float)arg1*1000));
}

void configQuadrotor::receivedNewCommand(QByteArray b) {
    QString s;
    bool ok;
    int value;

    switch(b[0]) {
    case 'X':
        s = b.mid(2);
        value = s.toInt(&ok);
        if(!ok) return;

        switch(b[1]) {
        case 'P':
            ui->doubleSpinBox_angularControl_X_P->setValue((float)value/1000);
            break;
        case 'I':
            break;
        case 'G':
            ui->doubleSpinBox_angularVelocityControl_X_P->setValue((float)value/1000);
            break;
        case 'A':
            ui->doubleSpinBox_angularVelocityControl_X_I->setValue((float)value/1000);
            break;
        case 'O':
            ui->doubleSpinBox_filter_X_P->setValue((float)value/1000);
            break;
        case 'L':
            ui->doubleSpinBox_filter_X_I->setValue((float)value/1000);
            break;
        case 'E':
            ui->doubleSpinBox_Gyro_X->setValue((float)value/1000);
        }
        break;
    case 'Y':
        s = b.mid(2);
        value = s.toInt(&ok);
        if(!ok) return;

        switch(b[1]) {
        case 'P':
            ui->doubleSpinBox_angularControl_Y_P->setValue((float)value/1000);
            break;
        case 'I':
            break;
        case 'G':
            ui->doubleSpinBox_angularVelocityControl_Y_P->setValue((float)value/1000);
            break;
        case 'A':
            ui->doubleSpinBox_angularVelocityControl_Y_I->setValue((float)value/1000);
            break;
        case 'O':
            ui->doubleSpinBox_filter_Y_P->setValue((float)value/1000);
            break;
        case 'L':
            ui->doubleSpinBox_filter_Y_I->setValue((float)value/1000);
            break;
        case 'E':
            ui->doubleSpinBox_Gyro_Y->setValue((float)value/1000);
        }
        break;
    case 'Z':
        s = b.mid(2);
        value = s.toInt(&ok);
        if(!ok) return;

        switch(b[1]) {
        case 'G':
            ui->doubleSpinBox_angularVelocityControl_Z_P->setValue((float)value/1000);
            break;
        case 'A':
            ui->doubleSpinBox_angularVelocityControl_Z_I->setValue((float)value/1000);
            break;
        case 'E':
            ui->doubleSpinBox_Gyro_Z->setValue((float)value/1000);
        }
        break;
    }
}

void configQuadrotor::on_pushButton_testVelocityFlight_released()
{
    static bool velocity_flight = false;

     if(velocity_flight) {
         velocity_flight = false;
         ui->pushButton_testVelocityFlight->setStyleSheet("background-color: rgb(255, 0, 0);");
         quadrotor->setX_PA((quint16)(ui->doubleSpinBox_angularControl_X_P->value()*1000));
         quadrotor->setY_PA((quint16)(ui->doubleSpinBox_angularControl_Y_P->value()*1000));
     } else {
         velocity_flight = true;
         ui->pushButton_testVelocityFlight->setStyleSheet("background-color: rgb(0, 255, 0);");
         quadrotor->setX_PA(0);
         quadrotor->setY_PA(0);
     }
}

void configQuadrotor::on_doubleSpinBox_angularVelocityControl_X_P_valueChanged(double arg1)
{
    quadrotor->setX_PG((quint16)((float)arg1*1000));
}

void configQuadrotor::on_doubleSpinBox_angularVelocityControl_X_I_valueChanged(double arg1)
{
    quadrotor->setX_IG((quint16)((float)arg1*1000));
}

void configQuadrotor::on_doubleSpinBox_angularVelocityControl_Y_P_valueChanged(double arg1)
{
    quadrotor->setY_PG((quint16)((float)arg1*1000));
}

void configQuadrotor::on_doubleSpinBox_angularVelocityControl_Y_I_valueChanged(double arg1)
{
    quadrotor->setY_IG((quint16)((float)arg1*1000));
}

void configQuadrotor::on_doubleSpinBox_angularVelocityControl_Z_P_valueChanged(double arg1)
{
//    quadrotor->setZ_PG((quint16)((float)arg1*1000));
}

void configQuadrotor::on_doubleSpinBox_angularVelocityControl_Z_I_valueChanged(double arg1)
{
//    quadrotor->setZ_IG((quint16)((float)arg1*1000));
}
