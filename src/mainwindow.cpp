#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QPainter>
#include <QRegularExpression>
#include <QTextStream>
#include <QtCharts/QAbstractAxis>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QLegend>
#include <QtCharts/QValueAxis>
#include <QColor>
#include <QPen>
#include <algorithm>
#include <cmath>
#include <limits>

namespace {
constexpr double kBoltzmann = 1.380649e-23;
constexpr double qElectron = 1.602176634e-19;

double clampArg(double value)
{
    return std::clamp(value, -80.0, 40.0);
}

double reverseRegionCurrent(double voltage,
                            double saturationCurrent,
                            double breakdownVoltage,
                            double breakdownCurrent,
                            double minVoltage)
{
    const double bv = std::abs(breakdownVoltage);
    const double clampIs = -std::abs(saturationCurrent);
    const double targetBreakdown = (breakdownCurrent < clampIs) ? breakdownCurrent : clampIs * 10.0;

    if (bv <= 0.0 || voltage > -bv)
        return clampIs;

    const double span = std::max(std::abs(minVoltage) - bv, 1.0);
    const double distance = std::abs(voltage) - bv;
    double t = std::clamp(distance / span, 0.0, 1.0);
    const double smooth = 1.0 - std::exp(-5.0 * t);
    return (1.0 - smooth) * clampIs + smooth * targetBreakdown;
}

} // namespace

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    if (ui->chartView) {
        if (!ui->chartView->chart()) {
            ui->chartView->setChart(new QChart());
        }
        ui->chartView->setRenderHint(QPainter::Antialiasing);
        ui->chartView->setRubberBand(QChartView::RectangleRubberBand);
    }

    connect(ui->loadButton, &QPushButton::clicked, this, &MainWindow::onLoadCSV);
    connect(ui->fitButton, &QPushButton::clicked, this, &MainWindow::onFitModel);
    connect(ui->resetZoomButton, &QPushButton::clicked, this, &MainWindow::resetZoom);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QVector<QPointF> MainWindow::loadCSV(const QString &filePath)
{
    QVector<QPointF> points;
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return points;

    QTextStream in(&file);
    bool firstLine = true;
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty())
            continue;

        if (firstLine) {
            firstLine = false;
            if (line.contains(QRegularExpression("[A-Za-z]")))
                continue;
        }

        line.replace(';', ',');
        const QStringList parts = line.split(QRegularExpression("[,\\t]"), Qt::SkipEmptyParts);
        if (parts.size() < 2)
            continue;

        bool okV = false;
        bool okI = false;
        const double voltage = parts[0].toDouble(&okV);
        const double current = parts[1].toDouble(&okI);
        if (okV && okI)
            points.append(QPointF(voltage, current));
    }
    return points;
}

void MainWindow::onLoadCSV()
{
    const QString fileName = QFileDialog::getOpenFileName(this, tr("Wybierz plik CSV"), QString(), tr("CSV Files (*.csv)"));
    if (fileName.isEmpty())
        return;

    m_data = loadCSV(fileName);

    if (m_data.isEmpty()) {
        QMessageBox::warning(this, tr("Blad"), tr("Nie udalo sie wczytac danych z pliku."));
        return;
    }

    updateChart();
    QMessageBox::information(this, tr("Sukces"), tr("Wczytano %1 punktow.").arg(m_data.size()));
}

