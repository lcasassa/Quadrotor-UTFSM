#include <qglobal.h>
#include <qwt_painter.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_layout.h>
#include <qwt_scale_widget.h>
#include <qwt_scale_draw.h>
#include "plot.h"

Plot::Plot(QWidget *parent): QwtPlot(parent), d_interval(10.0)
{
    // Assign a title
//    setTitle("Testing Refresh Rates");

    setCanvasBackground(Qt::white);

    alignScales();

    // Insert grid
    d_grid = new QwtPlotGrid();
    d_grid->attach(this);


    // Axis 
//    setAxisTitle(QwtPlot::xBottom, "Segundos");
    setAxisScale(QwtPlot::xBottom, -d_interval, 0.0);

//    setAxisTitle(QwtPlot::yLeft, "Centímetros");
    setAxisScale(QwtPlot::yLeft, -1.0, 1.0);

//    d_clock.start();

    //setSettings(d_settings);

    d_grid->setPen(Qt::NoPen);
    d_grid->setVisible(Qt::NoPen != Qt::NoPen);

    canvas()->setAttribute(Qt::WA_PaintOnScreen, false);

    canvas()->setPaintAttribute(QwtPlotCanvas::BackingStore, false);
    canvas()->setPaintAttribute(QwtPlotCanvas::ImmediatePaint, true);

    QwtPainter::setPolylineSplitting(true);


    for ( int i = 0; i < PLOT_SIZE; i++ ) {
        axisTime[i] = -(PLOT_SIZE-(double)i)/100;
    }

}

void Plot::addVariable(const QColor &color) {

    axisData.append(new double[PLOT_SIZE]);

    // Insert curve
    d_curve.append(new QwtPlotCurve("Data Moving Right"));
    d_curve.last()->setPen(QPen(Qt::black));
//    d_curve.last()->setData(new CircularBuffer(d_interval, 10));
    d_curve.last()->attach(this);

    d_curve.last()->setRawSamples(axisTime, axisData.last(), PLOT_SIZE);
    QPen *pen = new QPen(color);

    d_curve.last()->setPen(*pen);
    d_curve.last()->setBrush(Qt::NoBrush);

    d_curve.last()->setPaintAttribute(QwtPlotCurve::ClipPolygons, 0 & QwtPlotCurve::ClipPolygons);
    d_curve.last()->setRenderHint(QwtPlotCurve::RenderAntialiased, 0 & QwtPlotCurve::RenderAntialiased);

}

//
//  Set a plain canvas frame and align the scales to it
//
void Plot::alignScales()
{
    // The code below shows how to align the scales to
    // the canvas frame, but is also a good example demonstrating
    // why the spreaded API needs polishing.

    canvas()->setFrameStyle(QFrame::Box | QFrame::Plain );
    canvas()->setLineWidth(1);

    for ( int i = 0; i < QwtPlot::axisCnt; i++ )
    {
        QwtScaleWidget *scaleWidget = (QwtScaleWidget *)axisWidget(i);
        if ( scaleWidget )
            scaleWidget->setMargin(0);

        QwtScaleDraw *scaleDraw = (QwtScaleDraw *)axisScaleDraw(i);
        if ( scaleDraw )
            scaleDraw->enableComponent(QwtAbstractScaleDraw::Backbone, false);
    }

    plotLayout()->setAlignCanvasToScales(true);
}
/*
void Plot::setSettings(const Settings &s)
{
    if ( d_timerId >= 0 )
        killTimer(d_timerId);

    d_timerId = startTimer(s.updateInterval);

    d_grid->setPen(s.grid.pen);
    d_grid->setVisible(s.grid.pen.style() != Qt::NoPen);

    CircularBuffer *buffer = (CircularBuffer *)d_curve->data();
    if ( s.curve.numPoints != buffer->size() ||
        s.curve.functionType != d_settings.curve.functionType )
    {

        switch(s.curve.functionType)
        {
            case Settings::Wave:
                buffer->setFunction(wave);
                break;
            case Settings::Noise:
                buffer->setFunction(noise);
                break;
            default:
                buffer->setFunction(NULL);
        }

        buffer->fill(d_interval, s.curve.numPoints);
    }

    d_curve->setPen(s.curve.pen);
    d_curve->setBrush(s.curve.brush);

    d_curve->setPaintAttribute(QwtPlotCurve::ClipPolygons,
        s.curve.paintAttributes & QwtPlotCurve::ClipPolygons);
    d_curve->setRenderHint(QwtPlotCurve::RenderAntialiased,
        s.curve.renderHint & QwtPlotCurve::RenderAntialiased);

    canvas()->setAttribute(Qt::WA_PaintOnScreen, s.canvas.paintOnScreen);

    canvas()->setPaintAttribute(
        QwtPlotCanvas::BackingStore, s.canvas.useBackingStore);
    canvas()->setPaintAttribute(
        QwtPlotCanvas::ImmediatePaint, s.canvas.immediatePaint);

    QwtPainter::setPolylineSplitting(s.curve.lineSplitting);

    d_settings = s;
}
*/
/*
void Plot::timerEvent(QTimerEvent *)
{
    CircularBuffer *buffer = (CircularBuffer *)d_curve->data();
    buffer->setReferenceTime(d_clock.elapsed() / 1000.0);


    canvas()->replot(); // los ejes no cambian
//    replot();

}*/

void Plot::newData(QList<double> data) {
    double min=100000, max=-100000;

    if(data.size() < axisData.size()) {
        qWarning("newData.size() != axisData.size()");
        return;
    }

    for(int j=0; j<axisData.size(); j++) {

        axisData[j][PLOT_SIZE-1] = data[j];

        for ( int i = 0; i < PLOT_SIZE-1; i++ ) {
            axisData[j][i] = axisData[j][i+1];
        }

        for( int i = 0; i< PLOT_SIZE; i++) {
            if(min > axisData[j][i])
                min = axisData[j][i];
            if(max < axisData[j][i])
                max = axisData[j][i];
        }

    }
    if(min == max) {
        max+=2;
        min-=2;
    }
    setAxisScale(QwtPlot::yLeft, min, max);
//    canvas()->replot(); // los ejes no cambian
    replot(); // los ejes si cambian

}
