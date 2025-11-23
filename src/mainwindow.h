#pragma once
#include <QMainWindow>
#include <QVector>
#include <QPointF>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onLoadCSV();
    void onFitModel();
    void resetZoom();

private:
    enum class Method {
        Analytical = 0,
        FiniteDifference
    };

    Method currentMethod() const;

    Ui::MainWindow *ui;
    QVector<QPointF> m_data;
    QVector<QPointF> m_analyticalData;
    QVector<QPointF> m_fdmData;

    QVector<QPointF> loadCSV(const QString &filePath);
    QVector<QPointF> generateAnalyticalCurve(int samples) const;
    QVector<QPointF> generateFiniteDifferenceCurve(int samples) const;
    void updateChart();
};
