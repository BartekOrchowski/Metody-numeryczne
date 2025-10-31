#include "plotwidget.h"

PlotWidget::PlotWidget(QWidget *parent)
    : QChartView(new QChart(), parent)    // bez QtCharts::
{
    chart()->setTitle("Charakterystyka I–V");
}

void PlotWidget::plotData(const QVector<QPointF> &points) {
    auto *series = new QLineSeries();     // bez QtCharts::
    for (const auto &p : points) series->append(p);
    chart()->removeAllSeries();
    chart()->addSeries(series);
    chart()->createDefaultAxes();
}