void MainWindow::onFitModel()
{
    if (ui->minVoltageSpin->value() >= ui->maxVoltageSpin->value()) {
        QMessageBox::warning(this, tr("Parametry"), tr("Minimalne napiecie musi byc mniejsze od maksymalnego."));
        return;
    }

    const int samples = ui->samplePointsSpin->value();
    const bool showAll = ui->showAllMethodsCheck->isChecked();

    m_analyticalData.clear();
    m_fdmData.clear();
    auto computeMethod = [&](Method method) -> bool {
        switch (method) {
        case Method::Analytical:
            m_analyticalData = generateAnalyticalCurve(samples);
            return !m_analyticalData.isEmpty();
        case Method::FiniteDifference:
            m_fdmData = generateFiniteDifferenceCurve(samples);
            return !m_fdmData.isEmpty();
        }
        return false;
    };

    bool anyMethod = false;
    if (showAll) {
        anyMethod |= computeMethod(Method::Analytical);
        anyMethod |= computeMethod(Method::FiniteDifference);
    } else {
        anyMethod = computeMethod(currentMethod());
    }

    if (!anyMethod) {
        QMessageBox::warning(this, tr("Model"), tr("Nie udalo sie wygenerowac danych wybranych metod."));
        return;
    }

    updateChart();
}

MainWindow::Method MainWindow::currentMethod() const
{
    if (!ui->methodCombo)
        return Method::Analytical;

    switch (ui->methodCombo->currentIndex()) {
    case 1:
        return Method::FiniteDifference;
    default:
        return Method::Analytical;
    }
}

QVector<QPointF> MainWindow::generateAnalyticalCurve(int samples) const
{
    QVector<QPointF> points;
    if (!ui->chartView)
        return points;

    const double saturationCurrent = ui->saturationCurrentSpin->value();
    const double idealityFactor = ui->idealityFactorSpin->value();
    const double temperature = ui->temperatureSpin->value();
    const double minVoltage = ui->minVoltageSpin->value();
    const double maxVoltage = ui->maxVoltageSpin->value();
    const double breakdownVoltage = ui->breakdownVoltageSpin->value();
    const double breakdownCurrent = ui->breakdownCurrentSpin->value();

    if (saturationCurrent <= 0.0 || idealityFactor <= 0.0 || temperature <= 0.0 || minVoltage >= maxVoltage)
        return points;

    const double thermalVoltage = (kBoltzmann * temperature) / qElectron;
    const double step = (maxVoltage - minVoltage) / samples;
    points.reserve(samples + 1);

    for (int i = 0; i <= samples; ++i) {
        const double voltage = minVoltage + i * step;
        double current = 0.0;

        if (voltage >= 0.0) {
            const double arg = clampArg(voltage / (idealityFactor * thermalVoltage));
            current = saturationCurrent * (std::exp(arg) - 1.0);
        } else {
            current = reverseRegionCurrent(voltage, saturationCurrent, breakdownVoltage, breakdownCurrent, minVoltage);
        }

        points.append(QPointF(voltage, current));
    }

    return points;
}

QVector<QPointF> MainWindow::generateFiniteDifferenceCurve(int samples) const
{
    QVector<QPointF> points;
    if (!ui->chartView)
        return points;

    const double saturationCurrent = ui->saturationCurrentSpin->value();
    const double idealityFactor = ui->idealityFactorSpin->value();
    const double temperature = ui->temperatureSpin->value();
    const double minVoltage = ui->minVoltageSpin->value();
    const double maxVoltage = ui->maxVoltageSpin->value();
    const double breakdownVoltage = ui->breakdownVoltageSpin->value();
    const double breakdownCurrent = ui->breakdownCurrentSpin->value();

    if (saturationCurrent <= 0.0 || idealityFactor <= 0.0 || temperature <= 0.0 || minVoltage >= maxVoltage)
        return points;

    const double thermalVoltage = (kBoltzmann * temperature) / qElectron;
    const double step = (maxVoltage - minVoltage) / samples;

    double voltage = minVoltage;
    double current = (voltage >= 0.0)
        ? saturationCurrent * (std::exp(clampArg(voltage / (idealityFactor * thermalVoltage))) - 1.0)
        : reverseRegionCurrent(voltage, saturationCurrent, breakdownVoltage, breakdownCurrent, minVoltage);

    points.reserve(samples + 1);
    points.append(QPointF(voltage, current));

    for (int i = 0; i < samples; ++i) {
        double derivative = 0.0;
        if (voltage >= 0.0) {
            const double arg = clampArg(voltage / (idealityFactor * thermalVoltage));
            derivative = (saturationCurrent / (idealityFactor * thermalVoltage)) * std::exp(arg);
        } else if (breakdownVoltage > 0.0 && voltage <= -breakdownVoltage) {
            const double span = std::max(std::abs(minVoltage) - breakdownVoltage, 1e-9);
            derivative = (breakdownCurrent + saturationCurrent) / span;
        } else {
            derivative = 0.0;
        }

        voltage += step;
        current += derivative * step;
        points.append(QPointF(voltage, current));
    }

    return points;
}


