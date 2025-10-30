#pragma once
#include <QMainWindow>
#include <QVector>
#include <QPointF>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onLoadCSV();
    void onFitModel();

private:
    Ui::MainWindow *ui;
    QVector<QPointF> m_data;   // dane pomiarowe
    QVector<QPointF> loadCSV(const QString &filePath);  // funkcja pomocnicza
};
