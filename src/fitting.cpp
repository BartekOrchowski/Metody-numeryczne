#include "fitting.h"
#include <QtMath>

QPair<double, double> Fitting::fitShockley(const QVector<QPointF> &data)
{
    // Na razie prosta atrapa – później dodasz dopasowanie metodą najmniejszych kwadratów
    double IS = 1e-12;
    double n = 1.5;
    return {IS, n};
}
