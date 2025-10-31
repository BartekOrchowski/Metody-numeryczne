#pragma once
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QVector>
#include <QPointF>

class PlotWidget : public QChartView {   // bez QtCharts::
    Q_OBJECT
public:
    explicit PlotWidget(QWidget *parent = nullptr);
    void plotData(const QVector<QPointF> &points);
};
