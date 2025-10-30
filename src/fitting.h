#pragma once
#include <QVector>
#include <QPointF>

class Fitting
{
public:
    static QPair<double, double> fitShockley(const QVector<QPointF> &data);
};
