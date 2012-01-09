#ifndef _PLOT_H_
#define _PLOT_H_ 1

#include <qwt_plot.h>
#include <qwt_system_clock.h>
#include <QList>
//#include "settings.h"

class QwtPlotGrid;
class QwtPlotCurve;

#define PLOT_SIZE 1000

class Plot: public QwtPlot
{
    Q_OBJECT

public:
    Plot(QWidget* = NULL);
    void addVariable(const QColor &color);
    void newData(QList<double> data);

public Q_SLOTS:
//    void setSettings(const Settings &);

protected:
//    virtual void timerEvent(QTimerEvent *e);

private:
    void alignScales();

    QwtPlotGrid *d_grid;
    QList<QwtPlotCurve *> d_curve;
    QList<double *> axisData;
    double axisTime[PLOT_SIZE];
    double d_interval;

/*
    QwtSystemClock d_clock;

    int d_timerId;
*/
//    Settings d_settings;
};

#endif
