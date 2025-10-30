#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->loadButton, &QPushButton::clicked, this, &MainWindow::onLoadCSV);
    connect(ui->fitButton, &QPushButton::clicked, this, &MainWindow::onFitModel);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onLoadCSV()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Wybierz plik CSV", "", "CSV Files (*.csv)");
    if (fileName.isEmpty())
        return;
    QMessageBox::information(this, "CSV", "Wczytano plik: " + fileName);
}

void MainWindow::onFitModel()
{
    QMessageBox::information(this, "Model", "Dopasowywanie modelu...");
}
