#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QRegularExpression>
#include <QTextStream>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QLegend>
#include <QtCharts/QValueAxis>
#include <algorithm>
#include <cmath>

namespace {
constexpr double kBoltzmann = 1.380649e-23;   // J/K
constexpr double qElectron = 1.602176634e-19; // C
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    if (ui->chartView && !ui->chartView->chart()) {
        ui->chartView->setChart(new QChart());
    }

    connect(ui->loadButton, &QPushButton::clicked, this, &MainWindow::onLoadCSV);
    connect(ui->fitButton, &QPushButton::clicked, this, &MainWindow::onFitModel);
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
        QMessageBox::warning(this, tr("Błąd"), tr("Nie udało się wczytać danych z pliku."));
        return;
    }

    updateChart();
    QMessageBox::information(this, tr("Sukces"), tr("Wczytano %1 punktów.").arg(m_data.size()));
}

void MainWindow::onFitModel()
{
    if (ui->minVoltageSpin->value() >= ui->maxVoltageSpin->value()) {
        QMessageBox::warning(this, tr("Parametry"), tr("Minimalne napięcie musi być mniejsze od maksymalnego."));
        return;
    }

    m_modelData = generateShockleyCurve();
    if (m_modelData.isEmpty()) {
        QMessageBox::warning(this, tr("Model"), tr("Nie udało się wygenerować danych modelu."));
        return;
    }

    updateChart();
}

QVector<QPointF> MainWindow::generateShockleyCurve() const
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
    const int samples = 600;
    const double step = (maxVoltage - minVoltage) / samples;
    points.reserve(samples + 1);

    for (int i = 0; i <= samples; ++i) {
        const double voltage = minVoltage + i * step;
        double current = 0.0;

        if (voltage >= 0.0) {
            double arg = voltage / (idealityFactor * thermalVoltage);
            arg = std::clamp(arg, -80.0, 40.0);
            current = saturationCurrent * (std::exp(arg) - 1.0);
        } else {
            if (breakdownVoltage > 0.0 && voltage <= -breakdownVoltage) {
                const double span = std::abs(minVoltage) - breakdownVoltage;
                const double distance = std::abs(voltage) - breakdownVoltage;
                double t = span > 1e-9 ? distance / span : 1.0;
                t = std::clamp(t, 0.0, 1.0);
                current = (1.0 - t) * (-saturationCurrent) + t * breakdownCurrent;
            } else {
                current = -saturationCurrent;
            }
        }

        points.append(QPointF(voltage, current));
    }

    return points;
}

void MainWindow::updateChart()
{
    if (!ui->chartView)
        return;
    auto *chart = ui->chartView->chart();
    if (!chart)
        return;

    chart->removeAllSeries();

    if (!m_data.isEmpty()) {
        auto *measured = new QLineSeries(chart);
        measured->setName(tr("Dane pomiarowe"));
        measured->append(m_data);
        chart->addSeries(measured);
    }

    if (!m_modelData.isEmpty()) {
        auto *model = new QLineSeries(chart);
        model->setName(tr("Model Shockleya"));
        model->append(m_modelData);
        chart->addSeries(model);
    }

    if (chart->series().isEmpty()) {
        chart->setTitle(tr("Brak danych do wyświetlenia"));
        chart->legend()->hide();
        return;
    }

    chart->createDefaultAxes();
    chart->legend()->setVisible(true);
    chart->setTitle(tr("Charakterystyka I-V diody"));

    if (auto *axisX = qobject_cast<QValueAxis *>(chart->axisX())) {
        axisX->setTitleText(tr("Napięcie V (V)"));
        axisX->setLabelFormat("%.2f");
        if (!m_modelData.isEmpty()) {
            axisX->setRange(ui->minVoltageSpin->value(), ui->maxVoltageSpin->value());
        }
    }

    if (auto *axisY = qobject_cast<QValueAxis *>(chart->axisY())) {
        axisY->setTitleText(tr("Prąd I (A)"));
        axisY->setLabelFormat("%.3g");
    }
}
