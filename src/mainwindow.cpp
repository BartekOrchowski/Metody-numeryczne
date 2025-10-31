#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "plotwidget.h"
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QtCharts/QLineSeries>  // dodaj
// używaj: QLineSeries bez kwalifikacji

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Ensure chartView has a valid QChart instance
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
        if (firstLine) { // pomiń nagłówek
            firstLine = false;
            continue;
        }
        if (line.isEmpty()) continue;
        QStringList parts = line.split(',');
        if (parts.size() < 2) continue;

        bool ok1, ok2;
        double V = parts[0].toDouble(&ok1);
        double I = parts[1].toDouble(&ok2);
        if (ok1 && ok2)
            points.append(QPointF(V, I));
    }
    return points;
}

void MainWindow::onLoadCSV()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Wybierz plik CSV", "", "CSV Files (*.csv)");
    if (fileName.isEmpty())
        return;

    m_data = loadCSV(fileName);

    if (m_data.isEmpty()) {
        QMessageBox::warning(this, "Błąd", "Nie udało się wczytać danych z pliku.");
        return;
    }

    // Zakładamy, że masz widget wykresu w UI o nazwie 'chartView'
    auto plot = ui->chartView;  // np. QChartView lub PlotWidget
    if (plot) {
        QLineSeries *series = new QLineSeries();
        for (const auto &p : m_data)
            series->append(p);
        plot->chart()->removeAllSeries();
        plot->chart()->addSeries(series);
        plot->chart()->createDefaultAxes();
        plot->chart()->setTitle("Charakterystyka I–V (dane pomiarowe)");
    }

    QMessageBox::information(this, "Sukces", "Wczytano " + QString::number(m_data.size()) + " punktów.");
}

void MainWindow::onFitModel()
{
    QMessageBox::information(this, "Model", "Dopasowywanie modelu...");
}
