#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "quadrotor.h"

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
    void on_actionSelect_Serial_triggered();

private:
    Ui::MainWindow *ui;
    Quadrotor *quadrotor;
};

#endif // MAINWINDOW_H
