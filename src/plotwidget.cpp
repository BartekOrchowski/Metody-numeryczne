#include "plotwidget.h"

PlotWidget::PlotWidget(QWidget *parent)
    : QChartView(new QChart(), parent)
{
    chart()->setTitle("Charakterystyka I–V");
}

void PlotWidget::plotData(const QVector<QPointF> &points)
{
    QLineSeries *series = new QLineSeries();
    for (const auto &p : points)
        series->append(p);
    chart()->removeAllSeries();
    chart()->addSeries(series);
    chart()->createDefaultAxes();
}
