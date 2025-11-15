/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCharts/QChartView>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *topButtonsLayout;
    QPushButton *loadButton;
    QPushButton *fitButton;
    QSpacerItem *horizontalSpacer;
    QGroupBox *parametersGroup;
    QGridLayout *gridLayout;
    QLabel *labelSaturation;
    QDoubleSpinBox *saturationCurrentSpin;
    QLabel *labelIdeality;
    QDoubleSpinBox *idealityFactorSpin;
    QLabel *labelTemperature;
    QDoubleSpinBox *temperatureSpin;
    QLabel *labelMinVoltage;
    QDoubleSpinBox *minVoltageSpin;
    QLabel *labelMaxVoltage;
    QDoubleSpinBox *maxVoltageSpin;
    QLabel *labelBreakdownVoltage;
    QDoubleSpinBox *breakdownVoltageSpin;
    QLabel *labelBreakdownCurrent;
    QDoubleSpinBox *breakdownCurrentSpin;
    QChartView *chartView;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(900, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        topButtonsLayout = new QHBoxLayout();
        topButtonsLayout->setObjectName("topButtonsLayout");
        loadButton = new QPushButton(centralwidget);
        loadButton->setObjectName("loadButton");

        topButtonsLayout->addWidget(loadButton);

        fitButton = new QPushButton(centralwidget);
        fitButton->setObjectName("fitButton");

        topButtonsLayout->addWidget(fitButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        topButtonsLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(topButtonsLayout);

        parametersGroup = new QGroupBox(centralwidget);
        parametersGroup->setObjectName("parametersGroup");
        gridLayout = new QGridLayout(parametersGroup);
        gridLayout->setObjectName("gridLayout");
        labelSaturation = new QLabel(parametersGroup);
        labelSaturation->setObjectName("labelSaturation");

        gridLayout->addWidget(labelSaturation, 0, 0, 1, 1);

        saturationCurrentSpin = new QDoubleSpinBox(parametersGroup);
        saturationCurrentSpin->setObjectName("saturationCurrentSpin");
        saturationCurrentSpin->setDecimals(9);
        saturationCurrentSpin->setMinimum(0.000000000001000);
        saturationCurrentSpin->setMaximum(1.000000000000000);
        saturationCurrentSpin->setSingleStep(0.000001000000000);
        saturationCurrentSpin->setValue(0.000002000000000);

        gridLayout->addWidget(saturationCurrentSpin, 0, 1, 1, 1);

        labelIdeality = new QLabel(parametersGroup);
        labelIdeality->setObjectName("labelIdeality");

        gridLayout->addWidget(labelIdeality, 1, 0, 1, 1);

        idealityFactorSpin = new QDoubleSpinBox(parametersGroup);
        idealityFactorSpin->setObjectName("idealityFactorSpin");
        idealityFactorSpin->setDecimals(2);
        idealityFactorSpin->setMinimum(0.500000000000000);
        idealityFactorSpin->setMaximum(2.000000000000000);
        idealityFactorSpin->setSingleStep(0.100000000000000);
        idealityFactorSpin->setValue(1.000000000000000);

        gridLayout->addWidget(idealityFactorSpin, 1, 1, 1, 1);

        labelTemperature = new QLabel(parametersGroup);
        labelTemperature->setObjectName("labelTemperature");

        gridLayout->addWidget(labelTemperature, 2, 0, 1, 1);

        temperatureSpin = new QDoubleSpinBox(parametersGroup);
        temperatureSpin->setObjectName("temperatureSpin");
        temperatureSpin->setMinimum(200.000000000000000);
        temperatureSpin->setMaximum(600.000000000000000);
        temperatureSpin->setSingleStep(10.000000000000000);
        temperatureSpin->setValue(300.000000000000000);

        gridLayout->addWidget(temperatureSpin, 2, 1, 1, 1);

        labelMinVoltage = new QLabel(parametersGroup);
        labelMinVoltage->setObjectName("labelMinVoltage");

        gridLayout->addWidget(labelMinVoltage, 0, 2, 1, 1);

        minVoltageSpin = new QDoubleSpinBox(parametersGroup);
        minVoltageSpin->setObjectName("minVoltageSpin");
        minVoltageSpin->setDecimals(2);
        minVoltageSpin->setMinimum(-100.000000000000000);
        minVoltageSpin->setMaximum(0.000000000000000);
        minVoltageSpin->setSingleStep(0.100000000000000);
        minVoltageSpin->setValue(-5.000000000000000);

        gridLayout->addWidget(minVoltageSpin, 0, 3, 1, 1);

        labelMaxVoltage = new QLabel(parametersGroup);
        labelMaxVoltage->setObjectName("labelMaxVoltage");

        gridLayout->addWidget(labelMaxVoltage, 1, 2, 1, 1);

        maxVoltageSpin = new QDoubleSpinBox(parametersGroup);
        maxVoltageSpin->setObjectName("maxVoltageSpin");
        maxVoltageSpin->setDecimals(2);
        maxVoltageSpin->setMinimum(0.000000000000000);
        maxVoltageSpin->setMaximum(10.000000000000000);
        maxVoltageSpin->setSingleStep(0.100000000000000);
        maxVoltageSpin->setValue(1.000000000000000);

        gridLayout->addWidget(maxVoltageSpin, 1, 3, 1, 1);

        labelBreakdownVoltage = new QLabel(parametersGroup);
        labelBreakdownVoltage->setObjectName("labelBreakdownVoltage");

        gridLayout->addWidget(labelBreakdownVoltage, 2, 2, 1, 1);

        breakdownVoltageSpin = new QDoubleSpinBox(parametersGroup);
        breakdownVoltageSpin->setObjectName("breakdownVoltageSpin");
        breakdownVoltageSpin->setDecimals(1);
        breakdownVoltageSpin->setMinimum(0.000000000000000);
        breakdownVoltageSpin->setMaximum(100.000000000000000);
        breakdownVoltageSpin->setSingleStep(1.000000000000000);
        breakdownVoltageSpin->setValue(50.000000000000000);

        gridLayout->addWidget(breakdownVoltageSpin, 2, 3, 1, 1);

        labelBreakdownCurrent = new QLabel(parametersGroup);
        labelBreakdownCurrent->setObjectName("labelBreakdownCurrent");

        gridLayout->addWidget(labelBreakdownCurrent, 3, 2, 1, 1);

        breakdownCurrentSpin = new QDoubleSpinBox(parametersGroup);
        breakdownCurrentSpin->setObjectName("breakdownCurrentSpin");
        breakdownCurrentSpin->setDecimals(3);
        breakdownCurrentSpin->setMinimum(-10.000000000000000);
        breakdownCurrentSpin->setMaximum(0.000000000000000);
        breakdownCurrentSpin->setSingleStep(0.010000000000000);
        breakdownCurrentSpin->setValue(-0.010000000000000);

        gridLayout->addWidget(breakdownCurrentSpin, 3, 3, 1, 1);


        verticalLayout->addWidget(parametersGroup);

        chartView = new QChartView(centralwidget);
        chartView->setObjectName("chartView");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(chartView->sizePolicy().hasHeightForWidth());
        chartView->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(chartView);

        verticalLayout->setStretch(2, 1);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 900, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Shockley Model", nullptr));
        loadButton->setText(QCoreApplication::translate("MainWindow", "Wczytaj CSV", nullptr));
        fitButton->setText(QCoreApplication::translate("MainWindow", "Dopasuj model", nullptr));
        parametersGroup->setTitle(QCoreApplication::translate("MainWindow", "Parametry modelu", nullptr));
        labelSaturation->setText(QCoreApplication::translate("MainWindow", "Pr\304\205d nasycenia (A)", nullptr));
        labelIdeality->setText(QCoreApplication::translate("MainWindow", "Wsp\303\263\305\202czynnik idealno\305\233ci n", nullptr));
        labelTemperature->setText(QCoreApplication::translate("MainWindow", "Temperatura (K)", nullptr));
        labelMinVoltage->setText(QCoreApplication::translate("MainWindow", "Minimalne napi\304\231cie (V)", nullptr));
        labelMaxVoltage->setText(QCoreApplication::translate("MainWindow", "Maksymalne napi\304\231cie (V)", nullptr));
        labelBreakdownVoltage->setText(QCoreApplication::translate("MainWindow", "Napi\304\231cie przebicia |V_BR|", nullptr));
        labelBreakdownCurrent->setText(QCoreApplication::translate("MainWindow", "Pr\304\205d przebicia (A)", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
