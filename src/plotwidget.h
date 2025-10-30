#pragma once
#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>

QT_CHARTS_USE_NAMESPACE

class PlotWidget : public QChartView
{
    Q_OBJECT
public:
    explicit PlotWidget(QWidget *parent = nullptr);
    void plotData(const QVector<QPointF> &points);
};