void MainWindow::resetZoom()
{
    if (ui->chartView && ui->chartView->chart())
        ui->chartView->chart()->zoomReset();
}

void MainWindow::updateChart()
{
    if (!ui->chartView)
        return;

    auto *chart = ui->chartView->chart();
    if (!chart)
        return;

    chart->removeAllSeries();

    auto addSeries = [&](const QVector<QPointF> &points, const QString &name, const QColor &color, qreal width = 1.5) {
        if (points.isEmpty())
            return;
        auto *series = new QLineSeries(chart);
        series->setName(name);
        series->append(points);
        QPen pen(color);
        pen.setWidthF(width);
        series->setPen(pen);
        chart->addSeries(series);
    };

    if (!m_data.isEmpty()) {
        auto *series = new QLineSeries(chart);
        series->setName(tr("Dane pomiarowe"));
        series->append(m_data);
        QPen pen(Qt::darkGray);
        pen.setWidthF(1.2);
        series->setPen(pen);
        chart->addSeries(series);
    }

    addSeries(m_analyticalData, tr("Analityczna"), QColor("#0074D9"));
    addSeries(m_fdmData, tr("Roznice skonczone"), QColor("#FF851B"));

    if (chart->series().isEmpty()) {
        chart->setTitle(tr("Brak danych do wyswietlenia"));
        if (chart->legend())
            chart->legend()->hide();
        return;
    }

    if (chart->legend()) {
        chart->legend()->setVisible(true);
        chart->legend()->setAlignment(Qt::AlignBottom);
    }
    chart->setTitle(tr("Charakterystyka I-V diody"));

    const auto oldAxes = chart->axes();
    for (QAbstractAxis *axis : oldAxes) {
        chart->removeAxis(axis);
        axis->deleteLater();
    }

    const bool hasMethod = !m_analyticalData.isEmpty() || !m_fdmData.isEmpty();
    auto *axisX = new QValueAxis(chart);
    axisX->setTitleText(tr("Napiecie V (V)"));
    axisX->setLabelFormat("%.2f");
    if (hasMethod)
        axisX->setRange(ui->minVoltageSpin->value(), ui->maxVoltageSpin->value());
    chart->addAxis(axisX, Qt::AlignBottom);

    double minY = std::numeric_limits<double>::max();
    double maxY = std::numeric_limits<double>::lowest();
    auto accumulateRange = [&](const QVector<QPointF> &points) {
        for (const auto &pt : points) {
            minY = std::min(minY, pt.y());
            maxY = std::max(maxY, pt.y());
        }
    };
    accumulateRange(m_data);
    accumulateRange(m_analyticalData);
    accumulateRange(m_fdmData);

    auto *axisY = new QValueAxis(chart);
    axisY->setTitleText(tr("Prad I (A)"));
    axisY->setLabelFormat("%.3g");
    if (minY < maxY) {
        const double margin = 0.05 * (maxY - minY);
        axisY->setRange(minY - margin, maxY + margin);
    }
    chart->addAxis(axisY, Qt::AlignLeft);

    const auto seriesList = chart->series();
    for (QAbstractSeries *series : seriesList) {
        series->attachAxis(axisX);
        series->attachAxis(axisY);
    }
}
