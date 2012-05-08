#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    quadrotor = new Quadrotor();
    ui->widget_configQuadrotor->setQuadrotor(quadrotor);
}

MainWindow::~MainWindow()
{
    delete quadrotor;
    delete ui;
}

void MainWindow::on_actionSelect_Serial_triggered()
{
    quadrotor->selectSerialAndOpen(this);
}
